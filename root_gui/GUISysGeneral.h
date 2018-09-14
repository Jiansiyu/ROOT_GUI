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
class GUISysGeneral:public TGFrame {
public:
	GUISysGeneral();
	virtual ~GUISysGeneral(){};

public:
	const std::vector<std::string> FilesBrowser(const char *filetype[]={},const std::string path=".");
	inline const std::string *GetFilename(const std::string filename);
	inline const std::string *GetPathname(const std::string filename);
	inline const std::string *GetBasename(const std::string filename);
};

#endif /* GUISYSGENERAL_H_ */
