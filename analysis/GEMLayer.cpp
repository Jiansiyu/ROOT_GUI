/*
 * GEMLayer.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: Siyu
 */

#include "GEMLayer.h"

GEMLayer::GEMLayer() {
	Layer=-9999;
	PositionX=-999;
	PositionY=-999;
	PositionZ=-999;
	Peak_Charge=-9999;
	total_Charge=-999;
}

GEMLayer::~GEMLayer() {
	// TODO Auto-generated destructor stub
}

int GEMLayer::AddClusterPair(ClusterPair &cluster){
    int moduleid=cluster.module;
    int posX=cluster.x.Pos;
    int posY=cluster.y.Pos;
    Layer=LayerMapping[moduleid];
	PositionX=ModuleSizeX[moduleid]+posX;
	PositionY=ModuleSizeY[moduleid]+posY;
	PositionZ=ModuleSizeZ[moduleid];
    pair=cluster;
}

