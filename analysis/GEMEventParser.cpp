/*
 * GEMEventParser.cpp
 *
 *  Created on: Oct 8, 2018
 *      Author: newdriver
 */

#include "GEMEventParser.h"

GEMEventParser::GEMEventParser(TChain* chain):GEMTree(chain){
	// TODO Auto-generated constructor stub
	EvtID=0;
	entries=(int64_t) fChain->GetEntries();

}

Bool_t GEMEventParser::ReadBlock(){
	if(!fChain) return false;

}
GEMEventParser::~GEMEventParser() {
	// TODO Auto-generated destructor stub
}

