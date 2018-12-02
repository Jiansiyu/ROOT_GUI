/*
 * GEMDataStruct.h
 *
 *  Created on: Nov 30, 2018
 *      Author: newdriver
 */

#ifndef GEMDATASTRUCT_H_
#define GEMDATASTRUCT_H_

#include <stdint.h>
#include <vector>
#include <map>
namespace GEM {

struct apvAddr{
public:
	uint64_t UID;    //
	uint8_t crateID; // crate / ssp addr max 256 crate support
	uint8_t MPDID;   // MPD add attached to a crate or ssp. expect max 32
	uint8_t adcID;   // apv card adc addr attched to a MPD, max 16
	apvAddr(){};
	~apvAddr(){};

	bool operator==(const apvAddr *a, const apvAddr *b){
		if(a->UID == b->UID){
			return true;
		}else return false;
	}

};

struct apvFrame : apvAddr{
public:
	uint8_t pipelineAddr;
    uint8_t triggerTime;
    uint8_t timeSample;
    apvFrame(){};
    ~apvFrame(){};
};


struct apvStripData:apvFrame{
public:
	uint8_t stripNumber;
	int32_t adc;
	apvStripData(){};
	~apvStripData(){};
};


struct apvFrameData:apvFrame{
	std::vector<apvStripData> apvStripsData;   // contains the data for all the 128 channels
	apvFrameData(){};
};

struct apvData:apvFrame{
	std::vector<apvFrameData> apvFrame;
};


}




#endif /* GEMDATASTRUCT_H_ */
