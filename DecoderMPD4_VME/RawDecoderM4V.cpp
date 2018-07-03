/*
 * RawDecoderM4V.cpp
 *
 *  Created on: May 24, 2018
 *      Author: newdriver
 */

#include "RawDecoderM4V.h"
#include <arpa/inet.h>
#include <assert.h>
#include <utility>


#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>

#include <TH1F.h>
#include <TCanvas.h>
RawDecoderM4V::RawDecoderM4V(std::vector<uint32_t> &buffer, int start, int end) {
	// TODO Auto-generated constructor stub
	mAPVRawData.clear();
	mAPVRawHisto.clear();

	fBuf=end-start;
	buf = new unsigned int [fBuf];
	int bufp=0;
	for(int i =start; i < end;i++){
		buf[bufp]=buffer[i];
		bufp++;
	}
	if(bufp!=fBuf){std::cout<<"vector passed and vector doesnt match"<<std::endl;}
	if(fBuf <= 0)
	    {
	      std::cout<<"empty vector passed in..."<<std::endl;
	      return;
	    }
	Decoder();
}
 void RawDecoderM4V::Decoder(){
	unsigned int word32bit;

	int mpdid;
	int adc_ch;
	int hybridID;

	//map<int, vector<int> > mpd_event;
	std::vector<int> apv_event;
	std::vector<int> apv_margin;
	std::vector<int> mpd_margin;

	mpd_margin.clear();
	apv_margin.clear();

	//find MPD margin, find apv margin

#define BLOCK_HEADER    0x0	// {3'h0, MODULE_ID, EVENT_PER_BLOCK, BLOCK_CNT[7:0]}
#define BLOCK_TRAILER   0x1	// {3'h1, 1'b0, BlockWordCounter}
#define EVENT_HEADER    0x2	// {3'h2, 1'b0, EventCounterFifo_Data}
#define TRIGGER_TIME   0x3	// {3'h3, 1'b0, TimeCounterFifo_Data[39:20]}
	//#define TRIGGER_TIME2   0x3	// {3'h3, 1'b1, TimeCounterFifo_Data[19:0]}
#define APV_CH_DATA     0x4	// {3'h4, ChannelData[20:0]}
#define EVENT_TRAILER   0x5	// {3'h5, 1'b0, LoopDataCounter[11:0], TRIGGER_TIME_FIFO}
#define DATA_NOT_VALID  0x6	// {3'h6, 21'b0}
#define FILLER_WORD     0x7	// {3'h7, 21'b0}
	//find apv margin
	for(int i =0; i < fBuf ; i ++){
		uint32_t data = buf[i];
		uint32_t header;
		uint32_t apv_header;
		header = (data & 0x00e00000) >> 21;
		switch (header) {
		case BLOCK_HEADER:
			mpdid=(data&0x001F0000) >> 16;
			break;
		case APV_CH_DATA:
			switch((data& 0x00180000)>>19){
			case 0: //apv header  -------  {1'b0, 1'b0, 1'b0, MEAN[11], DATA_IN[12:0], CH_ID[3:0]};
				adc_ch = (data & 0xf);
				//hybridID = (mpdid << 12) | (adc_ch << 8);
				//   cout<<"adc_ch: "<<adc_ch<<endl;
				break;
			case 1: //data  -------  {1'b0, 1'b1, THRESHOLD_ADDRESS[6:0], data_minus_baseline[11:0]};
				mAPVRawData[mpdid][adc_ch].push_back(data & 0x00000fff);
				//mAPVRawSingleEvent[hybridID].push_back(data & 0x00000fff);
				break;
			case 2: //apv trailer  -------  {1'b1, 1'b0, 2'b0, MODULE_ID[4:0], DATA_IN[11:0]};
					//DATA_IN[11:0] = {ApvSampleCounterMinusOne[3:0], frame_counter[7:0]};
				mAPVRawData[mpdid][adc_ch].push_back((data & 0xf00) >> 8);
				//mAPVRawSingleEvent[hybridID].push_back((data & 0xf00) >> 8);
				//	    cout<<" sample count: "<<((data&0xf00)>>8)<<endl;
				break;
			case 3: //Trailer  -------  {1'b1, 1'b1, MEAN[10:0], word_count[7:0]};
				break;
			default:
				break;
			}
		}
	}

	for(auto iter_mpd=mAPVRawData.begin();
			iter_mpd!=mAPVRawData.end();iter_mpd++){
		for(auto iter_adc = iter_mpd->second.begin();
				iter_adc!=iter_mpd->second.end();iter_adc++){
			TH1F* h = new TH1F(Form("mpd_%d_ch_%d",iter_mpd->first, iter_adc->first), Form("mpd_%d_ch_%d_raw_data",iter_mpd->first, iter_adc->first), 780, 0, 779);
			int counter=1;
			(iter_adc->second.shrink_to_fit());
			for(auto chennal : iter_adc->second){
				h->Fill(counter++,chennal);
			}
			mAPVRawHisto[iter_mpd->first][iter_adc->first]=h;
			delete h;
		}
	}
 }


 //      MPD          APV             strip  time samples
