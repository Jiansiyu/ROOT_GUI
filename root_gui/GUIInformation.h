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

#include "TSystem.h"
#include "string"
#include "vector"
#include "GUIStructure.h"
#include  "GUISignalSlot.h"
#include "iostream"
#include "map"
//#include "../GEMDetector/GEMConfigure.h"
struct GUIDisplaySetting{

};

class GUIInformation {

public:
	void SetRunMode(GUIWorkMode);  // set and get the run mode
	void SetPedestalInputFile(std::string);
	void SetRawFileInputList_add(std::vector<std::string>);
	void SetRawFileInputList_remove(std::vector<std::string>);
	void SetRawFileInputList_remove(std::string);

	GUIWorkMode GetRunMode();
	std::string *GetPedestalInputFile();
	std::vector<std::string> GetRawFileInputList();
	std::map<int,std::string> GetWorkZoneTabList(GUIWorkMode=WORKMODE_RAW);
	CpuInfo_t GetCPUInfor();
	MemInfo_t GetMemeryInfor();
private:
	// control panel informations
	GUIWorkMode iWorkmode;
	GUIDisplaySetting iDisplaySetting;
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
	CpuInfo_t iCPUinfor;
	MemInfo_t iMemeryInfor;

//	GEMConfigure *gemcfg=GEMConfigure::GetInstance();
	///WorkZoneTab Name rules:

	std::map<int,std::string> iWorkZoneTabList_Raw;
	std::map<int,std::string> iWorkZoneTabList_Pedestal;
	std::map<int,std::string> iWorkZoneTabList_ZeroSubtraction;
	std::map<int,std::string> iWorkZoneTabList_Hit;
	std::map<int,std::string> iWorkZoneTabList_Analysis;
	std::map<int,std::string> iWorkZoneTabList_default;

	/*std::vector<std::string> iWorkZoneTabList_Raw;
	std::vector<std::string> iWorkZoneTabList_Pedestal;
	std::vector<std::string> iWorkZoneTabList_ZeroSubtraction;
	std::vector<std::string> iWorkZoneTabList_Hit;
	std::vector<std::string> iWorkZoneTabList_Analysis;
	std::vector<std::string> iWorkZoneTabList_default;*/

	void iWorkModeCalculation();
//+++++++++++++++++++++++++++++++++++
public:
	static GUIInformation *GetInstance();
private:
	static GUIInformation *m_instance;
	GUIInformation();
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
