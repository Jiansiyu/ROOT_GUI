#include "math.h"
#include <vector>
#include <cassert>
#include "TF1.h"
using namespace std;

#include "TMath.h"
#include "TRandom3.h"

#include "trackFinder.h"

planeHit* trackFinder::initHit = new planeHit(kInit, kPlanePosition[kInit]);

trackFinder::trackFinder(particleId id, Double_t mass, Double_t charge, Double_t initMomentum, tracker* uTrackers[kNTrackers]) : particle(id, mass, charge), fInitMomentum(initMomentum), fNSeeds(0), fNGoodTracks(0), fChi2PerMdimCut(kChi2PerMdimCut){
  fCoarseTracks = new TClonesArray("trackSystem", MAXNTRACKS, kTRUE);
  for(Int_t i=0; i<kNTrackers; i++) fTrackers[i] = uTrackers[i];
  fSeedVect.clear();
  fDafInfoVect.clear();
}
//______________________________________________________________________________

trackFinder::~trackFinder(){
  Clear();
  fCoarseTracks->SetOwner(kTRUE);
  delete fCoarseTracks;
}
//______________________________________________________________________________

void trackFinder::Clear( Option_t* opt ){
  fSeedVect.clear();
  fDafInfoVect.clear();

  map<Int_t, vector<planeHit> >::iterator it;
  for (it = fGoodHits.begin(); it != fGoodHits.end(); it++) { (it->second).clear(); }
  fGoodHits.clear();
}
//______________________________________________________________________________

Bool_t trackFinder::processHits(TClonesArray* theTracks){  
  if(!findSeed()) return kFALSE;
  if(!initDafInfo(kDafAnnealingFactor[0])) return kFALSE;

  for(Int_t iDaf=1; iDaf<kDafNTranings; iDaf++){
    if(fCoarseTracks->GetEntries()!=0) fCoarseTracks->Clear();
    if(!trackFilter()) return kFALSE;
    trackSmoothBack();

    if(iDaf != kDafNTranings -1){
      if(!updateDafInfo(kDafAnnealingFactor[iDaf])) return kFALSE;
    }

    cout<<"traing: "<<iDaf<<"!!!!!!"<<endl;
    for(unsigned i=0;i<fDafInfoVect.size();i++){
      cout<<"  track: "<<i<<" $$$"<<endl;
      for(Int_t j=0; j<kNTrackers; j++){ 
	cout<<"    tracker: "<<j<<endl;
	cout<<"      ";
	for(Int_t k=0; k<(fDafInfoVect[i].dafWeightVectArray)[j].size(); k++){
	  if(k!=(fDafInfoVect[i].dafWeightVectArray)[j].size()-1) cout<<(fDafInfoVect[i].dafWeightVectArray)[j][k]<<", ";
	  else cout<<(fDafInfoVect[i].dafWeightVectArray)[j][k]<<endl;
	  //cout<<(fDafInfoVect[i].trackerHitVectArray)[j][k].getX()<<endl;
	}
	cout<<endl;
      }
    }


  }

  if(!finalSelection(theTracks)) return kFALSE;

  return kTRUE;
}

//______________________________________________________________________________
void trackFinder::chooseTrackersForSeed (vector<Int_t> index, vector<Int_t> numOfHits, Int_t &trackerIndex1, Int_t &trackerIndex2){
  Int_t hitIndex1, hitIndex2;
  Int_t index1, index2;
  if(numOfHits[0] <= numOfHits[1]){
    hitIndex1 = 0;
    hitIndex2 = 1;
    index1 = index[0]; 
    index2 = index[1];
  }
  else{
    hitIndex1 = 1;
    hitIndex2 = 0;
    index1 = index[1]; 
    index2 = index[0];
  }

  for(Int_t i=2; i<index.size(); i++){
    if(numOfHits[i] < numOfHits[hitIndex1]){
      hitIndex2 = hitIndex1;
      hitIndex1 = i;
      index2 = index1;
      index1 = index[i];
    }
    else if(numOfHits[i] < numOfHits[hitIndex2] && numOfHits[i] >= numOfHits[hitIndex1]){
      hitIndex2 = i; 
      index2 = index[i];
    }
  }

  if(index1 < index2){
    trackerIndex1=index1;
    trackerIndex2=index2;
  }
  else if(index1 > index2){
    trackerIndex1=index2;
    trackerIndex2=index1;
  }
}

