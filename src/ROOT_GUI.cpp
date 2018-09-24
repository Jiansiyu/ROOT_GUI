/*
;p[- *	ROOT GUI
 *	author: Siyu Jian
 * 	University of Virginia
 * 	jiansiyu@gmail.com
 *  Notes: General GUI used for ROOT data analysis and display
 **/
#include "TApplication.h"
#include "TROOT.h"

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <getopt.h>
#include <iostream>
#include <time.h>
#include <TROOT.h>
#include <TError.h>
#include <TApplication.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>

#include "../gui/UserRootGUI.h"

int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);

   if(gROOT->IsBatch()){
	   fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
	         return 1;
   }
   UserRootGUI mainWindows(gClient->GetRoot(),400,400);
   theApp.Run();
}
