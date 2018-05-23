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
	void LoadGEMMap();
	void LoadGEMMapFile(std::string fname);
	void RegistAPV(GEM::apvMap);

public:
	std::vector<GEM::apvMap> apvs;

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
