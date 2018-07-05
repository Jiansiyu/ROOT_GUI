/*
 * GEMIDGenerator.h
 *
 *  Created on: Jul 5, 2018
 *      Author: newdriver
 */

#ifndef GEMIDGENERATOR_H_
#define GEMIDGENERATOR_H_

namespace GEM{

// uid componet
// [Crate/SSPID],MPDID,ADCID,ChannelID
// channelID 7 bit
// ADCID     4 bit
// MPDID     7 bit
// CrateID   8 bit
// need the 0xff term to be the broad mode, to select all the MPDs on the Crate \
or all the apvs on the MPDs

#define GEM_CHANNELID_SIZE 8
#define GEM_ADCID_SIZE     8
#define GEM_MPDID_SIZE     8
#define GEM_CRATEID_SIZE   8

#define GEM_CHANNELID_SHIFT 0
#define GEM_ADCID_SHIFT     GEM_CHANNELID_SIZE
#define GEM_MPDID_SHIFT     GEM_CHANNELID_SIZE+GEM_ADCID_SIZE
#define GEM_CRATEID_SHIFT   GEM_CHANNELID_SIZE+GEM_ADCID_SIZE+GEM_MPDID_SIZE

#define GEM_CHANNELID_POS  0xff
#define GEM_ADCID_POS	   0xff<<GEM_ADCID_SHIFT
#define GEM_MPDID_POS	   0xff<<GEM_MPDID_SHIFT
#define GEM_CRATEID_POS    0xff<<GEM_CRATEID_SHIFT

// @param CrateID/SSPid: used for the crate identification
// @param mpdID
// @param adcID
// @param channelID
// if set the param to -1 means select all the modules
template<class T>
T GetUID(T CrateID, T mpdID, T adcID, T channelID) {
	if (CrateID == -1) {
		CrateID = GEM_CRATEID_POS >> GEM_CRATEID_SHIFT;
	}
	if (mpdID == -1) {
		mpdID = GEM_MPDID_POS >> GEM_MPDID_SHIFT;
	}
	if (adcID == -1) {
		adcID = GEM_ADCID_POS >> GEM_ADCID_SHIFT;
	}
	if (channelID == -1) {
		channelID = GEM_CHANNELID_POS >> GEM_CHANNELID_SHIFT;
	}
	return ((CrateID << GEM_CRATEID_SHIFT) | (mpdID << GEM_MPDID_SHIFT)
			| (adcID << GEM_ADCID_SHIFT)) | channelID;
}
template<class T>
T getMPDID(T uid) {
	return (uid & GEM_MPDID_POS) >> GEM_MPDID_SHIFT;
}

template<class T>
T getADCID(T uid) {
	return (uid & GEM_ADCID_POS) >> GEM_ADCID_SHIFT;
}

template<class T>
T getChannelID(T uid) {
	return (uid & GEM_CHANNELID_POS) >> GEM_CHANNELID_SHIFT;
}

template<class T>
T getCrateID(T uid) {
	return (uid & GEM_CRATEID_POS) >> GEM_CRATEID_SHIFT;
}
}



#endif /* GEMIDGENERATOR_H_ */
