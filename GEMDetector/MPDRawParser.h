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
class MPDRawParser {
public:
	MPDRawParser();
	virtual ~MPDRawParser();

	void LoadRawData(std::vector<uint32_t>::iterator begin,std::vector<uint32_t>::iterator end);
	std::map<int,std::vector<int>> GetDecoded();
	void  CModeSubtraction();
	std::map<int,std::vector<int>> GetCModeSubtraction();
private:
	std::map<int,std::vector<int>> mAPVRawSingleEvent;
	std::map<int,std::vector<int>> mPedestalTimeSample;  // data after common mode subtraction
};

#endif /* MPDRAWPARSER_H_ */
