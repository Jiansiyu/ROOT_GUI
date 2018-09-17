/*
 * MPDRawParser.h
 *
 *  Created on: Jul 5, 2018
 *      Author: newdriver
 */

#ifndef MPDRAWPARSER_H_
#define MPDRAWPARSER_H_
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include "GEMEventData.h"

class MPDRawParser {
public:
	MPDRawParser();
	virtual ~MPDRawParser();
private:
//	virtual void GetDecoded(GEMEvent **,const std::vector<uint32_t> &);
//	virtual void GetDecoded(GEMEvent **,std::vector<uint32_t>::iterator begin,std::vector<uint32_t>::iterator end);
//	virtual void GetZeroSubtrData(GEMEvent *);
//	virtual void GetCommonModeSubtrData(GEMEvent *);

};

#endif /* MPDRAWPARSER_H_ */
