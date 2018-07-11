/*
 * MPDRawParser.cpp
 *
 *  Created on: Jul 5, 2018
 *      Author: newdriver
 */

#include <iostream>
#include "MPDRawParser.h"
#include "MPDStructure.h"
#include "GEMIDGenerator.h"
MPDRawParser::MPDRawParser() {
	// TODO Auto-generated constructor stub

}

MPDRawParser::~MPDRawParser() {
	// TODO Auto-generated destructor stub
}

//template<class T>
//void MPDRawParser::LoadRawData(std::vector<T> begin,std::vector<T> end)
void MPDRawParser::LoadRawData(std::vector<uint32_t>::iterator begin,std::vector<uint32_t>::iterator end){

	  int mpdid;
	  int adc_ch;
	  int UID;
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

void MPDRawParser::CModeSubtraction(){

}
std::map<int,std::vector<int>> MPDRawParser::GetDecoded(){
	return mAPVRawSingleEvent;
}
