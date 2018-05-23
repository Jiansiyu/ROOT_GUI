/*
 * GEMStructue.h
 *
 *  Created on: May 22, 2018
 *      Author: newdriver
 */

#ifndef GEMSTRUCTUE_H_
#define GEMSTRUCTUE_H_
#include <string>
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <sstream>


namespace GEM{

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
	int CrateID=-1;
	int MPDID=-1;
	int MPDUniqueID=-1;
	std::vector<apvMap> apvs;

	mpdMap(){apvs.clear();};

	mpdMap(apvMap apv){apvs.clear();RegistAPV(apv);};

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
};

struct gemModuleMap{
	int layer;
	int GEMID;
	std::vector<gemAxisMap> gemAxis;
};

struct GEMLayerMap{
	int layer;
	std::vector<gemModuleMap> gemModules;
};

struct GEMDetectorMap{
	std::vector<GEMLayerMap> gemLayers;
	std::vector<apvMap> apvs;
	std::vector<mpdMap> mpds;
	GEMDetectorMap(){};
	bool RegistAPVs(std::vector<apvMap> apvs){
	 this->apvs.insert(this->apvs.begin(),apvs.begin(),apvs.end());
	}
	bool RegistMPD(mpdMap);
	bool RegistAxis(gemAxisMap);
	bool RegistGEMModule(gemModuleMap);
	int GetMPDNumber();
	std::vector<int> GetMPDIDList();
	std::vector<mpdMap>GetMPDList();
	std::vector<gemModuleMap> GetGEMModuleList();
};

}

#endif /* GEMSTRUCTUE_H_ */