//______________________________________________________________________________
Bool_t trackFinder::findSeed(){ // All types of seeds are stored in the pool
  fSeedVect.clear();

  // Begin: get indices of trackers to construct seeds
  Int_t type;
  vector<planeHit> vectHit;
  Double_t initState[4];
  Double_t initCov[4];

  vector<Int_t> index;
  vector<Int_t> numOfHits;
  vector<Int_t> numOfHitsAllTrackers;
  index.clear();
  numOfHits.clear();
  numOfHitsAllTrackers.clear();
  Int_t num;
  for(Int_t i=0; i<kNTrackers; i++){
    num = fTrackers[i]->getNHits();
    numOfHitsAllTrackers.push_back(num);
    if(num > 0){
      index.push_back(i);
      numOfHits.push_back(num);
    }
  }
  if(index.size()<2) return kFALSE;
  else{
    Int_t trackerIndex1, trackerIndex2;
    chooseTrackersForSeed(index, numOfHits, trackerIndex1, trackerIndex2);
    type=(trackerIndex1+1)*10 + trackerIndex2+1;
    // End: get indices of trackers to construct seeds

    for(int i=0; i<numOfHitsAllTrackers[trackerIndex1]; i++){
      for(int j=0; j<numOfHitsAllTrackers[trackerIndex2]; j++){
	planeHit *hitA=(planeHit*)fTrackers[trackerIndex1]->getHits()->At(i);
	planeHit *hitB=(planeHit*)fTrackers[trackerIndex2]->getHits()->At(j); 
	calcInitStateCoV(hitA, hitB, initState, initCov);
	vectHit.clear();
	vectHit.push_back(*hitA);
	vectHit.push_back(*hitB);
	fSeedVect.push_back(seed(type, trackerIndex1, trackerIndex2, vectHit, initState, initCov));
      } 
    }

    return kTRUE;
  }
}

//______________________________________________________________________________
void trackFinder::calcInitStateCoV(planeHit* hitA, planeHit* hitB, Double_t initState[], Double_t initCov[]){

  Double_t xA = hitA->getX();
  Double_t yA = hitA->getY();
  Double_t xAErr = hitA->getXError();
  Double_t yAErr = hitA->getYError();
  Double_t xB = hitB->getX();
  Double_t yB = hitB->getY();
  Double_t xBErr = hitB->getXError();
  Double_t yBErr = hitB->getYError();

  Double_t zA = hitA->getPlanePosition();
  Double_t zB = hitB->getPlanePosition();
  Double_t z0 = kPlanePosition[kInit];

  Double_t x = (zB-z0)*xA/(zB-zA) - (zA-z0)*xB/(zB-zA);
  Double_t y = (zB-z0)*yA/(zB-zA) - (zA-z0)*yB/(zB-zA);
  Double_t tx = (xB-xA)/(zB-zA);
  Double_t ty = (yB-yA)/(zB-zA);

  Double_t varX = pow((zB-z0)/(zB-zA)*xAErr,2) + pow((zA-z0)/(zB-zA)*xBErr,2);
  Double_t varY = pow((zB-z0)/(zB-zA)*yAErr,2) + pow((zA-z0)/(zB-zA)*yBErr,2);
  Double_t varTx= pow(xAErr/(zB-zA),2) + pow(xBErr/(zB-zA),2);
  Double_t varTy= pow(yAErr/(zB-zA),2) + pow(yBErr/(zB-zA),2);

  initState[0] = x;
  initState[1] = y;
  initState[2] = tx;
  initState[3] = ty;

  Double_t factor = 4;
  initCov[0]=varX * factor;
  initCov[1]=varY * factor;
  initCov[2]=varTx * factor;
  initCov[3]=varTy * factor;
}
//______________________________________________________________________________

