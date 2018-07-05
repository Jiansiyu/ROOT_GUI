/*
 * GEMConfigure.h
 *
 *  Created on: Jul 5, 2018
 *      Author: newdriver
 */

#ifndef GEMCONFIGURE_H_
#define GEMCONFIGURE_H_
#include <iostream>
#include <string>
#include "GEMStructure.h"
#include "../src/GlobalStructure.h"

class GEMConfigure {
public:
	GEM::apvMapping GetMapping(){return mapping;};
	GlobStruct::SysConfig GetConfigure(){return Config;};

private:
	GEM::apvMapping mapping;
	GlobStruct::SysConfig DetConfig;

private:
	void DetConfigParser(std::string);
	void MappingParser(std::string);
	void GUIcfgParser(std::string);
//++++++++++++++++++++++++++++++++++++++++++++++++
public:
	static GEMConfigure *GetInstance();
private:
	GEMConfigure();
	static GEMConfigure *m_instance;
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
