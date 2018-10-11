/*
 * GUISysGeneral.h
 *
 *  Created on: Sep 14, 2018
 *      Author: newdriver
 */

#ifndef GUISYSGENERAL_H_
#define GUISYSGENERAL_H_
#include "TGClient.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TGTextEntry.h"
#include "TGFileDialog.h"
#include "string"
#include "vector"
#include "GUIInformation.h"
class GUISysGeneral:public TGFrame {
public:
	GUISysGeneral();
	virtual ~GUISysGeneral(){};

public:
	const std::vector<std::string> FilesBrowser(const char *filetype[]={},const std::string path=".");
	inline const std::string *GetFilename(const std::string filename);
	inline const std::string *GetPathname(const std::string filename);
	inline const std::string *GetBasename(const std::string filename);
	inline const std::string GetOutPutFileName(const std::string filename,const std::string pattern="Tracking_run%4d_%4d.root");
	inline const std::vector<unsigned int> GetNumberFromString(const std::string filename);
	inline const Bool_t IsFileExist();
	inline const Bool_t IsFolderExist();
	inline const Bool_t IsExtension();
	inline const Bool_t IsPattern();
private:
	GUIInformation *guiinfor=GUIInformation::GetInstance();
};

#endif /* GUISYSGENERAL_H_ */
