/*
 * GEMInforCenter.h
 *
 *  Created on: May 22, 2018
 *      Author: newdriver
 */

#ifndef GEMINFORCENTER_H_
#define GEMINFORCENTER_H_
#include <iostream>
#include "GEMStructue.h"
#include "ConfigInfor.h"
class GEMInforCenter {

public:
	GEM::GEMMap& LoadGEMMap(GEM::GEMMap &map){
		GEMMap = map;
		return map;
	};
	GEM::GEMMap& LoadGEMMapFile(std::string fname);


public:
	GEM::GEMMap GEMMap;

// test functions
private:

	void GEMMapCheck();

//++++++++++++++++++++++++++++++++++++++++++++++++
public:
	static GEMInforCenter *GetInstance();
private:
	static GEMInforCenter *m_instance;
	GEMInforCenter();
	virtual ~GEMInforCenter(){
		std::cout <<"Deconstruct the singleton function: "<<__FUNCTION__<<std::endl;
	}
	///Embedded function used for auto de-contrcut singletone class
	class Garbo{
	public:
		~Garbo(){
			if(GEMInforCenter::m_instance)
				delete GEMInforCenter::m_instance;
		}
	};
	static Garbo garbo;
};

#endif /* GEMINFORCENTER_H_ */
