/*
 * GEMEvent.h
 *
 *  Created on: Oct 2, 2018
 *      Author: newdriver
 */

#ifndef GEMEVENT_H_
#define GEMEVENT_H_
#include <cstdint>
#include <map>
#include <algorithm>
#include "GEMCluster.h"
#include "GEMHit.h"

class GEMEvent {
public:
	GEMEvent();
	GEMEvent(TObjArray &hitindex);
	virtual ~GEMEvent();
	void AddCluster(TObjArray &hitindex);
	uint16_t ClusterMatch();

private:
	std::map<GEMCluster, GEMCluster> mMacthedCluster;

};

#endif /* GEMEVENT_H_ */
