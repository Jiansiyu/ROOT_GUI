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

#include "UserGuiGeneralDialogProcess.h"


UserGuiGeneralDialogProcess::UserGuiGeneralDialogProcess() :  TGFrame() {
	// TODO Auto-generated constructor stub

}

UserGuiGeneralDialogProcess::~UserGuiGeneralDialogProcess() {
	// TODO Auto-generated destructor stub
}


// set or read the default path for the browser diaglog
std::string UserGuiGeneralDialogProcess::DefaultPath(std::string setpath ){
	if(setpath==NULL){
		return pDefaultPath;
	}else{
		pDefaultPath=setpath;
		return pDefaultPath;
	}
}


std::string UserGuiGeneralDialogProcess::Browser_file() {
	std::vector<std::string>tRawFileList;
	const char *filetype[]= {
					"ROOT files", "*.root",
					"Data files", "*.dat",
					"All files", "*",
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
	}else {
		if(fi.fFilename != NULL) {
		std::string filename = (fi.fFilename);
		std::string FileFullName = filename;
		tRawFileList.push_back(FileFullName);
	}else{
		return NULL;
	}
	}
	if(tRawFileList.size()){
		return tRawFileList[0];
	}else{
		return NULL;
	}
}

std::string UserGuiGeneralDialogProcess::Browser_file(std::string path) {
	std::vector<std::string>tRawFileList;
	const char *filetype[]= {
					"ROOT files", "*.root",
					"Data files", "*.dat",
					"All files", "*",
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
	if(tRawFileList.size()){
		return tRawFileList[0];
	}else{
		return NULL;
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
	if(tRawFileList.size()){
		return tRawFileList[0];
	}else{
		return NULL;
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
	if(tRawFileList.size()){
		return tRawFileList[0];
	}else{
		return NULL;
	}
};

std::string UserGuiGeneralDialogProcess::Browser_file(std::string path,std::string fileformate){
	const char *filetype[4];
	filetype[0] = (fileformate+" File").c_str();
	filetype[1] =("*."+fileformate).c_str();
	filetype[3]=0;
	filetype[4]=0;
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
	if(tRawFileList.size()){
		return tRawFileList[0];
	}else{
		return NULL;
	}

};

std::vector<std::string> UserGuiGeneralDialogProcess::Browser_files() {
	std::vector<std::string>tRawFileList;
	const char *filetype[]= {
					"ROOT files", "*.root",
					"Data files", "*.dat",
					"All files", "*",
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
					"ROOT files", "*.root",
					"Data files", "*.dat",
					"All files", "*",
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
	const char *filetype[4];
	filetype[0] = (fileformate+" File").c_str();
	filetype[1] =("*."+fileformate).c_str();
	filetype[3]=0;
	filetype[4]=0;
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

Bool_t UserGuiGeneralDialogProcess::CheckAppendix(std::string filename, std::string appendix) {
	if(filename.substr(filename.find_last_of(".")+1)==appendix){
		return 1;
	}else{
		return 0;
	}
}
