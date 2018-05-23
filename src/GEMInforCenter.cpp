/*
 * GEMInforCenter.cpp
 *
 *  Created on: May 22, 2018
 *      Author: newdriver
 */

#include "GEMInforCenter.h"
#include "GEMStructue.h"

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

GEMInforCenter *GEMInforCenter::m_instance=nullptr;
GEMInforCenter *GEMInforCenter::GetInstance(){
	if(m_instance == nullptr){
		m_instance = new GEMInforCenter;
	}
	return m_instance;
}
GEMInforCenter::GEMInforCenter(){
	std::cout<<"Initialize the singleton function:"<<std::endl;
	ConfigInfor *infor = ConfigInfor::GetInstance();
	LoadGEMMapFile(infor->GetGEMConfInfor().mapfilename.c_str());
}

GEM::GEMMap & GEMInforCenter::LoadGEMMapFile(std::string fname){
	GEMMap.clear();
	std::ifstream mapfilestream(fname.c_str(),std::ifstream::in);
	if(mapfilestream.good()){
		std::string line;
		while(getline(mapfilestream,line)){
			line.erase(std::remove_if(line.begin(),line.end(),::isspace),line.end());
			line = line.substr(0,line.find("#",0));
			line = line.substr(0,line.find("//",0));
			if(line.size()==0) continue;
			std::stringstream ss(line);
			int infor;
			std::vector<int> parsed_infor;
			while (ss >> infor){
				parsed_infor.push_back(infor);
				if(ss.peek()==',') ss.ignore();
			}

			if(parsed_infor.size()==7){
				GEM::APVMap apv(parsed_infor.at(0),parsed_infor.at(1),
								parsed_infor.at(2), parsed_infor.at(3),
								parsed_infor.at(4),parsed_infor.at(5),
								parsed_infor.at(6)
								);
				GEMMap.AddAPV(apv);
			}
		}

	}else {
		std::cout<<"["<<__FUNCTION__<<"("<<__LINE__<<")]: Fatal Error" << fname.c_str()<<" did not found"<<std::endl;
	}
	//GEMMapCheck();
}

void GEMInforCenter::GEMMapCheck(){
	for(auto iter = GEMMap.map.begin();
			iter!=GEMMap.map.end();
			iter++){
		std::cout<<"hash : "<<iter->first<<" MPDID:"<<iter->second.MPDid<<
				" GEMID:"<<iter->second.GEMid<<" Xis:"<<iter->second.Xis<<
				" ADCID:"<<iter->second.ADCid<<" I2C: "<<iter->second.I2C<<
				" Pos:"<<iter->second.Pos<<" Inverrt: "<<iter->second.Invert<<std::endl;
	}
}
