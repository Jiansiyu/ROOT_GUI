/*
 * APVCrossTalkSearch.h
 *
 *  Created on: Feb 4, 2017
 *      Author: newdriver
 */

#ifndef APVCROSSTALKSEARCH_H_
#define APVCROSSTALKSEARCH_H_
#include "TH1F.h"
#include "vector"
#include "map"
#include "string"

class APVCrossTalkSearch {
public:
	// single APV crosstalk search function
	APVCrossTalkSearch();
	APVCrossTalkSearch(TH1F *);
	APVCrossTalkSearch(std::vector<int>);
	APVCrossTalkSearch(std::map<int,int>);

	virtual ~APVCrossTalkSearch();
	// data return
public:
	void SetRawAPVData(TH1F *);
	void SetRawAPVData(std::vector<int>);
	void SetRawAPVData(std::map<int,int>);
	std::map<int,int> APVMapping(std::map<int,int>);    // initial strips, real strips

	int CrossTalkRemoveMethod(std::string); //method
	//int APVMapping(std::map<int,int>);

	std::map<int,int>GetCrossTalkData();
	std::map<int,int>GetRemoveCrossTalkData();

	// cross-talk searching engines
private:
	//the key point for here is that, for low event rate case, only the adjacent strips could be crosstalk.
	//for low rate case, it is less likely the adjacent strip to be a real events
	// maybe for future, can calculate the capacitance first, and use this capacitance to calculate the cross talk candidate
	int fRemoveCrosstalk();   // select the highest peak in the histogram

private:

	std::vector<std::map<int,int>> FindPeaks(std::map<int,int>);   // find the connected peaks from the data after zerosubtraction


	//Intermediate data
private:

	std::map<int,int> cAPVMapping;    // stripsNB, real strips position1
	std::map<int,int> Pos_str_mapping; //position to strips mapping
	const int CStripsNumber = 128;
	std::string cRemoveTalkMethod="regular";

	// final data
private:
	std::map<int,int> vAPVInputData;
	std::map<int,int> vAPVCrossTalkData;
	std::map<int,int> vAPVRemoveCrossTalkData;   //

};

#endif /* APVCROSSTALKSEARCH_H_ */
