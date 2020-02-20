/*
 * UserGuiGeneralDialogProcess.cpp
 *
 *  Created on: Jan 31, 2017
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

#include "string.h"
#include "vector"
#include <libgen.h>

#include "UserGuiGeneralDialogProcess.h"


UserGuiGeneralDialogProcess::UserGuiGeneralDialogProcess() :  TGFrame() {
	// TODO Auto-generated constructor stub

}

UserGuiGeneralDialogProcess::~UserGuiGeneralDialogProcess() {
	// TODO Auto-generated destructor stub
}


// set or read the default path for the browser diaglog
std::string UserGuiGeneralDialogProcess::DefaultPath(std::string setpath ){
	if(setpath.empty()){
		return pDefaultPath;
	}else{
		pDefaultPath=setpath;
		return pDefaultPath;
	}
}


std::string UserGuiGeneralDialogProcess::Browser_file() {
	std::vector<std::string>tRawFileList;
	const char *filetype[]= {
					"All files", "*",
					"ROOT files", "*.root",
					"Data files", "*.dat",
					0, 0
			};
	static TString dir(pDefaultPath.c_str());
	TGFileInfo fi;
	fi.fFileTypes = filetype;
	fi.fIniDir = StrDup(dir);
	new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);

	if (fi.fMultipleSelection && fi.fFileNamesList) {
		TObjString *el;
		TIter next(fi.fFileNamesList);
		while ((el = (TObjString *) next())) {
			std::string filename(el->GetString().Data());
			tRawFileList.push_back(filename);

		}
	} else {
		if (fi.fFilename != NULL) {
			std::string filename = (fi.fFilename);
			std::string FileFullName = filename;
			tRawFileList.push_back(FileFullName);
		}
	}
	if (tRawFileList.size()) {
		return tRawFileList[0];
	} else {
		std::string temp;
		temp.clear();
		return temp;
	}
}

std::string UserGuiGeneralDialogProcess::Browser_file(std::string path) {
	std::vector<std::string>tRawFileList;
	const char *filetype[]= {
					"All files", "*",
					"ROOT files", "*.root",
					"Data files", "*.dat",
					0, 0
			};
	static TString dir(path.c_str());
	TGFileInfo fi;
	fi.fFileTypes = filetype;
	fi.fIniDir = StrDup(dir);

	new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);

	if (fi.fMultipleSelection && fi.fFileNamesList) {
		TObjString *el;
		TIter next(fi.fFileNamesList);
		while ((el = (TObjString *) next())) {
			std::string filename(el->GetString().Data());
			tRawFileList.push_back(filename);

		}
	}else if(fi.fFilename != NULL) {
		std::string filename = (fi.fFilename);
		std::string FileFullName = filename;
		tRawFileList.push_back(FileFullName);
	}
	if (tRawFileList.size()) {
		return tRawFileList[0];
	} else {
		std::string temp;
		temp.clear();
		return temp;
	}
}
std::string UserGuiGeneralDialogProcess::Browser_file(const char *filetype_in[]) {
	std::vector<std::string>tRawFileList;
	static TString dir(pDefaultPath.c_str());
	TGFileInfo fi;
	fi.fFileTypes = filetype_in;
	fi.fIniDir = StrDup(dir);
	new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);

	if (fi.fMultipleSelection && fi.fFileNamesList) {
		TObjString *el;
		TIter next(fi.fFileNamesList);
		while ((el = (TObjString *) next())) {
			std::string filename(el->GetString().Data());
			tRawFileList.push_back(filename);

		}
	}else if(fi.fFilename != NULL) {
		std::string filename = (fi.fFilename);
		std::string FileFullName = filename;
		tRawFileList.push_back(FileFullName);
	}
	if (tRawFileList.size()) {
		return tRawFileList[0];
	} else {
		std::string temp;
		temp.clear();
		return temp;
	}
}

std::string UserGuiGeneralDialogProcess::Browser_file(std::string path,const char *filetype_in[]){
	std::vector<std::string>tRawFileList;
	static TString dir(path.c_str());
	TGFileInfo fi;
	fi.fFileTypes = filetype_in;
	fi.fIniDir = StrDup(dir);
	new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);

	if (fi.fMultipleSelection && fi.fFileNamesList) {
		TObjString *el;
		TIter next(fi.fFileNamesList);
		while ((el = (TObjString *) next())) {
			std::string filename(el->GetString().Data());
			tRawFileList.push_back(filename);
		}
	}else if(fi.fFilename != NULL) {
		std::string filename = (fi.fFilename);
		std::string FileFullName = filename;
		tRawFileList.push_back(FileFullName);
	}
	if (tRawFileList.size()) {
		return tRawFileList[0];
	} else {
		std::string temp;
		temp.clear();
		return temp;
	}
};

std::string UserGuiGeneralDialogProcess::Browser_file(std::string path,std::string fileformate){
	const char *filetype[6];
		filetype[0] = (fileformate+" File").c_str();
		filetype[1] =("*."+fileformate).c_str();
		filetype[2] ="All files";
		filetype[2] ="*";
		filetype[4]=0;
		filetype[5]=0;
	std::vector<std::string>tRawFileList;
	static TString dir(path.c_str());
	TGFileInfo fi;
	fi.fFileTypes =filetype;
	fi.fIniDir = StrDup(dir);
	new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);

	if (fi.fMultipleSelection && fi.fFileNamesList) {
		TObjString *el;
		TIter next(fi.fFileNamesList);
		while ((el = (TObjString *) next())) {
			std::string filename(el->GetString().Data());
			tRawFileList.push_back(filename);

		}
	}else if(fi.fFilename != NULL) {
		std::string filename = (fi.fFilename);
		std::string FileFullName = filename;
		tRawFileList.push_back(FileFullName);
	}
	if (tRawFileList.size()) {
		return tRawFileList[0];
	} else {
		std::string temp;
		temp.clear();
		return temp;
	}

};

std::vector<std::string> UserGuiGeneralDialogProcess::Browser_files() {
	std::vector<std::string>tRawFileList;
	const char *filetype[]= {
					"All files", "*",
					"ROOT files", "*.root",
					"Data files", "*.dat",
					0, 0
			};
	static TString dir(pDefaultPath.c_str());
	TGFileInfo fi;
	fi.fFileTypes = filetype;
	fi.fIniDir = StrDup(dir);
	new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);

	if (fi.fMultipleSelection && fi.fFileNamesList) {
		TObjString *el;
		TIter next(fi.fFileNamesList);
		while ((el = (TObjString *) next())) {
			std::string filename(el->GetString().Data());
			tRawFileList.push_back(filename);

		}
	}else if(fi.fFilename != NULL) {
		std::string filename = (fi.fFilename);
		std::string FileFullName = filename;
		tRawFileList.push_back(FileFullName);
	}
	return tRawFileList;
}

std::vector<std::string> UserGuiGeneralDialogProcess::Browser_files(std::string path) {
	std::vector<std::string>tRawFileList;
	const char *filetype[]= {
					"All files", "*",
					"ROOT files", "*.root",
					"Data files", "*.dat",
					0, 0
			};
	static TString dir(path.c_str());
	TGFileInfo fi;
	fi.fFileTypes = filetype;
	fi.fIniDir = StrDup(dir);

	new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);

	if (fi.fMultipleSelection && fi.fFileNamesList) {
		TObjString *el;
		TIter next(fi.fFileNamesList);
		while ((el = (TObjString *) next())) {
			std::string filename(el->GetString().Data());
			tRawFileList.push_back(filename);
		}
	}else if(fi.fFilename != NULL) {
		std::string filename = (fi.fFilename);
		std::string FileFullName = filename;
		tRawFileList.push_back(FileFullName);
	}
	return tRawFileList;
}

std::vector<std::string> UserGuiGeneralDialogProcess::Browser_files(const char *filetype_in[]) {
	std::vector<std::string>tRawFileList;
	static TString dir(pDefaultPath.c_str());
	TGFileInfo fi;
	fi.fFileTypes = filetype_in;
	fi.fIniDir = StrDup(dir);
	new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);

	if (fi.fMultipleSelection && fi.fFileNamesList) {
		TObjString *el;
		TIter next(fi.fFileNamesList);
		while ((el = (TObjString *) next())) {
			std::string filename(el->GetString().Data());
			tRawFileList.push_back(filename);

		}
	}else if(fi.fFilename != NULL) {
		std::string filename = (fi.fFilename);
		std::string FileFullName = filename;
		tRawFileList.push_back(FileFullName);
	}

	return tRawFileList;

}

std::vector<std::string> UserGuiGeneralDialogProcess::Browser_files(std::string path,const char *filetype_in[]){
	std::vector<std::string>tRawFileList;
	static TString dir(path.c_str());
	TGFileInfo fi;
	fi.fFileTypes = filetype_in;
	fi.fIniDir = StrDup(dir);
	new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);

	if (fi.fMultipleSelection && fi.fFileNamesList) {
		TObjString *el;
		TIter next(fi.fFileNamesList);
		while ((el = (TObjString *) next())) {
			std::string filename(el->GetString().Data());
			tRawFileList.push_back(filename);

		}
	}else if(fi.fFilename != NULL) {
		std::string filename = (fi.fFilename);
		std::string FileFullName = filename;
		tRawFileList.push_back(FileFullName);
	}

	return tRawFileList;
};

std::vector<std::string> UserGuiGeneralDialogProcess::Browser_files(std::string path,std::string fileformate){
	const char *filetype[6];
	filetype[0] = (fileformate+" File").c_str();
	filetype[1] =("*."+fileformate).c_str();
	filetype[2] ="All files";
	filetype[2] ="*";
	filetype[4]=0;
	filetype[5]=0;
	std::vector<std::string>tRawFileList;
	static TString dir(path.c_str());
	TGFileInfo fi;
	fi.fFileTypes =filetype;
	fi.fIniDir = StrDup(dir);
	new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);

	if (fi.fMultipleSelection && fi.fFileNamesList) {
		TObjString *el;
		TIter next(fi.fFileNamesList);
		while ((el = (TObjString *) next())) {
			std::string filename(el->GetString().Data());
			tRawFileList.push_back(filename);

		}
	}else if(fi.fFilename != NULL) {
		std::string filename = (fi.fFilename);
		std::string FileFullName = filename;
		tRawFileList.push_back(FileFullName);
	}
	return tRawFileList;
};

std::string UserGuiGeneralDialogProcess::GetBaseFileName(std::string filename){
	std::string filebasename=basename(strdup(filename.c_str()));
	return filebasename;
}

std::string UserGuiGeneralDialogProcess::GetAppendixLess_FileName(std::string filename) {
	std::string filebasename=basename(strdup(filename.c_str()));
	return filebasename.substr(0,filebasename.find_first_of("."));
}
int UserGuiGeneralDialogProcess::GetDividedNumber(std::string filename){
	std::string filebasename=basename(strdup(filename.c_str()));
	return atoi(filebasename.substr(filebasename.find_last_of(".")+1).c_str());
}
int UserGuiGeneralDialogProcess::GetNumberFromFilename(std::string filename){
	std::string filebasename=GetAppendixLess_FileName(filename);
	return atoi(filebasename.substr(filebasename.find_last_not_of("0123456789")+1).c_str());

}

//!
// it will extract the first number and cut the string after the first number
// if no number return -1
//
const int UserGuiGeneralDialogProcess::GetNumberFromString(std::string & str){
	int number = -1;
	std::string filename=str;
	if(filename.find_first_of("0123456789")!=std::string::npos){
		filename=filename.substr(filename.find_first_of("0123456789"));
		number = atoi(filename.substr(filename.find_first_of("0123456789"),filename.find_first_not_of("0123456789")).c_str());
		filename=filename.substr(filename.find_first_not_of("0123456789"));
		str=filename;
	}
	return number;
}

Bool_t UserGuiGeneralDialogProcess::CheckAppendix(std::string filename, std::string appendix) {
	if(filename.substr(filename.find_last_of(".")+1)==appendix){
		return 1;
	}else{
		return 0;
	}
}
