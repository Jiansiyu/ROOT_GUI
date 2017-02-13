#include "GEMHit.h"

//==================================================================================
GEMHit::GEMHit(Int_t module, Int_t plane, Int_t strip, vector<Double_t> &vtsadc)
{
  Strip = strip;
  Module = module;
  Plane = plane;
  vTsAdc = vtsadc;
  Charge = GetCharge();

  StartTime=0;
  ShappingTime=0;
  ReducedChisquareFromFitting=0;
  MaxFromFitting=0;
  MaxTimeFromFitting=0;
  MaxTimeBin=0;
  MaxTsAdc=0;

}

//==================================================================================
GEMHit::~GEMHit()
{
}

//==================================================================================
Double_t GEMHit::GetCharge()
{
  Double_t charge_sum=0;
  Double_t charge_max=0;
  vector<Double_t>::iterator it = vTsAdc.begin();

  for(;it!=vTsAdc.end();++it)
    {
      charge_sum+=*it;
      if(*it > charge_max)
	charge_max=*it;
    }

  return charge_max;
  //return charge_sum;
}

//==================================================================================
void GEMHit::SetTiming(void)
{
  TGraph agr;
  agr.SetMarkerStyle(22);
  Int_t TS = kMAXADC;
  Int_t ts_size = vTsAdc.size();
  assert(ts_size == TS);
  vector<Double_t>::const_iterator iter = vTsAdc.begin();
  for(;iter!=vTsAdc.end();iter++)
    {
      Int_t i = agr.GetN();
      Double_t time = 25.0/2.0 + 25.0*i;
      X[i] = time;
      Y[i] = *iter;
      //if(*iter > FittingThreshold)
      agr.SetPoint(i,time,*iter);
      if(*iter > MaxTsAdc)
	{
	  MaxTsAdc=*iter;
	  MaxTimeBin=i;
	}
    }

  //get pedestal sigma for each time sample
  Int_t apvNo = Strip/128;
  TString hName;
  if( Plane == 0 )
    {
      hName = Form("noise_apvNo%dapv%d_Id%d_sbsGEMProd2X_adcCh%d_FecId%d_Id%d_adcCh%d_FecId%d", apvNo, apvNo, XmapID[apvNo], XmapCH[apvNo], XmapFEC[apvNo], XmapID[apvNo], XmapCH[apvNo], XmapFEC[apvNo]);
    }
  else
    {
      hName = Form("noise_apvNo%dapv%d_Id%d_sbsGEMProd2Y_adcCh%d_FecId%d_Id%d_adcCh%d_FecId%d", apvNo+12, apvNo+12, YmapID[apvNo], YmapCH[apvNo], YmapFEC[apvNo], YmapID[apvNo], YmapCH[apvNo], YmapFEC[apvNo]);
    }
  TFile *file = new TFile(fPedFileName.Data(), "READ");
  Double_t ped  = ((TH1F*) file->Get(hName.Data()) ) -> GetBinContent(Strip%128 + 1);
  //assert(PedSigma.size() == 0);//xinzhan: make sure proper initialization
  for (int i=0;i<kMAXADC;i++)
    {
      PedSigma.push_back(ped);
      //cout<<PedSigma[i]<<endl;//for debug
    }
  file->Close();
  delete file;
  //debug
  //cout<<"GEMHit:"<<" "<<PedSigma.size()<<endl;
  for(int i=0;i<TS;i++)
    {
      Sigma[i] = PedSigma[i];
    }

  //For Fitting
  Int_t npar = 3;
  TMinuit minuit(npar);

  minuit.SetPrintLevel(-1);

  minuit.SetFCN(fcn); //fcn cannot be a member function, so define it to be global; fcn calculates Chi^2 or NLL

  //define the model to be used
  Double_t xmin = -500;
  Double_t xmax = (kMAXADC + .5)*25.0;
  TF1 fV("fV", Pdf, xmin, xmax, npar);

  //parameters for the model
  Double_t par[npar];
  Double_t stepSize[npar];
  Double_t minVal[npar];
  Double_t maxVal[npar];
  TString parName[npar];
  //inital value
  par[0] = MaxTsAdc * 1.5;//700;       // guesses for starting the fit
  par[1] = 85;       // this MUST be done by some means to get things started
  par[2] = (MaxTimeBin+0.5)*25.0 - 60.0;//120;       //start time
  //step size
  stepSize[0] = TMath::Abs(par[0]*0.0001);   // usually 10% is OK for an initial step size, YMMV
  stepSize[1] = TMath::Abs(par[1]*0.001);   // step size MUST be positive!
  stepSize[2] = TMath::Abs(par[2]*0.001);
  //parameter range
  minVal[0] = 0.0;      // if min and max values = 0, parameter is unbounded.
  maxVal[0] = 0;//5000000;
  minVal[1] = 0;//30.0; 
  maxVal[1] = 0;//300;
  minVal[2] = 0;//10;
  maxVal[2] = 0;//300;
  //parameter name
  parName[0] = "A";
  parName[1] = "shape";
  parName[2] = "start";

  for (int i=0;i<npar;i++)
    {
      //cout<<endl<<i<<":"<<minVal[i]<<"  "<<maxVal[i]<<endl;
      minuit.DefineParameter(i, parName[i].Data(), par[i], stepSize[i], minVal[i], maxVal[i]);
      //minuit.DefineParameter(i, parName[i].Data(), par[i], stepSize[i], 0, 0); //Xinzhan: According to TMinuit suggestion, should not have paramete boundaries.
    }

  fparam = &fV; // Here passes our pdf to fcn
  minuit.Migrad(); //Minuit's best minimization do the fitting

  //get fitting results
  Double_t outpar[npar];
  Double_t err[npar];
  for(int i=0;i<npar;i++)
    {
      minuit.GetParameter(i,outpar[i],err[i]);
    }

  fV.SetParameters(outpar);
  MaxFromFitting = fV.GetMaximum(xmin, xmax);
  MaxTimeFromFitting = fV.GetMaximumX();
  //ReducedChisquareFromFitting = fV.GetChisquare()/fV.GetNDF();

  StartTime=fV.GetParameter(2);
  ShappingTime=fV.GetParameter(1);

  //Print the fitting
#ifdef SHOW_EVENT_INFO_HIT
  //if(MaxTimeFromFitting < 1.0)
  PrintFittingGraph(agr, fV);
#endif

  //reset all global variables
  assert((int)(PedSigma.size())==kMAXADC);
  //cout<<"GEMHIT"<<PedSigma.size()<<endl;
  for (int i=0; i<npar; i++)
    {
      X[i]=0;
      Y[i]=0;
      Sigma[i]=0;
    }
}

