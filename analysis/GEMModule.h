/*
 * GEMModule.h
 *
 *  Created on: Oct 2, 2018
 *      Author: Siyu
 */

#ifndef GEMMODULE_H_
#define GEMMODULE_H_
#include "vector"
#include "map"
#include "algorithm"
#include "cstdint"
#include "GEMCluster.h"
#include "GEMStructure.h"
class GEMModule {
public:
	GEMModule();
	virtual ~GEMModule();
	int module;
	std::vector<ClusterPair> MatchedClusters;
	std::vector<GEMCluster > gostCluster;
	std::vector<GEMCluster > vxCluster;
	std::vector<GEMCluster > vyCluster;
	uint32_t MatchCluster(std::vector<GEMCluster> &);
private:

};

#endif /* GEMMODULE_H_ */
