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
#include <boost/filesystem.hpp>
#ifdef __MULT_THREAD
#include "thread"
#endif

MPDDecoder::MPDDecoder() : GUIInforCenter(){
	// TODO Auto-generated constructor stub

}

MPDDecoder::MPDDecoder(std::string fname){
	LoadFile(fname.c_str());
}

void MPDDecoder::LoadFile(std::string fname){
	rawfilename=fname;
	chan=new evio::evioFileChannel(fname.c_str(),"r");
	chan->open();
}

void MPDDecoder::RawDisplay(uint evtid) {
	if (!boost::filesystem::exists(
			Form(".data/%s.root", rawfilename.c_str()))) {

		// looking for the initial data value
		GEMConfigure *cfg = GEMConfigure::GetInstance();
		uint64_t mpdnumber = cfg->GetMapping().GetMPDNameList().size();
		int_fast64_t evid=0;
		int_fast64_t nch;
		int_fast64_t mpdid[mpdnumber];
		int_fast16_t value[mpdnumber][800];
		TFile *HitFileio=new TFile(Form(".data/%s.root", rawfilename.c_str()),"RECREATE");
		//create the tree
		TTree *Hit = new TTree("GEMRaw","raw Hit list");
		Hit->Branch("evtID",&evtid,"evtID/I");
		Hit->Branch("nch",&nch,"nch/I");
		for(int i = 0 ; i < mpdnumber ; i ++){
			Hit->Branch(Form("mpd%d",i),&(mpdid[i]),Form("mpd%d/I",i));
			Hit->Branch(Form("data%d",i),value[i],Form("data[%d][nch]",i));
		}
		MPDRawParser *rawparser = new MPDRawParser();
		while (ReadBlock()) {
			rawparser->LoadRawData(block_vec_mpd);
			rawparser->GetDecoded();
		}

	}
}
void MPDDecoder::PedestalMode(std::string savefname){
	std::unordered_map<int,TH1F *> Pedestal_temp;
	MPDRawParser *rawparser=new MPDRawParser();
	int i =0;
	Benchmark *timer=new Benchmark();
	while(ReadBlock()){
		if((i++)%100==0)timer->Print(i);
		//rawparser->LoadRawData(block_vec_mpd.begin(),block_vec_mpd.end());
		rawparser->LoadRawData(block_vec_mpd);
		auto data =rawparser->GetCommonModeSubtraction(); // get  the common mode subtracted data
		// calculate the mean of the six time sample
		for(auto iter= data.begin();iter!=data.end();iter++){
			if(Pedestal_temp.find(iter->first)==Pedestal_temp.end()){
				Pedestal_temp[iter->first] = new TH1F(Form("%d",iter->first),Form("%d",iter->first),5000,-1000,4000);
				//Pedestal_temp[iter->first]->GetYaxis()->SetRange(-500,2000);
			}
			// the mean of all the time samples
			Pedestal_temp[iter->first]->Fill((std::accumulate(iter->second.begin(),iter->second.end(),0.0)/(iter->second.size())));
		}
	}
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
		}
		pedestal_mean[uid]->Fill(channelid+1,iter->second->GetMean());
		pedestal_rms[uid]->Fill(channelid+1,iter->second->GetRMS());
	}
// save the histogram into root file
	TFile *file=new TFile(savefname.c_str(),"RECREATE");
	for(auto iter = pedestal_mean.begin();iter!=pedestal_mean.end();iter++){
		pedestal_mean[iter->first]->SetDirectory(file);
		pedestal_rms[iter->first]->SetDirectory(file);
	}
	file->Write();
	file->Close();
}



