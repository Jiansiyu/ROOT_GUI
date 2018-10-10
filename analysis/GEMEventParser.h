/*
 * GEMEventParser.h
 *
 *  Created on: Oct 8, 2018
 *      Author: newdriver
 *      rewrite xinzhan's original
 *
 */

#ifndef GEMEVENTPARSER_H_
#define GEMEVENTPARSER_H_

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <memory>

#include <TROOT.h>
#include <TError.h>
#include <Getline.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TF1.h>
#include <TFile.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTimer.h>

#include <string>

#include <TObject.h>
#include <TObjArray.h>


#include "GEMTree.h"


class GEMEventParser: public GEMTree {
public:
	GEMEventParser(TChain *chain);
	virtual ~GEMEventParser();
public:
	Bool_t ReadBlock();
	Bool_t GetEvent();
	int64_t EvtID;

protected:
	int64_t entries;
	int64_t entry;
};

#endif /* GEMEVENTPARSER_H_ */
