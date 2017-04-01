/*
 * GEMCalibration.h
 *
 *	Autor: Siyu Jian
 *
 *	Notes: Multi-Layer GEM detector Calibration function
 *
 *		   Using the cosmic raw to calibrate the relative position of GEM detectors
 *		   output the distortion matrix
 */

#ifndef GEMCALIBRATION_H_
#define GEMCALIBRATION_H_

#include <vector>
#include <map>
#include <math.h>

#include "GEMCluster.h"
#include "Config.h"


class GEMCalibration {
public:
	GEMCalibration(std::vector<GEMCluster>);
	virtual ~GEMCalibration();

public:
	void SetTolerence();
	void GetCaliPosition();
	void CosmicCalibrate();

	void Sort(vector<GEMCluster> &vC);
	void Init();
	void Clean();    // delete all the global parameters

	void Test_report();
private:
	void DistortionMatrixIO();  // used for save the distortion matrix to the file or read the distortion matrix from configure file

public:
	std::vector<GEMCluster> vGEMCluster;
	// number of clusters per plane (the minimum number of x and y)
	long unsigned int vNCluster[kNMODULE];

	// cluster buffer
	std::vector<GEMCluster> vxCluster[kNMODULE];
	std::vector<GEMCluster> vyCluster[kNMODULE];

	// those part can be used for the efficiency test
	Bool_t vGoodTrackingFlag[kNMODULE];			// only 1. find one tracking, fit sucessfully, and the predicted dot is within the range, this is a good tracking

	Double_t vCorrectedPosX[kNMODULE];
	Double_t vCorrectedPosY[kNMODULE];
	Double_t vCorrectedPosZ[kNMODULE];

	Double_t vOriginalPosX[kNMODULE];
	Double_t vOriginalPosY[kNMODULE];
	Double_t vOriginalPosZ[kNMODULE];

	// predicted position for good fit
	Double_t vPredictedPosX[kNMODULE];	  // predicted positision of X
	Double_t vPredictedPosY[kNMODULE];	  // predicted positision of y
	Double_t vPredictedPosZ[kNMODULE];    // predicted positision of z

	double_t vCorrelationEffXY=0;
	double_t vCorrelationEffXZ=0;
	double_t vCorrelationEffYZ=0;
public:
	double_t cDetectorRangeMaxX[kNMODULE];
	double_t cDetectorRangeMaxY[kNMODULE];
	double_t cDetectorRangeMinX[kNMODULE];
	double_t cDetectorRangeMinY[kNMODULE];

private:

	//const unsigned int KNMINCHAMBER=3;		// minimum event requirement for a tracking (3)
	const unsigned int NFIREDCHAMBERS=3;    // at least 3 chamber are fired in a tracking
	const unsigned int DETECTOR_RESOLUTION_SIGMA=1;
	// test variables
	Bool_t Display_flag=1;
};

#endif /* GEMCALIBRATION_H_ */
