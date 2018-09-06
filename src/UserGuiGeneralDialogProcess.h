/*
 * UserGuiGeneralDialogProcess.h
 *
 *  Created on: Jan 31, 2017
 *      Author: newdriver
 */

#ifndef USERGUIGENERALDIALOGPROCESS_H_
#define USERGUIGENERALDIALOGPROCESS_H_

#include "TGClient.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TGTextEntry.h"
#include "TGFileDialog.h"

#include "string.h"
#include "vector"



class UserGuiGeneralDialogProcess : public TGFrame {
public:
	UserGuiGeneralDialogProcess();
	virtual ~UserGuiGeneralDialogProcess();

// dialog process functions
public:
	std::string DefaultPath(std::string setpath=NULL);  // set or read the default path when browser files

	// browser the files and return the filesnames
	std::string Browser_file();
	std::string Browser_file(std::string path);
	std::string Browser_file(const char *filetype[]);
	std::string Browser_file(std::string path,std::string fileformate);
	std::string Browser_file(std::string path,const char *filetype[]);
	std::vector<std::string> Browser_files();
	std::vector<std::string> Browser_files(std::string path);
	std::vector<std::string> Browser_files(const char *filetype[]);
	std::vector<std::string> Browser_files(std::string path,std::string fileformate);
	std::vector<std::string> Browser_files(std::string path,const char *filetype[]);

	// file name process functions
public:
	std::string GetFileName(std::string filename, char *processmode);//
	std::string GetBaseFileName(std::string filename);
	std::string GetAppendixLess_FileName(std::string);  // get the filename with out appendix
	const int GetNumberFromString(std::string & filename);
	int GetDividedNumber(std::string);          // get the last digit of the name xxx_1095.dat.0   the last 0
	std::string GetFileNameAppendix(std::string);
	int GetNumberFromFilename(std::string filename);
	Bool_t CheckAppendix(std::string filename, std::string appendix);

private:
	std::string pDefaultPath=".";


};

#endif /* USERGUIGENERALDIALOGPROCESS_H_ */