Bool_t trackFinder::initDafInfo(Double_t annealingFactor){
  
  Bool_t isTrackerForSeed[kNTrackers];
  Int_t trackerIndex1, trackerIndex2;
  trackerIndex1 = fSeedVect[0].trackerIndex1;
  trackerIndex2 = fSeedVect[0].trackerIndex2;
  for(Int_t i=0; i<kNTrackers; i++){
    if(i == trackerIndex1 || i == trackerIndex2) isTrackerForSeed[i] = kTRUE;
    else isTrackerForSeed[i] = kFALSE;
  }

  vector<planeHit> hitVectArray[kNTrackers]; 
  vector<Double_t> weightVectArray[kNTrackers];
  Bool_t isForSeedVectArray[kNTrackers];

  Double_t planePosition;
  matrix S(kMdim, 1);

  if(fSeedVect.size()==0) return kFALSE;
  else{
    for(unsigned i=0;i<fSeedVect.size();i++){ // Begin loop for seeds
      for(Int_t j=0; j<kNTrackers; j++){
	hitVectArray[j].clear();
	weightVectArray[j].clear();
      }

      Int_t iSeedHit=0;
      for(Int_t j=0; j<kNTrackers; j++){ // Begin loop for all Trackers
	if(isTrackerForSeed[j] == kTRUE){ // If seed is constrcuted by this TRACKER
	  isForSeedVectArray[j] = kTRUE;
	  hitVectArray[j].push_back(fSeedVect[i].hits[iSeedHit]);
	  weightVectArray[j].push_back(1.);
	  iSeedHit++;
	} // If seed is constrcuted by this TRACKER

	else{ // If seed is not constrcuted by this TRACKER
	  isForSeedVectArray[j] = kFALSE;
	  for(Int_t k=0; k<fTrackers[j]->getNHits(); k++){
	    hitVectArray[j].push_back(*((planeHit*)fTrackers[j]->getHits()->At(k)));
	  }
	  planePosition = kPlanePosition[j+1];
	  S = estimateStateByStraightLine(fSeedVect[i], planePosition);
	  calcDafWeight(annealingFactor, S, hitVectArray[j], weightVectArray[j]);
	}
      } // End loop for all Trackers
      fDafInfoVect.push_back(dafInfo(hitVectArray, weightVectArray, isForSeedVectArray));
    } // End loop for seeds

    return kTRUE;
  }
}

//______________________________________________________________________________
matrix trackFinder::estimateStateByStraightLine(seed thisSeed, Double_t planePosition){
  Double_t xA = (thisSeed.hits)[0].getX();
  Double_t yA = (thisSeed.hits)[0].getY();
  Double_t xB = (thisSeed.hits)[1].getX();
  Double_t yB = (thisSeed.hits)[1].getY();

  Double_t zA = (thisSeed.hits)[0].getPlanePosition();
  Double_t zB = (thisSeed.hits)[1].getPlanePosition();
  Double_t z = planePosition;

  Double_t x = (zB-z)*xA/(zB-zA) - (zA-z)*xB/(zB-zA);
  Double_t y = (zB-z)*yA/(zB-zA) - (zA-z)*yB/(zB-zA);

  matrix S(kMdim, 1);
  S(kIdxX, 0)=x;
  S(kIdxY, 0)=y;

  return S;
}

//______________________________________________________________________________
trackSite* trackFinder::siteInit(seed* thisSeed){
  //-----------prepare seeds for Kalman Filter track finding------------//
  matrix svd(kSdim,1);
  svd.Zero();
  svd(kIdxX,0) = thisSeed->initState[0];
  svd(kIdxY,0) = thisSeed->initState[1];
  svd(kIdxTx,0) = thisSeed->initState[2];
  svd(kIdxTy,0) = thisSeed->initState[3];
  
  matrix C(kSdim,kSdim);
  C.Zero();
  C(kIdxX, kIdxX) = thisSeed->initCov[0]; 
  C(kIdxY, kIdxY) = thisSeed->initCov[1];
  C(kIdxTx, kIdxTx) = thisSeed->initCov[2];
  C(kIdxTy, kIdxTy) = thisSeed->initCov[3];

  trackSite* initSite = new trackSite(initHit, kMdim, kSdim, kMdim*fChi2PerMdimCut);
  initSite->SetOwner();
  initSite->setPathMomentum(fInitMomentum);
  initSite->setPathTime(0);
  trackState* trackStatePredicted=new trackState(svd, C, *initSite, trackSite::kPredicted, kPlanePosition[kInit]);
  trackStatePredicted->setPlanePosition(kPlanePosition[kInit]);
  trackStatePredicted->setPathMomentum(fInitMomentum);
  trackStatePredicted->setPathTime(0);
  initSite->Add(trackStatePredicted);
  trackState* trackStateFiltered=new trackState(svd, C, *initSite, trackSite::kFiltered, kPlanePosition[kInit]);
  trackStateFiltered->setPlanePosition(kPlanePosition[kInit]);
  trackStateFiltered->setPathMomentum(fInitMomentum);
  trackStateFiltered->setPathTime(0);
  initSite->Add(trackStateFiltered);
  
  return initSite; //Initial site is where the beam starts
}

