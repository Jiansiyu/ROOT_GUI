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
class MPDDecoder {
public:
	MPDDecoder();
	MPDDecoder(std::string fname);
	virtual ~MPDDecoder();

	void LoadFile(std::string fname);
	void PedestalMode();
	void HitMode();
	void RawDisplay();
	void HitDisplay();

private:

	void LoadPedestalFile();  // load and convert to array to increase efficiency
	void LoadMappingfile();   // load mapping file, and convert the file to array to increase efficiency
	void ZeroSubtraction();

private:
	evio::evioFileChannel chan;
	vector<uint32_t> *block_vec_mpd;
	bool ReadBlock();

// GUI interface
private:
	void Progressbar(int current); // control the progress bar
	void Informationbar(std::string infor);
	void Canvas();
};

#endif /* MPDDECODER_H_ */
