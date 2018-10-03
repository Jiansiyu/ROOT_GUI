/*
 * GEMLayer.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: Siyu
 */

#include "GEMLayer.h"

GEMLayer::GEMLayer() {
	Layer=-9999;
	// TODO Auto-generated constructor stub
	LayerMapping={0,0,0,1,1,1,2,2,2,3,3,3};
	ModuleSizeY={
			     0,51.2,102.4,
			     0,51.2,102.4,
				 0,51.2,102.4,
				 0,51.2,102.4
	             };
	ModuleSizeX={
			0,0,0,
			0,0,0,
			0,0,0,
			0,0,0
	             };
	// TODO
	ModuleSizeZ={0,0,0,0,0,0,0,0,0,0,0,0,0,0};

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

