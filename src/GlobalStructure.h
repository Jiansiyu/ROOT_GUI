/*
 * GlobalStructure.h
 *
 *  Created on: May 21, 2018
 *      Author: newdriver
 */

#ifndef GLOBALSTRUCTURE_H_
#define GLOBALSTRUCTURE_H_

#include <string>
#include <vector>

namespace GlobStruct {

struct GEMConfig {
	std::string mapfilename;
	double GEMSpacialResolution;
	double GEMReadoutBin;
};

struct DAQConfig{
	std::string DAQVersion;
	double DataStrcutureVersion;
	int NSample;			// how many time samples
	bool OnlinePedestalSubtraction;
};

}

#endif /* GLOBALSTRUCTURE_H_ */
