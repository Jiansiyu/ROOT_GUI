/***
*
*	Decoder the three files 
*
*	DATE: Sept 30th 2016
*	
*	Siyu 
*
**/


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
#include "GEMTreeGenerator.h"
using namespace std;
	

GEM_Tree_Generator::GEM_Tree_Generator(const char *tree_file_name){

	GEM_Tree_files=new TFile(tree_file_name,"RECREATE");
	GEM_tree=new TTree("GEMTreeCluster","GEM clsuter events by events");
	
	GEM_tree->Branch("evtID",&evtID,"evtID/I");
	GEM_tree->Branch("Module",&Module,"Module/I");
	GEM_tree->Branch("Plane",&Plane,"Plane/I");
	GEM_tree->Branch("Size",&Size,"Size/I");
	GEM_tree->Branch("StripIndex",&StripIndex,"StripIndex/I");
	GEM_tree->Branch("Pos",&Pos,"Pos/I");
	GEM_tree->Branch("Charge",&Charge,"Charge/I");
	GEM_tree->Branch("StartTime",&StartTime,"StartTime/I");
	
	GEM_tree->Branch("ShappingTime",&ShappingTime,"ShappingTime/I");
	GEM_tree->Branch("MaxTimeBin",&MaxTimeBin,"MaxTimeBin/I");
	
	GEM_tree->Branch("adc0",&adc0,"adc0/I");
	GEM_tree->Branch("adc1",&adc1,"adc1/I");
	GEM_tree->Branch("adc2",&adc2,"adc2/I");
	
	GEM_tree->Branch("adc3",&adc3,"adc3/I");
	GEM_tree->Branch("adc4",&adc4,"adc4/I");
	GEM_tree->Branch("adc5",&adc5,"adc5/I");
	GEM_tree->Branch("adc6",&adc6,"adc6/I");
	GEM_tree->Branch("adc7",&adc7,"adc7/I");
	GEM_tree->Branch("adc8",&adc8,"adc8/I");
}

void GEM_Tree_Generator::GEM_Tree_write()
{
  GEM_tree->Write();
  GEM_Tree_files->Write();
  GEM_Tree_files->Close();
 }
 
void GEM_Tree_Generator::GEM_Tree_fill()
{
  GEM_tree->Fill();
 }
 

