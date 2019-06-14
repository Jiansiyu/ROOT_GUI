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
#include <stdint.h>
#include "MPDRawParser.h"
#include "MPDStructure.h"
#include "GEMIDGenerator.h"
MPDRawParser::MPDRawParser() {
	// TODO Auto-generated constructor stub

}

MPDRawParser::~MPDRawParser() {
	// TODO Auto-generated destructor stub
}


void MPDRawParser::LoadRawData(const std::vector<uint32_t>  & data_in){

	  int mpdid;
	  int adc_ch;
	  int UID;
	  clear();
	  for(auto data : data_in){

		uint32_t header;
		uint32_t apv_header;
		header = (data & 0x00e00000)>>21;
		switch(header){
		case GEM::BLOCK_HEADER:
		{
			mpdid=(data&0x001F0000) >> 16;
			uint32_t eventPerBlock=(data&0x0000ff00)>>8;
			uint32_t blockCount=(data&0x000000ff);
		}
			break;
		case GEM::EVENT_HEADER:
			break;
		case GEM::TRIGGER_TIME:
		{
			uint32_t triggerTime1;
			uint32_t triggerTime2;
			// TODO this need to be changed
			if (((data & 0x00100000) >> 20)) {
				triggerTime2 = (data & 0xfffff);
			} else {
				triggerTime1 = (data & 0xfffff);
			}
		}
			break;
		case GEM::APV_CH_DATA:
			switch((data& 0x00180000)>>19){
			case 0:
			  {
			    //apv_header
				adc_ch=(data&0xf); // apvid
				// APV_HEADER 1b0, 3b111, 8b[apvcolumn], 1b apverror
				uint16_t apvHeader=(data&0x1fff0)>>4;

				uint8_t  apvHeaderBit=(apvHeader&0x1000)>>12;         // header bit
				uint8_t  apvFrameHeader111bit=((apvHeader&0xe00)>>9); // this should be 0b111
				uint8_t  apvFramePipelineAddr=((apvHeader&0x1fe)>>1); // 8 bits pipeline address
				uint8_t  apvFrameAPVErrorBit=(apvHeader&0x1);         // end of the apv frame, error bit should be 1

				UID=GEM::GetUID(0,mpdid,adc_ch,0);
			  }
				break;
			case 1:  //data
			     {
			    	 uint8_t apvChannelAddr =(data & 0x0007f000);            // apv channel addr
			    	 uint8_t apvChannelValue=(data & 0x00000fff);            // apv channel adc value
			    	 mAPVRawSingleEvent[UID].push_back(data & 0x00000fff);   // apv channel data
			     }
				break;
			case 2: //apv trailer
			     {
			    	 mAPVRawSingleEvent[UID].push_back((data&0xf00)>>8);
			    	 uint8_t apvFrameCount =(data &   0xff);
			    	 uint8_t apvSampleCount=(data &  0xf00)>>8;
			    	 uint8_t ModuleID   =(data &0x1f000)>>12;  // this is the MPD id
			     }
				break;
			case 3: // trailer
				{
					auto wordCount    =(data &    0xff);
					auto baselineValue=(data & 0x7ff00) >> 8;
				}

				break;
			default:
			    {
			    	// unexpected data structure
			    }
				break;
			};
			break;
		case GEM::EVENT_TRAILER:
		    {
		    	// event trailer
		    	auto apvFineTriggerTime=(data &    0xff);
		    	auto apvNWordsInEvent  =(data & 0xfff00)>>8;
		    }
			break;
		case GEM::BLOCK_TRAILER:
		    {
		     // it seems like, each APV will have one block data
		    }
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
}