void MPDDecoder::HitMode(std::string pedestalfname,std::string savefname){
	// uid , value
	std::unordered_map<int,int> mPedestal_mean;
	std::unordered_map<int,int> mPedestal_rms;

	// the hit mode  process
	// get all the apv list
	TFile *pfile=new TFile(pedestalfname.c_str(),"READ");
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
				std::cout<<"channel :"<<i<<" Mean :"<<hmean->GetBinContent(i+1)<<"  rms: "<<hRMS->GetBinContent(i+1)<<std::endl;
			}
		}else{
			std::cout<< "[WORNING] " << __FUNCTION__ << " " << __LINE__
					<< "MPD_" << mpdid << " APV_" << apvid
					<< " is declared in the map, but cannot file in the pedestal file"
					<< std::endl;
		}
	}



	TFile *HitFileio=new TFile(savefname.c_str(),"RECREATE");
	//create the tree
	TTree *Hit = new TTree("GEMHit","Hit list");
	Hit->SetDirectory(HitFileio);
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
	GEMConfigure *gemcfg=GEMConfigure::GetInstance();
	//Create tree and buffer
	MPDRawParser *rawparser=new MPDRawParser();
	while(ReadBlock()&&(EvtID<300)){

		// start a new event
		// initialize
		Int_t nstrips=0;
		EvtID++;
		rawparser->LoadRawData(block_vec_mpd.begin(),block_vec_mpd.end());
		std::map<int,std::vector<int>> data =rawparser->GetCommonModeSubtraction();

		// zero subtraction
		for(auto apv : (cfg->GetMapping().GetAPVList())){

			if(data.find(apv+1)!=data.end()){
				// zero subtraction
				int crateid=GEM::getCrateID(apv);
				int mpdid=GEM::getMPDID(apv);
				int adcid=GEM::getADCID(apv);
				int apvuid=GEM::GetUID(crateid,mpdid,adcid,0);
				for(int channel=0; channel<128; channel++){
					uint8_t tsnumber =data[GEM::GetUID(crateid,mpdid,adcid,channel)].size();
					int uid=GEM::GetUID(crateid,mpdid,adcid,channel);
					for (uint8_t i =0; i < tsnumber;i++){
#ifdef __DECODER_DEBUG_MODE
						std::cout<<__FUNCTION__<<"<"<<__LINE__<<"> before pedestal sub : "<< data[uid].at(i)<<"  - "<<mPedestal_mean[uid];
#endif
						data[uid].at(i)=data[uid].at(i)-mPedestal_mean[uid];
#ifdef __DECODER_DEBUG_MODE
						std::cout<<"(pedestal)  =  "<<data[uid].at(i)<<std::endl;
#endif
					}

					// apply five sigma cut
					if (std::accumulate(data[uid].begin(), data[uid].end(), 0.0)
							/ (data[uid].size())
							> (cfg->GetSysCondfig().Analysis_cfg.ZeroSubtrCutSigma)
									* mPedestal_rms[uid]) {
						int RstripNb=ChNb[channel];
						// calculate the invert channels
						RstripNb=RstripNb+(127-2*RstripNb)*(gemcfg->GetMapping().GetAPVmap()[apvuid].at(3));
						int RstripPos=RstripNb+128*(gemcfg->GetMapping().GetAPVmap()[apvuid].at(2));

						Vstrip[nstrips]=RstripPos;
#ifdef __DECODER_DEBUG_MODE
						std::cout<<__FUNCTION__<<"<"<<__LINE__<<">  time sample(6 by default) :"<<data[uid].size()<<std::endl;
#endif
						adc0[nstrips]=data[uid].at(0);
						adc1[nstrips]=data[uid].at(1);
						adc2[nstrips]=data[uid].at(2);
						adc3[nstrips]=data[uid].at(3);
						adc4[nstrips]=data[uid].at(4);
						adc5[nstrips]=data[uid].at(5);
						VdetID[nstrips]=gemcfg->GetMapping().GetAPVmap()[apvuid].at(0);
						VplaneID[nstrips]=gemcfg->GetMapping().GetAPVmap()[apvuid].at(1);
						nstrips++;
					}
				}
			}else{
				std::cout<< "[WORNING] " << __FUNCTION__ << " " << __LINE__
						<< " MPD_" << GEM::getMPDID(apv) << " APV_" << GEM::getADCID(apv)
						<< " is declared in the map, but cannot file in the raw data file !!!!"
						<< std::endl;
			}
		}
		nch=nstrips;
		if(nch!=0) Hit->Fill();
	}
	Hit->Write();
	HitFileio->Write();
	HitFileio->Class();
}



void MPDDecoder::HitMode(std::string pedestalfname,std::string savefname,std::string test_string){
	// uid , value
	std::unordered_map<int,int> mPedestal_mean;
	std::unordered_map<int,int> mPedestal_rms;

	// the hit mode  process
	// get all the apv list
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
	GEMConfigure *gemcfg=GEMConfigure::GetInstance();
	//Create tree and buffer
	int sigma_cut=gemcfg->GetSysCondfig().Analysis_cfg.ZeroSubtrCutSigma;
	std::map<int, std::vector<int>>mMapping=gemcfg->GetMapping().GetAPVmap();
	MPDRawParser *rawparser=new MPDRawParser();
	while(ReadBlock()){
		Int_t nstrips=0;
		EvtID++;
		if(EvtID%300)timer->Print(EvtID);
		rawparser->LoadRawData(block_vec_mpd);
		std::map<int,std::vector<int>> raw_data=rawparser->GetDecoded();

		for(auto  apv = raw_data.begin();apv!=raw_data.end();apv++){
			int uid=apv->first;
			std::vector<int> adc_temp=apv->second;
			int totalsize=adc_temp.size();
			int TSsize=totalsize/129;
#ifdef __DECODER_DEBUG_MODE
			std::cout<<"Size of raw data:" << adc_temp.size()<<" expected is : "<< 129*6<<std::endl;
#endif
			// subtract the common mode
			for(int counter=0; counter<totalsize; counter++){
				if((counter%129)!=128) adc_temp[counter]-=mPedestal_mean[apv->first+(counter%129)];
				//std::cout<<"mpd:"<<GEM::getMPDID(uid)<<" adc:"<<GEM::getADCID(uid)<<" Channel:"<<counter<<"  value:"<<adc_temp[counter]<<std::endl;
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
					RstripNb=RstripNb+(127-2*RstripNb)*mMapping[apv->first].at(3);
					RstripPos=RstripNb+128*mMapping[apv->first].at(2);
					nstrips++;
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
				}
			}
		}
		nch=nstrips;
		Hit->Fill();
	}

	Hit->SetDirectory(HitFileio);
	Hit->Write();
	HitFileio->Write();
	HitFileio->Close();
}

void MPDDecoder::HitDisplay(){

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

