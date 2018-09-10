/*
 * MPDDecoder.cpp
 *
 *  Created on: Jul 5, 2018
 *      Author: newdriver
 */

#include "MPDDecoder.h"
#include <vector>
#include <map>
#include <TH1F.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include "evioUtil.hxx"
#include "evioFileChannel.hxx"

#include "MPDStructure.h"
#include "MPDRawParser.h"
#include "GEMIDGenerator.h"
#include "GEMConfigure.h"
#include "../src/Benchmark.h"
#include "unordered_map"
#include <math.h>
#include <algorithm>
#ifdef __MULT_THREAD
#include "thread"
#endif

MPDDecoder::MPDDecoder() : GUIInforCenter(){
	// TODO Auto-generated constructor stub
	Initialize();
}

MPDDecoder::MPDDecoder(std::string fname){
	LoadFile(fname.c_str());
	Initialize();
}


void MPDDecoder::Initialize(){
	GEMConfigure *cfg=GEMConfigure::GetInstance();
	//int test[128]={0, 32, 64, 96, 8, 40, 72, 104, 16, 48, 80, 112, 24, 56, 88, 120, 1, 33, 65, 97, 9, 41, 73, 105, 17, 49, 81, 113, 25, 57, 89, 121, 2, 34, 66, 98, 10, 42, 74, 106, 18, 50, 82, 114, 26, 58, 90, 122, 3, 35, 67, 99, 11, 43, 75, 107, 19, 51, 83, 115, 27, 59, 91, 123, 4, 36, 68, 100, 12, 44, 76, 108, 20, 52, 84, 116, 28, 60, 92, 124, 5, 37, 69, 101, 13, 45, 77, 109, 21, 53, 85, 117, 29, 61, 93, 125, 6, 38, 70, 102, 14, 46, 78, 110, 22, 54, 86, 118, 30, 62, 94, 126, 7, 39, 71, 103, 15, 47, 79, 111, 23, 55, 87, 119, 31, 63, 95, 127};
	for(int i = 0 ; i <128; i ++){
	ChNb[i]=(cfg->GetSysCondfig().Analysis_cfg.nch)[i];
	//ChNb[i]=test[i];
	}
	//HitCut_sigma=cfg->GetSysCondfig().Analysis_cfg.ZeroSubtrCutSigma;
}

void MPDDecoder::LoadFile(std::string fname){
	Rawstream.clear();
	rawfilename=fname;

	chan=new evio::evioFileChannel(fname.c_str(),"r");
	chan->open();
}

void MPDDecoder::RawDisplay(uint id) {

	if ((Rawstream.size() == 0 )|| (id>Rawstream.end()->first)) {
		std::cout<<"read raw "<<(Rawstream.end()->first)<< " to " << Rawstream.end()->first+50<<std::endl;
		MPDRawParser *rawparser = new MPDRawParser();
		uint64_t evtid = 0;
		while (ReadBlock() && (evtid < 50)) {
			evtid++;
			rawparser->LoadRawData(block_vec_mpd);
			GUICanvasDataStream *stream = new GUICanvasDataStream();
			stream->LoadData(rawparser->GetDecoded());
			Rawstream[evtid] = stream;
		}
	}

	if(Rawstream.find(id)!=Rawstream.end()){
		CanvasTabDisplay(Rawstream[id]);      // load to display
	}else{
		std::cout<<"miao miao miao"<<std::endl;
	}
}


