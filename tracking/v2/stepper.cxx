#include <iostream>
#include <cassert>
#include <cmath>

#include "TMath.h"

#include "stepper.h"
using namespace std;

stepper *stepper::fStepper = 0;

//_________________________________________________________________
stepper::stepper(){
  fIsForward = kTRUE;
  fIsMSOn = kTRUE;
  fIsDEDXOn = kTRUE;
}
//_________________________________________________________________
stepper::~stepper(){
}

//////////////////////////////////// Propagation ///////////////////////////////
//_____________________________________________________________________________________________________________
void stepper::transport(trackState *svFrom, planeHit *hitTo, matrix &sv, matrix &F, matrix &Q, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type){
  Double_t tx = (*svFrom)(kIdxTx, 0);
  Double_t ty = (*svFrom)(kIdxTy, 0);

  Double_t deltaZ = hitTo->getPlanePosition() - svFrom->getPlanePosition();

  // Update state vector
  for (Int_t i=0; i<kSdim; i++){
    sv(i,0) = (*svFrom)(i,0);
  }
  sv(kIdxX, 0) += tx * deltaZ;
  sv(kIdxY, 0) += ty * deltaZ;

  // Update propagator matrix
  F.UnitMatrix();
  F(kIdxX, kIdxTx) = deltaZ;
  F(kIdxY, kIdxTy) = deltaZ;

  // Update momentum, Q, path time
  Int_t h;
  if(deltaZ >= 0){
    fIsForward = kTRUE;
    h = 1;
  }
  else{
    fIsForward = kFALSE;
    h = -1;
  }

  Double_t step;
  if(type == kAirMat) step = kStepSize * kStepSizeFactor;
  else step = kStepSize;

  fPathTime = svFrom->getPathTime();
  fPathMomentum = svFrom ->getPathMomentum();
  Q.Zero();

  Double_t energyLoss     = 0.;
  Double_t beta;
  Double_t betaGamma;  
  Double_t p2;
  Double_t e; 

  jstep          = 0;

  while (fabs(deltaZ) >= step){
    fStepLength =sqrt(tx*tx+ty*ty+1)*step;
    betaGamma=fPathMomentum/particleMass;
    beta=1/sqrt(1/betaGamma/betaGamma+1);
    fStepTime=fabs(fStepLength/beta/TMath::C())*pow(10,6); // ns

    if ( isMSOn() )     calcMultScat(Q, tx, ty, fPathMomentum, fStepLength, beta);
    if ( isDEDXOn() )   energyLoss = calcEnergyLoss(fPathMomentum, fStepLength, beta, particleId, particleMass, particleCharge, type); 

    p2 = fPathMomentum * fPathMomentum;
    e = TMath::Sqrt(p2 + particleMass*particleMass);

    if(fIsForward == kTRUE) {
      fPathTime += fStepTime;
      fPathMomentum = TMath::Sqrt(p2 - 2.*e*energyLoss + energyLoss*energyLoss);
    }
    else{
      fPathTime-=fStepTime;
      fPathMomentum = TMath::Sqrt(p2 + 2.*e*energyLoss + energyLoss*energyLoss);
    }

    // Decide if more steps must be done and calculate new step size
    deltaZ -= step * h; // Update deltaZ   

    jstep++; 
  }

  if(fabs(deltaZ) < step && fabs(deltaZ) > 0){
    step = fabs(deltaZ);
    fStepLength =sqrt(tx*tx+ty*ty+1)*step;
    betaGamma=fPathMomentum/particleMass;
    beta=1/sqrt(1/betaGamma/betaGamma+1);
    fStepTime=fabs(fStepLength/beta/TMath::C())*pow(10,6); // ns

    //calculate the energy loss and multiple scattering and update Q
    if ( isMSOn() )     calcMultScat(Q, tx, ty, fPathMomentum, fStepLength, beta);
    if ( isDEDXOn() )   energyLoss = calcEnergyLoss(fPathMomentum, fStepLength, beta, particleId, particleMass, particleCharge, type); 

    p2 = fPathMomentum * fPathMomentum;
    e = TMath::Sqrt(p2 + particleMass*particleMass);

    if(fIsForward == kTRUE) {
      fPathTime += fStepTime;
      fPathMomentum = TMath::Sqrt(p2 - 2.*e*energyLoss + energyLoss*energyLoss);
    }
    else{
      fPathTime-=fStepTime;
      fPathMomentum = TMath::Sqrt(p2 + 2.*e*energyLoss + energyLoss*energyLoss);
    }

    jstep++; 
  }
}
//////////////////////////////////// Propagation ///////////////////////////////

