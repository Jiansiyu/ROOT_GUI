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
#include "GEMConfigure.h"

class MPDDecoder : public GUIInforCenter {
public:
	MPDDecoder();
	MPDDecoder(std::string fname);
	virtual ~MPDDecoder();

	void LoadFile(std::string fname);
	void PedestalMode(std::string);
	void HitMode(std::string,std::string);
	void HiModeTest(std::string,std::string);
	void RawDisplay(uint evtid);
	void HitDisplay(std::string,int);

private:

	void LoadPedestalFile();  // load and convert to array to increase efficiency
	void LoadMappingfile();   // load mapping file, and convert the file to array to increase efficiency
	void ZeroSubtraction();
	int ChNb[128];
	void Initialize();
private:
	std::string rawfilename;
	evio::evioFileChannel *chan;
	vector<uint32_t> block_vec_mpd;
	std::map<int,GUICanvasDataStream*> Rawstream;
	GEMConfigure *gemConfig= GEMConfigure::GetInstance();;
	bool ReadBlock();
	void clear();
private:
	std::map<int/*evtid*/,std::map<int/*mpdid*/,std::vector<TH1F *>>> rawHistoBuffer;

// GUI interface
private:
	void Progressbar(int current); // control the progress bar
	void Informationbar(std::string infor);
	void Canvas();
};

#endif /* MPDDECODER_H_ */