void MPDDecoder::PedestalMode(std::string savefname){
	std::unordered_map<int,TH1F *> Pedestal_temp;
	MPDRawParser *rawparser=new MPDRawParser();
	int i =0;
	Benchmark *timer=new Benchmark();
	while(ReadBlock()){
		if((i++)%100==0)timer->Print(i);
		rawparser->LoadRawData(block_vec_mpd);
		auto data =rawparser->GetCommonModeSubtraction(); // get  the common mode subtracted data
		// calculate the mean of the six time sample
		for(auto iter= data.begin();iter!=data.end();iter++){
			if(Pedestal_temp.find(iter->first)==Pedestal_temp.end()){
				Pedestal_temp[iter->first] = new TH1F(Form("%d",iter->first),Form("%d",iter->first),3500,-500,3000);
				//Pedestal_temp[iter->first]->GetYaxis()->SetRange(-500,2000);
			}
			// the mean of all the time samples
			Pedestal_temp[iter->first]->Fill((std::accumulate(iter->second.begin(),iter->second.end(),0.0)/(iter->second.size())));
		}
	}
	// some pedestal evaluation histograms
	std::map<int,TH1F *> pedestal_meanhisto;
	std::map<int,TH1F *> pedestal_rmshisto;
	TH1F *pedestal_rms_allhisto;
	TH2F *pedestal_rms_2d_distri;
	TH1F *pedestal_rms_2d_histo;

	//--
	pedestal_rms_allhisto=new TH1F(Form("PedestalRMS_distribution"),
					Form("PedestalRMS_distribution"),
					50, 0, 50);

	// save the data into file
	std::map<int,TH1F *> pedestal_mean;
	std::map<int,TH1F *> pedestal_rms;
	for(auto iter = Pedestal_temp.begin();iter!=Pedestal_temp.end();iter++){
		int crateid=GEM::getCrateID(iter->first);
		int mpdid=GEM::getMPDID(iter->first);
		int adcid=GEM::getADCID(iter->first);
		int channelid=GEM::getChannelID(iter->first);
		int uid=GEM::GetUID(crateid,mpdid,adcid,0);

		if(pedestal_mean.find(uid)==pedestal_mean.end()){
			pedestal_mean[uid]=new TH1F(Form("PedestalMean(offset)_mpd_%d_ch_%d",mpdid, adcid),
					Form("PedestalMean(offset)_mpd_%d_ch_%d",mpdid, adcid),
					128, 0, 128);
			pedestal_rms[uid]=new TH1F(Form("PedestalRMS_mpd_%d_ch_%d",mpdid, adcid),
					Form("PedestalRMS_mpd_%d_ch_%d",mpdid, adcid),
					128, 0, 128);
			pedestal_rms[uid]->SetMinimum(0);

			// evaluation histos
			pedestal_meanhisto[uid]=new TH1F(Form("PedestalMean(offset)_mpd_%d_ch_%d_histo",mpdid, adcid),
					Form("PedestalMean(offset)_mpd_%d_ch_%d_histo",mpdid, adcid),
					50, 0, 50);

			pedestal_rmshisto[uid]=new TH1F(Form("PedestalRMS_mpd_%d_ch_%d_distribution",mpdid, adcid),
					Form("PedestalRMS_mpd_%d_ch_%d_distribution",mpdid, adcid),
					50, 0, 50);
		}


		pedestal_mean[uid]->Fill(channelid,iter->second->GetMean());
		pedestal_rms[uid]->Fill(channelid,iter->second->GetRMS());

		pedestal_meanhisto[uid]->Fill(iter->second->GetMean());
		pedestal_rmshisto[uid]->Fill(iter->second->GetRMS());
		pedestal_rms_allhisto->Fill(iter->second->GetRMS());
	}

	// generate the 2-d histo for the pedestal
	pedestal_rms_2d_distri=new TH2F("Pedestal_distribution","Pedestal_distribution",pedestal_meanhisto.size()+2,0,pedestal_meanhisto.size()+2,50,0,50);
	pedestal_rms_2d_histo=new TH1F("Pedestal_distribution_histo","Pedestal_distribution_hiso",pedestal_meanhisto.size()+2,0,pedestal_meanhisto.size()+2);
	int counter_temp=0;

	for (auto iter = pedestal_rmshisto.begin();iter!=pedestal_rmshisto.end();iter++){
		int uid = iter->first;
		int crateid = GEM::getCrateID(uid);
		int mpdid = GEM::getMPDID(uid);
		int apvid = GEM::getADCID(uid);

		pedestal_rms_2d_histo->Fill(counter_temp,iter->second->GetMean());
		pedestal_rms_2d_histo->SetBinError(counter_temp+1,iter->second->GetRMS());
		counter_temp++;
	}
	counter_temp=0;
	for(auto iter = pedestal_rms.begin();iter!=pedestal_rms.end();iter++){
		int uid = iter->first;
		int crateid = GEM::getCrateID(uid);
		int mpdid = GEM::getMPDID(uid);
		int apvid = GEM::getADCID(uid);
		for(int i=0;i<128;i++){
			pedestal_rms_2d_distri->Fill(counter_temp,iter->second->GetBinContent(i+1));
		}
		counter_temp++;
	}


// save the histogram into root file
	TFile *file=new TFile(savefname.c_str(),"RECREATE");

	for(auto pedestal =pedestal_rmshisto.begin(); pedestal!=pedestal_rmshisto.end();pedestal++ ){
			pedestal_meanhisto[pedestal->first]->SetDirectory(file);
			pedestal_rmshisto[pedestal->first]->SetDirectory(file);
		}
	pedestal_rms_allhisto->SetDirectory(file);
	pedestal_rms_2d_distri->SetDirectory(file);
	pedestal_rms_2d_histo->SetDirectory(file);

	for(auto iter = pedestal_mean.begin();iter!=pedestal_mean.end();iter++){
		pedestal_mean[iter->first]->SetDirectory(file);
		pedestal_rms[iter->first]->SetDirectory(file);
	}
	file->Write();
	file->Close();
}

