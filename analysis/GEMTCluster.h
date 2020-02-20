/*

Xinzhan: Store cluster info into a Tree.
         cluster info extracted from THit.

 */

#ifndef GEMTCLUSTER_H
#define GEMTCLUSTER_H

#include <TObject.h>
#include <TTree.h>
#include "Config.h"
#include "GEMCluster.h"

class GEMTCluster:public TObject
{
 public:
  TTree *GEMTreeCluster;

 public:
  Int_t evtID;
  Int_t Module;
  Int_t Plane;
  Int_t Size;
  Int_t StripIndex[kMaxStripsinCluster];
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

 public:
  GEMTCluster(const char* name, const char* title);
  ~GEMTCluster();

 public:
  void Fill(GEMCluster cluster, Int_t id);
  TTree* GetTree();
};

#endif
