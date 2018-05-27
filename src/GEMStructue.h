/*
 * GEMStructue.h
 *  Created on: May 22, 2018
 *      Author: newdriver
 */

#ifndef GEMSTRUCTUE_H_
#define GEMSTRUCTUE_H_
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <iostream>

#include "TH1F.h"

namespace GEM{

//Map need to load before hand, the load efficiency should not be a bit trouble ?
//

#define CRATE_SHIFT 20  // no larger than 4 in sbs
#define MPDID_SHIFT 16  // no larger than 32, 5 bits
#define ADCID_SHIFT 8   // no larger than 16, 4 bits
#define LAYID_SHIFT
#define GEMID_SHIFT


template <class T>
T getHashValue(T mpdid,T adcid){
	return ((mpdid << MPDID_SHIFT)|(adcid << ADCID_SHIFT));
}

template <class T>
T getHashValue(T crateid,T mpdid,T adcid){
	return ((crateid << CRATE_SHIFT)|(mpdid << mpdid)|(adcid << ADCID_SHIFT));
}

template <class T>
T getMPDHashValue(T crateid,T mpdid){
	return ((crateid << CRATE_SHIFT)|(mpdid << mpdid));
}

enum gemMapFormat{
	ITEM,
	CRATEID,
	LAYER,
	MPDID,
	GEMID,
	DIMENSION,
	ADCID,
	I2C,
	POS,
	INVERT,
	OTHER
};

enum Dimension {
	X,
	Y
};

struct apvMap{
	std::string item;
	int CrateID;
	int MPDID;
	int layer;
	int GEMID;
	int dimension;
	Dimension dim;
	int ADCID;
	int I2C;
	int Pos;
	int Invert;
	std::string other;
	int apvUniqueID;
	apvMap(){};
	apvMap(std::string line){
		std::stringstream ss(line);
		std::vector<std::string> result;
		while ( ss.good()){
			std::string substr;
			getline(ss,substr,',');
			result.push_back(substr);
		}
		if(result.size()==12){
			item=result.at(0);
			CrateID = atoi(result.at(1).c_str());
			layer   = atoi(result.at(2).c_str());
			MPDID   = atoi(result.at(3).c_str());
			GEMID   = atoi(result.at(4).c_str());
			dimension=atoi(result.at(5).c_str());
			if(!dimension){
				dim=Y;
			}else dim=X;
			ADCID    = atoi(result.at(6).c_str());
			I2C     = atoi(result.at(7).c_str());
			Pos	    = atoi(result.at(8).c_str());
			Invert  = atoi(result.at(9).c_str());
			other   = result.at(10);
			apvUniqueID=getHashValue(CrateID,MPDID,ADCID);
		}else{
			std::cerr <<"Unknown Mapping format"<<std::endl;
		}
	}
	int getUniqueAPVID(){
		return getHashValue(CrateID,MPDID,ADCID);
	}
	int GetUniqueMPDID(){
		return getHashValue(CrateID,MPDID,0);
	}
};

struct mpdMap{
	int CrateID;
	int MPDID;
	int MPDUniqueID;
	std::vector<apvMap> apvs;
	mpdMap(){
			apvs.clear();
			CrateID=-1;
			MPDID=-1;
			MPDUniqueID=-1;};

	mpdMap(apvMap apv){
				apvs.clear();
				CrateID=-1;
				MPDID=-1;
				MPDUniqueID=-1;
				RegistAPV(apv);};
	int SetMPDUniqueID(int id){
		MPDUniqueID=id;
		return MPDUniqueID;
	}
	int GetMPDUniqueID(){
		return MPDUniqueID;
	}
	int GetMPDID(){
		return MPDID;
	}
	int GetCrateID(){
		return CrateID;
	}
	bool RegistAPV(apvMap apv){
		if(MPDUniqueID==-1){
			CrateID=apv.CrateID;
			MPDID  =apv.MPDID;
			MPDUniqueID=getHashValue(CrateID,MPDID,0);
			apvs.push_back(apv);
			return true;
		}else{
			if(MPDUniqueID == getHashValue(apv.CrateID,apv.MPDID,0)){
				apvs.push_back(apv);
				return true;
			}else{
				return false;
			}
		}
	}
	bool operator+(mpdMap &mpd) {
		if(MPDUniqueID==mpd.MPDUniqueID){
			apvs.insert(apvs.end(),mpd.apvs.begin(),mpd.apvs.end());
			return true;
		}else return false;
	}
	bool operator==(mpdMap &mpd) const {
		return (MPDUniqueID==getHashValue(mpd.CrateID,mpd.MPDID,0));}