//! the zero subttraction mode display
void MPDDecoder::HitDisplay(std::string pedestalfname,int eventid){

	std::cout<<"[Test]"<<__func__<<" "<<__LINE__<<"This is a test"<<std::endl;
	// uid , value
	std::unordered_map<int,int> mPedestal_mean;
	std::unordered_map<int,int> mPedestal_rms;
	TFile *pfile=new TFile(pedestalfname.c_str(),"READ");
	if(!pfile->IsOpen()){
		std::cout<<__FUNCTION__<<"<"<<__LINE__<<"> [ERROR]: Pedestal file cannot open: "<<pedestalfname.c_str()<<std::endl;
		return;
	}

	std::cout<<"[Test]"<<__func__<<" "<<__LINE__<<"This is a test"<<std::endl;

	GEMConfigure *cfg= GEMConfigure::GetInstance();
	for(auto apvlist : (cfg->GetMapping().GetAPVList())){
		int crateid=GEM::getCrateID(apvlist);
		int mpdid=GEM::getMPDID(apvlist);
		int apvid=GEM::getADCID(apvlist);
		std::string hmean_str=Form("PedestalMean(offset)_mpd_%d_ch_%d",mpdid, apvid);
		std::string hRMS_str=Form("PedestalRMS_mpd_%d_ch_%d",mpdid, apvid);
		if(pfile->GetListOfKeys()->Contains(hmean_str.c_str())&&(pfile->GetListOfKeys()->Contains(hRMS_str.c_str()))){
			TH1F *hmean=(TH1F *) pfile->Get(hmean_str.c_str());
			TH1F *hRMS=(TH1F *) pfile->Get(hRMS_str.c_str());
			for(int i=0;i<128;i++){
				int uid=GEM::GetUID(crateid,mpdid,apvid,i);
				mPedestal_mean[uid]=(hmean->GetBinContent(i+1));
				mPedestal_rms[uid]=(hRMS->GetBinContent(i+1));
			}
		}else{
			std::cout<< "[WORNING] " << __FUNCTION__ << " " << __LINE__
					<< "MPD_" << mpdid << " APV_" << apvid
					<< " is declared in the map, but cannot file in the pedestal file"
					<< std::endl;
		}
	}
	std::cout<<"[Test]"<<__func__<<" "<<__LINE__<<"This is a test"<<std::endl;
	pfile->Close();
	// end of loading the pedestal
//TH1F * test = new TH1F("","",100,0,100);
//test->
	int sigma_cut=cfg->GetSysCondfig().Analysis_cfg.ZeroSubtrCutSigma;
	std::map<int, std::vector<int>>mMapping=cfg->GetMapping().GetAPVmap();
	MPDRawParser *rawparser=new MPDRawParser();
	// moduleID,         dimension histo
	std::map<int,std::map<int,std::vector<TH1F *>>> GEMModuleHisto;
	GEMModuleHisto.clear();

	std::cout<<"[Test]"<<__func__<<" "<<__LINE__<<"This is a test"<<std::endl;
	Int_t EvtID=0;
	while(ReadBlock()){
		GEMModuleHisto.clear();
		for(auto moduleid:cfg->GetMapping().GetGEMModuleList()){
			std::cout<<"GEM Module ID: "<<moduleid<<std::endl;
			GEMModuleHisto[moduleid][0].push_back(new TH1F(Form("module%d_x_raw",moduleid),Form("module%d_x_raw",moduleid),1600,0,1600));
			GEMModuleHisto[moduleid][1].push_back(new TH1F(Form("module%d_y_raw",moduleid),Form("module%d_y_raw",moduleid),1600,0,1600));

			GEMModuleHisto[moduleid][0].push_back(new TH1F(Form("module%d_x_msub",moduleid),Form("module%d_x_msub",moduleid),1600,0,1600));
			GEMModuleHisto[moduleid][1].push_back(new TH1F(Form("module%d_y_msub",moduleid),Form("module%d_y_msub",moduleid),1600,0,1600));

			GEMModuleHisto[moduleid][0].push_back(new TH1F(Form("module%d_x_csub",moduleid),Form("module%d_x_csub",moduleid),1600,0,1600));
			GEMModuleHisto[moduleid][1].push_back(new TH1F(Form("module%d_y_csub",moduleid),Form("module%d_y_csub",moduleid),1600,0,1600));

			GEMModuleHisto[moduleid][0].push_back(new TH1F(Form("module%d_x_hit",moduleid),Form("module%d_x_hit",moduleid),1600,0,1600));
			GEMModuleHisto[moduleid][1].push_back(new TH1F(Form("module%d_y_hit",moduleid),Form("module%d_y_hit",moduleid),1600,0,1600));

		}
		if(EvtID<eventid) {
			EvtID++;
			continue;
		}
		Int_t nstrips=0;
		rawparser->LoadRawData(block_vec_mpd);
		const std::map<int,std::vector<int>> raw_data=rawparser->GetDecoded();//       uid,          // the 6 time sample data 744
		for(auto  apv = raw_data.begin();apv!=raw_data.end();apv++){
			int uid=apv->first;
			if (mPedestal_mean.find(uid) == mPedestal_mean.end()) {
				continue;
				std::cout << "[Warning]: EvtID: " << EvtID << "  MPD:"
						<< GEM::getMPDID(uid) << " adc" << GEM::getADCID(uid)
						<< "   detected in the data, but not in the mapping, skip it"
						<< std::endl;
			}  // skip the apv that not defined in the map file
			std::vector<int> adc_temp=apv->second;

			int totalsize=adc_temp.size();
			if (totalsize % 129 != 0) {
				std::cout << "[WORNING]: " << __FUNCTION__ << "< " << __LINE__
						<< "> the size of the raw data is not expexted!!"
						<< std::endl;
				continue;
			}
			int TSsize = totalsize / 129;
//			std::cout<<"This is a test at" <<__FUNCTION__<<" <"<<__LINE__<<">"<<std::endl;
			for(int channel=0;channel<128;channel++){
				int RstripPos = channel;
				int RstripNb = ChNb[channel];
				RstripNb = RstripNb + (127 - 2 * RstripNb) * (mMapping[apv->first].at(3));
				RstripPos = RstripNb + 128 * (mMapping[apv->first].at(2));
				int dimension = mMapping[apv->first].at(1);    // x/y dimension
				int planeID = mMapping[apv->first].at(0);		// GEM Module ID
				int adcsum_temp=0;
				for (int ts_counter = 0; ts_counter < TSsize; ts_counter++) {
					adcsum_temp = adcsum_temp+ adc_temp[channel + 129 * ts_counter];
				}
				adcsum_temp=adcsum_temp/TSsize;
				GEMModuleHisto[planeID][dimension].at(0)->Fill(RstripPos,adcsum_temp);
//				std::cout<<planeID<<" dimension"<<dimension<<"  file"<< RstripPos<<std::endl;
			}
//			std::cout<<"This is a test at" <<__FUNCTION__<<" <"<<__LINE__<<">"<<std::endl;
			// subtract the common mode
			for (int counter = 0; counter < totalsize; counter++) {
				if ((counter % 129) != 128)
					adc_temp[counter] = adc_temp[counter]
							- mPedestal_mean[apv->first + (counter % 129)];
			}

			for(int channel=0;channel<128;channel++){
				int RstripPos = channel;
				int RstripNb = ChNb[channel];
				RstripNb = RstripNb + (127 - 2 * RstripNb) * (mMapping[apv->first].at(3));
				RstripPos = RstripNb + 128 * (mMapping[apv->first].at(2));
				int dimension = mMapping[apv->first].at(1);    // x/y dimension
				int planeID = mMapping[apv->first].at(0);		// GEM Module ID
				int adcsum_temp=0;
				for (int ts_counter = 0; ts_counter < TSsize; ts_counter++) {
					adcsum_temp = adcsum_temp+ adc_temp[channel + 129 * ts_counter];
				}
				adcsum_temp=adcsum_temp/TSsize;
				GEMModuleHisto[planeID][dimension].at(1)->Fill(RstripPos,adcsum_temp);
			}
//			std::cout<<"This is a test at" <<__FUNCTION__<<" <"<<__LINE__<<">"<<std::endl;
			int commonMode[TSsize];
			for(uint16_t ts_counter=0;ts_counter<TSsize;ts_counter++){
				commonMode[ts_counter]=0;
				std::vector<int> singleTSadc_temp_sorting;
				singleTSadc_temp_sorting.insert(singleTSadc_temp_sorting.end(),&adc_temp[129*ts_counter],&adc_temp[129*(ts_counter+1)]);
				std::sort(singleTSadc_temp_sorting.begin(),singleTSadc_temp_sorting.end()-1);
				for(int k =28; k < 100; k ++){
					commonMode[ts_counter]+=singleTSadc_temp_sorting[k];
				}
				commonMode[ts_counter]=commonMode[ts_counter]/72;
			}

			// loop on one apv card
			for(int channel=0;channel<128;channel++){
				int adcsum_temp=0;
				for(int ts_counter=0;ts_counter<TSsize; ts_counter++){
					adcsum_temp=adcsum_temp+adc_temp[channel+129*ts_counter]-commonMode[ts_counter];
				}
				// write to histogram
				adcsum_temp=adcsum_temp/TSsize;
				// calculate the address
				int RstripPos=channel;
				int RstripNb=ChNb[channel];
				RstripNb=RstripNb+(127-2*RstripNb)*(mMapping[apv->first].at(3));
				RstripPos=RstripNb+128*(mMapping[apv->first].at(2));
				int dimension = mMapping[apv->first].at(1);      //x/y dimension
				int planeID=mMapping[apv->first].at(0);			// GEM Module ID

				GEMModuleHisto[planeID][dimension].at(2)->Fill(RstripPos,adcsum_temp);
				if( adcsum_temp > (5*mPedestal_rms[apv->first+channel]) ){
					GEMModuleHisto[planeID][dimension].at(3)->Fill(RstripPos,adcsum_temp);
				}
			}
		}
		if(EvtID==eventid)break;
		EvtID++;
	}
   GUICanvasDataStream *stream = new GUICanvasDataStream();
   stream->LoadData(GEMModuleHisto);
   std::cout<<"This is a test at" <<__FUNCTION__<<" <"<<__LINE__<<">"<<std::endl;
   CanvasTabDisplay(stream);

}

