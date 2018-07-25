/*
 * SysMsgCenter.h
 *
 *  Created on: Jun 3, 2018
 *      Author: newdriver
 */

#ifndef USERSYSMSGCENTER_H_
#define USERSYSMSGCENTER_H_
#include <string>
#include <ctime>
//! Terminal color out put color defination

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


	void LogInfor(std::string);
	void LogInfor(std::string *);
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

private:
	std::string getOsName()
	{
	    #ifdef _WIN32
	    return "Windows 32-bit";
	    #elif _WIN64
	    return "Windows 64-bit";
	    #elif __unix || __unix__
	    return "Unix";
	    #elif __APPLE__ || __MACH__
	    return "Mac OSX";
	    #elif __linux__
	    return "Linux";
	    #elif __FreeBSD__
	    return "FreeBSD";
	    #else
	    return "Other";
	    #endif
	};

};

#endif /* SYSMSGCENTER_H_ */
