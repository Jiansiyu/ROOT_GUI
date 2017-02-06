/*
 * input_handler.h
 *
 *  Created on: Nov 22, 2016
 *      Author: newdriver
 */

#ifndef INPUT_HANDLER_H_
#define INPUT_HANDLER_H_

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <cassert>

#include <TH1F.h>
#include <TCanvas.h>
#include <TFile.h>

#include <stdio.h> //for getchar()

// evio
#include "evioUtil.hxx"
#include "evioFileChannel.hxx"

// user defined function
#include "raw_decoder.h"

class InputHandler {
public:
	InputHandler(string str);

	virtual ~InputHandler();

	//int ProcessAllEvents(int evtID = -1);
	int RawProcessAllEvents(int entries=0);
	// mpd      apv       all the timesample and the data
	map<int, map<int, std::vector<int>>> RawProcessAllEvents(int entries, string a="gui");  // used for gui mode
	map<int,map<int, map<int, std::vector<int> > > > RawProcessAllEvents(int entries , map<int,map<int, map<int, std::vector<int> > > > &);
	// detector_ID, MPD   APV        strips

	// pedestal process function
	int PedProcessAllEvents(int entries=0, string pedestal_file_name="");			// process certain number data
	int PedProcessAllEvents(string pedestal_file_name="");							// process all data

	// zero subtraction method
	int ZeroSProcessAllEvents(int entries=0, string pedestal_file_name="");
	map<int,map<int,int>> ZeroSProcessAllEvents(int entries,string gui,string pedestal_file_name="");  // used for GUI mode
	map<int,map<int,map<int,int> > > ZeroSProcessSingleEvents(int entries);   // used for multi-detector mode

	// hit process functions
	int HitProcessAllEvents(int entries=0, string pedestal_file_name="", string root_file_name="");
	int HitProcessAllEvents(string pedestal_file_name="", string root_file_name="");


	int RMS_Mean_ProcessAllEvents(int entries,string pedestal_file_name="",string root_file_name="");  //used for calculate the RMS and Means of each chennals of the raw data

	string SetMapping(string);
	// add the test functions
	int TestFunction();

private:
	//                   MPDID  ADC_id  N-strips, Six Time-sample
	int FindCrossTalk(map<int, map<int, map<int, vector<int> > > > sEvent_input, map<int, map<int, map<int, vector<int> > > > *sRemoveCrossTalk,map<int, map<int, map<int, vector<int> > > > *sCrossTalk);
	int FindCrossTalk(map<int, map<int, vector<int> > > sEvent_input,map<int, map<int, vector<int> > > *sRemoveCrossTalk,map<int, map<int, vector<int> > > *sCrossTalk);
	int FindCrossTalk(map<int, vector<int> > sEvent_input,map<int, vector<int> > *sRemoveCrossTalk,map<int, vector<int> > *sCrossTalk);
	// THE address correlation  events input return non-cross-talk  return cross talk
	int FindCrossTalk(map<int,int> Address_Correlation,map<int, int> sEvent_input, map<int, int> *sRemoveCrossTalk, map<int, int> *sCrossTalk);

private:
	vector<int> vSRSSingleEventData;
	map<int, map<int, TH1F*> > mAPVRawHistos;
	map<int, map<int, vector<int> > > mAPVRawTSs;

	// MPD_ID, ADC_channels(APV) stripsNB   six timesample
	map<int, map<int, map<int, vector<int> > > > mTsAdc;     //adc for each strip after common mode subtraction
	map<int, map<int, map<int, TH1F* > > > mPedestalHisto;
	map<int, map<int, TH1F*> > mPedestalMean;
	map<int, map<int, TH1F*> > mPedestalRMS;
	TFile *f;

	string filename;
	ifstream file;
	string vDefaultMappingPath="Mapping/temp_Mapping.cfg";

	RawDecoder *fRawDecoder;
};

#endif /* INPUT_HANDLER_H_ */