//______________________________________________________________________________
Bool_t trackFinder::trackFilter(){   
  //Construct tracks from Tracker1 to Tracker4
  //For a seed,  a track is attempted, and the best hits for detectors excluding the seed is choosen. So basically, one seed correponds one track.
  //All candidate tracks are stored in fCoarseTracks

  if(fCoarseTracks->GetEntries()!=0) fCoarseTracks->Clear();
  fNSeeds=0;
  Bool_t isTrackerForTrack[kNTrackers];
 
  for(unsigned i=0;i<fSeedVect.size();i++){ // Begin loop for seeds
    trackSystem *thisSystem=new ((*fCoarseTracks)[fNSeeds++]) trackSystem();
    fCoarseTracks->SetOwner(kTRUE);
    thisSystem->setSeedType(fSeedVect[i].type);
    thisSystem->SetOwner();
    trackSite* initSite=siteInit(&(fSeedVect[i]));
    thisSystem->Add(initSite);

    trackSite* currentSite;
    trackState* currentState;
    trackState* predictState;
    trackSite* newSite;

    Int_t nHit=0;
    Int_t startPlaneId, endPlaneId, deltaPlaneId;
    for(Int_t j=0; j<kNTrackers; j++){ // Begin loop for all Trackers

      thisSystem->checkTrackStatus();
      if(!thisSystem->getTrackStatus()){
	isTrackerForTrack[j] = kFALSE;  
	continue;
      }

      nHit = (fDafInfoVect[i].dafWeightVectArray)[j].size();
      if(nHit == 0) {// no hit for a TRACKER
	thisSystem->addMissingHits();
	isTrackerForTrack[j] = kFALSE;  
      }// no hit for a TRACKER
      else if (nHit == 1){// 1 hit for a TRACKER
	currentSite=thisSystem->getCurSite();
	currentState=currentSite->getCurState();

	//Begin: propagate from current measurement site to next measurement site
	startPlaneId=currentSite->getPlaneHit().getPlaneId();
	endPlaneId=(fDafInfoVect[i].trackerHitVectArray)[j][0].getPlaneId();
	deltaPlaneId = endPlaneId - startPlaneId;
	if(deltaPlaneId == 1){
	  planeHit backPlane((planeId)startPlaneId, kPlanePosition[startPlaneId] + kBackPlaneShift[startPlaneId]);
	  planeHit frontPlane((planeId)endPlaneId, kPlanePosition[endPlaneId] - kFrontPlaneShift[endPlaneId]);
	  planeHit middlePlane((planeId)endPlaneId, kPlanePosition[endPlaneId]);  
	  predictState=currentState->predictSVatFirstPlane(&backPlane,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[startPlaneId]);
	  predictState->predictSVatNextPlane(&frontPlane,fParticleId, fParticleMass, fParticleCharge, kAirMat);
	  predictState->predictSVatLastPlane(&middlePlane,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[endPlaneId]);
	}
	else if(deltaPlaneId > 1){
	  planeHit backPlaneStart((planeId)startPlaneId, kPlanePosition[startPlaneId] + kBackPlaneShift[startPlaneId]);
	  planeHit frontPlaneStart((planeId)(startPlaneId+1), kPlanePosition[startPlaneId+1] - kFrontPlaneShift[startPlaneId+1]);
	  planeHit middlePlaneStart((planeId)(startPlaneId+1), kPlanePosition[startPlaneId+1]);  
	  predictState=currentState->predictSVatFirstPlane(&backPlaneStart,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[startPlaneId]);
	  predictState->predictSVatNextPlane(&frontPlaneStart,fParticleId, fParticleMass, fParticleCharge, kAirMat);
	  predictState->predictSVatLastPlane(&middlePlaneStart,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[startPlaneId+1]);
	  for(Int_t iPlane=startPlaneId+1; iPlane<endPlaneId-1; iPlane++){
	    planeHit backPlane((planeId)iPlane, kPlanePosition[iPlane] + kBackPlaneShift[iPlane]);
	    planeHit frontPlane((planeId)(iPlane+1), kPlanePosition[iPlane+1] - kFrontPlaneShift[iPlane+1]);
	    planeHit middlePlane((planeId)(iPlane+1), kPlanePosition[iPlane+1]);
	    predictState->predictSVatNextPlane(&backPlane,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[iPlane]);
	    predictState->predictSVatNextPlane(&frontPlane,fParticleId, fParticleMass, fParticleCharge, kAirMat);
	    predictState->predictSVatNextPlane(&middlePlane,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[iPlane+1]);
	  }
	  planeHit backPlaneEnd((planeId)(endPlaneId-1), kPlanePosition[endPlaneId-1] + kBackPlaneShift[endPlaneId-1]);
	  planeHit frontPlaneEnd((planeId)endPlaneId, kPlanePosition[endPlaneId] - kFrontPlaneShift[endPlaneId]);
	  planeHit middlePlaneEnd((planeId)endPlaneId, kPlanePosition[endPlaneId]);
	  predictState->predictSVatNextPlane(&backPlaneEnd,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[endPlaneId-1]);
	  predictState->predictSVatNextPlane(&frontPlaneEnd,fParticleId, fParticleMass, fParticleCharge, kAirMat);
	  predictState->predictSVatNextPlane(&middlePlaneEnd,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[endPlaneId]);
	}
	//End: propagate from current measurement site to next measurement site

	newSite =new trackSite(&((fDafInfoVect[i].trackerHitVectArray)[j][0]), kMdim, kSdim, kMdim*fChi2PerMdimCut);
	newSite->Add(predictState);
	if(newSite->filter(fParticleCharge)){
	  thisSystem->Add(newSite);
	  thisSystem->increaseChi2(newSite->getDeltaChi2());
	  isTrackerForTrack[j] = kTRUE;  
	}
	else{    
	  thisSystem->addMissingHits();
	  delete newSite;  
	  isTrackerForTrack[j] = kFALSE;  
	}
      }// 1 hit for a TRACKER
      else{ // If number of hits for this TRACKER is not 0 or 1
	currentSite=thisSystem->getCurSite();
	currentState=currentSite->getCurState();
	fEffectiveHit[j]=calcDafEffectiveMeas((fDafInfoVect[i].trackerHitVectArray)[j], (fDafInfoVect[i].dafWeightVectArray)[j]);

	//Begin: propagate from current measurement site to next measurement site
	startPlaneId=currentSite->getPlaneHit().getPlaneId();
	endPlaneId=fEffectiveHit[j].getPlaneId();
	deltaPlaneId = endPlaneId - startPlaneId;
	if(deltaPlaneId == 1){
	  planeHit backPlane((planeId)startPlaneId, kPlanePosition[startPlaneId] + kBackPlaneShift[startPlaneId]);
	  planeHit frontPlane((planeId)endPlaneId, kPlanePosition[endPlaneId] - kFrontPlaneShift[endPlaneId]);
	  planeHit middlePlane((planeId)endPlaneId, kPlanePosition[endPlaneId]);  
	  predictState=currentState->predictSVatFirstPlane(&backPlane,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[startPlaneId]);
	  predictState->predictSVatNextPlane(&frontPlane,fParticleId, fParticleMass, fParticleCharge, kAirMat);
	  predictState->predictSVatLastPlane(&middlePlane,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[endPlaneId]);
	}
	else if(deltaPlaneId > 1){
	  planeHit backPlaneStart((planeId)startPlaneId, kPlanePosition[startPlaneId] + kBackPlaneShift[startPlaneId]);
	  planeHit frontPlaneStart((planeId)(startPlaneId+1), kPlanePosition[startPlaneId+1] - kFrontPlaneShift[startPlaneId+1]);
	  planeHit middlePlaneStart((planeId)(startPlaneId+1), kPlanePosition[startPlaneId+1]);  
	  predictState=currentState->predictSVatFirstPlane(&backPlaneStart,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[startPlaneId]);
	  predictState->predictSVatNextPlane(&frontPlaneStart,fParticleId, fParticleMass, fParticleCharge, kAirMat);
	  predictState->predictSVatLastPlane(&middlePlaneStart,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[startPlaneId+1]);
	  for(Int_t iPlane=startPlaneId+1; iPlane<endPlaneId-1; iPlane++){
	    planeHit backPlane((planeId)iPlane, kPlanePosition[iPlane] + kBackPlaneShift[iPlane]);
	    planeHit frontPlane((planeId)(iPlane+1), kPlanePosition[iPlane+1] - kFrontPlaneShift[iPlane+1]);
	    planeHit middlePlane((planeId)(iPlane+1), kPlanePosition[iPlane+1]);
	    predictState->predictSVatNextPlane(&backPlane,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[iPlane]);
	    predictState->predictSVatNextPlane(&frontPlane,fParticleId, fParticleMass, fParticleCharge, kAirMat);
	    predictState->predictSVatNextPlane(&middlePlane,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[iPlane+1]);
	  }
	  planeHit backPlaneEnd((planeId)(endPlaneId-1), kPlanePosition[endPlaneId-1] + kBackPlaneShift[endPlaneId-1]);
	  planeHit frontPlaneEnd((planeId)endPlaneId, kPlanePosition[endPlaneId] - kFrontPlaneShift[endPlaneId]);
	  planeHit middlePlaneEnd((planeId)endPlaneId, kPlanePosition[endPlaneId]);
	  predictState->predictSVatNextPlane(&backPlaneEnd,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[endPlaneId-1]);
	  predictState->predictSVatNextPlane(&frontPlaneEnd,fParticleId, fParticleMass, fParticleCharge, kAirMat);
	  predictState->predictSVatNextPlane(&middlePlaneEnd,fParticleId, fParticleMass, fParticleCharge, (matType)kTrackerId[endPlaneId]);
	}
	//End: propagate from current measurement site to next measurement site

	newSite =new trackSite(&fEffectiveHit[j], kMdim, kSdim, kMdim*fChi2PerMdimCut);
	newSite->Add(predictState);
	if(newSite->filter(fParticleCharge)){
	  thisSystem->Add(newSite);
	  thisSystem->increaseChi2(newSite->getDeltaChi2());
	  isTrackerForTrack[j] = kTRUE;  
	}
	else{    
	  thisSystem->addMissingHits();
	  delete newSite;  
	  isTrackerForTrack[j] = kFALSE;  
	}
      } // If number of hits for this TRACKER is not 0 or 1
    } // End loop for all TRACKERs
    thisSystem->setIsTrackerForTrack(isTrackerForTrack);
    thisSystem->checkTrackStatus();
  } // End loop for seeds
  
  if(fCoarseTracks->GetEntries()==0) return kFALSE;
  else return kTRUE;
}

