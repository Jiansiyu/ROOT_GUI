/*
 * HRSOpticsCut.cpp
 *
 *  Created on: Oct 13, 2019
 *      Author: newdriver
 */

#include "HRSOpticsCut.h"

HRSOpticsCut::HRSOpticsCut() {
	// TODO Auto-generated constructor stub

}

HRSOpticsCut::~HRSOpticsCut() {
	// TODO Auto-generated destructor stub
}


void HRSOpticsCut::EmbededCanvasConfig(){

}

void HRSOpticsCut::PlotToCanvas(){

}

void HRSOpticsCut::ReadCanvasConfig(){

}

Bool_t HRSOpticsCut::AddCustomizedCut(std::string plotItems,TCut *cut){

	return kTRUE;
}

Bool_t HRSOpticsCut::EnableDpCut(){
	return kTRUE;
}

Bool_t HRSOpticsCut::EnableVzCut(){
	return kTRUE;
}

Bool_t HRSOpticsCut::EnableSieveCut(){
	return kTRUE;
}
Bool_t HRSOpticsCut::SaveCut(){
	return kTRUE;
}
