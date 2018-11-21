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
#include "algorithm"
GUIInformation *GUIInformation::m_instance=nullptr;
GUIInformation *GUIInformation::GetInstance(){
	if(m_instance ==nullptr)
		m_instance = new GUIInformation;
	return m_instance;
}


void GUIInformation::SetRawFileInputList_add(std::vector<std::string> filelist){
	iRawFileList.insert(iRawFileList.end(),filelist.begin(),filelist.end());
	std::sort(iRawFileList.begin(),iRawFileList.end());
	iRawFileList.erase(std::unique(iRawFileList.begin(),iRawFileList.end()),iRawFileList.end()); // erase the duplicated files
}

void GUIInformation::SetRawFileInputList_remove(std::vector<std::string> fnames){

	for(auto i : fnames)
		SetRawFileInputList_remove(i.c_str());

}

void GUIInformation::SetRawFileInputList_remove(std::string fname){
	auto iter=std::find(iRawFileList.begin(),iRawFileList.end(),fname);
	if(iter!=iRawFileList.end()) iRawFileList.erase(iter);


}

void GUIInformation::SetPedestalInputFile(std::string file){
	iPedestal=file;
}


void GUIInformation::SetRunMode(GUIWorkMode command){
	iWorkmode=command;
}

std::map<int,std::string> GUIInformation::GetWorkZoneTabList(GUIWorkMode wmode){
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



void GUIInformation::iWorkModeCalculation(){
	//delete the buffered data
	iWorkZoneTabList_Raw.clear();
	iWorkZoneTabList_Pedestal.clear();
	iWorkZoneTabList_ZeroSubtraction.clear();
	iWorkZoneTabList_Hit.clear();
	iWorkZoneTabList_Analysis.clear();
	iWorkZoneTabList_default.clear();
	for(auto mpd: gemcfg->GetMapping().GetMPDList()){
		iWorkZoneTabList_Raw[mpd]=Form("Crate%d_MPD%d",GEM::getCrateID(mpd),GEM::getMPDID(mpd));
	}
	for(auto layer: gemcfg->GetMapping().GetGEMModuleList()){
		iWorkZoneTabList_ZeroSubtraction[layer]=(Form("Layer%d",layer));
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

GUIInformation::GUIInformation(){
	iWorkModeCalculation();
}
