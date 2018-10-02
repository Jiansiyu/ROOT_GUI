/*
 * GEMEvent.h
 *
 *  Created on: Sep 14, 2018
 *      Author: newdriver
 */

#ifndef GEMEVENTDATA_H_
#define GEMEVENTDATA_H_

#include "unordered_map"
#include "vector"
#include "string"
#include "cstdint"
#include <Rtypes.h>
#include "../GEMDetector/GEMIDGenerator.h"

struct GEMRaw{
	GEMRaw();
	GEMRaw(uint8_t crate,
			uint8_t mpd,
			uint8_t apv,
			uint8_t value){

	}

	GEMRaw(std::unordered_map<uint8_t /*crate*/,std::unordered_map<uint8_t/*mpd*/,std::unordered_map<uint8_t/*apv*/,std::vector<uint16_t>>>> value){
		gemRaw=value;
	}

	std::unordered_map<uint8_t /*crate*/,std::unordered_map<uint8_t/*mpd*/,std::unordered_map<uint8_t/*apv*/,std::vector<uint16_t>>>> const GetGEMRaw(){
		return gemRaw;
	}

	Bool_t IsEmpty(){
		if(gemRaw.size()==0){
			return kTRUE;
		}else{
			return kFALSE;
		}
	};

private:
	std::unordered_map<uint8_t /*crate*/,std::unordered_map<uint8_t/*mpd*/,std::unordered_map<uint8_t/*apv*/,std::vector<uint16_t>>>> gemRaw;
};


#endif /* GEMEVENTDATA_H_ */
