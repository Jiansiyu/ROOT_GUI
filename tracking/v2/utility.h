#ifndef ROOT_Utility
#define ROOT_Utility

//#include "THashTable.h"
#include "TObject.h"
#include <iostream>
#include <algorithm>
#include <cassert>
using namespace std;

// Setup for tracks
#define kNSeedType 6 // Number of seed types
#define MAXNTRACKS 1000 // Maximized number for candidate tracks
#define kNMissingHitsLimit 2 //tracking process terminates if number of missing hits is over limit
#define kSdim 4 // Dimension for state vector
#define kMdim 2 // Dimension for measurement
enum kalIndex{ kIdxX = 0, kIdxY, kIdxTx, kIdxTy};

// Setup for detectors 
#define kNTotPlanes 5 // Total number of planes
#define kNTrackers 4 // Number of TRACKERs  
enum planeId{kInit = 0, kTracker1, kTracker2, kTracker3, kTracker4}; // 0 is for initial plane; 1, 2, 3 ... must be for trackers; then, following numbers are for detecotrs which are not trackers, such as ToFs
const Int_t kTrackerId[kNTrackers] = {1, 2, 3 ,4}; // ID for trackers; Must start from 1;
//const Int_t kSeedType[kNSeedType]= {12, 13, 14, 23, 24, 34}; // ID for seeds, which is constructed by hits of two trackers
const Double_t kPlanePosition[kNTotPlanes]={0, 100, 200, 300, 400}; // mm; order corresponds planeId
const Double_t kFrontPlaneShift[kNTotPlanes]={0,0,0,0,0}; // mm; the first element = 0 for the initial plane; absolute value
const Double_t kBackPlaneShift[kNTotPlanes]={0,0,0,0,0}; // mm; the first element = 0 for the initial plane; absolute value

const Int_t kNMatType = 5;  //Total number of material types
enum matType{kAirMat = 0, kTracker1Mat, kTracker2Mat, kTracker3Mat, kTracker4Mat}; // 0 is for air; 1, 2, 3 ... must be for trackers; then, following numbers are for detecotrs which are not trackers, such as ToFs
enum matPropertyIdx{ kProtonOverAtomNum = 0, kExcitEnergy, kDensity, kRadLength}; //property array: effective proton number Z over effective atom number A, mean excitation energy (Mev), density (g/cm^3), radiation length(m)
const Double_t kDetMatProperties[kNMatType][4] = {{0.49919, 85.7e-6, 1.205e-3, 303.9}, {0.46481, 153.1e-6, 9.19e-3, 20.8}, {0.46481, 153.1e-6, 9.19e-3, 20.8}, {0.46481, 153.1e-6, 9.19e-3, 20.8}, {0.46481, 153.1e-6, 9.19e-3, 20.8} }; 

// Setup for particles
enum particleId{kMuonPlusId=0,kMuonMinusId, kPositronId, kElectronId, kPionPlusId, kPionMinusId};
const Double_t kParticleMass[6]={105.6583745, 105.6583745, 0.510998918, 0.510998918, 139.57018, 139.57018}; // MeV/c^2; order corresponds particleId
const Double_t kParticleCharge[6]={1., -1., 1., -1., 1., -1.}; // order corresponds particleId
const Double_t kElectronMass = 0.51099891;
const Double_t kPionPlusMass = 139.57018;
const Double_t kMuonMass = 105.6583745;

const Double_t kInitMomentum=150; //MeV/c
const Double_t kChi2PerMdimCut=1e8; // For each measurement in the filter,  DeltaChi2 limit = kChi2PerMdimCut * kMdim

// Parameters for deterministic annealing fitler
#define kDafNTranings 6 // Number of trainings for deterministic annealing filter
const Double_t kDafAnnealingFactor[kDafNTranings] = {81, 9, 4, 1, 1, 1};
const Double_t kDafChi2Cut = 4;

// Parameters for propagation
const Double_t kStepSize = 10; // mm; step size for propagation in non-air material
const Double_t kStepSizeFactor = 10; // step size in air material is kStepSize*kStepSizeFactor mm

// Default value for output
const Double_t kDefault = -10000;

#endif















