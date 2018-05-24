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

#include <TH1F.h>

class GEMDataParserM4V {

	RQ_OBJECT("GEMDataParserM4V");
public:
	GEMDataParserM4V();
	virtual ~GEMDataParserM4V();

	bool OpenFileIn(std::string);

private:
	std::string RawDatfileName;
	std::vector<std::string> RawDatfileNameList;

	//<eventid, <mpdid, <apvid,<hist> > > >
	std::map<int,std::map<int, std::map<int,TH1F>>> EventRaw;
	bool ParserRawDat();

};

#endif /* GEMDATAPARSERM4V_H_ */
