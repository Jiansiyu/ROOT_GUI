#ifndef ROOT_TRACK_STATE
#define ROOT_TRACK_STATE

#include "utility.h"
#include "matrix.h"
#include "planeHit.h"
#include "trackSite.h"
#include "stepper.h"

class trackSite;
class stepper;

class trackState: public matrix{
 public:
  trackState(Int_t type = 0, Int_t p = kSdim);
  trackState(const matrix &sv, Int_t type = 0, Int_t p = kSdim);
  trackState(const matrix &sv, const matrix &c, Int_t type = 0, Int_t p = kSdim);
  trackState(const matrix &sv, const trackSite &site, Int_t type = 0, Int_t p = kSdim);
  trackState(const matrix &sv, const matrix &c, const trackSite &site, Int_t type = 0, Int_t p = kSdim);
  ~trackState();

  void propagate(trackSite* siteTo, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type = kAirMat);

  trackState* moveToNextPlane(planeHit* nextPlaneHit, matrix &F, matrix &Q, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type = kAirMat);
  trackState* predictSVatPlane(planeHit* nextPlaneHit, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type = kAirMat);

  trackState* predictSVatFirstPlane(planeHit *nextPlaneHit, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type = kAirMat); // Must follow function predictSVatPlane()
  void predictSVatNextPlane(planeHit *nextPlaneHit, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type = kAirMat); // Must follow function predictSVatFirstPlane() or predictSVatNextPlane()
  void predictSVatLastPlane(planeHit *nextPlaneHit, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type = kAirMat); // Must follow function predictSVatFirstPlane() or predictSVatNextPlane()

  void moveToNextPlane(trackState* prea, planeHit* nextPlaneHit, matrix &F, matrix &Q, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type = kAirMat);
  void predictSVatPlane(trackState* prea, planeHit* nextPlaneHit, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type = kAirMat);


  inline int getSiteType() const {return fSiteType;}
  inline void setSitePtr (trackSite  *s)   { fSitePtr = s; }
  inline trackSite* getSitePtr () const {return fSitePtr;}
  //inline planeHit* getPlaneHitPtr() const {return fSitePtr->getPlaneHitPtr();}
  //inline Double_t getPlanePosition() const{return fSitePtr->getPlanePosition();}

  inline void setPlanePosition(Double_t planePosition) { fPlanePosition = planePosition; }
  inline Double_t  getPlanePosition() const { return fPlanePosition; }

  inline void setPathTime(Double_t pathTime) { fPathTime = pathTime; }
  inline Double_t  getPathTime() const { return fPathTime; }

  inline void setPathMomentum(Double_t pathMomentum) { fPathMomentum = pathMomentum; }
  inline Double_t  getPathMomentum() const { return fPathMomentum; }

  inline matrix getCovMat() const { return fC; }
  inline void setCovMat      (const matrix &c) { fC = c; }

  inline matrix getPropMat() const { return fF; }
  inline void setPropMat      (const matrix &f) { fF = f; }
  inline matrix getPropTranMat() const { return fFt; }
  inline void setPropTranMat      (const matrix &ft) { fFt = ft; }

  inline matrix getNoiseMat() const { return fQ; }
  inline void setNoiseMat      (const matrix &q) { fQ = q; }

 private:
  Int_t fSiteType;    // (0,1,2,3) = (predicted,filtered,smoothed, invFiltered)
  trackSite  *fSitePtr; // pointer to corresponding site

  matrix fF;       // propagator matrix to next site (F = @f/@a)
  matrix fFt;      // transposed propagator matrix (F^T = (@f/@a)^T)
  matrix fQ;       // process noise from this to the next sites
  matrix fC;       // covariance matrix

  Double_t fPlanePosition;

  Double_t fPathTime;
  Double_t fPathMomentum;
                                   
  ClassDef(trackState,1)      // Base class for state vector objects

};


#endif
