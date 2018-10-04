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
	int LayerMapping[12] = { 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3 };// the mapping for the GEMID and LayerID;
	double ModuleSizeX[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double ModuleSizeY[12] = { 0, 51.2, 102.4, 0, 51.2, 102.4, 0, 51.2, 102.4, 0,51.2, 102.4 };
	double ModuleSizeZ[12] = {
			   76,    76,     76,
			    0,     0,      0,
			115.9, 115.9,  115.9,
			40.4,   40.4,    40.4
	            };
	double vLayerPositionZ[4]={76.0,0,115.9,40.4};
};

#endif /* GEMLAYER_H_ */
