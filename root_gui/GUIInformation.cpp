/*
 * GUIInformation.cpp
 *
 *  Created on: Sep 13, 2018
 *      Author: newdriver
 */

#include "GUIInformation.h"

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
