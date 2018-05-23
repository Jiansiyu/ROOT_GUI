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
namespace GEM{
template <class T>
T getHashValue(T mpdid,T adcid){
	return ((mpdid << 12)|(adcid << 8));
}
struct APVMap{
	int MPDid;
	int ADCid;
	int I2C;
	int GEMid;
	int Xis;
	int Pos;
	int Invert;
	int HybridID;
	APVMap(){};
	APVMap( const int mpdid,
			const int gemid,
			const int xis,
			const int adcid,
			const int i2c,
			const int pos,
			const int invert):
				MPDid(mpdid),
				ADCid(adcid),
				I2C(i2c),
				GEMid(gemid),
				Xis(xis),
				Pos(pos),
				Invert(invert),
				HybridID(getHashValue(mpdid,adcid)){
	};

	bool operator == (const APVMap &apv) const {
		if((MPDid != apv.MPDid) ||
		   (ADCid != apv.ADCid) ||
		   (I2C   != apv.ADCid))
		{
			return false;
		}else return true;
	}
};

struct GEMMap {
	std::unordered_map<unsigned int, APVMap> map;

	GEMMap() {
	}

	void AddAPV(APVMap apv) {
		map[getHashValue(apv.MPDid, apv.ADCid)] = apv;
	}

	APVMap & getAPV(int mpdid, int adcid) {
		if (map.find(getHashValue(mpdid, adcid)) != map.end()) {
			return map[getHashValue(mpdid, adcid)];
		} else {
			std::cout << "Fatel error :" << __FUNCTION__ << " at " << __LINE__
					<< std::endl;
			exit(-1);
		}
	}

	std::vector<APVMap> & getAPVs(int mpdid) {
		std::vector<APVMap> apvarr;
		for (std::unordered_map<unsigned int, APVMap>::iterator it =
				map.begin(); it != map.end(); it++) {
			if ((it->first & getHashValue(mpdid, 0)) == getHashValue(mpdid, 0))
				apvarr.push_back(it->second);
		}
		return apvarr;
	}

	void clear() {
		map.clear();
	}
	void operator =(const GEMMap &map) {
		this->map.clear();
		this->map.insert(map.map.begin(), map.map.end());
	}
};

struct APVAddress{

};

struct GEMChannelAddress{
	unsigned int MPDID;
	unsigned char ADCaddr; //
	unsigned char I2Caddr;
	unsigned char strip;
};

struct GEMChannel_Data{
	GEMChannelAddress addr;
	std::vector<float> value;
};

struct GEM_Data{
	unsigned int ModuleID;
	unsigned int GEMid;
	std::vector<GEMChannel_Data> value;
};
}

// simple hash key generator
namespace std {
template<>
struct hash<GEM::APVMap> {
	size_t operator()(const GEM::APVMap &addr) const {
		return GEM::getHashValue(addr.MPDid,addr.ADCid);
	}
};
}
#endif /* GEMSTRUCTUE_H_ */
