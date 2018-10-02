#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
using namespace std;

#include "trackSite.h"



//trackSite::trackSite(Int_t m, Int_t p, Double_t chi2) : TObjArray(2), fCurStatePtr(0), fM(m,1), fV(m,m), fH(m,p), fHT(p,m), fDeltaChi2(0.), fMaxDeltaChi2(chi2){}
//_________________________________________________________________________
trackSite::trackSite(planeHit *hit, Int_t m, Int_t  p, Double_t chi2) : TObjArray(2), fCurStatePtr(0), fM(m,1), fV(m,m),fH(m,p), fHT(p,m), fDeltaChi2(0.), fMaxDeltaChi2(chi2), fPlaneHitPtr(hit) {

  fPlaneHit=*fPlaneHitPtr;

  fM(kIdxX, 0) = hit->getX(); 
  fM(kIdxY, 0) = hit->getY();

  fV(kIdxX, kIdxX) = pow(hit->getXError(), 2);
  fV(kIdxY, kIdxY) = pow(hit->getYError(), 2);

  fH.Zero();
  fH(0, 0) = 1.;
  fH(1, 1) = 1.;

  fHT = matrix(matrix::kTransposed, fH);

  fPlanePosition=hit->getPlanePosition();

  fPathMomentum=0;
  fPathTime=0;
}
//_________________________________________________________________________
trackSite::~trackSite(){
  SetOwner(kTRUE);
  Delete();
}
//_________________________________________________________________________

void trackSite::Add(TObject *obj){
   TObjArray::Add(obj);
   fCurStatePtr = static_cast<trackState*>(obj);
   fCurStatePtr->setSitePtr(this);
   setPathMomentum(fCurStatePtr->getPathMomentum());
   setPathTime(fCurStatePtr->getPathTime());
}

//___________________________________________________________________________
Bool_t trackSite::filter(Double_t particleCharge){
   // prea and preC should be preset
   trackState *preSV = getStatePtr(trackSite::kPredicted);
   matrix preC  = getStatePtr(trackSite::kPredicted)->getCovMat();
   matrix preh(kMdim, 1);
   preh(kIdxX, 0)=(*preSV)(kIdxX, 0);
   preh(kIdxY, 0)=(*preSV)(kIdxY, 0);
   matrix pull  = fM - preh;
   matrix preR    = fV + fH * preC * fHT;

   if(preR.Determinant() == 0) return kFALSE;
   matrix preRInv = matrix(matrix::kInverted, preR);

   matrix I(kSdim, kSdim);
   I.UnitMatrix();
   matrix K = preC * fHT *  preRInv;
   matrix curC = (I - K * fH) * preC;
   matrix curSV = *preSV + K * pull;

   trackState *newSV = new trackState(curSV, curC, kFiltered);
   newSV->setPlanePosition(preSV->getPlanePosition());
   newSV->setPathMomentum(preSV->getPathMomentum());
   newSV->setPathTime(preSV->getPathTime());
   newSV->setNoiseMat(preSV->getNoiseMat());
   newSV->setPropMat(preSV->getPropMat());
   newSV->setPropTranMat(preSV->getPropTranMat());
   Add(newSV);

   // Calculate chi2 increment
   matrix curh(kMdim, 1);
   curh(kIdxX, 0)=curSV(kIdxX, 0);
   curh(kIdxY, 0)=curSV(kIdxY, 0);
   matrix resVec = fM - curh;
   matrix resVecT = matrix(matrix::kTransposed, resVec);
   matrix VInv = matrix(matrix::kInverted, fV);
   matrix resSV = curSV - *preSV;
   matrix resSVT = matrix(matrix::kTransposed, resSV);
 
   if(preC.Determinant() == 0) return kFALSE;
   matrix preCInv = matrix(matrix::kInverted, preC);

   fDeltaChi2=(resSVT * preCInv * resSV + resVecT * VInv * resVec)(0,0);
 

   if(fDeltaChi2 < fMaxDeltaChi2) return kTRUE;
   else return kFALSE;
}
//______________________________________________________________________________
void trackSite::smooth(trackSite* pre){
   if (getStatePtr(trackSite::kSmoothed)) return;

   trackState* cura  = getStatePtr(trackSite::kFiltered);
   trackState* prea  = pre->getStatePtr(trackSite::kPredicted);
   trackState* sprea = pre->getStatePtr(trackSite::kSmoothed);

   matrix curC    = cura->getCovMat();
   matrix curFt   = cura->getPropTranMat();
   matrix preC    = prea->getCovMat();
   matrix spreC   = sprea->getCovMat();
   matrix preCinv = matrix(matrix::kInverted, preC);
   matrix curA    = curC * curFt * preCinv;
   matrix curAt   = matrix(matrix::kTransposed, curA);
   matrix scurC   = curC + curA * (spreC - preC) * curAt;

   matrix sv = *cura + curA * (*sprea - *prea);
   trackState *newSV = new trackState(sv, scurC, kSmoothed, cura->getPlanePosition());
   newSV->setPathMomentum(cura->getPathMomentum());
   newSV->setPathTime(cura->getPathTime());
   Add(newSV);
   SetOwner();
}

trackState* trackSite::getStatePtr(trackSite::siteType t){
  trackState *ap = 0;
  if (t >= 0 && t < GetEntries()) {
    ap = static_cast<trackState*>(UncheckedAt(t));
  }
  return ap;
}



ClassImp(trackSite)

























