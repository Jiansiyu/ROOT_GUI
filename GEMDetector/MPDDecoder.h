/*
 * MPDDecoder.h
 *
 *  Created on: Jul 5, 2018
 *      Author: newdriver
 */

#ifndef MPDDECODER_H_
#define MPDDECODER_H_
#include <string>

#include "evioUtil.hxx"
#include "evioFileChannel.hxx"
#include "MPDStructure.h"
#include "../src/GUIInforCenter.h"
class MPDDecoder : public GUIInforCenter {
public:
	MPDDecoder();
	MPDDecoder(std::string fname);
	virtual ~MPDDecoder();

	void LoadFile(std::string fname);
	void PedestalMode(std::string);
	void HitMode(std::string,std::string);
	void HitMode(std::string,std::string,std::string);
	void RawDisplay(uint evtid);
	void HitDisplay();

private:

	void LoadPedestalFile();  // load and convert to array to increase efficiency
	void LoadMappingfile();   // load mapping file, and convert the file to array to increase efficiency
	void ZeroSubtraction();
	int ChNb[128] = { 1, 33, 65, 97, 9, 41, 73, 105, 17, 49, 81, 113, 25, 57,
			89, 121, 3, 35, 67, 99, 11, 43, 75, 107, 19, 51, 83, 115, 27, 59,
			91, 123, 5, 37, 69, 101, 13, 45, 77, 109, 21, 53, 85, 117, 29, 61,
			93, 125, 7, 39, 71, 103, 15, 47, 79, 111, 23, 55, 87, 119, 31, 63,
			95, 127, 0, 32, 64, 96, 8, 40, 72, 104, 16, 48, 80, 112, 24, 56, 88,
			120, 2, 34, 66, 98, 10, 42, 74, 106, 18, 50, 82, 114, 26, 58, 90,
			122, 4, 36, 68, 100, 12, 44, 76, 108, 20, 52, 84, 116, 28, 60, 92,
			124, 6, 38, 70, 102, 14, 46, 78, 110, 22, 54, 86, 118, 30, 62, 94,
			126 }; // infn GEM APV card mapping

private:
	std::string rawfilename;
	evio::evioFileChannel *chan;
	vector<uint32_t> block_vec_mpd;
	bool ReadBlock();
	void clear();

	std::map<int/**/,GUICanvasDataStream*> Rawstream;

private:
	std::map<int/*evtid*/,std::map<int/*mpdid*/,std::vector<TH1F *>>> rawHistoBuffer;
// GUI interface
private:
	void Progressbar(int current); // control the progress bar
	void Informationbar(std::string infor);
	void Canvas();
};

#endif /* MPDDECODER_H_ */
