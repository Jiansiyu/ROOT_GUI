/*
 * GEMSystem.h
 *
 *  Created on: Oct 5, 2018
 *      Author: newdriver
 */

#ifndef GEMSYSTEM_H_
#define GEMSYSTEM_H_
#include "vector"
#include "map"
#include "GEMStructure.h"
#include "GEMHit.h"
#include "GEMCluster.h"
#include <unordered_map>
class GEMSystem {

public:
	GEMSystem();
	virtual ~GEMSystem();
	virtual void Reconstruct(std::vector<GEMHit> const &); // load the raw hit file from the constructed file
	void Reconstruct();
	void GetEvents();
    void GetLayerList();

private:
	virtual std::vector<GEMCluster> clusterSearch(std::vector<GEMHit> const &hits);
	//virtual std::vector<GEMCluster> clusterMatch(std::unordered_map<int16_t,std::unordered_map<int8_t,std::vector<GEMCluster>>> const &);
	//virtual std::vector<GEMCluster> clusterMatch(std::vector<GEMCluster> const &);

	void trackingSearch();
	void detLayerMatch();
	void hitDecoder();
	void hitMatch();

	/*Used for do the position correction algorithm */
	GEMCluster clusterPositionCorrection(GEMCluster & hit);
    GEMHit  hitPositionCorrention(GEMCluster & cluster);

    /*Used for the cut*/
    Bool_t trackingCut();
	Bool_t clusterCut();
    Bool_t hitCut();

private:
    std::vector<GEMCluster> findcluster(std::vector<GEMHit> &);
};

#endif /* GEMSYSTEM_H_ */
