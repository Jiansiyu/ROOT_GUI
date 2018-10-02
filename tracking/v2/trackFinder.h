#ifndef ROOT_TRACK_FINDER
#define ROOT_TRACK_FINDER
//ROOT
#include <vector>
#include <map>
using namespace std;

#include "TVector3.h"
#include "TClonesArray.h"
#include "TObject.h"

#include "utility.h"
#include "particle.h"
#include "planeHit.h"
#include "tracker.h"
#include "trackState.h"
#include "trackSite.h"
#include "track.h"
#include "trackSystem.h"
#include "matrix.h"

class planeHit;
class trackState;
class trackSite;
class trackSystem;

class trackFinder: public particle{
 public:
  trackFinder(particleId id, Double_t mass, Double_t charge, Double_t initMomentum, tracker* uTrackers[kNTrackers]);
  ~trackFinder();
  void Clear( Option_t* opt="");



  Bool_t processHits(TClonesArray* theTracks);
  Bool_t findSeed();

  Bool_t initDafInfo(Double_t annealingFactor);
  Bool_t trackFilter();
  void trackSmoothBack();
  Bool_t updateDafInfo(Double_t annealingFactor);

  Bool_t finalSelection(TClonesArray* theTracks);

  struct seed{
    Int_t type;
    Int_t trackerIndex1;
    Int_t trackerIndex2;
    vector<planeHit> hits; //
    Bool_t isActive;
    Double_t initState[4];
    Double_t initCov[4]; // covariance between x, y, tx and ty
  
    seed() {}
  seed(Int_t& t, Int_t& index1, Int_t& index2, vector<planeHit> h, Double_t state[4], Double_t cov[4]): type(t), trackerIndex1(index1), trackerIndex2(index2),  hits(h), isActive(kTRUE){
    for(Int_t i=0;i<4;i++){
      initState[i] = state[i];
      initCov[i] = cov[i];
    }
  }

    ~seed(){hits.clear();}
    void deactive() { isActive = kFALSE; }
  };

  struct dafInfo{
    vector<planeHit> trackerHitVectArray[kNTrackers];
    vector<Double_t> dafWeightVectArray[kNTrackers];
    Bool_t isTrackerForSeedVectArray[kNTrackers];

    dafInfo() {}
    dafInfo(vector<planeHit> hitVectArray[kNTrackers], vector<Double_t> weightVectArray[kNTrackers], Bool_t isForSeedVectArray[kNTrackers]) {
      for(Int_t i=0; i<kNTrackers; i++){
	trackerHitVectArray[i] = hitVectArray[i];
	dafWeightVectArray[i] = weightVectArray[i];
	isTrackerForSeedVectArray[i] = isForSeedVectArray[i];
      }
    }

    ~dafInfo(){
      for(Int_t i=0; i<kNTrackers; i++){
	trackerHitVectArray[i].clear();
	dafWeightVectArray[i].clear();
      }
    }
  };


  void setInitMomentum(Double_t initMomentum) { fInitMomentum = initMomentum; }
  Double_t getInitMomentum() const {return fInitMomentum; }

  void chooseTrackersForSeed (vector<Int_t> index, vector<Int_t> numOfHits, Int_t &trackerIndex1, Int_t &trackerIndex2);
  void calcInitStateCoV(planeHit* hitA, planeHit* hitB, Double_t initState[], Double_t initCov[]); // Calculate initial covariance
  trackSite* siteInit(seed* thisSeed); //Set inital state and covariance for each seed
  matrix estimateStateByStraightLine(seed thisSeed, Double_t planePosition);

  planeHit calcDafEffectiveMeas(vector<planeHit> hitVect, vector<Double_t> weightVect);

  void calcDafWeight(Double_t annealingFactor, trackState* smoothedState, vector<planeHit> hitVect, vector<planeHit> &hitOutPutVect, vector<Double_t> &weightOutPutVect);
  void calcDafWeight(Double_t annealingFactor, matrix S, vector<planeHit> hitVect, vector<Double_t> &weightOutPutVect);

  void copyTrack(track* newTrack, trackSystem* thisSystem);


  TClonesArray* getCoarseTracks() const {return fCoarseTracks;}

 private:
  static planeHit* initHit;

  Double_t fInitMomentum;
  tracker* fTrackers[kNTrackers]; 

  TClonesArray* fCoarseTracks;
  int fNSeeds;
  vector<seed> fSeedVect;
  map<Int_t, vector<planeHit> > fGoodHits;
  Int_t fNGoodTracks;
  Double_t fChi2PerMdimCut;

  vector<dafInfo> fDafInfoVect; // store DAF information for all coarse tracks iteration by iteration
  planeHit fEffectiveHit[kNTrackers];

  ClassDef(trackFinder,1) 

    };





#endif
