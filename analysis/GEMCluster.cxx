#include "GEMCluster.h"
#include <cassert>

using namespace std;

//==================================================================================
GEMCluster::GEMCluster(Int_t module, Int_t plane, Int_t size, TObjArray &hitindex)
{
  Size = size;
  Module = module;
  Plane = plane;
  HitIndex=hitindex;
  //HitIndex->SetOwner(); //to be proved...

  Charge = GetCharge();
  Pos = GetPos();

  StartTime = 0;
  ShappingTime = 0;
  MaxTimeBin = 0;
  Fitting_ReducedChisquare = 0;
  MaxFromFitting=0;
  MaxTimeFromFitting=0;

  assert( (Int_t)HitIndex.GetEntries() == Size);

}

//==================================================================================
GEMCluster::~GEMCluster()
{
};

//==================================================================================
void GEMCluster::SetTiming(void)
{
  Int_t nhits = HitIndex.GetEntries();
  //double check that all strips are consecutive, remove this part for production
  for(int i=0;i<nhits-1;i++)
    {
      Int_t inext = ((GEMHit*)HitIndex[i+1])->Strip;
      Int_t icur =  ((GEMHit*)HitIndex[i])->Strip;
      assert( (inext-icur) == 1);
    }
  //Fill vector<Double_t> vTsAdc
  for(int ii=0;ii<kMAXADC;ii++)
    {
      Double_t adc=0;
      Double_t ped = 0;
      for (int j=0;j<nhits;j++)
	{
	  adc+=((GEMHit*)HitIndex[j])->vTsAdc[ii];
	  //cout<<(int)( ((GEMHit*)HitIndex[j])->PedSigma.size() )<<endl;
	  //assert((int)( ((GEMHit*)HitIndex[j])->PedSigma.size() ) == kMAXADC);
	  ped+=((GEMHit*)HitIndex[j])->PedSigma[ii];
	}
      vTsAdc.push_back(adc);
      //cout<<ped<<endl;
      PedSigma.push_back(ped);
      //cout<<ped<<endl;
    }

  TGraph gr;
  gr.SetMarkerStyle(22);

  vector<Double_t>::iterator iter = vTsAdc.begin();
  Double_t max_adc=0;
  for(;iter!=vTsAdc.end();++iter)
    {
      Int_t k = gr.GetN();
      //if(*iter > FittingThreshold)
      gr.SetPoint(k,25*((Double_t)k+0.5),*iter);
      X[k] = 25.0*((Double_t)k+0.5);
      Y[k] = *iter;
      if(*iter>max_adc)
	{
	  max_adc=*iter;
	  MaxTimeBin = k; //MaxTimeBin starts with 0
	}
    }
  for(int i=0;i<kMAXADC;i++)
    {
      Sigma[i]=PedSigma[i];
      //cout<<Sigma[i]<<endl;
    }

  //For the Fitting
  Int_t npar = 3;
  TMinuit minuit(npar);

  minuit.SetPrintLevel(-1);

  minuit.SetFCN(fcn); //fcn cannot be a member function, so define it to be global; fcn calculates Chi^2 or NLL

  //define the model to be used
  Double_t xmin = -500;
  Double_t xmax = (kMAXADC + 0.5)*25.0;
  TF1 fV("fV", Pdf, xmin, xmax, npar);

  //parameters for the model
  Double_t par[npar];
  Double_t stepSize[npar];
  Double_t minVal[npar];
  Double_t maxVal[npar];
  TString parName[npar];
  //inital value
  par[0] = max_adc * 1.5;//4200;       // guesses for starting the fit
  par[1] = 85;       // this MUST be done by some means to get things started
  par[2] = (MaxTimeBin + 0.5)*25.0 -60.0;//120;       //start time
  //step size
  stepSize[0] = TMath::Abs(par[0]*0.0001);   // usually 10% is OK for an initial step size, YMMV
  stepSize[1] = TMath::Abs(par[1]*0.001);   // step size MUST be positive!
  stepSize[2] = TMath::Abs(par[2]*0.001);
  //parameter range
  minVal[0] = 0;      // if min and max values = 0, parameter is unbounded.
  maxVal[0] = 0;//5000000;
  minVal[1] = 0;//30; 
  maxVal[1] = 0;//300;
  minVal[2] = 0;//10;
  maxVal[2] = 0;//300;
  //parameter name
  parName[0] = "A";
  parName[1] = "shape";
  parName[2] = "start";

  for (int i=0;i<npar;i++)
    {
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

  //Fill timing variables
  StartTime = fV.GetParameter(2);
  ShappingTime = fV.GetParameter(1);
  //Fitting_ReducedChisquare = fV.GetChisquare()/fV.GetNDF();
  MaxFromFitting = fV.GetMaximum(xmin, xmax);
  MaxTimeFromFitting = fV.GetMaximumX();

  //Print the fitting
#ifdef SHOW_EVENT_INFO_CLUSTER
  //if(MaxTimeFromFitting < 1.0)
  PrintFittingGraph(gr, fV);
#endif

  //reset all global variables
  for (int i=0; i<npar; i++)
    {
      X[i]=0;
      Y[i]=0;
      Sigma[i]=0;
    }
}

//==================================================================================
void GEMCluster::PrintFittingGraph(TGraph &gr, TF1 &fV)
{
  TTimer *fTimer = new TTimer("gSystem->ProcessEvents();", 50, 0);
  TCanvas *ccStrips =  new TCanvas("ccStrips","",200,10,800,600);
  gr.SetTitle("fit for signal in a cluster");
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
Double_t GEMCluster::GetPos(void)
{
  Double_t position=0;

  Double_t xstart = (-1.0)* kNbXAPVModule[Module]*128/2*kStripPitchX[Module];
  Double_t ystart = (-1.0)* kNbYAPVModule[Module]*128/2*kStripPitchY[Module];
  Double_t pitch=0;
  Double_t origin=0;
  if(Plane == 0)
    {
      origin = xstart;
      pitch = kStripPitchX[Module];
    }
  else
    {
      origin = ystart;
      pitch = kStripPitchY[Module];
    }
  Int_t entries = HitIndex.GetEntries();
  
  for(int i=0;i<entries;i++)//charge centroid position
    {
      position+=(origin+(((GEMHit*)HitIndex[i])->Strip)*pitch)*((GEMHit*)HitIndex[i])->Charge;
    }
  position/=Charge;
  return position;
}

//==================================================================================
Double_t GEMCluster::GetCharge(void)        ///******************************** 
{
  Double_t charge=0;
  Int_t entries = HitIndex.GetEntries();
  assert(entries == Size);
  for(int i=0; i<entries;i++)
    {
      charge+=((GEMHit*)HitIndex[i])->Charge;
    }
  return charge;
}

//==================================================================================
void GEMCluster::Clone(GEMCluster &gem_cluster)
{
  gem_cluster.Module = this->Module;
  gem_cluster.Plane = this->Plane;
  gem_cluster.Size = this->Size;
  gem_cluster.Charge = this->Charge;
  gem_cluster.Pos = this->Pos;

  gem_cluster.HitIndex = this->HitIndex;//(TObjArray*)this->HitIndex.Clone();
  //gem_cluster.HitIndex->SetOwner();

  gem_cluster.StartTime = this->StartTime;
  gem_cluster.ShappingTime = this->ShappingTime;
  gem_cluster.MaxFromFitting = this->MaxFromFitting;
  gem_cluster.MaxTimeFromFitting = this->MaxTimeFromFitting;
  gem_cluster.MaxTimeBin = this->MaxTimeBin;
  gem_cluster.Fitting_ReducedChisquare = this->Fitting_ReducedChisquare;

  gem_cluster.vTsAdc = this->vTsAdc;
  gem_cluster.PedSigma = this->PedSigma;
}
