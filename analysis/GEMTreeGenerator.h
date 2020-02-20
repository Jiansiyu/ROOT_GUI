/***
*
*	Decoder the three files 
*
*	DATE: Sept 30th 2016
*	
*	Siyu 
*
**/

#ifndef __GEM_TREE_GENERATOR__
#define __GEM_TREE_GENERATOR__

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

#include <TH1F.h>
#include <TCanvas.h>

using namespace std;



class GEM_Tree_Generator
{

	public:
		GEM_Tree_Generator(const char *tree_file_name);   // initilize the tree files 
		~GEM_Tree_Generator();
		
		void GEM_Tree_write();
		void GEM_Tree_fill();
	public:
		TFile  *GEM_Tree_files;
		TTree  *GEM_tree;
	
	public:
		Int_t evtID;
  		Int_t Module;
  		Int_t Plane;
  		Int_t Size;
  		Int_t StripIndex;
  		Double_t Pos;
  		Double_t Charge;
  		Double_t StartTime;
  		Double_t ShappingTime;
  		Double_t MaxTimeBin;

  		Int_t adc0;
  		Int_t adc1;
  		Int_t adc2;
  		Int_t adc3;
  		Int_t adc4;
  		Int_t adc5;
  		Int_t adc6;
  		Int_t adc7;
  		Int_t adc8;
	
};

#endif
