/*
 * GUIInformation.h
 *
 *  Created on: Sep 13, 2018
 *      Author: newdriver
 *      notes: Singleton function that used for pass the GUI current settings.
 *      	   Also it can be used for set the default values for the canvas.
 *      dev plan: * pass the GUI setting to the analysis code
 *      		  * set and update the status of the code
 *      		  *
 */

#ifndef GUIINFORMATION_H_
#define GUIINFORMATION_H_
#include "string"
#include "vector"
#include "GUIStructure.h"
#include  "GUISignalSlot.h"

class GUIInformation {

public:
	void SetRunMode(GUIWorkMode);  // set and get the run mode
	void SetPedestalInputFile(std::string);
	void SetRawFileInputList_add(std::vector<std::string>);
	void SetRawFileInputList_remove(std::vector<std::string>);

	GUIWorkMode GetRunMode();
	std::string *GetPedestalInputFile();
	std::vector<std::string> *GetRawFileInputList();

private:
	// control panel informations
	GUIWorkMode iWorkmode;
	GUIDisplaySetting iSetting;
	std::string iPedestal;
	std::vector<std::string> iRawFileList;
	std::string iOutputFilePattern;
	int iEventNumber;
	// main canvas informations

	// status bar inform center
	std::string iStatusBarInfor;
	int iStatusBarProcessBar;
	int iStatusBarColor;

	// system information center
	unsigned int iCPUCount;
	unsigned int iMemerySize;
	unsigned int iCPULoad;
	unsigned int iThreadUsed;
	unsigned int iWorkProgress;

	// code ralated
	std::string iCurrentPath;
	std::string iCodePath;
//+++++++++++++++++++++++++++++++++++
public:
	static GUIInformation *GetInstance();
private:
	static GUIInformation *m_instance;
	GUIInformation(){};
	virtual ~GUIInformation(){};
	class Garbo{
	public:
		~Garbo(){
			if(GUIInformation::m_instance)
				delete GUIInformation::m_instance;
		}
	};
	static Garbo garbo;
};

#endif /* GUIINFORMATION_H_ */
