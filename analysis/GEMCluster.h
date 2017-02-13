/********************************************

Xinzhan: 
        v1, This class defines a cluster
        v2, removed Double_t pos, Double_t charge, vector<Int_t> stripindex;
            added TObjArray &HitIndex; 
            added Double_t Pos(void); Double_t Charge(void);
        V3, added adc in different Time Samples: vector<Double_t> vTsAdc;

Usage:
    Before you use timing information from this class, you have to 
    apply its SetTiming() method to fill all timing variables.

 *******************************************/

#ifndef _GEMCLUSTER_H
#define _GEMCLUSTER_H

#include <vector>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <Getline.h>
#include <TTimer.h>
#include <TGraph.h>
#include <TObject.h>
#include <TObjArray.h>
#include <TMinuit.h>
#include "Config.h"
#include "GEMHit.h"
//#include "GEMFittingShape.h"

using namespace std;

extern TF1 *fparam;
extern Double_t X[];
extern Double_t Y[];
extern Double_t Sigma[];
extern Double_t Pdf(Double_t *, Double_t*);
extern Double_t CalcNLL(Int_t, Double_t*, Double_t*, Double_t*, TF1*);
extern void fcn(int& npar, double* deriv, double& f, double par[], int flag);

class GEMCluster : public TObject
{
 public:
  GEMCluster(){};//for copy 
  GEMCluster(Int_t module, Int_t plane, Int_t size, TObjArray &hitindex);
  ~GEMCluster();

 public:
  Int_t Size;
  Int_t Plane;
  Int_t Module;
  Double_t Charge;
  Double_t Pos;
  TObjArray HitIndex;

 public:
  Double_t GetPos(void);
  Double_t GetCharge(void);

 public:
  void SetTiming(void);

 public:
  void PrintFittingGraph(TGraph &gr, TF1 &fV);

  //Timing Infomation
  Double_t StartTime;
  Double_t ShappingTime;
  Double_t MaxFromFitting;
  Double_t MaxTimeFromFitting;
  Double_t MaxTimeBin;
  Double_t Fitting_ReducedChisquare;
  //ADC in Time Samples
  vector<Double_t> vTsAdc;
  vector<Double_t> PedSigma;

 public:
  void Clone(GEMCluster &gem_cluster);
};

#endif
