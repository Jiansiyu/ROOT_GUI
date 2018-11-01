/*
 * GEMAnalysis.h
 *
 *  Created on: Sep 19, 2018
 *      Author: newdriver
 */

#ifndef GEMANALYSIS_H_
#define GEMANALYSIS_H_
#include <string>
#include <vector>
#include <mutex>
class GEMAnalysis {
public:
	GEMAnalysis();
	virtual ~GEMAnalysis();
	void Analysis(std::string const);
	void AnalysisMT(std::vector<std::string> const );
	void TrainingData(std::string const);
	std::mutex thread_mutex;
};

#endif /* GEMANALYSIS_H_ */