//______________________________________________________________________________   
planeHit trackFinder::calcDafEffectiveMeas(vector<planeHit> hitVect, vector<Double_t> weightVect){
  Int_t nHits = hitVect.size();
  if(nHits == 0) {
    cerr<<"Fatal error: no hit for effective measurement calculation"<<endl;
    exit (EXIT_FAILURE);
  }

  matrix V(kMdim, kMdim);
  vector<matrix> VInvVect;
  VInvVect.clear();
  for(Int_t i=0; i<nHits; i++){
    V.Zero();
    V(kIdxX, kIdxX) = pow(hitVect[i].getXError(), 2);
    V(kIdxY, kIdxY) = pow(hitVect[i].getYError(), 2);
    VInvVect.push_back(matrix(matrix::kInverted, V));
  }
  matrix effectiveVInv(kMdim,kMdim);
  effectiveVInv.Zero();
  for(Int_t i=0; i<nHits; i++) effectiveVInv += (VInvVect[i] *= weightVect[i]);
  matrix effectiveV = matrix(matrix::kInverted, effectiveVInv);
  
  matrix M(kMdim, 1);
  matrix R(kMdim, 1);
  R.Zero();
  for(Int_t i=0; i<nHits; i++){
    M.Zero();
    M(kIdxX, 0) = hitVect[i].getX();
    M(kIdxY, 0) = hitVect[i].getY();
    R += ((VInvVect[i] * M));
  }
  matrix effectiveM = effectiveV * R;
  
  planeHit effectiveHit(hitVect[0].getPlaneId(), hitVect[0].getPlanePosition(), -1, -1, effectiveM(kIdxX, 0), effectiveM(kIdxY, 0), sqrt(effectiveVInv(kIdxX, kIdxX)), sqrt(effectiveVInv(kIdxY, kIdxY)));

  return effectiveHit;
}

