/*
 * GEMLayer.h
 *
 *  Created on: Oct 2, 2018
 *      Author: newdriver
 */

#ifndef GEMLAYER_H_
#define GEMLAYER_H_
#include <map>
#include <vector>
#include "GEMHit.h"
#include "GEMCluster.h"

#include "GEMEvent.h"
class GEMLayer {
public:
	GEMLayer();
	virtual ~GEMLayer();
	std::map<int,GEMEvent> Layerevent;
private:
	void GetPosition(GEMCluster &); //  change the position directly
	void GetLayerPosition(GEMEvent &); // calculate  the position for one GEM event

};

#endif /* GEMLAYER_H_ */
