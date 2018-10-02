#include "TCollection.h"

#include "trackSystem.h"


trackSystem *trackSystem::fgCurInstancePtr = 0;
//_____________________________________________________________________
trackSystem::trackSystem(Int_t n) : TObjArray(n), fNMissingHits(0),  fNHits(-1), fChi2(0.), fNDF(0), fCurSitePtr(0), fIsGood(kTRUE){
  for(Int_t i=0; i<kNTrackers; i++) fIsTrackerForTrack[i] = kFALSE;
}
//_____________________________________________________________________
trackSystem::~trackSystem(){
  if (this == fgCurInstancePtr) fgCurInstancePtr = 0;
  SetOwner(kTRUE);
  Delete();
}
//_____________________________________________________________________
void trackSystem::checkTrackStatus(){
  if(fNMissingHits > kNMissingHitsLimit) fIsGood=kFALSE;
  else fIsGood=kTRUE;
}
//_____________________________________________________________________
inline void trackSystem::Add(TObject *obj){
  TObjArray::Add(obj);
  fCurSitePtr = static_cast<trackSite*>(obj);
  fNHits++;
  fNDF = fNHits*kMdim - kSdim;
}

void trackSystem::smoothBackTo(Int_t k){
   TIter previous(this,kIterBackward);
   TIter cur     (this,kIterBackward);

   trackSite* prePtr;
   trackSite* curPtr = static_cast<trackSite *>(cur());
   trackState* cura   = curPtr->getStatePtr(trackSite::kFiltered);
   trackState* scura  = curPtr->getStatePtr(trackSite::kSmoothed);
   if (!scura) {
     trackState *newSV = new trackState(*cura, cura->getCovMat(), trackSite::kSmoothed);
     newSV->setPlanePosition(cura->getPlanePosition());
     newSV->setPathMomentum(cura->getPathMomentum());
     newSV->setPathTime(cura->getPathTime());
     curPtr->Add(newSV);
   }

   while ((curPtr = static_cast<trackSite *>(cur())) && (prePtr = static_cast<trackSite *>(previous()))) {
     curPtr->smooth(prePtr);
     fCurSitePtr = curPtr;
     if (IndexOf(curPtr) == k) break;
   }
}
//__________________________________________________________________________
Int_t trackSystem::Compare( const TObject* obj ) const{
   // Used for sorting tracks in a TSeqCollection (e.g. TList, TClonesArray).
   //a track is considered better than the other if it has more hits 
   //and has a smaller chi2 per degree of freedom
   assert( dynamic_cast<const trackSystem*>(obj) );
   const trackSystem* rhs = static_cast<const trackSystem*>(obj);
   
   if (getNHits() < rhs->getNHits()){
     return 1;
   }
   else if (getNHits() > rhs->getNHits()){
     return -1;
   }
   else{
     if (getChi2PerNDF() > rhs->getChi2PerNDF()){
       return 1;
     }
     else{
       return -1;
     }
   }
}


ClassImp(trackSystem)














