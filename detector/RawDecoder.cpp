/*
 * RawDecoder.cpp
 *
 *  Created on: Sep 15, 2018
 *      Author: newdriver
 */

#include "RawDecoder.h"

#include "string"
#include "vector"
#include "iostream"
#include "unordered_map"
#include "evioUtil.hxx"
#include "evioFileChannel.hxx"
#include "evioException.hxx"
#include "../GEMDetector/MPDStructure.h"

RawDecoder::RawDecoder() {
	// TODO Auto-generated constructor stub

}

RawDecoder::~RawDecoder() {
	// TODO Auto-generated destructor stub
}



Bool_t RawDecoder::OpenFile(const std::string fname){
	try {
		efChan= new evio::evioFileChannel(fname.c_str(),"r");
		efChan->open();
		return kTRUE;
	} catch (evio::evioException e) {

		std::cout<<__FUNCTION__<<"["<<__LINE__<<"] error in open file:"<<fname.c_str()
				<< "  error code:"<<e.toString()<<std::endl;
		return kFALSE;
	}
}


Bool_t RawDecoder::Read(){
	dDataBlock.clear(); // clean the old event before read the next one
	dRawDataEvent.clear();
	if(efChan->read()){
		try {
			evio::evioDOMTree event(efChan);
			evio::evioDOMNodeListP EventList=event.getNodeList(evio::isLeaf());
			for(evio::evioDOMNodeList::iterator iter = EventList->begin();
					iter!=EventList->end();iter ++){
				int banktag=(*iter)->tag;
				std::vector<uint32_t> *block_vec=(*iter)->getVector<uint32_t>();
				switch (banktag) {
					case GEM::MPD_tag:
						{
							dDataBlock.insert(dDataBlock.end(),block_vec->begin(),block_vec->end());
							// seperate block
							unsigned int iend=0;
							unsigned int ibegin=0;
							std::vector<uint32_t> eventData;
							for(unsigned int iloop = 0; iloop<block_vec->size();iloop++){
								uint32_t tag=((*block_vec).at(iloop) >> 24) & 0xf8;
								if(tag==0x90||tag==0x88){
									iend=iloop;
									eventData.insert(eventData.end(),block_vec->begin()+ibegin,block_vec->begin()+iend);
									ibegin=iloop+1;
									dRawDataEvent.push_back(eventData);
									eventData.clear();
								}
							}
						}
						break;

					case GEM::FADC_tag:
						break;

					case GEM::TDC_tag:

						break;

					default:
						break;

				}
				if(dRawDataEvent.size()==0){
					std::cout<<__FUNCTION__<<"["<<__LINE__<<"] no data was found"<<std::endl;
				}else{
					std::cout<<__FUNCTION__<<"["<<__LINE__<<"] "<<dRawDataEvent.size()<<" data block was found"<<std::endl;
				}
				return kTRUE;
			}

		} catch (evio::evioException e) {
			std::cout<<__FUNCTION__<<"["<<__LINE__<<"] error in decoding data"
					<< "  error code:"<<e.toString()<<std::endl;
			return kFALSE;
		}
	}else{
		return kFALSE;
	}

}


std::vector<uint32_t> const RawDecoder::GetRawBlock(){
	return dDataBlock;
}


GEMRaw *RawDecoder::GEMRawParser(std::vector<uint32_t> const value){
	uint8_t crateid=0;
	uint8_t mpdid;
	uint8_t adcid;
	std::unordered_map<uint8_t /*crate*/,std::unordered_map<uint8_t/*mpd*/,std::unordered_map<uint8_t/*apv*/,std::vector<uint16_t>>>> gemdata;
	for(auto data : value){
		uint32_t header;
		uint32_t apv_header;
		header=(data & 0x00e00000)>>21;
		switch (header) {
			case GEM::BLOCK_HEADER:
				mpdid=(data&0x001F0000) >> 16;
				break;
			case GEM::EVENT_HEADER:
				break;
			case GEM::TRIGGER_TIME:
				break;
			case GEM::APV_CH_DATA:
				{
					switch ((data& 0x00180000)>>19) {
						case 0:   // apv header
							adcid=(data&0xf);
							break;
						case 1:   // data
							gemdata[crateid][mpdid][adcid].push_back(data & 0x00000fff);
							break;
						case 2:   // apv trailer
							gemdata[crateid][mpdid][adcid].push_back((data&0xf00)>>8);
							break;
						case 3:  // trailer

							break;
						default:
							break;
					}
				}
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
	GEMRaw *gemRaw= new GEMRaw(gemdata);
	return gemRaw;
}


GEMRaw *RawDecoder::FillCommonModeSubtracted(GEMRaw *, const GEMRaw *){

}

GEMRaw *RawDecoder::FillZeroSubtraction(GEMRaw *, const GEMRaw *){

}
