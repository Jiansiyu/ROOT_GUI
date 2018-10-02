#ifndef ROOT_FIELD_STEPPER
#define ROOT_FIELD_STEPPER

#include "TVector3.h"

#include "utility.h"
#include "matrix.h"
#include "planeHit.h"
#include "trackSite.h"
#include "trackState.h"
class trackSite;
class trackState;

class stepper:public TObject{
 public:
  stepper();
  ~stepper();

  static stepper* getInstance() {
    if (fStepper == NULL) fStepper = new stepper();
    return fStepper;
  }

  void turnOnMS()       { fIsMSOn = kTRUE;  }
  void turnOffMS()      { fIsMSOn = kFALSE; }
  void turnOnDEDX ()    { fIsDEDXOn = kTRUE;  }
  void turnOffDEDX ()   { fIsDEDXOn = kFALSE; }
  
  inline Bool_t isMSOn()   const { return fIsMSOn;     }
  inline Bool_t isDEDXOn() const { return fIsDEDXOn;   }

  inline Double_t getPathTime() const { return fPathTime; }
  inline Double_t getPathMomentum() const { return fPathMomentum; }

  void transport(trackState *svFrom, planeHit *hitTo, matrix &sv, matrix &F, matrix &Q, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type);

  Double_t calcMultScat(matrix &Q, Double_t tx, Double_t ty, Double_t momentum, Double_t length, Double_t beta, matType type = kAirMat);
  Double_t calcEnergyLoss(Double_t momentum, Double_t length, Double_t beta, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type = kAirMat);
  Double_t calcRadLoss(Double_t momentum, Double_t length, Double_t radLength);
  Double_t calcDEDXIonElectron(Double_t momentum, Double_t ZoverA, Double_t density, Double_t I);
  Double_t calcDEDXIonPositron(Double_t momentum, Double_t ZoverA, Double_t density, Double_t I);
  Double_t calcDEDXBetheBloch(Double_t beta, Double_t ZoverA, Double_t density, Double_t I, Double_t particleMass, Double_t particleCharge);
 
 protected:  
  static stepper* fStepper;

  Bool_t    fIsForward;     //kTRUE: forward propagation; kFALSE: backward propagation
  Bool_t    fIsMSOn;         //! switch for multiple scattering
  Bool_t    fIsDEDXOn;       //! switch for energy loss

  Double_t fPathTime; // time of the track; ns
  Double_t fPathMomentum; // Momentum of tracks; MeV/c
  Double_t  fStepLength;   //distance between start and end points; mm
  Double_t fStepTime; //time between start and end points; ns

  Double_t  stepSize;  // mm; step size
  Int_t     jstep;        // step number

 private:
  ClassDef(stepper,1) 
  
};

#endif