//==================================================================================
void GEMHit::PrintFittingGraph(TGraph &gr, TF1 &fV)
{
  TTimer *fTimer = new TTimer("gSystem->ProcessEvents();", 50, 0);
  TCanvas *ccStrips =  new TCanvas("ccStrips","",200,10,800,600);
  gr.SetTitle("Fit for signal on a single strip");
  gr.Draw("APL");
  fV.Draw("same");
  ccStrips->Update();
  fTimer->TurnOn();
  fTimer->Reset();
  Getline("Type <return> to continue: ");
  fTimer->TurnOff();
  if(fTimer) delete fTimer;
  if(ccStrips) delete ccStrips;
}

//==================================================================================
void GEMHit::Clone(GEMHit &hit)
{
  hit.Module = this->Module;
  hit.Plane = this->Plane;
  hit.Strip = this->Strip;
  hit.Charge = this->Charge;

  hit.StartTime = this->StartTime;
  hit.ShappingTime = this->ShappingTime;
  hit.ReducedChisquareFromFitting = this->ReducedChisquareFromFitting;
  hit.MaxFromFitting = this->MaxFromFitting;
  hit.MaxTimeFromFitting = this->MaxTimeFromFitting;
  hit.MaxTimeBin = this->MaxTimeBin;
  hit.MaxTsAdc = this->MaxTsAdc;

  hit.vTsAdc = this->vTsAdc;
  hit.PedSigma = this->PedSigma;
}
