#include <cassert>
using namespace std;

#include "TMath.h"

#include "trackState.h"

//_________________________________________________________________
trackState::trackState(Int_t type, Int_t p) : matrix(p,1), fSiteType(type), fSitePtr(NULL), fF(p,p), fFt(p,p), fQ(p,p), fC(p,p){
   fF.UnitMatrix();
   fFt.UnitMatrix();
   fPlanePosition = 0;
   fPathTime=0;
   fPathMomentum=0;
}
//_________________________________________________________________
trackState::trackState(const matrix &sv, Int_t type, Int_t p) : matrix(sv), fSiteType(type), fSitePtr(NULL), fF(p,p), fFt(p,p), fQ(p,p), fC(p,p){
  fF.UnitMatrix();
  fFt.UnitMatrix();
  fPlanePosition = 0;
  fPathTime=0;
  fPathMomentum=0;
}
//_________________________________________________________________
trackState::trackState(const matrix &sv, const matrix &c, Int_t type, Int_t p) : matrix(sv), fSiteType(type), fSitePtr(nullptr), fF(p,p), fFt(p,p), fQ(p,p), fC(c){
  fF.UnitMatrix();
  fFt.UnitMatrix();
  fPlanePosition = 0;
  fPathTime=0;
  fPathMomentum=0;
}

//__________________________________________________________________
trackState::trackState(const matrix &sv, const trackSite &site, Int_t type, Int_t p) : matrix(sv), fSiteType(type), fSitePtr((trackSite*)&site), fF(p,p), fFt(p,p), fQ(p,p), fC(p,p){
  fF.UnitMatrix();
  fFt.UnitMatrix();
  fPlanePosition = 0;
  fPathTime=0;
  fPathMomentum=0;
}
//__________________________________________________________________
trackState::trackState(const matrix &sv, const matrix &c, const trackSite &site, Int_t type, Int_t p) : matrix(sv), fSiteType(type), fSitePtr((trackSite*)&site), fF(p,p), fFt(p,p), fQ(p,p), fC(c){
  fF.UnitMatrix();
  fFt.UnitMatrix();
  fPlanePosition = 0;
  fPathTime=0;
  fPathMomentum=0;
}
//___________________________________________________________________
trackState::~trackState(){}

//___________________________________________________________________
void trackState::propagate(trackSite* siteTo, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type){
   // Calculate 
   //    prea:  predicted state vector      : a^k-1_k = f_k-1(a_k-1)
   //    fF:    propagator derivative       : F_k-1   = (@f_k-1/@a_k-1)
   //    fQ:    process noise from k-1 to k : Q_k-1)

  trackState* prea = moveToNextPlane(siteTo->getPlaneHitPtr(), fF, fQ, particleId, particleMass, particleCharge, type);

  fFt = matrix(matrix::kTransposed, fF);

   // Calculate covariance matrix
   matrix preC = fF * fC * fFt + fQ;
   // Set predicted state vector and covariance matrix to next site
   prea->setCovMat(preC);
   siteTo->Add(prea);
   siteTo->SetOwner();
}

//______________________________________________________________________
trackState* trackState::moveToNextPlane(planeHit* nextPlaneHit, matrix &F, matrix &Q, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type){
   if(&Q){
     matrix sv(kSdim,1); 
     stepper::getInstance()->transport(this, nextPlaneHit, sv, F, Q, particleId, particleMass, particleCharge, type);
     trackState* thisState =  new trackState(sv, trackSite::kPredicted, kSdim);
     thisState->setPlanePosition(nextPlaneHit->getPlanePosition());
     thisState->setPathMomentum(stepper::getInstance()->getPathMomentum());
     thisState->setPathTime(stepper::getInstance()->getPathTime());
     return thisState;
   } 
   else{
     return nullptr;
   }
}
//______________________________________________________________________
trackState* trackState::predictSVatPlane(planeHit* nextPlaneHit, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type){
  //simply pass the current filtered state vector to the next plane
  //it is up to the track finder to decide whether we have a hit in the 
  //next measurement layer

  matrix thisF (kSdim, kSdim);
  matrix thisQ (kSdim, kSdim); 

  trackState* prea    = moveToNextPlane(nextPlaneHit, thisF, thisQ, particleId, particleMass, particleCharge, type);
  matrix thisFt = matrix(matrix::kTransposed, thisF);

  // Calculate covariance matrix
  matrix preC = thisF * fC * thisFt + thisQ;

  prea->setPropMat(thisF);
  prea->setPropTranMat(thisFt);
  prea->setNoiseMat(thisQ);
  prea->setCovMat(preC);

  return prea;
}
//______________________________________________________________________

