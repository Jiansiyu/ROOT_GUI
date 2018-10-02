#include <iostream>

#include "TVector3.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

#include "utility.h"
#include "matrix.h"
#include "planeHit.h"
#include "plane.h"
#include "tracker.h"
#include "trackState.h"
#include "trackSite.h"
#include "trackSystem.h"
#include "track.h"
#include "trackFinder.h"

using namespace std;


int main(){

  particleId partId;

  //if(i%100==0) cout<<i<<" events are processed"<<endl;
  //cout<<"----------------------------------------------------"<<endl;



  tracker *uTrackers[kNTrackers];

  for(Int_t i=0; i<kNTrackers; i++) uTrackers[i]=new tracker((planeId)kTrackerId[i]);
    
  Double_t posTrackerX[kNTrackers][250], posTrackerY[kNTrackers][250], posTrackerXErr[kNTrackers][250], posTrackerYErr[kNTrackers][250];

  posTrackerX[0][0]=10.5;
  posTrackerY[0][0]=9.7;
  posTrackerX[1][0]=20.9;
  posTrackerY[1][0]=19.2;
  posTrackerX[2][0]=29.7;
  posTrackerY[2][0]=30.7;
  posTrackerX[3][0]=39.5;
  posTrackerY[3][0]=40.6;

  posTrackerXErr[0][0]=1;
  posTrackerYErr[0][0]=1;
  posTrackerXErr[1][0]=1;
  posTrackerYErr[1][0]=1;
  posTrackerXErr[2][0]=1;
  posTrackerYErr[2][0]=1;
  posTrackerXErr[3][0]=1;
  posTrackerYErr[3][0]=1;

  posTrackerX[0][1]=0.6;
  posTrackerY[0][1]=-0.7;
  posTrackerX[1][1]=-10.7;
  posTrackerY[1][1]=-9.2;
  posTrackerX[2][1]=-19.5;
  posTrackerY[2][1]=-21.2;
  posTrackerX[3][1]=-30.5;
  posTrackerY[3][1]=-29.7;

  posTrackerXErr[0][1]=1;
  posTrackerYErr[0][1]=1;
  posTrackerXErr[1][1]=1;
  posTrackerYErr[1][1]=1;
  posTrackerXErr[2][1]=1;
  posTrackerYErr[2][1]=1;
  posTrackerXErr[3][1]=1;
  posTrackerYErr[3][1]=1;



  //Int_t nHitsX[kNTrackers]={2,2,2,2};
  //Int_t nHitsY[kNTrackers]={2,2,2,2};

  Int_t nHitsX[kNTrackers]={2,2,2,2};
  Int_t nHitsY[kNTrackers]={2,2,2,2};


  for(Int_t i=0; i<kNTrackers; i++){
    uTrackers[i]->init(nHitsX[i], nHitsY[i], posTrackerX[i], posTrackerY[i], posTrackerXErr[i], posTrackerYErr[i]);
  }


    partId=kMuonPlusId;
    trackFinder *trackFind=new trackFinder(partId, kParticleMass[partId], kParticleCharge[partId], kInitMomentum, uTrackers);
    TClonesArray* theTracks=new TClonesArray("track", MAXNTRACKS, kTRUE);
    theTracks->Clear();

    Int_t nTracks;
    Int_t nHits[100];

    if(!trackFind->processHits(theTracks)){
      nTracks=0;
    }
    else   nTracks=theTracks->GetEntries();

    Double_t trackersMomentum[kNTrackers];

    for(int j=0; j<nTracks; j++){
      //-----------------------------------Muon---------------------------------------//
      nHits[j]=((track*)theTracks->At(j))->getNHits();

      ((track*)theTracks->At(j))->getPathTrackersMomentum(trackersMomentum);
      cout<<"track: "<<j<<endl;
      for(Int_t i=0; i<kNTrackers; i++){
	cout<<trackersMomentum[i]<<endl;
      }
      cout<<endl;
    }

    theTracks->SetOwner(kTRUE);
    delete theTracks;
    theTracks=NULL;
    delete trackFind;
    trackFind=NULL;

    for(Int_t i=0; i<kNTrackers; i++){
      delete uTrackers[i];
      uTrackers[i]=NULL;
    }


  return 0;
}

