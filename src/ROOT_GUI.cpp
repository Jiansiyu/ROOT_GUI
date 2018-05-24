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
#include "ConfigInfor.h"
#include "GEMInforCenter.h"
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

   ConfigInfor *test=ConfigInfor::GetInstance();
   test->ReadConfig();
   GEMInforCenter * infor = GEMInforCenter::GetInstance();

   UserGuiMainFrame mainWindows(gClient->GetRoot(),400,200);
   theApp.Run();
}
