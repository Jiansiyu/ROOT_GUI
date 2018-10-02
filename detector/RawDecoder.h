/*
 * RawDecoder.h
 *
 *  Created on: Sep 15, 2018
 *      Author: newdriver
 */

#ifndef RAWDECODER_H_
#define RAWDECODER_H_

#include <TApplication.h>
#include <vector>
#include <string>
#include "evioUtil.hxx"
#include "evioFileChannel.hxx"

#include "../GEMDetector/GEMEventData.h"

class RawDecoder {
public:
	RawDecoder();
	virtual ~RawDecoder();

	Bool_t Read();       // read block data
	Bool_t OpenFile(const std::string);
	Bool_t OpenFile(const std::vector<std::string>);
	std::vector<uint32_t> const GetRawBlock();
	GEMRaw *GEMRawParser(std::vector<uint32_t> const value = std::vector<uint32_t>());
	GEMRaw *GEMRawParser(GEMRaw *,std::vector<uint32_t> value=std::vector<uint32_t>());
	GEMRaw *FillCommonModeSubtracted(GEMRaw *,const GEMRaw *);
	GEMRaw *FillZeroSubtraction(GEMRaw *,const GEMRaw *);

private:
	evio::evioFileChannel *efChan;
	std::vector<uint32_t> dDataBlock;
	std::vector<std::vector<uint32_t>> dRawDataEvent;    // seperate the data block by  event


};

#endif /* RAWDECODER_H_ */
