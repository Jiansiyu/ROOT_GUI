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
class MPDRawParser {
public:
	MPDRawParser();
	virtual ~MPDRawParser();

	void LoadRawData(std::vector<uint32_t>::iterator begin,std::vector<uint32_t>::iterator end);
	void LoadRawData(const std::vector<uint32_t> &);

	std::map<int,std::vector<int>> GetDecoded();

	std::map<int,std::vector<int>> GetCommonModeSubtraction();
private:
	// UID   data
	std::map<int,std::vector<int>> mAPVRawSingleEvent;	 // raw data from dat file
	std::map<int,std::vector<int>> mCommonModeSubtractedEvent;
	std::map<int,std::vector<int>> mPedestalTimeSample;  // data after common mode subtraction


	void  CommonModeSubtraction();
	void clear();
};

#endif /* MPDRAWPARSER_H_ */
