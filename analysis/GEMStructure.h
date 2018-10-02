/*
 * GEMStructure.h
 *
 *  Created on: Oct 2, 2018
 *      Author: Siyu
 */

#ifndef GEMSTRUCTURE_H_
#define GEMSTRUCTURE_H_
#include "GEMCluster.h"
#include <vector>
#include <map>


struct ClusterPair{
	ClusterPair(){};
	ClusterPair(int moduleid,GEMCluster x_in, GEMCluster y_in){
		module=moduleid;
		x=x_in;
		y=y_in;
	}

	void AddCluster(GEMCluster x_in, GEMCluster y_in){
		x=x_in;
		y=y_in;
	}

	int module;
	GEMCluster x;
	GEMCluster y;
};



#endif /* GEMSTRUCTURE_H_ */
