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
//	template<class T>
	inline const std::vector<unsigned int> GetNumberFromString(const std::string filename);
private:
	GUIInformation *guiinfor=GUIInformation::GetInstance();
};

#endif /* GUISYSGENERAL_H_ */