//______________________________________________________________________________
Bool_t trackFinder::updateDafInfo(Double_t annealingFactor){
  Int_t nCoarseTracks = fCoarseTracks->GetEntries();
  if(nCoarseTracks == 0) return kFALSE;
  else{
    vector<dafInfo> dafInfoVect;
    dafInfoVect.clear();
    vector<planeHit> hitVectArray[kNTrackers]; 
    vector<Double_t> weightVectArray[kNTrackers];
    Bool_t isForSeedVectArray[kNTrackers];

    Bool_t isTrackerForTrack[kNTrackers];
    trackSite* siteTrackers;
    trackState* stateTrackers;

    for(Int_t i=0;i<nCoarseTracks;i++){ // Begin loop for coarse tracks
      trackSystem *thisSystem = (trackSystem*)(fCoarseTracks->At(i));
      thisSystem->getIsTrackerForTrack(isTrackerForTrack);
      for(Int_t j=0; j<kNTrackers; j++){
	hitVectArray[j].clear();
	weightVectArray[j].clear();
      }
      Int_t numOfSmoothedSite = 0;
      for(Int_t j=0; j<kNTrackers; j++){// Begin loop for TRACKERs
	if((fDafInfoVect[i].isTrackerForSeedVectArray)[j] == kTRUE){ // If a TRACKER is used to construct seed
	  numOfSmoothedSite++;
	  isForSeedVectArray[j] = kTRUE;
	  hitVectArray[j].push_back((fDafInfoVect[i].trackerHitVectArray)[j][0]);
	  weightVectArray[j].push_back((fDafInfoVect[i].dafWeightVectArray)[j][0]);
	}
	else{// If a TRACKER is not used to construct seed
	  isForSeedVectArray[j] = kFALSE;
	  if(isTrackerForTrack[j] == kTRUE){ // If a TRACKER is applied to constrcut track
	    numOfSmoothedSite++;
	    siteTrackers=(trackSite*)thisSystem->At(numOfSmoothedSite);
	    stateTrackers=siteTrackers->getStatePtr(trackSite::kSmoothed);
	    calcDafWeight(annealingFactor, stateTrackers, (fDafInfoVect[i].trackerHitVectArray)[j], hitVectArray[j], weightVectArray[j]);
	  }// If a TRACKER is applied to constrcut track
	}// If a TRACKER is not used to construct seed
      }// End loop for TRACKERs
      dafInfoVect.push_back(dafInfo(hitVectArray, weightVectArray, isForSeedVectArray));
    }// End loop for coarse tracks

    fDafInfoVect.clear();
    fDafInfoVect = dafInfoVect;
    return kTRUE;
  }
}

