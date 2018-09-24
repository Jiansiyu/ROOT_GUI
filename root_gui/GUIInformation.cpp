/*
 * GUIInformation.cpp
 *
 *  Created on: Sep 13, 2018
 *      Author: newdriver
 */

#include "GUIInformation.h"
#include "TSystem.h"
#include "vector"
#include "string"
#include "iostream"
GUIInformation *GUIInformation::m_instance=nullptr;
GUIInformation *GUIInformation::GetInstance(){
	if(m_instance ==nullptr)
		m_instance = new GUIInformation;
	return m_instance;
}


void GUIInformation::SetRawFileInputList_add(std::vector<std::string> filelist){
	iRawFileList.insert(iRawFileList.end(),filelist.begin(),filelist.end());
	for(auto file : iRawFileList){
		std::cout<<"File input : "<< file.c_str()<<std::endl;
	}
}


void GUIInformation::SetPedestalInputFile(std::string file){
	iPedestal=file;
}


void GUIInformation::SetRunMode(GUIWorkMode command){
	iWorkmode=command;
}

std::vector<std::string> GUIInformation::GetWorkZoneTabList(GUIWorkMode wmode){
	switch (wmode) {
		case WORKMODE_RAW:
			return iWorkZoneTabList_Raw;
			break;
		case WORKMODE_PEDESTAL:
			return iWorkZoneTabList_Pedestal;
			break;
		case WORKMODE_ZERSUBTRACTION:
			return iWorkZoneTabList_ZeroSubtraction;
			break;
		case WORKMODE_HIT:
			return iWorkZoneTabList_Hit;
			break;
		case WORKMODE_ANALYSIS:
			return iWorkZoneTabList_Analysis;
			break;
		default:
			return iWorkZoneTabList_default;
			break;
	}
}

//! Return the work mode
//
GUIWorkMode GUIInformation::GetRunMode(){
	return iWorkmode;
}


std::string *GUIInformation::GetPedestalInputFile(){
	return &iPedestal;
}


std::vector<std::string> GUIInformation::GetRawFileInputList(){
	return iRawFileList;
}

CpuInfo_t GUIInformation::GetCPUInfor(){
	gSystem->GetCpuInfo(&iCPUinfor,100);
	return iCPUinfor;
}

MemInfo_t GUIInformation::GetMemeryInfor(){
	gSystem->GetMemInfo(&iMemeryInfor);
	return iMemeryInfor;
}
