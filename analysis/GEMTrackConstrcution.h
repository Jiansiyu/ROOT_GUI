/*
 * GEMTrackConstrcution.h
 *
 *: used for constrution of the tracking,
 *: originally used for the efficiency test, predicted the position in the last detector
 *:
 *:
 *: usage:
 *: 	0>
 *: 	1> check vGoodTrackingFlag, whether this is a good tracking
 *: 	2> check whether on certain chamber, the event is detected
 */

#ifndef GEMTRACKCONSTRCUTION_H_
#define GEMTRACKCONSTRCUTION_H_

#include <vector>
#include "GEMCluster.h"
#include "Config.h"


// used for the GEM detector 3D position
typedef struct {
	Double_t X_Pos;
	Double_t Y_Pos;
	Double_t Z_Pos;
} Position_3D;

// used for the GEM detector 2D position
typedef struct {
	Double_t X_Pos;
	Double_t Y_Pos;
} Position_2D;

class GEMTrackConstrcution {
public:
	GEMTrackConstrcution(std::vector<GEMCluster>);
	virtual ~GEMTrackConstrcution();

	void CosmicEff(int Detector_ID=-1);
	Bool_t SetDisplay(Bool_t);
	void Clean();

	void Sort(vector<GEMCluster> &vC);
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

	Bool_t vWithInRange[kNMODULE]; 		// the predicted position is with in the range of the effective range of GEM detector
	Bool_t vEventDetected[kNMODULE];    // if on the certain chamber there is effective signal detected

	// used for the fitting of the tracking
	Bool_t vChiSquareFlag[kNMODULE];   // whether the tracking fitting is a good fit, (but not sure all the point is located in the chamvber effective area)
	Double_t ChiSquareTHRD=50;         // the criteria for the fit chisquare


private :
	Position_3D fPositionCorrection(int DetectorID,Position_3D Initial_Pos);   // us the correction matrix to calculate the corrected position
	void SingleTrackFilter(unsigned int);    // selecte the events that all the chamber except the given chamber have effective event
	void Msg();

	// test function
	void TestFunctions();
	void ResultReport();

private:
	Bool_t Display_flag=0;

};

#endif /* GEMTRACKCONSTRCUTION_H_ */
