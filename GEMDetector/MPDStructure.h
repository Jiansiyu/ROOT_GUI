/*
 * MPDStructure.h
 *
 *  Created on: Jul 5, 2018
 *      Author: newdriver
 */

#ifndef MPDSTRUCTURE_H_
#define MPDSTRUCTURE_H_

#include <cstddef>
#include <cstdint>

namespace GEM{


enum DAQTag{
	MPD_tag  = 10,
	FADC_tag = 3,
	TDC_tag  = 6
};

enum MPD4_VME_RawTag{
	BLOCK_HEADER   = 0x0,
	BLOCK_TRAILER  = 0x1,
	EVENT_HEADER   = 0x2,
	TRIGGER_TIME   = 0x3,
	APV_CH_DATA    = 0x4,
	EVENT_TRAILER  = 0x5,
	CRATE_ID       = 0x6,
	FILLER_WORD    = 0x7
};
}




#endif /* MPDSTRUCTURE_H_ */
