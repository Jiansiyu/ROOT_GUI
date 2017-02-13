// -*- C++ -*-

/* class GEMTree
 * Define the structure of the data tree.
 */

// History:
//   Jul 2013, Original version. Unknown previous history.
//   Jul 2013, K. Jin & N. Ton.
//   Sep 2013, C. Gu, Use kMAXADC and kMAXNCH in Config.h to build the tree.
//

#include <cstdlib>
#include <cstdio>
#include <cassert>

#include <TROOT.h>
#include <TError.h>
#include <TChain.h>
#include <TFile.h>

#include "Config.h"
#include "GEMTree.h"

using namespace std;

//==================================================================================
GEMTree::GEMTree(TChain *tree) {
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  assert(tree);

  Init(tree);
}

//==================================================================================
GEMTree::~GEMTree() {
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

//==================================================================================
Int_t GEMTree::GetEntry(Long64_t entry) {
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

//==================================================================================
Long64_t GEMTree::LoadTree(Long64_t entry) {
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class())) return centry;
  TChain *chain = (TChain*) fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    Notify();
  }
  return centry;
}

//==================================================================================
void GEMTree::Init(TChain *tree) {
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("evtID", &EvtID, &b_EvtID);
  fChain->SetBranchAddress("nch", &digi_gem_nch, &b_digi_gem_nch);
  //fChain->SetBranchAddress("hitTimebin", digi_gem_hitTimebin, &b_digi_gem_hitTimebin);
  fChain->SetBranchAddress("detID", digi_gem_chamber, &b_digi_gem_chamber);
  fChain->SetBranchAddress("planeID", digi_gem_plane, &b_digi_gem_plane);
  fChain->SetBranchAddress("strip", digi_gem_strip, &b_digi_gem_strip);
  for (Int_t i = 0; i < kMAXADC; i++) {
    fChain->SetBranchAddress(Form("adc%d", i), digi_gem_adc[i], &b_digi_gem_adc[i]);
  }
  Notify();
}

//==================================================================================
Bool_t GEMTree::Notify() {
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

//==================================================================================
void GEMTree::Show(Long64_t entry) {
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}

//==================================================================================
Int_t GEMTree::Cut(Long64_t entry) {
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}

//==================================================================================
void GEMTree::Loop() {
  // In a ROOT session, you can do:
  //    Root > .L GEMTree.C
  //    Root > GEMTree t
  //    Root > t.GetEntry(12); // Fill t data members with entry number 12
  //    Root > t.Show();       // Show values of entry 12
  //    Root > t.Show(16);     // Read and show values of entry 16
  //    Root > t.Loop();       // Loop on all entries
  //
  // This is the loop skeleton where:
  //    jentry is the global entry number in the chain
  //    ientry is the entry number in the current Tree
  // Note that the argument to GetEntry must be:
  //    jentry for TChain::GetEntry
  //    ientry for TTree::GetEntry and TBranch::GetEntry
  //
  // To read only selected branches, Insert statements like:
  // METHOD1:
  //    fChain->SetBranchStatus("*",0);  // disable all branches
  //    fChain->SetBranchStatus("branchname",1);  // activate branchname
  // METHOD2: replace line
  //    fChain->GetEntry(jentry);       //read all branches
  //by  b_branchname->GetEntry(ientry); //read only this branch
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);
    nbytes += nb;
    // if (Cut(ientry) < 0) continue;
  }
}
