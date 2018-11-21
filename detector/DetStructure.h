/*
 * DetStructure.h
 *
 *  Created on: Nov 21, 2018
 *      Author: newdriver
 */

#ifndef DETSTRUCTURE_H_
#define DETSTRUCTURE_H_
#include <stdio.h>
#include <math.h>
#include <TVector3.h>
struct DetHit{
public:
	DetHit(){};
	~DetHit(){};

private:
	int EvtID;
	int64_t uid;
	double_t channel;
	double_t value;
};

struct DetEvent{
	int EvtID;
	double_t Enegy;
	double_t Momentum;
	TVector3 Position;
	double_t SharpingTime;
	double_t HitTime;
};
#endif /* DETSTRUCTURE_H_ */
