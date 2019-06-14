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
#include <TH2F.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include "evioUtil.hxx"
#include "evioFileChannel.hxx"

#include "MPDStructure.h"
#include "MPDRawParser.h"
#include "GEMIDGenerator.h"

#include "unordered_map"
#include <math.h>
#include <algorithm>
#include <numeric>
#ifdef __MULT_THREAD
#include "thread"
#endif

MPDDecoder::MPDDecoder() {
	// TODO Auto-generated constructor stub
	Initialize();
}

MPDDecoder::MPDDecoder(std::string fname){
	LoadFile(fname.c_str());
	Initialize();
}


void MPDDecoder::Initialize(){
}

void MPDDecoder::LoadFile(std::string fname){

	rawfilename=fname;

	chan=new evio::evioFileChannel(fname.c_str(),"r");
	chan->open();
}

void MPDDecoder::RawDisplay(uint id) {
}


void MPDDecoder::PedestalMode(std::string savefname){
	std::unordered_map<int,TH1F *> Pedestal_temp;
	MPDRawParser *rawparser=new MPDRawParser();
	int i =0;
	while(ReadBlock()){
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

