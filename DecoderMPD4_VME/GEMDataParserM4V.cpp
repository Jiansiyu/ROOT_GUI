/*
 * GEMDataParserM4V.cpp
 *
 *  Created on: May 22, 2018
 *      Author: newdriver
 */

#include "GEMDataParserM4V.h"
#include "datastructureM4V.h"
#include "RawDecoderM4V.h"
#include "../src/GEMStructue.h"

#include <string>
#include <bitset>

#include "evioUtil.hxx"
#include "evioFileChannel.hxx"

GEMDataParserM4V::GEMDataParserM4V() {
	// TODO Auto-generated constructor stub
}

GEMDataParserM4V::~GEMDataParserM4V() {
	// TODO Auto-generated destructor stub
}

bool GEMDataParserM4V::OpenFileIn(std::string fname) {
	RawDatfileName = fname;
	ParserRawDat();
//	try {
//
//	} catch (evio::evioException e) {
//	 std::cerr <<e.toString()<<std::endl<<std::endl;
//	 exit(EXIT_FAILURE);
//	}
}
bool GEMDataParserM4V::ParserRawDat(){
	try {
		evio::evioFileChannel chan(RawDatfileName.c_str(),"r");
		chan.open();
		int evtID=0;
		while(chan.read() && evtID <=5000){
			evio::evioDOMTree event(chan);
			evio::evioDOMNodeListP mpdEventList = event.getNodeList( evio::isLeaf() );

			for(auto iter = mpdEventList->begin();
					iter != mpdEventList->end(); iter++){
				vector<uint32_t> *block_vec = (*iter)->getVector<uint32_t>();
				if((*iter)->tag == MPD_tag){
					int iend;
					int vec_size=(*block_vec).size();
					//std::cout <<"Vectsize :"<< vec_size<<"  Eventid : "<<evtID << std::endl;
					for(iend =1 ; iend < vec_size; iend++){
						uint32_t tag = (((*block_vec)[iend])>>24)&0xf8;
						if(tag==0x90||tag==0x88) break;
					}
					RawDecoderM4V *decoder=new RawDecoderM4V(*block_vec, 0, iend);
					EventRaw[evtID]=decoder->GetAPVRawData();
					//delete decoder;
					evtID++;
				}
			}
		}
	} catch (evio::evioException e) {
		std::cerr << e.toString() << std::endl << std::endl;
		exit(EXIT_FAILURE);
	}
}

void PedestalMode(std::string fname,std::string savename){
	try {
		evio::evioFileChannel chan(fname.c_str(),"r");
		chan.open();
		int evtID=0;
		while(chan.read() && evtID <=5000){
			evio::evioDOMTree event(chan);
			evio::evioDOMNodeListP mpdEventList = event.getNodeList( evio::isLeaf() );

			for(auto iter = mpdEventList->begin();
					iter != mpdEventList->end(); iter++){
				vector<uint32_t> *block_vec = (*iter)->getVector<uint32_t>();
				if((*iter)->tag == MPD_tag){
					int iend;
					int vec_size=(*block_vec).size();
					//std::cout <<"Vectsize :"<< vec_size<<"  Eventid : "<<evtID << std::endl;
					for(iend =1 ; iend < vec_size; iend++){
						uint32_t tag = (((*block_vec)[iend])>>24)&0xf8;
						if(tag==0x90||tag==0x88) break;
					}
					RawDecoderM4V *decoder=new RawDecoderM4V(*block_vec, 0, iend);
					//EventRaw[evtID]=decoder->GetAPVRawData();

					//TODO

					//delete decoder;
					evtID++;
				}
			}
		}
	} catch (evio::evioException e) {
		std::cerr << e.toString() << std::endl << std::endl;
		exit(EXIT_FAILURE);
	}
}
