/*
 * GEMConfigure.h
 *
 *  Created on: Jul 12, 2018
 *      Author: newdriver
 */

#ifndef GEMCONFIGURE_H_
#define GEMCONFIGURE_H_
#include "iostream"
#include "GEMStructure.h"
#include "../src/GlobalStructure.h"

class GEMConfigure {

public:
	GEM::apvMapping GetMapping(){return mapping;};
	GlobStruct::SysConfig GetSysCondfig(){return DetConfig;};

public:
	void LoadNewMapping(std::string fname){
		MappingParser(fname.c_str());
	}


private:
	void DetConfigParser(std::string);
	void MappingParser(std::string);
	void GUIcfgParser(std::string);
private:
	GEM::apvMapping mapping;
	GlobStruct::SysConfig DetConfig;


//+++++++++++++++++++++++++++++++++++
public:
	static GEMConfigure *GetInstance();
private:
	static GEMConfigure *m_instance;
	GEMConfigure();
	virtual ~GEMConfigure(){
		std::cout <<"Deconstruct the singleton function: "<<__FUNCTION__<<std::endl;
	}
	class Garbo{
	public:
		~Garbo(){
					if(GEMConfigure::m_instance)
						delete GEMConfigure::m_instance;
				}
	};
	static Garbo garbo;
};

#endif /* GEMCONFIGURE_H_ */
