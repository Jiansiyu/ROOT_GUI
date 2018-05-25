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
		if(EventRawHisto.find(evtID)!=EventRawHisto.end()){
			GEM::EventHistStruct *histstruct=new GEM::EventHistStruct(EventRawHisto[evtID]);
			Emit("GEMDrawRaw(GEM::EventHistStruct)",histstruct);
		}else{

		}

	}

	void Sendtest(){
		std::cout<<"Test send"<<std::endl;
		Emit("emittest(Int_t)",v);
	}
	Int_t v=5;
	// signal slot communication mechanism
	void emittest(Int_t);//*SIGNAL*
	void GEMDrawRaw(GEM::EventHistStruct);//*SIGNAL*
private:

	std::string RawDatfileName;
	std::vector<std::string> RawDatfileNameList;

	//<eventid, <mpdid, <apvid,<hist> > > >
	std::map<int,std::map<int, std::map<int,TH1F*>>> EventRawHisto;
	std::map<int,std::map<int, std::map<int,std::vector<int>>>> EventRaw;

	//ClassDef(GEMDataParserM4V,0);

};

#endif /* GEMDATAPARSERM4V_H_ */
