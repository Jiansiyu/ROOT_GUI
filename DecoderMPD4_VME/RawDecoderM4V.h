/*
 * RawDecoderM4V.h
 *
 *  Created on: May 24, 2018
 *      Author: newdriver
 */

#ifndef RAWDECODERM4V_H_
#define RAWDECODERM4V_H_

#include <TH1F.h>
#include <TCanvas.h>
#include <TFile.h>

#include <map>
#include <vector>
#include <stdio.h>
#include <TH1F.h>

class RawDecoderM4V {
public:
	RawDecoderM4V(std::vector<uint32_t> &, int, int);
	virtual ~RawDecoderM4V();

	// mpdID      ADCid
	std::map<int, std::map<int, std::vector<int>>> GetAPVRawData() {
		return mAPVRawData;
	}
	;
	std::map<int, std::map<int, TH1F*>> GetAPVRawHisto() {
		return mAPVRawHisto;
	}
	;
private:
	unsigned int * buf;
	int fBuf;
	std::map<int, std::map<int, std::map<int, std::vector<int>>>> mAPVTSRawData;
	//            mpd     adc             129*time sample
	std::map<int, std::map<int, std::vector<int>>> mAPVRawData;
	std::map<int, std::map<int, TH1F*>> mAPVRawHisto;
	int ChNb[128] = { 1, 33, 65, 97, 9, 41, 73, 105, 17, 49, 81, 113, 25, 57,
			89, 121, 3, 35, 67, 99, 11, 43, 75, 107, 19, 51, 83, 115, 27, 59,
			91, 123, 5, 37, 69, 101, 13, 45, 77, 109, 21, 53, 85, 117, 29, 61,
			93, 125, 7, 39, 71, 103, 15, 47, 79, 111, 23, 55, 87, 119, 31, 63,
			95, 127, 0, 32, 64, 96, 8, 40, 72, 104, 16, 48, 80, 112, 24, 56, 88,
			120, 2, 34, 66, 98, 10, 42, 74, 106, 18, 50, 82, 114, 26, 58, 90,
			122, 4, 36, 68, 100, 12, 44, 76, 108, 20, 52, 84, 116, 28, 60, 92,
			124, 6, 38, 70, 102, 14, 46, 78, 110, 22, 54, 86, 118, 30, 62, 94,
			126 }; //this is original and believed to be the right one,Jan 09 2017

private:
	void Decoder();
	//  Timesamples      128 channels
	std::map<int,std::vector<int>> SeperateSamples(std::vector<int>);
	//   MPDID         ADCid       Timesamples      128 channels
	std::map<int,std::map<int,std::map<int,std::vector<int>>>> SeperateSamples(std::map<int, std::map<int, std::vector<int>>>);
public:
	std::map<int,std::map<int,std::map<int,std::vector<int>>>> GetStripTsAdcMap();
};

#endif /* RAWDECODERM4V_H_ */