std::map<int,std::map<int,std::map<int,std::vector<int>>>> RawDecoderM4V::GetStripTsAdcMap(){
	std::map<int,std::map<int,std::map<int,std::vector<int>>>> data_return;
	std::map<int,std::map<int,std::map<int,std::vector<int>>>> seperated=SeperateSamples(mAPVRawData);

	//common mode subtraction algorithm
	for(auto iter_mpd=seperated.begin();iter_mpd!=seperated.end();iter_mpd++){
		for(auto iter_apv=iter_mpd->second.begin();iter_apv!=iter_mpd->second.end();iter_apv++){
			for(auto iter_ts=iter_apv->second.begin();iter_ts!=iter_apv->second.end();iter_ts++){
				std::vector<int> frame_sort;
				frame_sort.insert(frame_sort.begin(),iter_ts->second.begin(),(iter_ts->second.end()--));
				std::sort(frame_sort.begin(),frame_sort.end());
				int commonmode=0;
				for(int k = 28 ; k < 100; k ++){
					commonmode+=frame_sort[k];
				}
				commonmode=commonmode/72;
				int counter=0;
				for(auto element : iter_ts->second){
					element=element-commonmode;
					//std::cout<<__FUNCTION__<<__LINE__<<"  Check FUNCTION "<<element<<std::endl;
					data_return[iter_mpd->first][iter_apv->first][counter].push_back(element);
					counter++;
				}
			}
		}
	}
	return data_return;
 }

//!         MPD          APV       129*timesamples
//!std::map<int, std::map<int, std::vector<int>>>
//!
//!
std::map<int,std::map<int,std::map<int,std::vector<int>>>> RawDecoderM4V::SeperateSamples(std::map<int, std::map<int, std::vector<int>>> data){
	std::map<int,std::map<int,std::map<int,std::vector<int>>>>data_return;
	for(auto iter_mpd=data.begin();iter_mpd!=data.end();iter_mpd++){
		for(auto iter_apv=iter_mpd->second.begin();
				iter_apv!=iter_mpd->second.end();iter_apv++){
			data_return[iter_mpd->first][iter_apv->first]=SeperateSamples(iter_apv->second);
		}
	}
	return data_return;
}

std::map<int,std::vector<int>> RawDecoderM4V::SeperateSamples(std::vector<int> data){
	std::map<int,std::vector<int>> data_return;
	if(data.size()%129!=0){
		std::cerr<<__FUNCTION__<<" The Size of the data is not expected!!"<<std::endl;
		exit(EXIT_FAILURE);
	}

	for(int tsid=0;tsid<(data.size()/129);tsid++){
		data_return[tsid].insert(data_return[tsid].begin(),&data[129*tsid],&data[129*(tsid+1)]);
	}
	return data_return;
}



RawDecoderM4V::~RawDecoderM4V() {
	// TODO Auto-generated destructor stub
}