void MPDDecoder::HiModeTest(std::string pedestalfname,std::string savefname){
	// load the pedestals
	std::map<int,int> mPedestal_mean;// uid , value
	std::map<int,int> mPedestal_rms;
	TFile *pfile=new TFile(pedestalfname.c_str(),"READ");
	if(!pfile->IsOpen()){
		std::cout<<__FUNCTION__<<"<"<<__LINE__<<"> [ERROR]: Pedestal file cannot open: "<<pedestalfname.c_str()<<std::endl;
		return;
	}
	for(auto apvlist : (gemConfig->GetMapping().GetAPVList())){
		int crateid=GEM::getCrateID(apvlist);
		int mpdid=GEM::getMPDID(apvlist);
		int apvid=GEM::getADCID(apvlist);

		std::string hmean_str=Form("PedestalMean(offset)_mpd_%d_ch_%d",mpdid, apvid);
		std::string hRMS_str=Form("PedestalRMS_mpd_%d_ch_%d",mpdid, apvid);

		if(pfile->GetListOfKeys()->Contains(hmean_str.c_str())&&(pfile->GetListOfKeys()->Contains(hRMS_str.c_str()))){
			TH1F *hmean=(TH1F *) pfile->Get(hmean_str.c_str());
			TH1F *hRMS=(TH1F *) pfile->Get(hRMS_str.c_str());
			for(int i=0;i<128;i++){
				int uid=GEM::GetUID(crateid,mpdid,apvid,i);
				mPedestal_mean[uid]=(hmean->GetBinContent(i+1));
				mPedestal_rms[uid]=(hRMS->GetBinContent(i+1));
			}
		}else{
			std::cout<< "[WORNING] " << __FUNCTION__ << " " << __LINE__
					<< "MPD_" << mpdid << " APV_" << apvid
					<< " is declared in the map, but cannot file in the pedestal file"
					<< std::endl;
		}
	}
	std::cout<<"Reading Pedestal Done!"<<std::endl;
	unsigned int NTSample=gemConfig->GetSysCondfig().DAQ_cfg.NSample;   // read how many time samples
	// finish reading the pedestals
	// TODO change the code to more flexiable time sample compatible
	Benchmark *timer=new Benchmark();
	TFile *HitFileio=new TFile(savefname.c_str(),"RECREATE");
	TTree *Hit = new TTree("GEMHit","Hit list");   	//create the tree

	Int_t EvtID=0;
	Int_t nch=0;
	Int_t *Vstrip;
	Int_t *VdetID;
	Int_t *VplaneID;
	Int_t *adc0;
	Int_t *adc1;
	Int_t *adc2;
	Int_t *adc3;
	Int_t *adc4;
	Int_t *adc5;
	Int_t *adc[NTSample];

	Vstrip   = new Int_t[20000];  // strip ID
	VdetID   = new Int_t[20000];
	VplaneID = new Int_t[20000];
	adc0     = new Int_t[20000];
	adc1     = new Int_t[20000];
	adc2     = new Int_t[20000];
	adc3     = new Int_t[20000];
	adc4     = new Int_t[20000];
	adc5     = new Int_t[20000];

	Hit->Branch("evtID",&EvtID,"evtID/I");
	Hit->Branch("nch",&nch,"nch/I");
	Hit->Branch("strip",Vstrip,"strip[nch]/I");
	Hit->Branch("detID",VdetID,"detID[nch]/I");
	Hit->Branch("planeID",VplaneID,"planeID[nch]/I");
	for(int tsid=0; tsid < NTSample ; tsid++){
		adc[tsid]= new Int_t[20000];
		Hit->Branch(Form("adc%d",tsid),adc[tsid],Form("adc%d[nch]/I",tsid));
	}
	//Create tree and buffer
	int sigma_cut=gemConfig->GetSysCondfig().Analysis_cfg.ZeroSubtrCutSigma;
	std::map<int, std::vector<int>>mMapping=gemConfig->GetMapping().GetAPVmap();
	MPDRawParser *rawparser=new MPDRawParser();
	while(ReadBlock()){
		// detID, plantid, stripid, sixtimesamples
		std::map<int,std::map<int,std::map<int,std::vector<int>>>> hit_data;
		if(EvtID%300)timer->Print(EvtID);
		rawparser->LoadRawData(block_vec_mpd);
		std::map<int,std::vector<int>> raw_data=rawparser->GetDecoded();
		for(auto  apv = raw_data.begin();apv!=raw_data.end();apv++){    // loop on all the apvs
			int uid=apv->first;
			if(mPedestal_mean.find(uid)==mPedestal_mean.end()) {
				continue;
			}
			std::vector<int> adc_temp=apv->second;
			int totalsize=adc_temp.size();
			int TSsize=totalsize/129;
			if((totalsize%129!=0)||(TSsize!=NTSample)){
				std::cout<<"[ERROR] "<<__FUNCTION__<<" <"<<__LINE__<<">"<<" This size is not expected"<<std::endl;
			}

#ifdef __DECODER_DEBUG_MODE
			std::cout<<"EvtID: "<<EvtID<<"  Size of raw data:" << adc_temp.size()<<" expected is : "<< 129*6<<std::endl;
#endif

			// subtract the common mode
			for(int counter=0; counter<totalsize; counter++){
				if((counter%129)!=128) adc_temp[counter]=adc_temp[counter]-mPedestal_mean[apv->first+(counter%129)];
			}

			int commonMode[TSsize];
			for(uint16_t ts_counter=0;ts_counter<TSsize;ts_counter++){
				commonMode[ts_counter]=0;
				std::vector<int> singleTSadc_temp_sorting;
				singleTSadc_temp_sorting.insert(singleTSadc_temp_sorting.end(),&adc_temp[129*ts_counter],&adc_temp[129*(ts_counter+1)]);
				std::sort(singleTSadc_temp_sorting.begin(),singleTSadc_temp_sorting.end()-1);
				for(int k =28; k < 100; k ++){
					commonMode[ts_counter]+=singleTSadc_temp_sorting[k];
				}
				commonMode[ts_counter]=commonMode[ts_counter]/72;
			}
			// loop on one apv card
			for(int channel=0;channel<128;channel++){
				int adcsum_temp=0;
				for(int ts_counter=0;ts_counter<TSsize; ts_counter++){
					adcsum_temp=adcsum_temp+adc_temp[channel+129*ts_counter]-commonMode[ts_counter];
				}
				adcsum_temp=adcsum_temp/TSsize;
				if( adcsum_temp >= (5*mPedestal_rms[apv->first+channel]) ){
					int RstripPos;
					int RstripNb=ChNb[channel];
					RstripNb=RstripNb+(127-2*RstripNb)*(mMapping[apv->first].at(3)); // invert
					RstripPos=RstripNb+128*(mMapping[apv->first].at(2));			 // calculate position
					for(int ts_id=0;ts_id<TSsize;ts_id++){
						hit_data[mMapping[apv->first].at(0)][mMapping[apv->first].at(1)][RstripPos].push_back(adc_temp[channel+ts_id*129]-commonMode[ts_id]);
					}
				}
			}
		}
		Int_t nstrips=0;
		for(auto VdetID_iter = hit_data.begin();VdetID_iter!=hit_data.end();VdetID_iter++){
			for(auto VplaneID_iter=VdetID_iter->second.begin();VplaneID_iter!=VdetID_iter->second.end();VplaneID_iter++){
				for(auto Vstrip_iter=VplaneID_iter->second.begin();Vstrip_iter!=VplaneID_iter->second.end();Vstrip_iter++){
					Vstrip[nstrips]=Vstrip_iter->first;
					VdetID[nstrips]=VdetID_iter->first;
					VplaneID[nstrips]=VplaneID_iter->first;
					for(int i = 0; i < (Vstrip_iter->second.size());i++){
						adc[i][nstrips]=(Vstrip_iter->second).at(i);
					}
					nstrips++;
				}
			}
		}

		nch=nstrips;
		EvtID++;
		if(nch!=0) Hit->Fill();
#ifdef __DECODER_DEBUG_MODE
			std::cout<<"EvtID: "<<EvtID<<" End of process this event, ready to process next one "<<std::endl;
#endif
	}

	Hit->SetDirectory(HitFileio);
	Hit->Write();
	HitFileio->Write();
	HitFileio->Close();
}


