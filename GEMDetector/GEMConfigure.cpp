/*
 * GEMConfigure.cpp
 *
 *  Created on: Jul 12, 2018
 *      Author: newdriver
 */
#include "string"
#include "sstream"
#include "fstream"
#include "algorithm"
#include <libconfig.h++>

#include "GEMConfigure.h"


GEMConfigure *GEMConfigure::m_instance=nullptr;
GEMConfigure *GEMConfigure::GetInstance(){
	if(m_instance==nullptr){
		m_instance=new GEMConfigure;
	}
	return m_instance;
}

void GEMConfigure::DetConfigParser(std::string fname="./cfg/DetConfig.cfg"){
	libconfig::Config cfg;
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
		//std::cout << "Store name: " << name << std::endl << std::endl;
	} catch (const libconfig::SettingNotFoundException &nfex) {
		std::cerr << "No 'name' setting in configuration file." << std::endl;
	}

	const libconfig::Setting &ConfigRoot=cfg.getRoot();

	try {
		const libconfig::Setting &GEMConfig = ConfigRoot["GEMConfig"];

		const libconfig::Setting &DAQinfor = GEMConfig["DAQinfor"];
		DAQinfor.lookupValue("DAQVersion", DetConfig.DAQ_cfg.DAQVersion);
		DAQinfor.lookupValue("DataStructureVersion",
				DetConfig.DAQ_cfg.DataStrcutureVersion);
		DAQinfor.lookupValue("NSample", DetConfig.DAQ_cfg.NSample);
		DAQinfor.lookupValue("OnlinePedestalSubtraction",
				DetConfig.DAQ_cfg.OnlinePedestalSubtraction);



		const libconfig::Setting &GEMDetectorConfig =
				GEMConfig["GEMDetectorConfig"];
		GEMDetectorConfig.lookupValue("GEMMapFile",
				DetConfig.GEM_cfg.mapfilename);
		GEMDetectorConfig.lookupValue("SpacialResolution",
				DetConfig.GEM_cfg.GEMSpacialResolution);
		GEMDetectorConfig.lookupValue("ReadOutStripBin",
				DetConfig.GEM_cfg.GEMReadoutBin);


		const libconfig::Setting &nch_array=cfg.lookup("GEMConfig.GEMDetectorConfig.stripMapping");
		DetConfig.Analysis_cfg.nch=new int[nch_array.getLength()];
		for(int n =0 ; n < nch_array.getLength();n++){
			DetConfig.Analysis_cfg.nch[n]=int(nch_array[n]);
		}


		const libconfig::Setting &AnalysisConfig = GEMConfig["DataAnalysisConfig"];

					AnalysisConfig.lookupValue("PedestalFname",DetConfig.Analysis_cfg.Pedestalfname);
					AnalysisConfig.lookupValue("PedestalSavePattern",DetConfig.Analysis_cfg.PedestalSavePattern);
					AnalysisConfig.lookupValue("HitSavePattern",DetConfig.Analysis_cfg.HitSavePattern);
					AnalysisConfig.lookupValue("ZeroSubtrCutSigma",DetConfig.Analysis_cfg.ZeroSubtrCutSigma);
					AnalysisConfig.lookupValue("DecodeWhenLoad",DetConfig.Analysis_cfg.DecodeWhenLoading);
					AnalysisConfig.lookupValue("RawStripPosCorrection",DetConfig.Analysis_cfg.RawStripPosCorrection);

	} catch (libconfig::SettingNotFoundException &nfex) {
		std::cout<<nfex.what()<<std::endl;
	}
}

void GEMConfigure::MappingParser(std::string fname=""){
	if(fname==""){
		if(DetConfig.GEM_cfg.mapfilename.empty()){
			std::cerr<<__FUNCTION__<<"["<<__LINE__<<"] DetConfig file need to be parsered first"<<std::endl;
			DetConfigParser();
			MappingParser(fname.c_str());
		}
		fname=DetConfig.GEM_cfg.mapfilename.c_str();
	}

	std::ifstream mapfile(fname.c_str(),std::ifstream::in);
	std::vector<std::string> lines;
	if(mapfile.good()){
		std::string line;
		while(getline(mapfile,line)){
			line.erase(std::remove_if(line.begin(),line.end(),::isspace),line.end());
			line = line.substr(0,line.find("#",0));
			line = line.substr(0,line.find("//",0));
			line = line.substr(0,line.find("!",0));
			if(line.size()==0) continue;
			lines.push_back(line);
		}
	}
	mapping.addAPVs(lines);
#ifdef __CONFIGURE_DEBUG_MODE
	mapping.Print();
#endif
}

void GEMConfigure::GUIcfgParser(std::string fname="./cfg/GUIConfig.cfg"){

}

GEMConfigure::GEMConfigure() {
//	// TODO Auto-generated constructor stub
	//std::cout<<"Initialize the singleton function:"<<std::endl;
	DetConfigParser("./cfg/DetConfig.cfg");
	MappingParser("");
}
//
//GEMConfigure::~GEMConfigure() {
//	// TODO Auto-generated destructor stub
//}
//
