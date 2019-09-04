/* 

class GEMTree
Define the structure of the data tree.

//   History:
//   Jul 2013, Original version. Unknown previous history.
//   Jul 2013, K. Jin & N. Ton.
//   Sep 2013, C. Gu, Use kMAXADC and kMAXNCH in Config.h to build the tree.
//   Jul 2014, X. Bai

*/

#ifndef GEMTREE_H
#define GEMTREE_H

#include <TROOT.h>
#include <TObject.h>

#include "Config.h"

class TChain;
class TBranch;

class GEMTree : public TObject {
public:
    GEMTree(TChain *chain = 0);
    virtual ~GEMTree();

    virtual Int_t Cut(Long64_t entry);
    virtual Int_t GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void Init(TChain *tree);
    virtual void Loop();
    virtual Bool_t Notify();
    virtual void Show(Long64_t entry = -1);

protected:
    TChain *fChain; // Pointer to the analyzed TTree or TChain
    Int_t fCurrent; // Current Tree number in a TChain

    // Declaration of leaf types
    Int_t EvtID;
    Int_t digi_gem_nch;
    //Int_t digi_gem_hitTimebin[kMAXNCH];//Xinzhan: new tree feature.
    Int_t digi_gem_chamber[kMAXNCH]; //[digi.gem.nch]
    Int_t digi_gem_plane[kMAXNCH]; //[digi.gem.nch]
    Int_t digi_gem_strip[kMAXNCH]; //[digi.gem.nch]
    Int_t digi_gem_adc[kMAXADC][kMAXNCH]; //[digi.gem.nch]   digi_gem_adc[6 time sample][channel index]

    // List of branches
    TBranch *b_EvtID; //!
    TBranch *b_digi_gem_nch; //!
    //TBranch *b_digi_gem_hitTimebin;
    TBranch *b_digi_gem_chamber; //!
    TBranch *b_digi_gem_plane; //!
    TBranch *b_digi_gem_strip; //!
    TBranch *b_digi_gem_adc[kMAXADC]; //!
};

#endif
