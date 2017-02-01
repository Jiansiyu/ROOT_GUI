/*
 *	ROOT GUI
 *	author: Siyu Jian
 * 	University of Virginia
 * 	jiansiyu@gmail.com
 *  Notes: General GUI used for ROOT data analysis and display
 **/
#include "TApplication.h"
#include "TROOT.h"

// user defined classes
#include "UserGuiMainFrame.h"

int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);

   if(gROOT->IsBatch()){
	   fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
	         return 1;
   }
   UserGuiMainFrame mainWindows(gClient->GetRoot(),400,200);
   theApp.Run();
}
