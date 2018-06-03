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
#include <unordered_map>
namespace GEM {

// uid componet
// [	Crate/SSPID],MPDID,ADCID,ChannelID
// channelID 7 bit
// ADCID     4 bit
// MPDID     7 bit
// CrateID   8 bit
// need the 0xff term to be the broad mode, to select all the MPDs on the Crate \
or all the apvs on the MPDs

#define GEM_CHANNELID_SIZE 8
#define GEM_ADCID_SIZE     8
#define GEM_MPDID_SIZE     8
#define GEM_CRATEID_SIZE   8


#define GEM_CHANNELID_SHIFT 0
#define GEM_ADCID_SHIFT     GEM_CHANNELID_SIZE
#define GEM_MPDID_SHIFT     GEM_CHANNELID_SIZE+GEM_ADCID_SIZE
#define GEM_CRATEID_SHIFT   GEM_CHANNELID_SIZE+GEM_ADCID_SIZE+GEM_MPDID_SIZE

#define GEM_CHANNELID_POS  0xff
#define GEM_ADCID_POS	   0xff<<GEM_ADCID_SHIFT
#define GEM_MPDID_POS	   0xff<<GEM_MPDID_SHIFT
#define GEM_CRATEID_POS    0xff<<GEM_CRATEID_SHIFT


// @param CrateID/SSPid: used for the crate identification
// @param mpdID
// @param adcID
// @param channelID
// id set the param to -1 means select all the modules
template <class T>
T GetUID(T CrateID,T mpdID, T adcID,T channelID){
	if(CrateID == -1){
		CrateID=GEM_CRATEID_POS>>GEM_CRATEID_SHIFT;
	}
	if( mpdID==-1){
		mpdID=GEM_MPDID_POS>>GEM_MPDID_SHIFT;
	}
	if(adcID ==-1){
		adcID=GEM_ADCID_POS>>GEM_ADCID_SHIFT;
	}
	if(channelID == -1){
		channelID=GEM_CHANNELID_POS>>GEM_CHANNELID_SHIFT;
	}
	return ((CrateID << GEM_CRATEID_SHIFT) |
			(mpdID   << GEM_MPDID_SHIFT)   |
			(adcID   << GEM_ADCID_SHIFT))|
			channelID;
}
template <class T>
T getMPDID(T uid){
	return (uid & GEM_MPDID_POS)>>GEM_MPDID_SHIFT;
}

template <class T>
T getADCID(T uid){
	return (uid & GEM_ADCID_POS)>>GEM_ADCID_SHIFT;
}

template <class T>
T getChannelID(T uid){
	return (uid&GEM_CHANNELID_POS)>>GEM_CHANNELID_SHIFT;
}

template <class T>
T getCrateID(T uid){
	return (uid & GEM_CRATEID_POS)>>GEM_CRATEID_SHIFT;
}

struct gemChannelAddr{
	int16_t CrateID;
	int16_t mpdID;
	int16_t adcID;
	int16_t channelID;
	uint32_t UID;

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
		gem_data.clear();
		gem_data.insert(gem_data.begin(),data.begin(),data.end());
	};

	std::vector<gemChannelData> GetDataMPD(int16_t CrateID,int16_t mpdID){
	};
	
	std::vector<gemChannelData> GetData(int16_t CrateID,int16_t mpdID=-1,int16_t APVID=-1,int16_t ChannelID=-1){

	}

private:
	std::vector<gemChannelData> & GetChannelData(int32_t uid){
		std::vector<gemChannelData> data;
		for(auto gem : gem_data){
			// this is not right
			if(gem.addr.UID&uid==gem.addr.UID){
				data.push_back(gem);
			}
		}
		data.shrink_to_fit();
		return data;
	}

};


}  // namespace GEM



#endif /* EVENTSTRUCTURE_H_ */