	bool operator!=(mpdMap &mpd) const {
		return (MPDUniqueID!=getHashValue(mpd.CrateID,mpd.MPDID,0));}
};

struct gemAxisMap{
	int layer;
	int GEMID;
	Dimension axis;
	std::vector<apvMap> apvs;
	gemAxisMap(){
		layer=-1;
		GEMID=-1;
		apvs.clear();
	}
	bool RegistAPV(apvMap apv){
		if(layer ==-1 && GEMID ==-1){
			layer=apv.layer;
			GEMID=apv.GEMID;
			axis=apv.dim;
			apvs.push_back(apv);
			return true;
		}else{
			if(layer==apv.layer && GEMID == apv.GEMID && axis == apv.dim){
				apvs.push_back(apv);
				return true;
			}else return false;
		}
	}
};

// single GEM detector
struct gemModuleMap{
	int layer;
	int GEMID;
	std::vector<gemAxisMap> gemAxis;
	gemModuleMap(){
		layer=-1;
		GEMID=-1;
		gemAxis.clear();
	};

};

// multi GEM detector on one layer
struct GEMLayerMap{
	int layer;
	std::vector<gemModuleMap> gemModules;
	GEMLayerMap(){};
};

struct GEMDetectorMap{
private:
	std::vector<GEMLayerMap> gemLayers;
	std::vector<apvMap> apvs;
	std::vector<mpdMap> mpds;
	// <crate ID,<mpdid, <apvs> > >
	std::map<int,std::map<int,std::vector<apvMap>>> CrateMPDList;
	// < layerID, <GEMid, <  Dimension, <APVs> > > >
	std::map<int,std::map<int,std::map<Dimension,std::vector<apvMap>>>> LayerAPVList;
	std::vector<int> MPDUniqueIDList;
	std::vector<std::string> MPDNameList;
public:
	GEMDetectorMap(){
		gemLayers.clear();
		apvs.clear();
		mpds.clear();
	};
	bool RegistAPVs(std::vector<apvMap> apvs){
		this->apvs.insert(this->apvs.end(),apvs.begin(),apvs.end());
		for(auto apv : apvs){
			CrateMPDList[apv.CrateID][apv.MPDID].push_back(apv);
			LayerAPVList[apv.layer][apv.layer][apv.dim].push_back(apv);
		}
		for (auto iter_crate=CrateMPDList.begin();
				iter_crate!=CrateMPDList.end(); iter_crate++){
			for(auto iter_mpd = iter_crate->second.begin();
					iter_mpd != iter_crate->second.end();iter_mpd++){
				mpdMap mpd;
				mpd.SetMPDUniqueID(getHashValue(iter_crate->first,iter_mpd->first,0));
				for ( auto apv : iter_mpd->second){
					mpd.RegistAPV(apv);
				}
				mpds.push_back(mpd);
				MPDUniqueIDList.push_back(getHashValue(iter_crate->first,iter_mpd->first,0));
				std::string name("Crate"+std::to_string(iter_crate->first)+"_MPD"+std::to_string(iter_mpd->first));
				MPDNameList.push_back(name.c_str());
			};
		}
	}

	bool RegistMPD(mpdMap);
	bool RegistAxis(gemAxisMap);
	bool RegistGEMModule(gemModuleMap);
	int GetMPDNumber(){
		return MPDUniqueIDList.size();
	}
	std::vector<int> GetMPDIDList(){
		return MPDUniqueIDList;
	}
	std::vector<mpdMap>GetMPDList(){
		return  mpds;
	}
	std::vector<std::string> GetMPDNameList(){
		return MPDNameList;
	}
	std::vector<gemModuleMap> GetGEMModuleList();
};

struct gemChannelAddr{
	int layer;
	int GEMID;
	int mpdID;
	int ADCID;
	int stripid;
	int pos;
	gemChannelAddr(){};
	gemChannelAddr(
			int layer,
			int GEMID,
			int mpdID,
			int ADCID,
			int stripid,
			int pos){};

	void SetAddress(){

	};
};

// GEM data structure
struct gemChannelData{
	gemChannelAddr addr;
	std::vector<int> data;  // buffer all the time samples in one single channel
};

struct apvData{
	int layer;
	int GEMID;
	int mpdID;
	int ADCID;
	std::vector<gemChannelData> data;
};

struct EventDataRaw {
	int64_t evtID;
	std::vector<gemChannelData> gem_data;
};

struct EventRawStruct{
	// mpdID, ADCID
	std::map<int,std::map<int,TH1F*>> histos;
	std::map<int, std::map<int,std::vector<int>>> raw;
	EventRawStruct(){};
	EventRawStruct(std::map<int,std::map<int,TH1F*>> &histos){
		this->histos = histos;
	}
	EventRawStruct(std::map<int, std::map<int,std::vector<int>>> &raw){
		this->raw = raw;
	}
};

}

#endif /* GEMSTRUCTUE_H_ */
