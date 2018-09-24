/*
 * GUISysGeneral.cpp
 *
 *  Created on: Sep 14, 2018
 *      Author: newdriver
 */

#include "TGClient.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TGTextEntry.h"
#include "TGFileDialog.h"
#include "TObjString.h"
#include "TApplication.h"
#include "TROOT.h"

#include "vector"
#include "string"
//#include <libgen.h>
#include "iostream"
#include <algorithm>
#include "GUISysGeneral.h"

GUISysGeneral::GUISysGeneral():  TGFrame(){};

const std::vector<std::string> GUISysGeneral::FilesBrowser(const char **filetype,const std::string path){
	std::vector<std::string> filelist;
	static TString dir(path.c_str());
	TGFileInfo fi;
	fi.fFileTypes=filetype;
	fi.fIniDir=StrDup(dir);
	new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);
	if (fi.fMultipleSelection && fi.fFileNamesList) {
		TObjString *el;
		TIter next(fi.fFileNamesList);
		while ((el = (TObjString *) next())) {
			std::string filename(el->GetString().Data());
			filelist.push_back(filename);
		}
	}else if(fi.fFilename != NULL) {
		std::string filename = (fi.fFilename);
		std::string FileFullName = filename;
		filelist.push_back(filename);
	}
	return filelist;
}

//template<class T>
inline const std::vector<unsigned int> GUISysGeneral::GetNumberFromString(const std::string fname){
	std::vector<unsigned int> num;
	std::string filename(fname.c_str());
	while(filename.find_first_of("0123456789")!=std::string::npos){
		filename=filename.substr(filename.find_first_of("0123456789"));
		auto num_t=atoi(filename.substr(filename.find_first_of("0123456789"),filename.find_first_not_of("0123456789")).c_str());
		num.emplace_back(num_t);
		filename=filename.substr(filename.find_first_not_of("0123456789"));
	}
	return num;
}
inline const std::string GUISysGeneral::GetOutPutFileName(const std::string fname,const std::string f_pattern){
	std::string name(f_pattern.c_str());
	std::vector<unsigned int> numberinfile=GetNumberFromString(fname.c_str());
	for(auto number :numberinfile ){
		name=Form(name.c_str(),number);
	}
	return name;
}
