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
#include "iostream"
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


