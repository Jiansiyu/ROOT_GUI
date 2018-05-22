/*
 * ConfigInfor.cpp
 *
 *  Created on: May 20, 2018
 *      Author: newdriver
 */

#include "ConfigInfor.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>


ConfigInfor *ConfigInfor::m_instance=nullptr;
ConfigInfor *ConfigInfor::GetInstance(){
	if(m_instance==nullptr){
		m_instance = new ConfigInfor;
	}
	return m_instance;
}

//
//
void ConfigInfor::ReadConfig(std::string fname){

	try {
		cfg.readFile(fname.c_str());
	}
	catch (const libconfig::FileIOException &fioex) {
		std::cerr <<"I/O error while reading file." <<std::endl;
		return;
	}
	catch (const libconfig::ParseException &pex) {
		std::cerr<< "Parse error at " << pex.getFile() << ":" << pex.getLine()
                      << " - " << pex.getError() << std::endl;
		return;
	}


	try {
		std::string name = cfg.lookup("name");
		std::cout << "Store name: " << name << std::endl << std::endl;
	} catch (const libconfig::SettingNotFoundException &nfex) {
		std::cerr << "No 'name' setting in configuration file." << std::endl;
	}

	// try to decode the configuration for GEM detector
	const libconfig::Setting &ConfigRoot=cfg.getRoot();
	try {
		const libconfig::Setting &GEMConfig = ConfigRoot["GEMConfig"];
		try {
			const libconfig::Setting &DAQinfor = GEMConfig["DAQinfor"];
			DAQinfor.lookupValue("DAQVersion", daqconfig.DAQVersion);
			DAQinfor.lookupValue("DataStructureVersion",
					daqconfig.DataStrcutureVersion);
			DAQinfor.lookupValue("NSample", daqconfig.NSample);
			DAQinfor.lookupValue("OnlinePedestalSubtraction",
					daqconfig.OnlinePedestalSubtraction);
		} catch (libconfig::SettingNotFoundException &npex) {
		}

		try {
			const libconfig::Setting &GEMDetectorConfig = GEMConfig["GEMDetectorConfig"];
			GEMDetectorConfig.lookupValue("GEMMapFile",gemconfig.mapfilename);
			GEMDetectorConfig.lookupValue("SpacialResolution",gemconfig.GEMSpacialResolution);
			GEMDetectorConfig.lookupValue("ReadOutStripBin",gemconfig.GEMReadoutBin);
		} catch (libconfig::SettingNotFoundException &npex) {
		}

	} catch (libconfig::SettingNotFoundException &nfex) {
	}
}

// test functions
void ConfigInfor::PrintConfig(){
	std::cout<<"GEMConfig::"<<std::endl
			<<"*=> MapFile   : "<<gemconfig.mapfilename<<std::endl
			<<"*=> Resolution: "<<gemconfig.GEMSpacialResolution<<std::endl
			<<"*=> Strip Bin : "<<gemconfig.GEMReadoutBin<<std::endl<<std::endl;

	std::cout<<"DAQConfig::"<<std::endl
			<<"*=> DAQVersion: "<<daqconfig.DAQVersion<<std::endl
			<<"*=>DataVersion: "<<daqconfig.DataStrcutureVersion<<std::endl
			<<"*=> TimeSample: "<<daqconfig.NSample<<std::endl
			<<"*=> OnlinePed : "<<daqconfig.OnlinePedestalSubtraction<<std::endl;
}
