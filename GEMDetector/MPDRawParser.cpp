/*
 * MPDRawParser.cpp
 *
 *  Created on: Jul 5, 2018
 *      Author: newdriver
 */

#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>

#include "MPDRawParser.h"
#include "MPDStructure.h"
#include "GEMIDGenerator.h"

MPDRawParser::MPDRawParser() {
	// TODO Auto-generated constructor stub

}

MPDRawParser::~MPDRawParser() {
	// TODO Auto-generated destructor stub
}


//void MPDRawParser::GetDecoded(GEMEvent **gem,
//		const std::vector<uint32_t> &gemRaw) {
//
//}
//
//void MPDRawParser::GetDecoded(GEMEvent **gem,
//		std::vector<uint32_t>::iterator begin,
//		std::vector<uint32_t>::iterator end) {
//}
//
//
//void MPDRawParser::GetZeroSubtrData(GEMEvent *){
//
//}
//
//void MPDRawParser::GetCommonModeSubtrData(GEMEvent *){

//}

/*//template<class T>
//void MPDRawParser::LoadRawData(std::vector<T> begin,std::vector<T> end)
void MPDRawParser::LoadRawData(std::vector<uint32_t>::iterator begin,std::vector<uint32_t>::iterator end){

	  int mpdid;
	  int adc_ch;
	  int UID;
	  clear();
	  for(auto iter= begin;iter!=end;iter++){

		uint32_t data=*iter;
		uint32_t header;
		uint32_t apv_header;
		header = (data & 0x00e00000)>>21;
		switch(header){
		case GEM::BLOCK_HEADER:
			mpdid=(data&0x001F0000) >> 16;
			break;
		case GEM::EVENT_HEADER:
			break;
		case GEM::TRIGGER_TIME:
			break;
		case GEM::APV_CH_DATA:
			switch((data& 0x00180000)>>19){
			case 0: //apv_header
				adc_ch=(data&0xf);
				UID=GEM::GetUID(0,mpdid,adc_ch,0);
				break;
			case 1:  //data
				mAPVRawSingleEvent[UID].push_back(data & 0x00000fff);
				//std::cout<<__func__<<(data & 0x00000fff)<<std::endl;
				break;
			case 2: //apv trailer
				mAPVRawSingleEvent[UID].push_back((data&0xf00)>>8);
				break;
			case 3: // trailer
				break;
			default:
				break;
			};
			break;
		case GEM::EVENT_TRAILER:
			break;
		case GEM::BLOCK_TRAILER:
			break;
		case GEM::DATA_NOT_VALID:
			break;
		case GEM::FILLER_WORD:
			break;
		default:
			break;
		}
	}
}


void MPDRawParser::LoadRawData(const std::vector<uint32_t>  & data_in){

	  int mpdid;
	  int adc_ch;
	  int UID;
	  clear();
	  for(auto data : data_in){

//		uint32_t data=*iter;
		uint32_t header;
		uint32_t apv_header;
		header = (data & 0x00e00000)>>21;
		switch(header){
		case GEM::BLOCK_HEADER:
			mpdid=(data&0x001F0000) >> 16;
			break;
		case GEM::EVENT_HEADER:
			break;
		case GEM::TRIGGER_TIME:
			break;
		case GEM::APV_CH_DATA:
			switch((data& 0x00180000)>>19){
			case 0: //apv_header
				adc_ch=(data&0xf);
				UID=GEM::GetUID(0,mpdid,adc_ch,0);
				break;
			case 1:  //data
				mAPVRawSingleEvent[UID].push_back(data & 0x00000fff);
				//std::cout<<__func__<<(data & 0x00000fff)<<std::endl;
				break;
			case 2: //apv trailer
				mAPVRawSingleEvent[UID].push_back((data&0xf00)>>8);
				break;
			case 3: // trailer
				break;
			default:
				break;
			};
			break;
		case GEM::EVENT_TRAILER:
			break;
		case GEM::BLOCK_TRAILER:
			break;
		case GEM::DATA_NOT_VALID:
			break;
		case GEM::FILLER_WORD:
			break;
		default:
			break;
		}
	}
}

//
//
void MPDRawParser::CommonModeSubtraction(){

	int crateid = 0;
	int mpdid = 0;
	int adcid = 0;

	for(auto iter_adc=mAPVRawSingleEvent.begin();iter_adc!=mAPVRawSingleEvent.end();iter_adc++){
		int uid=iter_adc->first;
		crateid=GEM::getCrateID(uid);
		mpdid=GEM::getMPDID(uid);
		adcid=GEM::getADCID(uid);
		std::vector<int> adc_temp=iter_adc->second;
		// single adc data
		int TSsize=adc_temp.size()/129;
		for(int ts_counter=0; ts_counter<TSsize;ts_counter++){
			std::vector<int> singleTSadc_temp;
			singleTSadc_temp.insert(singleTSadc_temp.end(),&adc_temp[129*ts_counter],&adc_temp[129*(ts_counter+1)]);
			std::vector<int> singleTSadc_temp_sorting;
			singleTSadc_temp_sorting.insert(singleTSadc_temp_sorting.end(),singleTSadc_temp.begin(),singleTSadc_temp.end());
			std::sort(singleTSadc_temp_sorting.begin(),singleTSadc_temp_sorting.end()-1);
			int commonmode=0;
			for(int k =28; k <100; k ++){
				commonmode+=singleTSadc_temp_sorting.at(k);
			}
			commonmode=commonmode/72;
			for(int k =0; k < singleTSadc_temp.size()-1;k++){
				singleTSadc_temp[k]-=commonmode;
			}
			for(int i = 0; i < 128; i ++){
				mCommonModeSubtractedEvent[iter_adc->first+i].push_back(singleTSadc_temp[i]);
			}

		}
	}
}

void MPDRawParser::clear(){
	mAPVRawSingleEvent.clear();
	mCommonModeSubtractedEvent.clear();
	mPedestalTimeSample.clear();
}

//! return data stcuture:  <uid, <six time sample>>
//
std::map<int,std::vector<int>> MPDRawParser::GetCommonModeSubtraction(){
	CommonModeSubtraction();
	return mCommonModeSubtractedEvent;
}

std::map<int,std::vector<int>> MPDRawParser::GetDecoded(){
	return mAPVRawSingleEvent;
}*/