//////////////////////////////////// Energy loss and multi-scattering ///////////////////////////////
//_____________________________________________________________________________________________________________
Double_t stepper::calcMultScat(matrix &Q, Double_t tx, Double_t ty, Double_t momentum, Double_t length, Double_t beta, matType type){

  // Add multiple scattering to the process noise covariance.
  //
  // Input and output:
  // fProc:     Process noise covariance. Contribution of multiple scattering is added to this matrix.
  //
  // Input:
  // stateVec:  Track state vector at start of an RK step.
  // length:    Track length in cm.
  // radLength: Radiation length of passed material in cm.
  // beta:      v/c of particle.
  // pid:       Geant particle ID.

  // 1/beta^2
  Double_t beta2Inv = 1. / (beta*beta);

  // 1/momentum^2
  Double_t mom2Inv  = 1/TMath::Power(momentum, 2);
  
  Double_t lx0 = length * 1.e-3 / kDetMatProperties[type][kRadLength]; 
  Double_t cms2 = 13.6*13.6 * beta2Inv * mom2Inv * lx0 * TMath::Power((1 + 0.038 * TMath:: Log(lx0) ),2); 

  // Update process noise.
  Double_t t  = 1. + tx*tx + ty*ty;
  Q(kIdxTx, kIdxTx) += (1 + tx*tx) * t * cms2;
  Q(kIdxTy, kIdxTy) += (1 + ty*ty) * t * cms2;
  Q(kIdxTx, kIdxTy) += tx * ty     * t * cms2;
  Q(kIdxTy, kIdxTx)    = Q(kIdxTx, kIdxTy); // matrix is symmetric

  return cms2;
}
//_____________________________________________________________________________________________________________
Double_t stepper::calcEnergyLoss(Double_t momentum, Double_t length, Double_t beta, particleId particleId, Double_t particleMass, Double_t particleCharge, matType type)
{
  Double_t ZoverA = kDetMatProperties[type][kProtonOverAtomNum];
  Double_t elossRad = 0.;
  Double_t elossIon = 0.;
  
  if(particleId == kPositronId || particleId == kElectronId) {
    // Critical energy for gases:
    // E_c = 710 MeV / (Z + 0.92)
    // From "Passage of particles through matter", Particle Data Group, 2009
    elossRad = calcRadLoss(momentum, length, kDetMatProperties[type][kRadLength]);
    if(particleId == kElectronId) elossIon = length * calcDEDXIonElectron(momentum, ZoverA, kDetMatProperties[type][kDensity],kDetMatProperties[type][kExcitEnergy]);
    else if(particleId == kPositronId) elossIon = length * calcDEDXIonPositron(momentum, ZoverA, kDetMatProperties[type][kDensity],kDetMatProperties[type][kExcitEnergy]);
  } 
  else { // Energy loss for heavy particles.
    // Energy loss due to ionization.
    elossIon = length * calcDEDXBetheBloch(beta, ZoverA, kDetMatProperties[type][kDensity],kDetMatProperties[type][kExcitEnergy], particleMass, particleCharge);
    
  }
  return elossRad+elossIon;
}

//______________________________________________________________________________________________________________
Double_t stepper::calcRadLoss(Double_t momentum, Double_t length, Double_t radLength){
  // Calculates energy loss due to bremsstrahlung for electrons or positrons with Bethe-Heitler formula.

  Double_t t = length * 1.e-3 / radLength; // t := l/xr. Track length in units of radiation length.

  // Bethe and Heitler formula:
  // - dE/dx = E / xr
  // Integrating yields:
  // E' = E * exp(-t)
  // E'/E = exp(-t)
  // with t := l/xr
  // l = track length
  // xr = radiation length

  Double_t me = 0.510998918; // electron/positron mass
  Double_t elossRad = (1 - TMath::Exp(-t)) * sqrt(momentum * momentum + me * me);

  // The variance of E'/E as done in:
  // D. Stampfer, M. Regler and R. Fruehwirth,
  // Track fitting with energy loss.
  // Comp. Phys. Commun. 79 (1994) 157-164
  // http://www-linux.gsi.de/~ikisel/reco/Methods/Fruehwirth-FittingWithEnergyLoss-CPC79-1994.pdf

  return elossRad;
}