//______________________________________________________________________________
void trackFinder::calcDafWeight(Double_t annealingFactor, trackState* smoothedState, vector<planeHit> hitVect, vector<planeHit> &hitOutPutVect, vector<Double_t> &weightOutPutVect){

  matrix S(kMdim, 1);
  S(kIdxX, 0)=(*smoothedState)(kIdxX, 0);
  S(kIdxY, 0)=(*smoothedState)(kIdxY, 0);

  Int_t nHits = hitVect.size();

  matrix V(kMdim, kMdim);
  vector<matrix> VVect;
  VVect.clear();
  matrix M(kMdim, 1);
  vector<Double_t> chi2Vect;
  chi2Vect.clear();

  for(Int_t i=0; i<nHits; i++){
    V.Zero();
    V(kIdxX, kIdxX) = pow(hitVect[i].getXError(), 2);
    V(kIdxY, kIdxY) = pow(hitVect[i].getYError(), 2);
    VVect.push_back(V);

    M.Zero();
    M(kIdxX, 0) = hitVect[i].getX();
    M(kIdxY, 0) = hitVect[i].getY();

    chi2Vect.push_back( (matrix(matrix::kTransposed, (M - S)) * matrix(matrix::kInverted, V) * (M - S))(0, 0)  );
  }

  vector<Double_t> phiVect, lambdaVect;
  phiVect.clear();
  Double_t factor, lambda, sum = 0;
  for(Int_t i=0; i<nHits; i++){
    factor = 1/pow((2*TMath::Pi()), kMdim/2.)/sqrt(annealingFactor * VVect[i].Determinant());
    phiVect.push_back(factor * TMath::Exp(-1./(2*annealingFactor)*chi2Vect[i]));
    lambda = factor * TMath::Exp(-kDafChi2Cut/(2*annealingFactor));
    sum += phiVect[i] + lambda;
  }

  hitOutPutVect.clear();
  weightOutPutVect.clear();
  Double_t weight;
  for(Int_t i=0; i<nHits; i++){
    weight = phiVect[i]/sum;
    weightOutPutVect.push_back(phiVect[i]/sum);
    hitOutPutVect.push_back(hitVect[i]);
  }
}

//______________________________________________________________________________
void trackFinder::calcDafWeight(Double_t annealingFactor, matrix S, vector<planeHit> hitVect, vector<Double_t> &weightOutPutVect){

  Int_t nHits = hitVect.size();

  matrix V(kMdim, kMdim);
  vector<matrix> VVect;
  VVect.clear();
  matrix M(kMdim, 1);
  vector<Double_t> chi2Vect;
  chi2Vect.clear();

  for(Int_t i=0; i<nHits; i++){
    V.Zero();
    V(kIdxX, kIdxX) = pow(hitVect[i].getXError(), 2);
    V(kIdxY, kIdxY) = pow(hitVect[i].getYError(), 2);
    VVect.push_back(V);

    M.Zero();
    M(kIdxX, 0) = hitVect[i].getX();
    M(kIdxY, 0) = hitVect[i].getY();

    chi2Vect.push_back( (matrix(matrix::kTransposed, (M - S)) * matrix(matrix::kInverted, V) * (M - S))(0, 0)  );
  }

  vector<Double_t> phiVect, lambdaVect;
  phiVect.clear();
  Double_t factor, lambda, sum = 0;
  for(Int_t i=0; i<nHits; i++){
    factor = 1/pow((2*TMath::Pi()), kMdim/2.)/sqrt(annealingFactor * VVect[i].Determinant());
    phiVect.push_back(factor * TMath::Exp(-1./(2*annealingFactor)*chi2Vect[i]));
    lambda = factor * TMath::Exp(-kDafChi2Cut/(2*annealingFactor));
    sum += phiVect[i] + lambda;
  }

  weightOutPutVect.clear();
  Double_t weight;
  for(Int_t i=0; i<nHits; i++){
    weight = phiVect[i]/sum;
    weightOutPutVect.push_back(phiVect[i]/sum);
  }
}

