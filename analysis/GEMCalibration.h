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

class GEMCalibration {
public:
	GEMCalibration();
	virtual ~GEMCalibration();

public:
	void SetTolerence();
	void GetCaliPosition();

	void Clean();    // delete all the global parameters

private:
	void DistortionMatrixIO();  // used for save the distortion matrix to the file or read the distortion matrix from configure file
};

#endif /* GEMCALIBRATION_H_ */
