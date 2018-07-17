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


#ifdef __MULT_THREAD
#include "thread"
#endif

MPDDecoder::MPDDecoder() {
	// TODO Auto-generated constructor stub

}

MPDDecoder::MPDDecoder(std::string fname){
	LoadFile(fname.c_str());
}

void MPDDecoder::LoadFile(std::string fname){
	chan=new evio::evioFileChannel(fname.c_str(),"r");
	chan->open();
}

void MPDDecoder::PedestalMode(std::string savefname){
	std::map<int,TH1F *> Pedestal_temp;
	MPDRawParser *rawparser=new MPDRawParser();
	int i =0;
	while(ReadBlock()){
		rawparser->LoadRawData(block_vec_mpd.begin(),block_vec_mpd.end());
		std::map<int,std::vector<int>> data =rawparser->GetCommonModeSubtraction(); // get  the common mode subtracted data
		// calculate the mean of the six time sample
		for(auto iter= data.begin();iter!=data.end();iter++){
			if(Pedestal_temp.find(iter->first)==Pedestal_temp.end()){
				Pedestal_temp[iter->first] = new TH1F(Form("%d",iter->first),Form("%d",iter->first),9000,-4500,4500);
			}
			// the mean of all the time samples
			Pedestal_temp[iter->first]->Fill((std::accumulate(iter->second.begin(),iter->second.end(),0)/(iter->second.size())));
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
	std::map<int,int> mPedestal_mean;
	std::map<int,int> mPedestal_rms;

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
	while(ReadBlock()){

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
						data[uid].at(i)-mPedestal_mean[uid];
#ifdef __DECODER_DEBUG_MODE
						std::cout<<"(pedestal)  =  "<<data[uid].at(i)<<std::endl;
#endif
					}

					// apply five sigma cut
					if (std::accumulate(data[uid].begin(), data[uid].end(), 0)
							/ (data[uid].size())
							> (cfg->GetSysCondfig().Analysis_cfg.ZeroSubtrCutSigma)
									* mPedestal_rms[uid]) {
						int RstripNb=ChNb[channel];
						// calculate the invert channels
						RstripNb=RstripNb+(127-2*RstripNb)*(gemcfg->GetMapping().GetAPVmap()[apvuid].at(3));
						int RstripPos=RstripNb+128*(gemcfg->GetMapping().GetAPVmap()[apvuid].at(2));

						Vstrip[nstrips]=RstripPos;
#ifdef __DECODER_DEBUG_MODE
						std::cout<<__FUNCTION__<<"<"<<__LINE__<<">  time sample(6 by default) :"<<data[uid]>size()<<std::endl;
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
						<< "MPD_" << GEM::getMPDID(apv) << " APV_" << GEM::getADCID(apv)
						<< " is declared in the map, but cannot file in the raw data file !!!!"
						<< std::endl;
			}
		}
		nch=nstrips;
		Hit->Write();
	}
	HitFileio->Write();
	HitFileio->Class();
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
					int iend;
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

