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

struct AnalysisConfig{
	std::string Pedestalfname;
	std::string PedestalSavePattern;
	std::string HitSavePattern;
	bool DecodeWhenLoading;
	int ZeroSubtrCutSigma;
	bool RawStripPosCorrection;

};

struct SysConfig{
	GlobStruct::AnalysisConfig Analysis_cfg;
	GlobStruct::DAQConfig	   DAQ_cfg;
	GlobStruct::GEMConfig      GEM_cfg;
};

enum Dimension_3D {
	X,
	Y,
	Z,
	null
};
}

#endif /* GLOBALSTRUCTURE_H_ */
