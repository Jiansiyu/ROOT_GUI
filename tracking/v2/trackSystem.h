#ifndef ROOT_TRACK_SYSTEM
#define ROOT_TRACK_SYSTEM

#include "TObjArray.h"
#include "TMath.h"

#include "utility.h"
#include "trackSite.h"
#include "trackState.h"

//class matrix;
class trackState;

class trackSystem: public TObjArray{
  friend class trackSite;
  
  public:
  trackSystem(Int_t n = 1);
  ~trackSystem();
  void Add(TObject *obj);
  void smoothBackTo(Int_t k);


  inline void      setSeedType(Int_t type) { fSeedType = type; }
  inline Int_t  getSeedType() const { return fSeedType; }

  void checkTrackStatus();
  inline Bool_t    getTrackStatus() const   { return fIsGood; }
  inline void      setTrackStatus(Bool_t isGood) { fIsGood = isGood; }
  
  static  trackSystem *getCurInstancePtr() { return fgCurInstancePtr; }
  void setCurInstancePtr(trackSystem *ksp) { fgCurInstancePtr = ksp; }

  inline trackSite* getCurSite() { return fCurSitePtr;}
  void increaseChi2(Double_t deltaChi2) { fChi2 += deltaChi2; }

  inline void addMissingHits() { fNMissingHits++; } 
  inline void setMissingHits(Int_t n) { fNMissingHits = n;  }
  inline Double_t  getMissingHits() const   { return fNMissingHits; }

  inline void setNHits(Int_t n) { fNHits = n;  }
  inline Int_t getNHits() const   { return fNHits; }

  inline void setChi2(Double_t chi2) { fChi2 = chi2;  }
  inline Double_t getChi2() const   { return fChi2; }
  inline void setNDF(Int_t ndf ) { fNDF = ndf;  }
  inline Double_t getNDF() const   { return fNDF; }

  inline Double_t getChi2PerNDF() const {return fChi2/fNDF;}

  virtual Int_t Compare( const TObject* obj ) const;
  virtual Bool_t IsSortable () const { return kTRUE; }

  inline void setIsTrackerForTrack(Bool_t isTrackerForTrack[]) { for(Int_t i=0; i<kNTrackers; i++) fIsTrackerForTrack[i] = isTrackerForTrack[i]; }
  inline void getIsTrackerForTrack(Bool_t isTrackerForTrack[]) { for(Int_t i=0; i<kNTrackers; i++) isTrackerForTrack[i] = fIsTrackerForTrack[i]; }
       
 private:
  static trackSystem *fgCurInstancePtr;  //! currently active instance

  Int_t fSeedType;
  Int_t fNMissingHits;
  Int_t fNHits;

  Double_t fChi2;  // current total chi2
  Int_t fNDF;

  Double_t     fMomentum;

  trackSite* fCurSitePtr;  // pointer to current site

  Bool_t       fIsGood;      // for track fitting monitering 

  Bool_t fIsTrackerForTrack[kNTrackers]; // if a tracker is used to construct track

  vector<planeHit*> trackerHitVectArray[kNTrackers];
  vector<Double_t> dafWeightVectArray[kNTrackers];

  ClassDef(trackSystem,1)  // Base class for Kalman Filter

};
#endif
