/*************************************

All tracking work done by this class.

**************************************/

#ifndef GEMTRACKING_H
#define GEMTRACKING_H

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <memory>

#include <TROOT.h>
#include <TError.h>
#include <Getline.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TF1.h>
#include <TFile.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTimer.h>

#include <string>

#include <TObject.h>
#include <TObjArray.h>

#include "GEMTree.h"
#include "GEMCluster.h"
#include "GEMHit.h"
#include "Config.h"

using namespace std;

class GEMTracking : public GEMTree
{
 public:
  GEMTracking(TChain* chain);
  virtual ~GEMTracking();

  void Run(Int_t event);
  void Run(Int_t event, const char *filename);   // June 28th 2016 modified so as to change the output file name 
  void Reset();
  void FillHistograms();

  void Calibration(Int_t, const char *);

 protected:
  void Decode(Int_t module, Int_t plane);

  //APV Index
  Int_t APVIndex(Int_t module, Int_t plane, Int_t strip);

  //Sort Cluster in a vector by charge
  void Sort(vector<GEMCluster> &vC);
  void Sort(vector<Struct_MaxTimeBin> &vC);

  void FindCluster(Int_t module, Int_t plane, vector<GEMHit> vHit, Int_t cut);

  vector<GEMHit> vHit;
  vector<GEMHit> vHit_cut;
  vector<GEMCluster> vCluster;
  vector<GEMCluster> vCluster_cut;

  // add temparatly
  float_t vResiduex[kNMODULE];
  float_t vResiduey[kNMODULE];



  //IsBestHit() adopts a much stricter event selection rule
  Int_t IsBestHit(GEMHit hit);    // select the events that maximun appears on 3rd or 4th sample
  Int_t IsBestHit(GEMHit hit,int max_sample_1,int max_sample_2=-1,int max_sample_3=-1); // select the events Maximum cluster on certen samples, negtive diabled.   Sept 15th 2016 Siyu 
 public:
  Int_t evtID;
};

#endif
