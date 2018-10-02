/********************************************

Xinzhan: 
        v1, This class defines strip hit
        v2, added a vector to store adcs in different TS'es
            removed Double_t pos; Double_t charge
            added a function Double_t Charge() to return the total charge
Usage:
    Before you use timing infomation from this class, you have to 
    use its SetTiming() method to fill all the timing variables, 
    otherwise you'll get all infomation with 0 values.

 *******************************************/

#ifndef _GEMHIT_H
#define _GEMHIT_H

#include <TH1F.h>
#include <TObject.h>
#include <TCanvas.h>
#include <Getline.h>
#include <TGraph.h>
#include <TF1.h>
#include <TTimer.h>
#include <TMinuit.h>
#include <TFile.h>
#include <TString.h>

#include <vector>
#include <cassert>
#include "Config.h"
//#include "GEMFittingShape.h"

using namespace std;

extern TF1 *fparam;
extern Double_t X[];
extern Double_t Y[];
extern Double_t Sigma[];
extern Double_t Pdf(Double_t *, Double_t*);
extern Double_t CalcNLL(Int_t, Double_t*, Double_t*, Double_t*, TF1*);
extern void fcn(int& npar, double* deriv, double& f, double par[], int flag);

class GEMHit : public TObject
{
 public:
  GEMHit(Int_t module, Int_t plane, Int_t strip, vector<Double_t> &vtsadc);   //
  GEMHit(){};
  ~GEMHit();

 public:
  Int_t Strip;
  Int_t Module;
  Int_t Plane;
  Double_t Charge;

  vector<Double_t> vTsAdc;   //vector stores adc's in different time samples.  6 time samples, 

  vector<Double_t> PedSigma; //store pedestal sigma value for each time sample

 public:
  Double_t GetCharge(void);  //total adc in strip. sum of all TS'es.

 public:
  Double_t StartTime;
  Double_t ShappingTime;
  Double_t ReducedChisquareFromFitting;
  Double_t MaxFromFitting;
  Double_t MaxTimeFromFitting;
  Int_t MaxTimeBin;
  Double_t MaxTsAdc;
 public:
  void SetTiming(void);

 public:
  void Clone(GEMHit &hit);

 public:
  void PrintFittingGraph(TGraph &agr, TF1 &fV);
};

#endif
