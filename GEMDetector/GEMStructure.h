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
#include <algorithm>

#include "GEMIDGenerator.h"
namespace GEM{

struct apvMapping {
public:
	apvMapping() {
		mpdUIDList.clear();
		MPDNameList.clear();
		apvArray.clear();
		apvUIDList.clear();
		apvMap.clear();
	};

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
			int apvUniqueID = GEM::GetUID(CrateID, MPDID, ADCID,0);

			apvMap[apvUniqueID].push_back(GEMID);      // 0
			apvMap[apvUniqueID].push_back(dimension);  // 1
			apvMap[apvUniqueID].push_back(Pos);        // 2
			apvMap[apvUniqueID].push_back(Invert);     // 3

			CalculateMap();
		} else {
			std::cerr <<__FUNCTION__<<"<"<<__LINE__<<">"<<"Unknown Mapping format "<<result.size() << std::endl;
		}
	};

	void addAPVs(std::vector<std::string> lines){
		for (auto line : lines){
			addAPV(line.c_str());
		}
	}

	std::map<int, std::vector<int>> GetAPVmap(){
		return apvMap;
	}
	std::vector<int> GetAPVmap(int UID){
		if(apvMap.find(UID)!=apvMap.end()){
			return apvMap[UID];
		}else{
			std::cerr<<__FUNCTION__<<" Cannot find "<<UID<<std::endl;
			std::vector<int> test;
			test.clear();
			return test;
		}
	}
	std::vector<int> GetMPDList(){
		return mpdUIDList;
	}
	std::vector<std::string> GetMPDNameList(){
		return MPDNameList;
	}

	const std::string GetMPDName(int uid){
		int crateid=GEM::getCrateID(uid);
		int mpdid=GEM::getMPDID(uid);
		std::string name("Crate"+std::to_string(crateid)+"_MPD"+std::to_string(mpdid));
		return name;
	}

	std::vector<int> GetAPVList(int UID){
		int crateid=GEM::getCrateID(UID);
		int mpdid=GEM::getMPDID(UID);
		if((apvArray.find(crateid)!=apvArray.end())&&(apvArray[crateid].find(mpdid)!=apvArray[crateid].end())){
			return apvArray[crateid][mpdid];
		}else{
			std::cerr<<__FUNCTION__<<" Cannot find "<<UID<<std::endl;
			std::vector<int> test;
			test.clear();
			return test;
		}
	}

	std::vector<int> GetGEMModuleList(){
		std::vector<int> GEMModuleList;
		for(auto apv=apvMap.begin();apv!=apvMap.end();apv++){
			GEMModuleList.push_back((apv->second)[0]);
		}
		std::sort(GEMModuleList.begin(),GEMModuleList.end());
		GEMModuleList.erase(unique(GEMModuleList.begin(),GEMModuleList.end()),GEMModuleList.end());
		return GEMModuleList;
	}

	std::vector<int> GetAPVList(){
		return apvUIDList;
	}
	void Print(){
		for(auto name : MPDNameList){
			std::cout<<name.c_str()<<std::endl;
		}
	}
	std::map<int,std::map<uint8_t,uint32_t>> GetModuleStripCount(){
		std::map<int, std::map<uint8_t,std::vector<int>>> posmax;
		for(auto iter =apvMap.begin();iter != apvMap.end();iter++){
			posmax[iter->second[0]][iter->second[1]].push_back(iter->second[2]);
		}
		for(auto iter = posmax.begin(); iter != posmax.end();iter++){
			for(auto itter=iter->second.begin();itter!=iter->second.end();itter++){
				module_strip_count[iter->first][itter->first]=*max_element(itter->second.begin(),itter->second.begin())*128;
			}
		}
		return module_strip_count;
	}
private:
	std::map<int, std::vector<int>> apvMap;   // unique ID, apv
	std::vector<int> mpdUIDList;
	std::vector<std::string> MPDNameList;
	std::map<int,std::map<int,std::vector<int>>> apvArray; // crateid, mpdid, apv
	std::vector<int> apvUIDList;
	std::map<int,std::map<uint8_t,uint32_t>> module_strip_count;
	void CalculateMap(){
		mpdUIDList.clear();
		MPDNameList.clear();
		apvArray.clear();
		apvUIDList.clear();
		for(auto apv = apvMap.begin();apv!=apvMap.end();apv++){
			mpdUIDList.push_back(GetUID(getCrateID(apv->first),getMPDID(apv->first),0,0));
			std::string name("Crate"+std::to_string(getCrateID(apv->first))+"_MPD"+std::to_string(getMPDID(apv->first)));
			MPDNameList.push_back(name.c_str());
			apvArray[getCrateID(apv->first)][getMPDID(apv->first)].push_back(getADCID(apv->first));
			apvUIDList.push_back(apv->first);
		}

		std::sort(mpdUIDList.begin(),mpdUIDList.end());
		mpdUIDList.erase(std::unique(mpdUIDList.begin(),mpdUIDList.end()),mpdUIDList.end());

		std::sort(MPDNameList.begin(),MPDNameList.end());
		MPDNameList.erase(std::unique(MPDNameList.begin(),MPDNameList.end()),MPDNameList.end());

		std::sort(apvUIDList.begin(),apvUIDList.end());
		apvUIDList.erase(std::unique(apvUIDList.begin(),apvUIDList.end()),apvUIDList.end());
	}
};  // end of apvmap

} // end of namespace



#endif /* GEMSTRUCTURE_H_ */
