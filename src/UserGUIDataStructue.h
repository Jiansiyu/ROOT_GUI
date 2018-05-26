/*
 * UserGUIDataStructue.h
 *
 *  Created on: May 25, 2018
 *      Author: newdriver
 */

#ifndef USERGUIDATASTRUCTUE_H_
#define USERGUIDATASTRUCTUE_H_
#include<TColor.h>
#include <vector>
#include <string>
namespace UserGUI{
const char *filetype[] = {
			"ROOT files", "*.root",
			"Data files", "*.dat",
			"All files", "*",
			0, 0
	};

const char *datfiletype[]={
		"Data files", "*.dat",
		"All files", "*",
					0, 0
};

enum WorningLevel{
	WorningLevel_NORMAL,
	WorningLevel_WORNING,
	WorningLevel_ERROR
};

struct WorningColor{
	WorningColor(){};
	WorningColor(WorningColor){

	};
	TColor GetTColor(){

	}
};

struct GUIInfor{
	char vWorkMode;
	std::string MapFileName;
	std::string PedestalDialogString;
	std::vector<std::string> RawDataDialogString;
	int vEventNumber;
};

}




#endif /* USERGUIDATASTRUCTUE_H_ */
