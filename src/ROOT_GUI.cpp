/*
;p[- *	ROOT GUI
 *	author: Siyu Jian
 * 	University of Virginia
 * 	jiansiyu@gmail.com
 *  Notes: General GUI used for ROOT data analysis and display
 **/
#include "TApplication.h"
#include "TROOT.h"

// user defined classes
#include "UserGuiMainFrame.h"
//#include "ConfigInfor.h"
//#include "GEMInforCenter.h"
////
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <getopt.h>
#include <iostream>

// for test uasage
// process time display
#include <time.h>

#include <TROOT.h>
#include <TError.h>
#include <TApplication.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>


int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);

   if(gROOT->IsBatch()){
	   fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
	         return 1;
   }
//   std::string filename="963test123asjas456.root";
//   std::string test = "miaomiao";
//   while(filename.find_first_of("0123456789")!=std::string::npos){
//	   filename=filename.substr(filename.find_first_of("0123456789"));
//	   int number = atoi(filename.substr(filename.find_first_of("0123456789"),filename.find_first_not_of("0123456789")).c_str());
//	   filename=filename.substr(filename.find_first_not_of("0123456789"));
//	   std::cout<<filename.c_str()<<"   "<<number <<std::endl;
//   }
 //  std::string test=filename.substr(filename.find_first_of("0123456789"))
 //  std::cout<<filename.substr(filename.find_first_of("0123456789"),filename.find_first_not_of("0123456789")).c_str()<<std::endl;

   //ConfigInfor *test=ConfigInfor::GetInstance();
   //test->ReadConfig();
   //GEMInforCenter * infor = GEMInforCenter::GetInstance();

   UserGuiMainFrame mainWindows(gClient->GetRoot(),400,200);
   theApp.Run();
}