//______________________________________________________________________________________________________________
Double_t stepper::calcDEDXIonElectron(Double_t momentum, Double_t ZoverA, Double_t density, Double_t I){
  // Calculates energy loss dE/dx in MeV/mm due to ionization for relativistic electrons.
  //
  // For formula used, see:
  // Track fitting with energy loss
  // Stampfer, Regler and Fruehwirth
  // Computer Physics Communication 79 (1994), 157-164
  //
  // ZoverA:     atomic number / atomic mass of passed material
  // density:    density of material in g/mm^3
  // I : mean excitation energy in GeV


  Double_t K = 0.307075; // Mev * cm^2/g

  Double_t me       = 0.510998918;   // electron mass in MeV/c^2
  Double_t E        = sqrt(momentum * momentum + me * me);             // Energy for relativistic electrons.
  Double_t gamma    = E / me;                          // Relativistic gamma-factor.

  // Formula is slightly different for electrons and positrons.
  // Factors for positrons:
  Double_t gammaFac = 3.;
  Double_t corr     = 1.95;

  Double_t dedx = 0.5 * K * density * ZoverA * (2 * TMath::Log(2*me/I) + gammaFac * TMath::Log(gamma) - corr);
  
  return dedx*0.1;//convert from MeV/cm to MeV/mm
}

//______________________________________________________________________________________________________________
Double_t stepper::calcDEDXIonPositron(Double_t momentum, Double_t ZoverA, Double_t density, Double_t I){
  // Calculates energy loss dE/dx in MeV/mm due to ionization for relativistic positrons.
  //
  // For formula used, see:
  // Track fitting with energy loss
  // Stampfer, Regler and Fruehwirth
  // Computer Physics Communication 79 (1994), 157-164
  //
  // ZoverA:     atomic number / atomic mass of passed material
  // density:    density of material in g/mm^3
  // I : mean excitation energy in GeV


  Double_t K = 0.307075; // Mev * cm^2/g

  Double_t me       = 0.510998918;   // electron mass in MeV/c^2
  Double_t E        = sqrt(momentum * momentum + me * me);             // Energy for relativistic electrons.
  Double_t gamma    = E / me;                          // Relativistic gamma-factor.

  // Formula is slightly different for electrons and positrons.
  // Factors for positrons:
  Double_t gammaFac = 4.;
  Double_t corr     = 2;

  Double_t dedx = 0.5 * K * density * ZoverA * (2 * TMath::Log(2*me/I) + gammaFac * TMath::Log(gamma) - corr);
  
  return dedx*0.1;//convert from MeV/cm to MeV/mm
}

//________________________________________________________________________________________________________________
Double_t stepper::calcDEDXBetheBloch(Double_t beta, Double_t ZoverA, Double_t density, Double_t I, Double_t particleMass, Double_t particleCharge){
  // Returns the ionization energy loss for thin materials with Bethe-Bloch formula.
  // - dE/dx = K/A * Z * z^2 / beta^2 * (0.5 * ln(2*me*beta^2*gamma^2*T_max/I^2) - beta^2)
  // T_max = (2*me*beta^2*gamma^2) / (1 + 2*gamma*me/mass + me^2/mass^2)
  //
  // beta:       v/c of particle
  // mass:       mass of the particle in MeV/c^2 traversing through the material
  // ZoverA:     atomic number / atomic mass of passed material
  // density:    density of passed material in g/cm^3
  // exciteEner: mean excitation energy of passed material in MeV
  // z:          atomic number of particle

  Double_t me         = 0.510998918; // electron mass in MeV/c^2
  Double_t K          = 0.307075;               // in MeV * cm^2 / g for A = 1 g/mol

  Double_t beta2      = beta*beta;
  Double_t gamma      = 1./TMath::Sqrt(1 - beta2);
  Double_t betagamma  = beta * gamma;
  Double_t betagamma2 = betagamma * betagamma;

  if(betagamma < 0.1 || betagamma > 1000.) {
    //cout<<"Bethe-Bloch formula is only good for values between 0.1 and 1000."<<" beta= "<<beta<<" gamma="<<gamma<<endl;
    return 0.;
  }
  // Maximum kinetic energy that can be imparted on a free electron in a single collision.
  Double_t tmax = (2. * me * betagamma2) / (1 + 2*gamma*me/(particleMass) + (me*me)/(particleMass*particleMass));
  Double_t dedx = K *density * particleCharge * particleCharge * ZoverA / beta2 * (0.5 * TMath::Log((2. * me * betagamma2 * tmax)/(I*I)) - beta2);

  return dedx*0.1;//convert from MeV/cm to Mev/mm

}
//________________________________________________________________________________________________________________

//////////////////////////////////// Energy loss and multi-scattering ///////////////////////////////

ClassImp(stepper)


















