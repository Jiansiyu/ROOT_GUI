/*
 * HRSOpticsLog.h
 *
 *  Created on: Oct 14, 2019
 *      Author: newdriver
 */

#ifndef HRSOPTICSLOG_H_
#define HRSOPTICSLOG_H_

#include "HRSOpticsGeneral.h"

enum HRSLogLevel{
	HRSLogFatal=999,
	HRSLogError=998,
	HRSLogWorning=997,
	HRSLogINFOR=996,
	HRSLogLevel1=1,
	HRSLogLevel2,
	HRSLogLevel3
};
class HRSOpticsLog: public HRSOpticsGeneral {
public:
	HRSOpticsLog();
	HRSOpticsLog(const HRSLogLevel,const char *log...);
	virtual ~HRSOpticsLog();
private:
	Bool_t Write2File();
	Bool_t LogPrintOut();
};

#endif /* HRSOPTICSLOG_H_ */