//!
//!
void MPDDecoder::HitMode(std::string pedestalfname,std::string savefname){
	// uid , value
	std::unordered_map<int,int> mPedestal_mean;
	std::unordered_map<int,int> mPedestal_rms;
	TFile *pfile=new TFile(pedestalfname.c_str(),"READ");
	if(!pfile->IsOpen()){
		std::cout<<__FUNCTION__<<"<"<<__LINE__<<"> [ERROR]: Pedestal file cannot open: "<<pedestalfname.c_str()<<std::endl;
		return;
	}
	GEMConfigure *cfg= GEMConfigure::GetInstance();
	for(auto apvlist : (cfg->GetMapping().GetAPVList())){
		int crateid=GEM::getCrateID(apvlist);
		int mpdid=GEM::getMPDID(apvlist);
		int apvid=GEM::getADCID(apvlist);
		std::string hmean_str=Form("PedestalMean(offset)_mpd_%d_ch_%d",mpdid, apvid);
		std::string hRMS_str=Form("PedestalRMS_mpd_%d_ch_%d",mpdid, apvid);
		if(pfile->GetListOfKeys()->Contains(hmean_str.c_str())&&(pfile->GetListOfKeys()->Contains(hRMS_str.c_str()))){
			TH1F *hmean=(TH1F *) pfile->Get(hmean_str.c_str());
			TH1F *hRMS=(TH1F *) pfile->Get(hRMS_str.c_str());
			for(int i=0;i<128;i++){
				int uid=GEM::GetUID(crateid,mpdid,apvid,i);
				mPedestal_mean[uid]=(hmean->GetBinContent(i+1));
				mPedestal_rms[uid]=(hRMS->GetBinContent(i+1));
			}
		}else{
			std::cout<< "[WORNING] " << __FUNCTION__ << " " << __LINE__
					<< "MPD_" << mpdid << " APV_" << apvid
					<< " is declared in the map, but cannot file in the pedestal file"
					<< std::endl;
		}
	}
	// finish reading the pedestals

	Benchmark *timer=new Benchmark();
	TFile *HitFileio=new TFile(savefname.c_str(),"RECREATE");
	//create the tree
	TTree *Hit = new TTree("GEMHit","Hit list");
	Int_t EvtID=0;
	Int_t nch=0,*Vstrip,*VdetID,*VplaneID,*adc0,*adc1,*adc2,*adc3,*adc4,*adc5;//GEM branch

	Vstrip   = new Int_t[20000];  // strip ID
	VdetID   = new Int_t[20000];
	VplaneID = new Int_t[20000];
	adc0     = new Int_t[20000];
	adc1     = new Int_t[20000];
	adc2     = new Int_t[20000];
	adc3     = new Int_t[20000];
	adc4     = new Int_t[20000];
	adc5     = new Int_t[20000];
	Hit->Branch("evtID",&EvtID,"evtID/I");
	Hit->Branch("nch",&nch,"nch/I");
	Hit->Branch("strip",Vstrip,"strip[nch]/I");
	Hit->Branch("detID",VdetID,"detID[nch]/I");
	Hit->Branch("planeID",VplaneID,"planeID[nch]/I");
	Hit->Branch("adc0",adc0,"adc0[nch]/I");
	Hit->Branch("adc1",adc1,"adc1[nch]/I");
	Hit->Branch("adc2",adc2,"adc2[nch]/I");
	Hit->Branch("adc3",adc3,"adc3[nch]/I");
	Hit->Branch("adc4",adc4,"adc4[nch]/I");
	Hit->Branch("adc5",adc5,"adc5[nch]/I");

	//GEMConfigure *gemcfg=GEMConfigure::GetInstance();
	//Create tree and buffer
	int sigma_cut=cfg->GetSysCondfig().Analysis_cfg.ZeroSubtrCutSigma;
	std::map<int, std::vector<int>>mMapping=cfg->GetMapping().GetAPVmap();
	MPDRawParser *rawparser=new MPDRawParser();
	while(ReadBlock()){
		Int_t nstrips=0;
		if(EvtID%300)timer->Print(EvtID);
		rawparser->LoadRawData(block_vec_mpd);
		std::map<int,std::vector<int>> raw_data=rawparser->GetDecoded();

		for(auto  apv = raw_data.begin();apv!=raw_data.end();apv++){
			int uid=apv->first;
			// loop on all the apvs, if this apv is not included in the mapping, then skip it
			if(mPedestal_mean.find(uid)==mPedestal_mean.end()) {
				continue;
#ifdef __DECODER_DEBUG_MODE
			std::cout<<"[Warning]: EvtID: "<<EvtID<<"  MPD:" <<GEM::getMPDID(uid) <<" adc"<<GEM::getADCID(uid)<<"   detected in the data, but not in the mapping, skip it"<<std::endl;
#endif
			}
			std::vector<int> adc_temp=apv->second;
			int totalsize=adc_temp.size();
			int TSsize=totalsize/129;

#ifdef __DECODER_DEBUG_MODE
			std::cout<<"EvtID: "<<EvtID<<"  Size of raw data:" << adc_temp.size()<<" expected is : "<< 129*6<<std::endl;
#endif
			// subtract the common mode
			for(int counter=0; counter<totalsize; counter++){
				if((counter%129)!=128) adc_temp[counter]=adc_temp[counter]-mPedestal_mean[apv->first+(counter%129)];
			}

			int commonMode[TSsize];
			for(uint16_t ts_counter=0;ts_counter<TSsize;ts_counter++){
				commonMode[ts_counter]=0;
				std::vector<int> singleTSadc_temp_sorting;
				singleTSadc_temp_sorting.insert(singleTSadc_temp_sorting.end(),&adc_temp[129*ts_counter],&adc_temp[129*(ts_counter+1)]);
				std::sort(singleTSadc_temp_sorting.begin(),singleTSadc_temp_sorting.end()-1);
				for(int k =28; k < 100; k ++){
					commonMode[ts_counter]+=singleTSadc_temp_sorting[k];
				}
				commonMode[ts_counter]=commonMode[ts_counter]/72;
			}

			// loop on one apv card
			for(int channel=0;channel<128;channel++){
				int adcsum_temp=0;
				for(int ts_counter=0;ts_counter<TSsize; ts_counter++){
					adcsum_temp=adcsum_temp+adc_temp[channel+129*ts_counter]-commonMode[ts_counter];
				}

				adcsum_temp=adcsum_temp/TSsize;
				if( adcsum_temp > (5*mPedestal_rms[apv->first+channel]) ){
					int RstripPos=channel;
					int RstripNb=ChNb[channel];
					RstripNb=RstripNb+(127-2*RstripNb)*(mMapping[apv->first].at(3)); // invert
					RstripPos=RstripNb+128*(mMapping[apv->first].at(2));			 // calculate position
					// write the six time samples
					// temprarily usage need to put into a 2-d array
					Vstrip[nstrips]=RstripPos;
					VdetID[nstrips]=mMapping[apv->first].at(0);
					VplaneID[nstrips]=mMapping[apv->first].at(1);
					adc0[nstrips]=(adc_temp[channel]-commonMode[0]);
					adc1[nstrips]=adc_temp[channel+129]-commonMode[1];
					adc2[nstrips]=adc_temp[channel+129*2]-commonMode[2];
					adc3[nstrips]=adc_temp[channel+129*3]-commonMode[3];
					adc4[nstrips]=adc_temp[channel+129*4]-commonMode[4];
					adc5[nstrips]=adc_temp[channel+129*5]-commonMode[5];
					nstrips++;

				}
			}
		}
		nch=nstrips;
		EvtID++;
		Hit->Fill();
#ifdef __DECODER_DEBUG_MODE
			std::cout<<"EvtID: "<<EvtID<<" End of process this event, ready to process next one "<<std::endl;
#endif
	}

	Hit->SetDirectory(HitFileio);
	Hit->Write();
	HitFileio->Write();
	HitFileio->Close();
}



bool MPDDecoder::ReadBlock(){
	clear(); // start a new event
	if(chan->read()){
		evio::evioDOMTree event(chan);
		evio::evioDOMNodeListP mpdEventList = event.getNodeList( evio::isLeaf() );
		for(evio::evioDOMNodeList::iterator iter = mpdEventList->begin();
				iter != mpdEventList->end(); iter++){
			int banktag=(*iter)->tag;
			vector<uint32_t> *block_vec = (*iter)->getVector<uint32_t>();
			switch (banktag) {
				case GEM::MPD_tag:
					unsigned int iend;
					for(iend =0; iend< block_vec->size();iend++){
						uint32_t tag=((*block_vec).at(iend)>>24)&0xf8;
						if(tag==0x90||tag==0x88)break;
					}
					block_vec_mpd.insert(block_vec_mpd.begin(),block_vec->begin(),block_vec->begin()+iend);
					break;
				default:
					break;
			}
		}
		return true;
	}else{
		return false;
	}
}

void MPDDecoder::clear(){
	block_vec_mpd.clear();
}

MPDDecoder::~MPDDecoder() {
	// TODO Auto-generated destructor stub
}

