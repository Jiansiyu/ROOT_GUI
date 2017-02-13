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

enum RemoveCrossTalkMethod {
	RMTC_REGULER,
	RMTC_PEAKSEARCH
};

class APVCrossTalkSearch {
public:
	// single APV crosstalk search function
	APVCrossTalkSearch();
	APVCrossTalkSearch(TH1F *);
	APVCrossTalkSearch(std::vector<int>);
	APVCrossTalkSearch(std::map<int,int>);

	// for SingleSample Usage
	APVCrossTalkSearch(std::map<int,std::map<int,int> >);     // tsample  stripsID ADC
	APVCrossTalkSearch(std::map<int,std::vector<int> >);	  // StripsID  tsample<ADC>

	virtual ~APVCrossTalkSearch();
	// data return
public:
	// those set the raw data function will erease all the data before, and load the new data,
	// but all those function will keep all the old mappings until set the new mapping with APVMapping function
	void SetRawAPVData(TH1F *);
	void SetRawAPVData(std::vector<int>);
	void SetRawAPVData(std::map<int,int>);

	void SetRawAPVData(std::map<int,std::map<int,int > >);
	void SetRawAPVData(std::map<int,std::vector<int> >);   // SingleEvent Usage
	std::map<int,int> APVMapping(std::map<int,int>);    // initial strips, real strips

	int SingleSampleRun();								            // after set new calculation method. this function must call afterward to get the result
	int SingleEventRun();
	int CrossTalkRemoveMethod(std::string);             //method

	std::map<int,int>GetSingleSampleCrossTalkData();						//
	std::map<int,int>GetSingleSampleRemoveCrossTalkData();

	std::map<int, std::map<int,int>> GetSingleEventCrossTalkData();  		 //
	std::map<int, std::map<int,int>> GetSingleEventRemoveCrossTalkData();

	// cross-talk searching engines
private:
	//the key point for here is that, for low event rate case, only the adjacent strips could be crosstalk.
	//for low rate case, it is less likely the adjacent strip to be a real events
	// maybe for future, can calculate the capacitance first, and use this capacitance to calculate the cross talk candidate
	int fRemoveCrosstalk();   // select the highest peak in the histogram
	int initilize();
	int Massage(std::string MSG_TYPE, std::string MSG);
private:

	std::vector<std::map<int,int>> FindPeaks(std::map<int,int>);   // find the connected peaks from the data after zerosubtraction
	//Intermediate data
private:

	std::map<int,int> cAPVMapping;    		// stripsNB, real strips position1
	std::map<int,int> Pos_str_mapping; 		//position to strips mapping
	const int CStripsNumber = 128;			// just used for the automap generating function   for INFN V4 cards
	std::string cRemoveTalkMethod="regular";

	// final data
private:
	// stripsID ADC
	std::map<int,int> vAPVInputData;
	std::map<int,int> vAPVCrossTalkData;
	std::map<int,int> vAPVRemoveCrossTalkData;   //

	// SampleID   stripsID ADC
	std::map<int,std::map<int,int>> vAPVSingleEventInputData;
	std::map<int,std::map<int,int>> vAPVSingleEventCrossTalkData;
	std::map<int,std::map<int,int>> vAPVSingleEventRemoveCrossTalkData;   //
	std::map<int,std::map<int,int>> cSingleSampleAPVMapping;			  //

};

#endif /* APVCROSSTALKSEARCH_H_ */