trackState* trackState::predictSVatFirstPlane(planeHit* nextPlaneHit, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type){
  //If detectors are considered as several planes (front, middle/measurement, back, or other planes), predicted state vector need to be updated until measurement planes

  matrix thisF (kSdim, kSdim);
  matrix thisQ (kSdim, kSdim); 
  trackState* prea    = moveToNextPlane(nextPlaneHit, thisF, thisQ, particleId, particleMass, particleCharge, type);
  matrix thisFt = matrix(matrix::kTransposed, thisF);

  prea->setPropMat(thisF);
  prea->setNoiseMat(thisQ);
  prea->setCovMat(fC);

  return prea;
}
//______________________________________________________________________
void trackState::predictSVatNextPlane(planeHit* nextPlaneHit, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type){
  //this function can be called only if PredictSVatFirstPlane or predictSVatNextPlane has been called
  assert(fSiteType == trackSite::kPredicted);
  matrix thisSV (kSdim, 1);
  matrix thisF (kSdim, kSdim);
  matrix thisQ (kSdim, kSdim);

  stepper::getInstance()->transport(this, nextPlaneHit, thisSV, thisF, thisQ, particleId, particleMass, particleCharge, type);
  for(Int_t i=0; i< kSdim; i++){(*this)(i,0)=thisSV(i,0);}
  fF = thisF*fF;
  fFt = matrix(matrix::kTransposed, fF);
  fQ = fQ + thisQ;

  this->setPropMat(fF);
  this->setNoiseMat(fQ);
  this->setPlanePosition(nextPlaneHit->getPlanePosition());
  this->setPathMomentum(stepper::getInstance()->getPathMomentum());
  this->setPathTime(stepper::getInstance()->getPathTime());
}

void trackState::predictSVatLastPlane(planeHit* nextPlaneHit, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type){
  //this function can be called only if predictSVatNextPlane has been called
  assert(fSiteType == trackSite::kPredicted);
  matrix thisSV (kSdim, 1);
  matrix thisF (kSdim, kSdim);
  matrix thisQ (kSdim, kSdim);   

  stepper::getInstance()->transport(this, nextPlaneHit, thisSV, thisF, thisQ, particleId, particleMass, particleCharge, type);
  for(Int_t i=0; i< kSdim; i++){(*this)(i,0)=thisSV(i,0);}
  fF = thisF*fF;
  fFt = matrix(matrix::kTransposed, fF);
  fQ = fQ + thisQ;
  
  matrix preC = fF * fC * fFt + fQ;
  this->setPropMat(fF);
  this->setPropTranMat(fFt);
  this->setNoiseMat(fQ);
  this->setCovMat(preC);
  this->setPlanePosition(nextPlaneHit->getPlanePosition());
  this->setPathMomentum(stepper::getInstance()->getPathMomentum());
  this->setPathTime(stepper::getInstance()->getPathTime());
}


//______________________________________________________________________
void trackState::moveToNextPlane(trackState* prea, planeHit* nextPlaneHit, matrix &F, matrix &Q, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type){
  matrix sv(kSdim,1); 
  stepper::getInstance()->transport(this, nextPlaneHit, sv, F, Q, particleId, particleMass, particleCharge, type);
  for(Int_t i=0; i< kSdim; i++){(*prea)(i,0)=sv(i,0);}
  prea->setPlanePosition(nextPlaneHit->getPlanePosition());
  prea->setPathMomentum(stepper::getInstance()->getPathMomentum());
  prea->setPathTime(stepper::getInstance()->getPathTime());

}
//______________________________________________________________________
void trackState::predictSVatPlane(trackState* prea, planeHit* nextPlaneHit, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type){
  //simply pass the current filtered state vector to the next plane
  //it is up to the track finder to decide whether we have a hit in the 
  //next measurement layer
  matrix thisF (kSdim, kSdim);
  matrix thisQ (kSdim, kSdim); 
  moveToNextPlane(prea, nextPlaneHit, thisF, thisQ, particleId, particleMass, particleCharge, type);
  matrix thisFt = matrix(matrix::kTransposed, thisF);

  // Calculate covariance matrix
  matrix preC = thisF * fC * thisFt + thisQ;

  prea->setPropMat(thisF);
  prea->setPropTranMat(thisFt);
  prea->setNoiseMat(thisQ);
  prea->setCovMat(preC);
}
//______________________________________________________________________


                         
ClassImp(trackState)




















