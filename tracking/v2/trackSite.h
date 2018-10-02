#ifndef ROOT_TRACK_SITE
#define ROOT_TRACK_SITE

#include "TObjArray.h"
#include "TMath.h"

#include "utility.h"
#include "matrix.h"
#include "planeHit.h"
#include "trackState.h"

class trackState;

class trackSite : public TObjArray{
  friend class trackSystem;
  
 public:
  enum siteType {kPredicted = 0, kFiltered, kSmoothed, kInvFiltered};
                  
  //trackSite(Int_t m = kMdim, Int_t p = kSdim, Double_t chi2 = 150.);
  trackSite(planeHit* hit, Int_t m = kMdim, Int_t p = kSdim , Double_t chi2 = 150.);

  ~trackSite();

  void Add(TObject *obj);
  Bool_t filter(Double_t particleCharge);
  void smooth(trackSite* pre);

  inline planeHit* getPlaneHitPtr() const { return fPlaneHitPtr; }  
  inline planeHit getPlaneHit() const { return fPlaneHit; }
  inline Double_t getPlanePosition() const {return fPlanePosition;}

  inline void setPathTime(Double_t pathTime) { fPathTime = pathTime; }
  inline Double_t  getPathTime() const { return fPathTime; }

  inline void setPathMomentum(Double_t pathMomentum) { fPathMomentum = pathMomentum; }
  inline Double_t  getPathMomentum() const { return fPathMomentum; }

  inline trackState* getCurState() {return fCurStatePtr; }
  trackState* getStatePtr (siteType t);
  inline Double_t getDeltaChi2() const {return fDeltaChi2;}

  inline void setM(matrix M) { fM = M; }
  inline matrix  getM() const { return fM; }

  inline void setV(matrix V) { fV = V; }
  inline matrix  getV() const { return fV; }

  inline void setH(matrix H) { fH = H; }
  inline matrix  getH() const { return fH; }

  inline void setHT(matrix HT) { fHT = HT; }
  inline matrix  getHT() const { return fHT; }

 private:
  trackState*  fCurStatePtr; // pointer to current best state
  matrix fM;           // measurement vector: M(m,1)
  matrix fV;           // noise matrix: M(m,m)
  matrix fH;           // H = (@h/@a): M(m,p)
  matrix fHT;          // H^t = (@h/@a)^t: M(p,m)

  Double_t fDeltaChi2;   // chi2 increment
  Double_t fMaxDeltaChi2;
  
  planeHit* fPlaneHitPtr;
  planeHit fPlaneHit;
  Double_t fPlanePosition;

  Double_t fPathMomentum;
  Double_t fPathTime;

  
  ClassDef(trackSite,1)      // Base class for measurement vector objects

};


#endif
