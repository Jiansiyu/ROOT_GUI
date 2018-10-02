#ifndef ROOT_TRACK
#define ROOT_TRACK

#include <vector>
#include <iostream>
using namespace std;

#include "utility.h"
#include "TObject.h"

class track : public TObject{
 public:
  track();
  ~track(){}

  inline void setNHits(Int_t n) { fNHits = n; }
  inline Int_t  getNHits() const { return fNHits; } 

  inline void setChi2PerNDF(Double_t chi2PerNDF) { fChi2PerNDF = chi2PerNDF; }
  inline Double_t  getChi2PerNDF() const { return fChi2PerNDF; } 

  inline void setTrackersPathMomentum(Double_t trackersPathMomentum[]) {for(Int_t i=0; i<kNTrackers; i++) fTrackersPathMomentum[i]=trackersPathMomentum[i];}  
  inline void  getPathTrackersMomentum(Double_t trackersPathMomentum[]) {for(Int_t i=0; i<kNTrackers; i++) trackersPathMomentum[i]=fTrackersPathMomentum[i];} 

  inline void setTrackersPathTime(Double_t trackersPathTime[]) {for(Int_t i=0; i<kNTrackers; i++) fTrackersPathTime[i] = trackersPathTime[i];}
  inline void  getTrackersPathTime(Double_t trackersPathTime[]) { for(Int_t i=0; i<kNTrackers; i++) trackersPathTime[i] = fTrackersPathTime[i];} 

  inline void setTrackersMX(Double_t trackersMX[]) {for(Int_t i=0; i<kNTrackers; i++) fTrackersMX[i] = trackersMX[i];}
  inline void  getTrackersMX(Double_t trackersMX[]) { for(Int_t i=0; i<kNTrackers; i++) trackersMX[i] = fTrackersMX[i];} 
  inline void setTrackersMY(Double_t trackersMY[]) {for(Int_t i=0; i<kNTrackers; i++) fTrackersMY[i] = trackersMY[i];}
  inline void  getTrackersMY(Double_t trackersMY[]) { for(Int_t i=0; i<kNTrackers; i++) trackersMY[i] = fTrackersMY[i];} 
  inline void setTrackersMXErr(Double_t trackersMXErr[]) {for(Int_t i=0; i<kNTrackers; i++) fTrackersMXErr[i] = trackersMXErr[i];}
  inline void  getTrackersMXErr(Double_t trackersMXErr[]) { for(Int_t i=0; i<kNTrackers; i++) trackersMXErr[i] = fTrackersMXErr[i];} 
  inline void setTrackersMYErr(Double_t trackersMYErr[]) {for(Int_t i=0; i<kNTrackers; i++) fTrackersMYErr[i] = trackersMYErr[i];}
  inline void  getTrackersMYErr(Double_t trackersMYErr[]) { for(Int_t i=0; i<kNTrackers; i++) trackersMYErr[i] = fTrackersMYErr[i];} 

  inline void setTrackersSX(Double_t trackersSX[]) {for(Int_t i=0; i<kNTrackers; i++) fTrackersSX[i] = trackersSX[i];}
  inline void  getTrackersSX(Double_t trackersSX[]) { for(Int_t i=0; i<kNTrackers; i++) trackersSX[i] = fTrackersSX[i];} 
  inline void setTrackersSY(Double_t trackersSY[]) {for(Int_t i=0; i<kNTrackers; i++) fTrackersSY[i] = trackersSY[i];}
  inline void  getTrackersSY(Double_t trackersSY[]) { for(Int_t i=0; i<kNTrackers; i++) trackersSY[i] = fTrackersSY[i];} 
  inline void setTrackersSTx(Double_t trackersSTx[]) {for(Int_t i=0; i<kNTrackers; i++) fTrackersSTx[i] = trackersSTx[i];}
  inline void  getTrackersSTx(Double_t trackersSTx[]) { for(Int_t i=0; i<kNTrackers; i++) trackersSTx[i] = fTrackersSTx[i];} 
  inline void setTrackersSTy(Double_t trackersSTy[]) {for(Int_t i=0; i<kNTrackers; i++) fTrackersSTy[i] = trackersSTy[i];}
  inline void  getTrackersSTy(Double_t trackersSTy[]) { for(Int_t i=0; i<kNTrackers; i++) trackersSTy[i] = fTrackersSTy[i];} 
  
 private:
  Int_t fNHits;
  Double_t fChi2PerNDF;

  Double_t fTrackersPathMomentum[kNTrackers];
  Double_t fTrackersPathTime[kNTrackers];

  Double_t fTrackersMX[kNTrackers];
  Double_t fTrackersMY[kNTrackers];
  Double_t fTrackersMXErr[kNTrackers];
  Double_t fTrackersMYErr[kNTrackers];

  Double_t fTrackersSX[kNTrackers];
  Double_t fTrackersSY[kNTrackers];
  Double_t fTrackersSTx[kNTrackers];
  Double_t fTrackersSTy[kNTrackers];

  ClassDef(track, 1)
};
#endif
























