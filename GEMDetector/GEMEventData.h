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
//
//struct GEMRawChannelAddress{
//	uint8_t CrateAddr;
//	uint8_t MPDAddr;
//	uint8_t ADCAddr;
//	uint8_t stripAddr;
//	GEMRawChannelAddress(){};
//	GEMRawChannelAddress(const uint8_t &crate,
//						 const uint8_t &mpd,
//						 const uint8_t &adc,
//						 const uint8_t &strip):
//						 CrateAddr(crate),
//						 MPDAddr(mpd),
//						 ADCAddr(adc),
//						 stripAddr(strip){};
//};
//
//struct GEMRaw_Data{
//	GEMRawChannelAddress addr;
//	std::vector<int16_t> value;
//	GEMRaw_Data(){};
//	GEMRaw_Data(const uint8_t &crate,
//			 const uint8_t &mpd,
//			 const uint8_t &adc,
//			 const uint8_t &strip):
//				 addr(crate,mpd,adc,strip){};
//	void Set_Address(const uint8_t &crate,
//			 const uint8_t &mpd,
//			 const uint8_t &adc,
//			 const uint8_t &strip){
//		addr.CrateAddr=crate;
//		addr.MPDAddr=mpd;
//		addr.ADCAddr=adc;
//		addr.stripAddr=strip;
//	}
//	void Add_Value(const int16_t &v){
//		value.push_back(v);
//	}
//};
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
struct GEMEvent{
//	std::unordered_map<uint8_t/*crate*/,std::unordered_map<uint8_t/*mpd*/,std::unordered_map<uint8_t/*apv*/,std::vector<uint16_t>>>> gemData;
//	std::unordered_map<uint8_t/*crate*/,std::unordered_map<uint8_t/*mpd*/,std::unordered_map<uint8_t/*apv*/,std::vector<uint16_t>>>> gemData_CommonModeSubtracted;
//	std::unordered_map<uint8_t/*crate*/,std::unordered_map<uint8_t/*mpd*/,std::unordered_map<uint8_t/*apv*/,std::vector<uint16_t>>>> gemData_ZeroSubtracted;
//	//std::unordered_map<uint8_t,std::unordered_map<dimension,std::unordered_map<uint32_t,uint16_t>>> GEMdetectorData;
//	GEMEvent(){};
//	void GEMRaw_add(std::unordered_map<uint8_t/*crate*/,std::unordered_map<uint8_t/*mpd*/,std::unordered_map<uint8_t/*apv*/,std::vector<uint16_t>>>> *data){
//		};
};


#endif /* GEMEVENTDATA_H_ */
