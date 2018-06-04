/*
 * GEMDataParserM4V.h
 *
 *  Created on: May 22, 2018
 *      Author: newdriver
 */

#ifndef GEMDATAPARSERM4V_H_
#define GEMDATAPARSERM4V_H_

// used for slot and signal communication
#include <TQObject.h>
#include <RQ_OBJECT.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "iostream"
#include <TH1F.h>

#include "../src/GEMStructue.h"

class GEMDataParserM4V {

	RQ_OBJECT("GEMDataParserM4V");
public:
	GEMDataParserM4V();
	virtual ~GEMDataParserM4V();

	bool OpenFileIn(std::string);
	bool ParserRawDat();
	void DrawRawDisplay(int evtID){
		if(EventRaw.find(evtID)!=EventRaw.end()){
			GEM::EventRawStruct *raw=new GEM::EventRawStruct(EventRaw[evtID]);
			std::cout<<"emmit :"<<raw->raw.size()<<std::endl;
			Emit("GEMDrawRaw(GEM::EventRawStruct)",raw);
		}else{
			std::cout<<"ERROR :"<<__FUNCTION__<<std::endl;
		}

	}
	void HitMode(std::string,std::string,std::string);
	void PedestalMode(std::string fname,std::string savename);
	std::map<int,std::map<int,std::map<std::string,std::vector<int>>>> LoadPedestal(std::string);
private:
	// signal slot communication mechanism
	void GEMDrawRaw(GEM::EventRawStruct);//*SIGNAL*
	void GEMDrawZeroSub();               //*SIGNAL*
	void ProgressStatus(int);			 //*SIGNAL*


private:
	std::string RawDatfileName;
	std::vector<std::string> RawDatfileNameList;

	//<eventid, <mpdid, <apvid,<hist> > > >
	std::map<int,std::map<int, std::map<int,TH1F*>>> EventRawHisto;
	//       eventid       MPD         ADC
	std::map<int,std::map<int, std::map<int,std::vector<int>>>> EventRaw;

	//ClassDef(GEMDataParserM4V,0);

};

#endif /* GEMDATAPARSERM4V_H_ */
