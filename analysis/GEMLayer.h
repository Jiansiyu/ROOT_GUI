/*
 * GEMLayer.h
 *
 *  Created on: Oct 2, 2018
 *      Author: Siyu
 */

#ifndef GEMLAYER_H_
#define GEMLAYER_H_
#include "GEMStructure.h"
#include "map"
class GEMLayer {
public:
	GEMLayer();
	GEMLayer(ClusterPair &);
	virtual ~GEMLayer();
	int AddClusterPair(ClusterPair &);
	int Layer;
	double PositionX;
	double PositionY;
	double PositionZ;
	double total_Charge;
	double Peak_Charge;
	ClusterPair pair;
private:
	void CalculatePosition(); // calculate the position according to the input cluster
	int LayerMapping[]={0,0,0,1,1,1,2,2,2,3,3,3};   // the mapping for the GEMID and LayerID;
	int ModuleSizeX[]={};
	int ModuleSizeY[]={};
};

#endif /* GEMLAYER_H_ */
