/*
 * HRSOpticsGeneral.h
 * some common functions
 *  Created on: Oct 14, 2019
 *      Author: newdriver
 */

#ifndef HRSOPTICSGENERAL_H_
#define HRSOPTICSGENERAL_H_


#include <iostream>
#include <ctime>
#include <string.h>
#include <TROOT.h>
#include <vector>
#include <TTree.h>
class HRSOpticsGeneral {
public:
	HRSOpticsGeneral();
	virtual ~HRSOpticsGeneral();

public:
	inline std::string getDatetime();
	inline std::string getDate();
	inline std::string GenerateNewFolder(); // generate new foldder according to the datetime
	inline Bool_t IsFileExist(std::string);
	inline Bool_t IsFolderExist(std::string);
	inline std::vector<double_t> GetNumber(std::string);   // get number from string
	inline std::vector<std::string> GetFilesInFolder(std::string, std::string exten=".*");

	inline Bool_t IsBranchExist(std::string , TTree *);  // check whether branch exist

};

#endif /* HRSOPTICSGENERAL_H_ */
