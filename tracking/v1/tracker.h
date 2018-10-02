#ifndef ROOT_MWPC
#define ROOT_MWPC

#include "TClonesArray.h"
#include "TVector3.h"

#include "utility.h"
#include "planeHit.h"
#include "plane.h"



class tracker: public plane{
 public:
  tracker(planeId id);
  ~tracker();

  Int_t getNHits() const{return fNHits;}
  void init(UInt_t nHitsX, UInt_t nHitsY, Double_t posX[250], Double_t posY[250], Double_t posXErr[250], Double_t posYErr[250]);
  TClonesArray* getHits() const{return fHits;}

 private:
  TClonesArray* fHits;
  Int_t fNHits;
  ClassDef(tracker,1)      

};

#endif
