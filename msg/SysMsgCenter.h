/*
 * SysMsgCenter.h
 *
 *  Created on: Jun 3, 2018
 *      Author: newdriver
 */

#ifndef SYSMSGCENTER_H_
#define SYSMSGCENTER_H_
#include <string>

//! Terminal color out put color defination
//
enum terminal_color{

};

enum PrintLevel{
	STATUS,
	WORNING,
	ERROR
};

class SysMsgCenter {
public:
	SysMsgCenter();
	virtual ~SysMsgCenter();

//++++++++++++++++++++++++++++++++
// The log book interface
	void PrintfLog(std::string);
	void PrintfLog(int);
	void PrintfLog(float);
//++++++++++++++++++++++++++++++++
// The GUI signal/slot interface
	void PrintfGUI(std::string);
	void PrintfGUI(int);
	void PrintfGUI();      // canvas & dashboard interface
//++++++++++++++++++++++++++++++++
// terminal display interface
	void Printf(std::string);


};

#endif /* SYSMSGCENTER_H_ */