//______________________________________________________________________________
void trackFinder::trackSmoothBack(){
  for (Int_t i=0; i<fCoarseTracks->GetLast()+1; i++){
    trackSystem *thisSystem = (trackSystem*)(fCoarseTracks->At(i));
    if ( !(thisSystem->getTrackStatus()) ) continue; 
    thisSystem->smoothBackTo(1); 
  }
}

//______________________________________________________________________________
Bool_t trackFinder::finalSelection(TClonesArray* theTracks){
  fNGoodTracks=0;
  fCoarseTracks->Sort();
  cout<<fCoarseTracks->GetEntries()<<"!!!!!"<<endl;

  for(int i=0;i<fCoarseTracks->GetEntries();i++){
    trackSystem *thisSystem = (trackSystem*)(fCoarseTracks->At(i));
    if ( !(thisSystem->getTrackStatus()) ) continue; 
    Int_t flag = 0;
    //start from 1 because the 0th is the dummy site that we used to initialize Kalman Filter
    //One hit cannot be shared by two or more tracks

    for (Int_t j=1;j!=thisSystem->GetLast()+1;j++){
      planeHit thisHit = ((trackSite*)thisSystem->At(j))->getPlaneHit();
      Int_t planeId = thisHit.getPlaneId();
      map< Int_t, vector<planeHit> >::iterator it = fGoodHits.find(planeId);
      if (it != fGoodHits.end()){
	for (UInt_t n = 0; n<(it->second).size(); n++){
	  if ((thisHit.getX() == ((it->second).at(n)).getX()) || (thisHit.getY() == ((it->second).at(n)).getY())) { flag = 1; }
	}
      }
    }

    if (flag == 0){
      track* newTrack = new ((*theTracks)[fNGoodTracks++]) track();
      copyTrack(newTrack, thisSystem);

      for (Int_t j=1; j!=thisSystem->GetLast()+1;j++){
	planeHit thisHit = ((trackSite*)thisSystem->At(j))->getPlaneHit();
	Int_t planeId = thisHit.getPlaneId();
	map< Int_t, vector<planeHit> >::iterator it = fGoodHits.find(planeId);
	if (it != fGoodHits.end()){
	  (it->second).push_back(thisHit);
	}
	else{
	  vector<planeHit> thisVector;
	  thisVector.push_back(thisHit);
	  fGoodHits.insert(std::pair<Int_t, vector<planeHit> >(planeId, thisVector));
	}
      }

    }
  }

  if(theTracks->GetEntries()==0) return kFALSE;
  else return kTRUE;
}

//______________________________________________________________________________
void trackFinder::copyTrack(track* newTrack, trackSystem* thisSystem){
  Int_t nHits=thisSystem->getNHits();

  newTrack->setNHits(nHits);

  Bool_t isTrackerForTrack[kNTrackers];
  thisSystem->getIsTrackerForTrack(isTrackerForTrack);
  vector<Int_t> indexTrackerForTrack;
  indexTrackerForTrack.clear();
  Double_t trackerMomentum[kNTrackers];
  for(Int_t i=0; i<kNTrackers; i++){
    if(isTrackerForTrack[i] == kTRUE) indexTrackerForTrack.push_back(i);
    trackerMomentum[i] = kDefault;
  }

  trackSite* siteTrackers[nHits];
  trackState* stateTrackers[nHits];

  for(Int_t i=0; i<thisSystem->getNHits(); i++){
    siteTrackers[i]=(trackSite*)thisSystem->At(i+1);
    stateTrackers[i]=siteTrackers[i]->getStatePtr(trackSite::kSmoothed);
    trackerMomentum[indexTrackerForTrack[i]] = stateTrackers[i]->getPathMomentum();
    stateTrackers[i]->Print();
  }


  newTrack->setTrackersPathMomentum(trackerMomentum);
  
  return;
}


ClassImp(trackFinder)
