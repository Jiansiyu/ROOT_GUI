/***********************************************************************************

Xinzhan:
    This header file: 1),defines the shape that used to fit 
    signal, and supply this shape to TMinuit. 2), contains 
    the fcn global function that TMinuit will minimize. 

***********************************************************************************/

#ifndef _GEMFITTINGSHAPE_H_
#define _GEMFITTINGSHAPE_H_

#include <TMath.h>
#include "Config.h"

//==================================================================================
//Global Variables, needed by TMinuit
//passing the fit function
TF1 *fparam;
//passing the data points for fcn
Int_t nTS = kMAXADC;
Double_t X[kMAXADC];
Double_t Y[kMAXADC];
Double_t Sigma[kMAXADC];

//==================================================================================
Double_t Pdf(Double_t *xPtr, Double_t par[])
{
  //Distribution function AKA model
  /* 
  //Original fitting function
  //Use this shape to fit data
  TF1 fV("fV", "[0]*(x-[2])/([1])*exp(-(x-[2])/[1])"); //fitting funtion 
  fV.SetRange(-300,500);
  fV.SetParameters(700.0, 65.0, 20.0);
  fV.SetParLimits(0, 0.0, 5000000);
  fV.SetParLimits(1, 0.0, 500);
  fV.SetParLimits(2, -100.0, 1000);
  */
  Double_t x = *xPtr;
  Double_t A = par[0]; //normalization
  Double_t start = par[2]; //Start Time
  Double_t shape = par[1]; //Shapping Time
  Double_t f=0;
  f = A * (x-start)/(shape)*TMath::Exp(-(x-start)/shape);
  return f;
}

//==================================================================================
Double_t CalcNLL(Int_t nTS, Double_t *X, Double_t *Y, Double_t *Sigma, TF1* f)
{
  //NLL calculation Function
  //nTS -> # of Time Bins AKA # of data points
  //X -> coordinates of x 
  //Y -> coordinates of y
  //Sigma -> error on y
  //f -> model
  Double_t nll = 0;
  for(int i=0;i<nTS;i++)
    {
      Double_t x = X[i];
      Double_t y = Y[i];
      Double_t sigma = Sigma[i];
      Double_t mu = f->Eval(x);
      if(mu < 1e-10) mu = 1e-10;  // avoid log(0) problems
      nll -= (-0.5)*TMath::Log(2*TMath::Pi()*sigma) - 0.5*(y-mu)*(y-mu)/sigma/sigma;
    }
  return 2*nll;  //factor of -2 so minuit gets the errors right
}

//==================================================================================
void fcn(int& npar, double* deriv, double& f, double par[], int flag)
{
  //Function to be passed to TMinuit; This function is in fixed format
  for (int i=0; i<npar; i++)
    {
      fparam->SetParameter(i,par[i]);
    }
  f = CalcNLL(nTS, X, Y, Sigma ,fparam);
} 

#endif
