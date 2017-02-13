#ifndef CONFIG_H
#define CONFIG_H


//#define SHOW_EVENT_INFO_HIT
//#define SHOW_EVENT_INFO_CLUSTER

const int kNMODULE = 1;
//How many chambers
const int kMAXADC = 6;
//How many Time Samples

const Double_t kNbXAPVModule[] = {2, 12, 2, 2, 10};
const Double_t kNbYAPVModule[] = {2, 10, 2, 2, 10};
//This is how many APVs a plane can hold, not how many APVs are actually on that plane.
//Postition Constants; each APV has 128 channels, each channel is 4e-2cm, so we can compute the position based on the # of APVs
//Thus, x side positions for the first module:
//X: from (-1.0)*kNbAPVXModule[0]/2*128*kStripPitchX[0] to (1.0)*kNbAPVXModule[0]/2*128*kStripPitchX[0]
//Y: from (-1.0)*kNbAPVYModule[0]/2*128*kStripPitchY[0] to (1.0)*kNbAPVYModule[0]/2*128*kStripPitchY[0]

//distance between each two strips
const Double_t kStripPitchX[] = {4e-2, 4e-2, 4e-2, 4e-2, 4e-2, 4e-2};
const Double_t kStripPitchY[] = {4e-2, 4e-2, 4e-2, 4e-2, 4e-2, 4e-2};

//Position Constants; Units in cm
//not using the following two lines
const Double_t kXStartModule[] = {-20.48, -10.24, 0, 0, 0, 0};  // Here, Units is in cm, origin of the chamber is in their bottom left corner
const Double_t kYStartModule[] = {-20.48, -10.24, 0, 0, 0, 0};

const Double_t kStartingTimeLow = 0;
const Double_t kStartingTimeHigh = 30;
//Actually we have a offset ~12.5 hard-coded, so the real starting time is from 7.5-12.5 = -5ns to 15-12.5 = 2.5ns
const Double_t kMaxChargeRatio = 1.5;
const Double_t kMinChargeRatio = 0.5;
//Put a cut to the charge sharing ratio distribution

const Int_t kMAXNCH = 1209;
//How many strips can be fired in one event
const Int_t kNAPV = 640; 
//Maximum number of APVs this program can hold
//Refer to GEMTracking::APVIndex(Int_t aKey)  -->  each module has two planes, each plane can hold up to 40 APVs
//kNAPV = 640 means maximum 8 layers of modules.

//To secure the code
const Double_t kMaxStripADC = 2000; //maximum value of the shape on a strip
const Double_t kMaxSignalADC = 15000; //Max for the Integral of the signal shape on a strip
const Double_t kMinSignalADC = 0;
const Int_t kMaxStripsPerEvent = 1000;
const Int_t kMinStripsinCluster = 2;
const Int_t kMaxStripsinCluster = 100;

//Xinzhan: I changed to anther fitting method, so the following section is not being used any more
//Fitting starts at ith time sample
const Double_t FittingTSLow=0.0;
const Double_t FittingTSHigh=kMAXADC;
const Double_t FittingThreshold = 100.0;//not used for now

//Read pedestal
//Input the pedestal File name here, put it under the same directory with ./gem
const TString fPedFileName = "./pedestal/pedestal_20150317.root";
//GEM mapping
//Mapping : apv id (left to right) increase (x: -1 - 11; y: 12-21)
//apvNo = Strip%128
const Int_t XmapID[]={96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107};
const Int_t XmapCH[]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const Int_t XmapFEC[]={6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6};
const Int_t YmapID[]={32, 33, 34, 35, 36, 38, 39, 40, 41, 42};
const Int_t YmapCH[]={0, 1, 2, 3, 4, 6, 7, 8, 9, 10};
const Int_t YmapFEC[]={2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

//The following two sections are not being used any more; obosolete
//Hitting Position for SBS1 50X50
const Int_t kXDelimit[] = {0, 150, 400, 650, 900, 1150, 1280}; // 6 sections
const Int_t kYDelimit[] = {0, 300, 650, 950, 1280}; // 4 sections
//Thus in all, 24 hitting spots

typedef struct{
  Int_t section;
  Double_t charge;
  Double_t timebin;
} Struct_MaxTimeBin;

//char out_put_filename_global[100];

#endif
