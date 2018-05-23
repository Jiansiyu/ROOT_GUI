/*
 * ConfigInfor.h
 *
 *  Created on: May 20, 2018
 *      Author: newdriver
 */
#include <iostream>
#include <string>
#include <libconfig.h++>
#include "GlobalStructure.h"
#ifndef CONFIGINFOR_H_
#define CONFIGINFOR_H_

class ConfigInfor {

public:
	void ReadConfig(std::string fname="./cfg/DetConfig.cfg");
	void PrintConfig();

	GlobStruct::GEMConfig GetGEMConfInfor(){
		return gemconfig;
	}
private:
	libconfig::Config cfg;
	GlobStruct::DAQConfig  daqconfig;
	GlobStruct::GEMConfig gemconfig;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
	static ConfigInfor *GetInstance();
private:
	static ConfigInfor *m_instance;
	ConfigInfor(){
		std::cout<<"Initialize the singleton function:"<<std::endl;
	};
	virtual ~ConfigInfor(){
		std::cout <<"Deconstruct the singleton function: "<<__FUNCTION__<<std::endl;
	};
	///Embedded function used for auto de-contrcut singletone class
	class Garbo{
	public:
		~Garbo(){
			if(ConfigInfor::m_instance)
				delete ConfigInfor::m_instance;
		}
	};
	static Garbo garbo;
};

#endif /* CONFIGINFOR_H_ */
