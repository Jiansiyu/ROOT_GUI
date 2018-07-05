/*
 * GEMStructure.h
 *
 *  Created on: Jul 5, 2018
 *      Author: newdriver
 */

#ifndef GEMDETECTOR_GEMSTRUCTURE_HH_
#define GEMDETECTOR_GEMSTRUCTURE_HH_
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include "iostream"
#include "stdio.h"

#include "GEMIDGenerator.h"
namespace GEM{

struct apvMapping {
	apvMapping() {apvMap.clear();};
	void addAPV(std::string line){
		std::stringstream ss(line);
		std::vector<std::string> result;
		while ( ss.good()){
			std::string substr;
			getline(ss,substr,',');
			result.push_back(substr);
		}
		if (result.size() == 12) {
			std::string item = result.at(0);
			int CrateID = atoi(result.at(1).c_str());
			int layer = atoi(result.at(2).c_str());
			int MPDID = atoi(result.at(3).c_str());
			int GEMID = atoi(result.at(4).c_str());
			int dimension = atoi(result.at(5).c_str());
			int ADCID = atoi(result.at(6).c_str());
			int I2C = atoi(result.at(7).c_str());
			int Pos = atoi(result.at(8).c_str());
			int Invert = atoi(result.at(9).c_str());
			std::string other = result.at(10);
			int apvUniqueID = GetUID(CrateID, MPDID, ADCID,0);
			apvMap[apvUniqueID].push_back(GEMID);
			apvMap[apvUniqueID].push_back(dimension);
			apvMap[apvUniqueID].push_back(Pos);
			apvMap[apvUniqueID].push_back(Invert);
			CalculateMPDList();
		} else {
			std::cerr << "Unknown Mapping format" << std::endl;
		}
	};

	void addAPVs(std::vector<std::string> lines){
		for (auto line : lines){
			addAPV(line.c_str());
		}
	}

	std::map<int, std::vector<int>> & GetAPVmap(){
		return apvMap;
	}
	std::vector<int> &GetAPVmap(int UID){
		if(apvMap.find(UID)!=apvMap.end()){
			return apvMap[UID];
		}else{
			std::cerr<<__FUNCTION__<<" Cannot find "<<UID<<std::endl;
			std::vector<int> test;
			test.clear();
			return test;
		}
	}
	std::vector<int> &GetMPDList(){
		return mpdUIDList;
	}
	std::vector<std::string> &GetMPDNameList(){
		return MPDNameList;
	}
	std::vector<int> &GetAPVList(int UID){
		int crateid=getCrateID(UID);
		int mpdid=getMPDID(UID);
		if((apvArray.find(crateid)!=apvArray.end())&&(apvArray[crateid].find(mpdid)!=apvArray[crateid].end())){
			return apvArray[crateid];
		}else{
			std::cerr<<__FUNCTION__<<" Cannot find "<<UID<<std::endl;
			std::vector<int> test;
			test.clear();
			return test;
		}
	}
	void Print(){

	}
private:
	std::map<int, std::vector<int>> apvMap;
	std::vector<int> mpdUIDList;
	std::vector<std::string> MPDNameList;
	std::map<int,std::map<int,std::vector<int>>> apvArray;
	void CalculateMPDList(){
		mpdUIDList.clear();
		MPDNameList.clear();
		for(auto apv = apvMap.begin();apv!=apvMap.end();apv++){
			mpdUIDList.push_back(GetUID(getCrateID(apv->first),getMPDID(apv->first),0,0));
			std::string name("Crate"+std::to_string(getCrateID(apv->first))+"_MPD"+std::to_string(getMPDID(apv->first)));
			MPDNameList.push_back(name.c_str());
			apvArray[getCrateID(apv->first)][getMPDID(apv->first)].push_back(getADCID(apv->first));
		}
		MPDNameList.clear();
	};
};

}



#endif /* GEMSTRUCTURE_H_ */
