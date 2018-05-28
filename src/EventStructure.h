/*
 * EventStructure.h
 *
 *  Created on: May 28, 2018
 *      Author: newdriver
 */

#ifndef EVENTSTRUCTURE_H_
#define EVENTSTRUCTURE_H_
#include <cstdint>
#include <vector>

namespace GEM {

// uid componet
// [	Crate/SSPID],MPDID,ADCID,ChannelID
// channelID 7 bit
// ADCID     4 bit
// MPDID     7 bit
// CrateID   8 bit

#define GEM_CHANNELID_SIZE 8
#define GEM_ADCID_SIZE     4
#define GEM_MPDID_SIZE     8
#define GEM_CRATEID_SIZE   8

#define GEM_CHANNELID_SHIFT 0
#define GEM_ADCID_SHIFT     GEM_CHANNELID_SIZE
#define GEM_MPDID_SHIFT     GEM_CHANNELID_SIZE+GEM_ADCID_SIZE
#define GEM_CRATEID_SHIFT   GEM_CHANNELID_SIZE+GEM_ADCID_SIZE+GEM_MPDID_SIZE

#define GEM_CHANNELID_POS  0x000000ff
#define GEM_ADCID_POS	   0x0000000f<<GEM_ADCID_SHIFT
#define GEM_MPDID_POS	   0xff<<GEM_MPDID_SHIFT
#define GEM_CRATEID_POS    0xff<<GEM_CRATEID_SHIFT


template <class T>
T GetUID(T CrateID,T mpdID, T adcID,T channelID){
	return ((CrateID << GEM_CRATEID_SHIFT) |
			(mpdID   << GEM_MPDID_SHIFT)   |
			(adcID   << GEM_ADCID_SHIFT));
}

struct gemChannelAddr{
	int16_t CrateID;
	int16_t mpdID;
	int16_t adcID;
	int16_t channelID;
	int32_t UID;
	gemChannelAddr(){};
	gemChannelAddr(
			int16_t Crateid,
			int16_t mpdid,
			int16_t adcid,
			int16_t channelid
			){
		CrateID=Crateid;
		mpdID=mpdid;
		adcID=adcid;
		channelID=channelid;
		UID=GetUID(CrateID,mpdID,adcID,channelID);
			};
	bool operator == (const gemChannelAddr &addr) const{
		return (UID == GetUID(addr.CrateID,mpdID,addr.adcID,addr.channelID));
	}
};

struct gemChannelData{
	gemChannelAddr addr;
	uint16_t value;
};

struct gemEventRaw{
	std::vector<gemChannelData> gem_data;
	gemEventRaw(){};
	gemEventRaw(std::vector<gemChannelData> &data){

	};

}


}  // namespace GEM



#endif /* EVENTSTRUCTURE_H_ */
