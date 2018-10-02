#include <algorithm>
#include "track.h"

track::track(){
  fNHits=-10000;
  fChi2PerNDF=-10000;

  for(Int_t i=0; i<kNTrackers; i++){
    fTrackersPathMomentum[i]=-10000;
    fTrackersPathTime[i]=-10000;
    
    fTrackersMX[i]=-10000;
    fTrackersMY[i]=-10000;
    fTrackersMXErr[i]=-10000;
    fTrackersMYErr[i]=-10000;

    fTrackersSX[i]=-10000;
    fTrackersSY[i]=-10000;
    fTrackersSTx[i]=-10000;
    fTrackersSTy[i]=-10000;
  }
}

ClassImp(track)

