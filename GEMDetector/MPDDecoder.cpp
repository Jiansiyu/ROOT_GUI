/*
 * MPDDecoder.cpp
 *
 *  Created on: Jul 5, 2018
 *      Author: newdriver
 */

#include "MPDDecoder.h"
#include "evioUtil.hxx"
#include "evioFileChannel.hxx"
#include "MPDStructure.h"
#include "MPDRawParser.h"

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
	chan.open();
}

void MPDDecoder::PedestalMode(){
	MPDRawParser *rawparser=new MPDRawParser();
	while(ReadBlock()){
		rawparser->LoadRawData(block_vec_mpd->begin(),block_vec_mpd->end());

	}
}

bool MPDDecoder::ReadBlock(){
	if(chan.read()){
		evio::evioDOMTree event(chan);
		evio::evioDOMNodeListP mpdEventList = event.getNodeList( evio::isLeaf() );

		for(auto iter = mpdEventList->begin();
				iter != mpdEventList->end(); iter++){
			std::vector<uint32_t> *block_vec = (*iter)->getVector<uint32_t>();
			if((*iter)->tag == GEM::MPD_tag){
				int iend;
				int vec_size=(*block_vec).size();
				//std::cout <<"Vectsize :"<< vec_size<<"  Eventid : "<<evtID << std::endl;
				for(iend =1 ; iend < vec_size; iend++){
					uint32_t tag = (((*block_vec)[iend])>>24)&0xf8;
					if(tag==0x90||tag==0x88) break;
				}
				block_vec_mpd=&(block_vec->resize(iend));
			}
		}
		return true;
	}else{
		chan.close();
		return false;
	}
}



MPDDecoder::~MPDDecoder() {
	// TODO Auto-generated destructor stub
}

