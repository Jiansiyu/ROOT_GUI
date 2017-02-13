/***************************************************

Xinzhan: This header file: 
         1), defines all Histograms that to be produced. 
         2), save all Histograms to root files. 
         3), to use this file:
         #include "GEMHistoManager.h"
         using namespace GEMHistoManager;

***************************************************/

#ifndef GEMHISTOMANAGER_H
#define GEMHISTOMANAGER_H

#include <TH1F.h>
#include <TH2F.h>
#include <TObject.h>
#include <TF1.h>
#include <TFile.h>

#include "Config.h"
#include "GEMTCluster.h"

namespace GEMHistoManager
{
  //==================================================================================
  // @ Define Cluster Tree to be saved.
  GEMTCluster cluster_tree("GEMTreeCluster","cluster extracted from THit");

  //==================================================================================
  // @ Define all histograms
  //Xinzhan: without special notification, histograms are for strips.
  //         histograms for clusters will be marked by "Cluster"
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // $$ Group 1, No Cut
  TH1F *hChargeFromFit[kNMODULE]; 
  TH1F *hHitTimeX[kNMODULE];
  TH1F *hAPVConstX[kNMODULE];
  TH1F *hHitTimeY[kNMODULE];
  TH1F *hAPVConstY[kNMODULE];
  TH1F *hReducedChisquare[kNMODULE];
  //starting time and shaping time for each APV
  TH1F *hChargeFromFitAPV[kNAPV];
  TH1F *hHitTimeAPV[kNAPV];
  TH1F *hAPVConstAPV[kNAPV];
  TH1F *hReducedChisquareAPV[kNAPV];

  TH1F *hMaxADConStrip[kNMODULE];
  TH1F *hMaxADConStripTime[kNMODULE];
  //for each APV
  TH1F *hMaxADConStripAPV[kNAPV];
  TH1F *hMaxADConStripTimeAPV[kNAPV];

  TH2F *hhRMSvsMeanTime;
  TH2F *hhRMSvsMeanConst;
  TH1F *hRMSStartTime;

  //Positions
  //2D Hit and CLuster Distribution for each chamber
  TH2F *hhClusterDist[kNMODULE];
  //1D Hit and CLuster Distribution for each chamber
  TH1I *hHitDistX[kNMODULE];
  TH1I *hHitDistY[kNMODULE];
  TH1F *hClusterDistX[kNMODULE];
  TH1F *hClusterDistY[kNMODULE];
  TH1F *hClusterAdcDistX[kNMODULE];
  TH1F *hClusterAdcDistY[kNMODULE];

  TH1F *hClusterSize;
  TH1F *hClusterSize_x;  // cluster size in x dimension 
  TH1F *hClusterSize_y;  // cluster size in y dimension 
  
  TH1I *hNbClusterPerPlane;
  TH1I *hNbClusterPerPlane_x;  // number of clusters in x dimension
  TH1I *hNbClusterPerPlane_y;  // number of cluster  in y dimension 
 
  TH2F *hhClusterChargeRatio[kNMODULE];

  //with charge ratio cut
  TH2F *hhClusterDistRatioMethod[kNMODULE];
  TH2F *hhClusterChargeRatioRatioMethod[kNMODULE];

  TH1I *hMaxTimeBinXModule[kNMODULE];
  TH1I *hMaxTimeBinYModule[kNMODULE];
  TH1I *hMaxTimeBinClusterXModule[kNMODULE];
  TH1I *hMaxTimeBinClusterYModule[kNMODULE];

  TH2F *hhClusterDistXMaxBin[kNMODULE];
  TH2F *hhClusterDistXStartTime[kNMODULE];
  TH2F *hhClusterDistXShappingTime[kNMODULE];
  TH2F *hhClusterStartTimeCorrelation[kNMODULE];
  TH2F *hhClusterShappingTimeCorrelation[kNMODULE];

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // $$ Group 2, with Cut on Starting Time
  // $$ starting time between kStartingTimeLow ~ kStartingTimeHigh
  TH1F *hChargeFromFitCut[kNMODULE];
  TH1F *hHitTimeXCut[kNMODULE];
  TH1F *hAPVConstXCut[kNMODULE];
  TH1F *hHitTimeYCut[kNMODULE];
  TH1F *hAPVConstYCut[kNMODULE];
  TH1F *hReducedChisquareCut[kNMODULE];
  //starting time and shaping time for each APV
  TH1F *hChargeFromFitAPVCut[kNAPV];
  TH1F *hHitTimeAPVCut[kNAPV];
  TH1F *hAPVConstAPVCut[kNAPV];
  TH1F *hReducedChisquareAPVCut[kNAPV];
  TH1F *hMaxADConStripCut[kNMODULE];
  TH1F *hMaxADConStripTimeCut[kNMODULE];
  //for each APV
  TH1F *hMaxADConStripAPVCut[kNAPV];
  TH1F *hMaxADConStripTimeAPVCut[kNAPV];
  TH2F *hhRMSvsMeanTimeCut;
  TH2F *hhRMSvsMeanConstCut;

  TH1F *hRMSStartTimeCut;

  //Positions
  //2D Hit and CLuster Distribution for each chamber
  TH2F *hhClusterDistCut[kNMODULE];
  //1D Hit and CLuster Distribution for each chamber
  TH1I *hHitDistXCut[kNMODULE];
  TH1I *hHitDistYCut[kNMODULE];
  TH1F *hClusterDistXCut[kNMODULE];
  TH1F *hClusterDistYCut[kNMODULE];

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // $$ Group 3, with Cut on Signal Shape; see function IsBest() in GEMTracking class.
  // $$ Following histograms with a much stricter criterion for cut
  TH1F *hChargeFromFitBest[kNMODULE];
  TH1F *hHitTimeXBest[kNMODULE];
  TH1F *hAPVConstXBest[kNMODULE];
  TH1F *hHitTimeYBest[kNMODULE];
  TH1F *hAPVConstYBest[kNMODULE];
  TH1F *hReducedChisquareBest[kNMODULE];
  //starting time and shaping time for each APV
  TH1F *hChargeFromFitAPVBest[kNAPV];
  TH1F *hHitTimeAPVBest[kNAPV];
  TH1F *hAPVConstAPVBest[kNAPV];
  TH1F *hReducedChisquareAPVBest[kNAPV];
  TH1F *hMaxADConStripBest[kNMODULE];
  TH1F *hMaxADConStripTimeBest[kNMODULE];
  //for each APV
  TH1F *hMaxADConStripAPVBest[kNAPV];
  TH1F *hMaxADConStripTimeAPVBest[kNAPV];
  TH2F *hhRMSvsMeanTimeBest;
  TH2F *hhRMSvsMeanConstBest;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // $$ Group 4
  // $$ Timing Results for Clusters
  //TH1F *hHitTimeCluster[kNMODULE];
  //TH1F *hAPVConstCluster[kNMODULE];
  TH1F *hHitTimeClusterX[kNMODULE];
  TH1F *hHitTimeClusterY[kNMODULE];
  TH1F *hAPVConstClusterX[kNMODULE];
  TH1F *hAPVConstClusterY[kNMODULE];
  TH1F *hReducedChisquareCluster[kNMODULE];

  //==================================================================================
  // added by siyu 
  // Strips ADC distribution, ADC values for each channels vs counts
  TH1F *hStripsADCdist[kNMODULE];
  TH1F *hStripsADCdistX[kNMODULE];
  TH1F *hStripsADCdistY[kNMODULE];
  TH1F *hStripsADCdist_cut[kNMODULE];
  TH1F *hStripsADCdistX_cut[kNMODULE];
  TH1F *hStripsADCdistY_cut[kNMODULE];
  
  
  // @ initialize histograms.
  void InitHistograms()
  {
    for(int i=0;i<kNMODULE;i++)
      {
	hChargeFromFit[i] = new TH1F(Form("hChargeFromFit_module%d",i), Form("Charge From Fitted Shape on a strip_module%d",i),1000, 0, 5000);
	hChargeFromFitCut[i] = new TH1F(Form("hChargeFromFitCut_module%d",i), Form("Charge From Fitted Shape on a strip with Cut_module%d",i),1000, 0, 5000);
	hChargeFromFitBest[i] = new TH1F(Form("hChargeFromFitBest_module%d",i), Form("Charge From Fitted Shape on a strip Best_module%d",i),1000, 0, 5000);

	hHitTimeX[i] = new TH1F(Form("hHitTimeX_module%d",i), Form("X Strip raw hit start time_module%d",i), 500, -200, 200);
	hHitTimeX[i]->GetXaxis()->SetTitle("Time [ns]");
	hHitTimeXCut[i] = new TH1F(Form("hHitTimeXCut_module%d",i), Form("X Strip raw hit start time with Cut_module%d",i), 500, -200, 200);
	hHitTimeXCut[i]->GetXaxis()->SetTitle("Time [ns]");
	hHitTimeXBest[i] = new TH1F(Form("hHitTimeXBest_module%d",i), Form("X Strip raw hit start time Best_module%d",i), 500, -200, 200);
	hHitTimeXBest[i]->GetXaxis()->SetTitle("Time [ns]");
	hHitTimeY[i] = new TH1F(Form("hHitTimeY_module%d",i), Form("Y Strip raw hit start time_module%d",i), 500, -200, 200);
	hHitTimeY[i]->GetXaxis()->SetTitle("Time [ns]");
	hHitTimeYCut[i] = new TH1F(Form("hHitTimeYCut_module%d",i), Form("Y Strip raw hit start time with Cut_module%d",i), 500, -200, 200);
	hHitTimeYCut[i]->GetXaxis()->SetTitle("Time [ns]");
	hHitTimeYBest[i] = new TH1F(Form("hHitTimeYBest_module%d",i), Form("Y Strip raw hit start time Best_module%d",i), 500, -200, 200);
	hHitTimeYBest[i]->GetXaxis()->SetTitle("Time [ns]");

	hAPVConstX[i] = new TH1F(Form("hAPVConstX_module%d",i), Form("X APV shaping time_module%d",i), 1000, 0, 500);
	hAPVConstX[i]->GetXaxis()->SetTitle("Time [ns]");
	hAPVConstXCut[i] = new TH1F(Form("hAPVConstXCut_module%d",i), Form("X APV shaping time with Cut_module%d",i), 1000, 0, 500);
	hAPVConstXCut[i]->GetXaxis()->SetTitle("Time [ns]");
	hAPVConstXBest[i] = new TH1F(Form("hAPVConstXBest_module%d",i), Form("X APV shaping time Best_module%d",i), 1000, 0, 500);
	hAPVConstXBest[i]->GetXaxis()->SetTitle("Time [ns]");
	hAPVConstY[i] = new TH1F(Form("hAPVConstY_module%d",i), Form("Y APV shaping time_module%d",i), 1000, 0, 500);
	hAPVConstY[i]->GetXaxis()->SetTitle("Time [ns]");
	hAPVConstYCut[i] = new TH1F(Form("hAPVConstYCut_module%d",i), Form("Y APV shaping time with Cut_module%d",i), 1000, 0, 500);
	hAPVConstYCut[i]->GetXaxis()->SetTitle("Time [ns]");
	hAPVConstYBest[i] = new TH1F(Form("hAPVConstYBest_module%d",i), Form("Y APV shaping time Best_module%d",i), 1000, 0, 500);
	hAPVConstYBest[i]->GetXaxis()->SetTitle("Time [ns]");

	hMaxADConStrip[i] = new TH1F(Form("hMaxADConStrip_module%d",i), Form("Maximum ADC from fit_module%d",i), 1000, 0, 5000);
	hMaxADConStrip[i]->GetXaxis()->SetTitle("ADC");
	hMaxADConStripCut[i] = new TH1F(Form("hMaxADConStripCut_module%d",i), Form("Maximum ADC from fit with Cut_module%d",i), 1000, 0, 5000);
	hMaxADConStripCut[i]->GetXaxis()->SetTitle("ADC");
	hMaxADConStripBest[i] = new TH1F(Form("hMaxADConStripBest_module%d",i), Form("Maximum ADC from fit Best_module%d",i), 1000, 0, 5000);
	hMaxADConStripBest[i]->GetXaxis()->SetTitle("ADC");

	hMaxADConStripTime[i] = new TH1F(Form("hMaxADConStripTime_module%d",i), Form("Time for Max ADC from fit_module%d",i), 500, -200, 200);
	hMaxADConStripTime[i]->GetXaxis()->SetTitle("Time [ns]");
	hMaxADConStripTimeCut[i] = new TH1F(Form("hMaxADConStripTimeCut_module%d",i), Form("Time for Max ADC from fit with Cut_module%d",i), 500, -200, 200);
	hMaxADConStripTimeCut[i]->GetXaxis()->SetTitle("Time [ns]");
	hMaxADConStripTimeBest[i] = new TH1F(Form("hMaxADConStripTimeBest_module%d",i), Form("Time for Max ADC from fit Best_module%d",i), 500, -200, 200);
	hMaxADConStripTimeBest[i]->GetXaxis()->SetTitle("Time [ns]");

	hReducedChisquare[i] = new TH1F(Form("hReducedChisquaremodule_%d",i),Form("Chi^2 Module_%d",i),1e4,0,1e4);
	hReducedChisquareCut[i] = new TH1F(Form("hReducedChisquareCutmodule_%d",i),Form("Chi^2 Cut Module_%d",i),1e4,0,1e4);
	hReducedChisquareBest[i] = new TH1F(Form("hReducedChisquareBestmodule_%d",i),Form("Chi^2 Best Module_%d",i),1e4,0,1e4);
      }
    for(int i=0;i<kNAPV;i++)
      {
	hChargeFromFitAPV[i] = new TH1F(Form("hChargeFromFitAPV%d",i),"",1000,0,5000);
	hChargeFromFitAPVCut[i] = new TH1F(Form("hChargeFromFitAPVCut%d",i),"",1000,0,5000);
	hChargeFromFitAPVBest[i] = new TH1F(Form("hChargeFromFitAPVBest%d",i),"",1000,0,5000);

	hHitTimeAPV[i] = new TH1F(Form("hHitTimeAPV%d",i),"",500, -200, 200);
	hHitTimeAPV[i]->GetXaxis()->SetTitle("Time [ns]");
	hHitTimeAPVCut[i] = new TH1F(Form("hHitTimeAPVCut%d",i),"",500, -200, 200);
	hHitTimeAPVCut[i]->GetXaxis()->SetTitle("Time [ns]");
	hHitTimeAPVBest[i] = new TH1F(Form("hHitTimeAPVBest%d",i),"",500, -200, 200);
	hHitTimeAPVBest[i]->GetXaxis()->SetTitle("Time [ns]");

	hAPVConstAPV[i] = new TH1F(Form("hAPVConstAPV%d",i),"",1000, 0, 500);
	hAPVConstAPV[i]->GetXaxis()->SetTitle("Time [ns]");
	hAPVConstAPVCut[i] = new TH1F(Form("hAPVConstAPVCut%d",i),"",1000, 0, 500);
	hAPVConstAPVCut[i]->GetXaxis()->SetTitle("Time [ns]");
	hAPVConstAPVBest[i] = new TH1F(Form("hAPVConstAPVBest%d",i),"",1000, 0, 500);
	hAPVConstAPVBest[i]->GetXaxis()->SetTitle("Time [ns]");

	hMaxADConStripAPV[i] = new TH1F(Form("hMaxADConStripAPV%d",i),"",1000,0,5000);
	hMaxADConStripAPV[i]->GetXaxis()->SetTitle("ADC channels");
	hMaxADConStripAPVCut[i] = new TH1F(Form("hMaxADConStripAPVCut%d",i),"",1000,0,5000);
	hMaxADConStripAPVCut[i]->GetXaxis()->SetTitle("ADC channels");
	hMaxADConStripAPVBest[i] = new TH1F(Form("hMaxADConStripAPVBest%d",i),"",1000,0,5000);
	hMaxADConStripAPVBest[i]->GetXaxis()->SetTitle("ADC channels");

	hMaxADConStripTimeAPV[i] = new TH1F(Form("hMaxADConStripTimeAPV%d",i),"",500, -200, 200);
	hMaxADConStripTimeAPV[i]->GetXaxis()->SetTitle("Time [ns]");
	hMaxADConStripTimeAPVCut[i] = new TH1F(Form("hMaxADConStripTimeAPVCut%d",i),"",500, -200, 200);
	hMaxADConStripTimeAPVCut[i]->GetXaxis()->SetTitle("Time [ns]");
	hMaxADConStripTimeAPVBest[i] = new TH1F(Form("hMaxADConStripTimeAPVBest%d",i),"",500, -200, 200);
	hMaxADConStripTimeAPVBest[i]->GetXaxis()->SetTitle("Time [ns]");

	hReducedChisquareAPV[i] = new TH1F(Form("hReducedChisquareAPV_%d",i),Form("Chi^2 APV_%d",i),1e4,0,1e4);
	hReducedChisquareAPVCut[i] = new TH1F(Form("hReducedChisquareAPVCut_%d",i),Form("Chi^2 Cut APV_%d",i),1e4,0,1e4);
	hReducedChisquareAPVBest[i] = new TH1F(Form("hReducedChisquareAPVBest_%d",i),Form("Chi^2 Best APV_%d",i),1e4,0,1e4);
      }
  
    hhRMSvsMeanTime = new TH2F("rmsvsmeantime", "RMS vs Mean for Starting Time",100,-50,50,100,0,50);
    hhRMSvsMeanConst = new TH2F("rmsvsmeanconst","RMS vs Mean for APV Const", 100, 25,80, 100, 0,50);
    hhRMSvsMeanTimeCut = new TH2F("rmsvsmeantimecut", "RMS vs Mean for Starting Time with Cut",100,-50,50,100,0,50);
    hhRMSvsMeanConstCut = new TH2F("rmsvsmeanconstcut","RMS vs Mean for APV Const with Cut", 100, 25,80, 100, 0,50);
    hhRMSvsMeanTimeBest = new TH2F("rmsvsmeantimeBest", "RMS vs Mean for Starting Time Best",100,-50,50,100,0,50);
    hhRMSvsMeanConstBest = new TH2F("rmsvsmeanconstBest","RMS vs Mean for APV Const Best", 100, 25,80, 100, 0,50);

    //Starting Time Histograms sigma distribution for all APVs
    hRMSStartTime = new TH1F("hRMSStartTime","#sigma Distribution for Starting Time Histograms",100,0,10);
    hRMSStartTimeCut = new TH1F("hRMSStartTimeCut","#sigma Distribution for Starting Time Histograms with cut",100,0,10);
  
    //2D position distributions for all modules
    for(int i=0;i<kNMODULE;i++)
      {
	Int_t nbinsx = 50*(Int_t)kNbXAPVModule[i];
	Double_t xrange = kNbXAPVModule[i]/2*128*kStripPitchX[i];
	Int_t nbinsy = 50*(Int_t)kNbYAPVModule[i];
	Double_t yrange = kNbYAPVModule[i]/2*128*kStripPitchY[i];
	Int_t nbChannelsX = (Int_t)kNbXAPVModule[i]*128;
	Int_t nbChannelsY = (Int_t)kNbYAPVModule[i]*128;

	hhClusterDist[i] = new TH2F(Form("hhClusterDist_%d",i), Form("Cluster Distributions module_%d",i),nbinsx, (-1.0)*xrange, xrange, nbinsy, (-1.0)*yrange, yrange);
	hHitDistX[i] = new TH1I(Form("hHitDistX_%d",i), Form("X side Hit Distributions module_%d",i), nbChannelsX-1,0,nbChannelsX);
	hHitDistY[i] = new TH1I(Form("hHitDistY_%d",i), Form("Y side Hit Distributions module_%d",i), nbChannelsY-1,0,nbChannelsY);
	hClusterDistX[i] = new TH1F(Form("hClusterDistX_%d",i), Form("X side Cluster Distributions module_%d",i),nbinsx-1, (-1.0)*xrange, xrange);
	hClusterDistY[i] = new TH1F(Form("hClusterDistY_%d",i), Form("Y side Cluster Distributions module_%d",i),nbinsy-1, (-1.0)*yrange, yrange);
	hClusterAdcDistX[i] = new TH1F(Form("hClusterAdcDistX_%d",i), Form("X axis cluster charge_%d",i), 2000,0,20000);
	hClusterAdcDistY[i] = new TH1F(Form("hClusterAdcDistY_%d",i), Form("Y axis cluster charge_%d",i), 2000,0,20000);


	hhClusterDistCut[i] = new TH2F(Form("hhClusterDistCut_%d",i), Form("Cluster Distributions module_%d with cut",i),nbinsx, (-1.0)*xrange, xrange, nbinsy, (-1.0)*yrange, yrange);
	hHitDistXCut[i] = new TH1I(Form("hHitDistXCut_%d",i), Form("X side Hit Distributions module_%d with cut",i),nbChannelsX,0,nbChannelsX );
	hHitDistYCut[i] = new TH1I(Form("hHitDistYCut_%d",i), Form("Y side Hit Distributions module_%d with cut",i), nbChannelsY,0,nbChannelsY);
	hClusterDistXCut[i] = new TH1F(Form("hClusterDistXCut_%d",i), Form("X side Cluster Distributions module_%d with cut",i),nbinsx, (-1.0)*xrange, xrange);
	hClusterDistYCut[i] = new TH1F(Form("hClusterDistYCut_%d",i), Form("Y side Cluster Distributions module_%d with cut",i), nbinsy, (-1.0)*yrange, yrange );
	//charge ratio cut
	hhClusterDistRatioMethod[i] = new TH2F(Form("hhClusterDistRatioMethod_%d",i), Form("Cluster Distributions with Ratio Cut module_%d",i),nbinsx, (-1.0)*xrange, xrange, nbinsy, (-1.0)*yrange, yrange);

	hhClusterChargeRatio[i] = new TH2F(Form("hhClusterChargeRatio_%d", i),Form("Cluster Charge Sharing module_%d", i),200,0,2500,200,0,2500);
	// used for the selected best event ration   Sept 16th 2016
	

	//charge ratio cut
	hhClusterChargeRatioRatioMethod[i] = new TH2F(Form("hhClusterChargeRatioRatioMethod_%d",i),Form("Cluster Charge Sharing with Ratio Cut module_%d", i),200,0,2500,200,0,2500);

	hMaxTimeBinXModule[i] = new TH1I(Form("hMaxTimeBinXModule_%d",i),Form("X plane Max Time Bin Module_%d",i),20,0,20);
	hMaxTimeBinYModule[i] = new TH1I(Form("hMaxTimeBinYModule_%d",i),Form("Y plane Max Time Bin Module_%d",i),20,0,20);
	hMaxTimeBinClusterXModule[i] = new TH1I(Form("hMaxTimeBinClusterXModule_%d",i),Form("Cluster X plane Max Time Bin Module_%d",i),20,0,20);
	hMaxTimeBinClusterYModule[i] = new TH1I(Form("hMaxTimeBinClusterYModule_%d",i),Form("Cluster Y plane Max Time Bin Module_%d",i),20,0,20);

	//2D Cluster Distribution weighted by maxtimebin, start time, and shapping time
	hhClusterDistXMaxBin[i] = new TH2F(Form("hhClusterDistXMaxBin_module%d",i), Form("Cluster Distribution weighted by X Plane Max Time Bin module_%d",i),nbinsx, (-1.0)*xrange, xrange, nbinsy, (-1.0)*yrange, yrange ); 

	hhClusterDistXStartTime[i] = new TH2F(Form("hhClusterDistXStartTime_module%d",i), Form("Cluster Distribution weighted by X Plane Start Time module_%d",i),nbinsx, (-1.0)*xrange, xrange, nbinsy, (-1.0)*yrange, yrange ); 

	hhClusterDistXShappingTime[i] = new TH2F(Form("hhClusterDistXShappingTime_module%d",i), Form("Cluster Distribution weighted by X Plane Shappint Time module_%d",i),nbinsx, (-1.0)*xrange, xrange, nbinsy, (-1.0)*yrange, yrange ); 

	hhClusterStartTimeCorrelation[i] = new TH2F(Form("hhClusterStartTimeCorrelation_module%d",i), Form("Cluster Start Time Correlation module_%d", i), 300, -100.0,200, 300, -100.0, 200);

	hhClusterShappingTimeCorrelation[i] = new TH2F(Form("hhClusterShappingTimeCorrelation_module%d",i), Form("Cluster Shapping Time Correlation module_%d", i), 300, -100.0,200, 300, -100.0, 200);
      }
  
    //Timing Histograms for Clusters
    for(int i=0;i<kNMODULE;i++)
      {
	/*
	  hHitTimeCluster[i] = new TH1F(Form("hHitTimeCluster_module%d",i), Form("Strip raw hit start time for Cluster_module%d",i), 500, -200, 200);
	  hHitTimeCluster[i]->GetXaxis()->SetTitle("Time [ns]");
      
	  hAPVConstCluster[i] = new TH1F(Form("hAPVConstCluster_module%d",i), Form("APV shaping time Cluster_module%d",i), 500, 0, 500);
	  hAPVConstCluster[i]->GetXaxis()->SetTitle("Time [ns]");
	*/
	hHitTimeClusterX[i] = new TH1F(Form("hHitTimeClusterX_module%d",i), Form("X side Raw hit start time for Cluster_module%d",i), 500, -200, 200);
	hHitTimeClusterX[i]->GetXaxis()->SetTitle("Time [ns]");
	hHitTimeClusterY[i] = new TH1F(Form("hHitTimeClusterY_module%d",i), Form("Y side Raw hit start time for Cluster_module%d",i), 500, -200, 200);
	hHitTimeClusterY[i]->GetXaxis()->SetTitle("Time [ns]");
      
	hAPVConstClusterX[i] = new TH1F(Form("hAPVConstClusterX_module%d",i), Form("X side APV shaping time Cluster_module%d",i), 1000, 0, 500);
	hAPVConstClusterX[i]->GetXaxis()->SetTitle("Time [ns]");
	hAPVConstClusterY[i] = new TH1F(Form("hAPVConstClusterY_module%d",i), Form("Y side APV shaping time Cluster_module%d",i), 1000, 0, 500);
	hAPVConstClusterY[i]->GetXaxis()->SetTitle("Time [ns]");

	hReducedChisquareCluster[i] = new TH1F(Form("hReducedChisquareCluster_module%d",i),Form("Cluster Chi^2 Module_%d",i),1e4,0,1e4);
      }
  
    //Cluster Infomation
    hClusterSize = new TH1F("hClusterSize","Cluster Size",15,0,15);
    
    hClusterSize_x= new TH1F("hClusterSize_x","Cluster Size x",15,0,15);
    hClusterSize_y= new TH1F("hClusterSize_y","Cluster Size y",15,0,15);
    
    hNbClusterPerPlane = new TH1I("hNbClusterPerPlane", "# of Clusters Per Plane", 10, 0,10);
    hNbClusterPerPlane_x = new TH1I("hNbClusterPerPlane_x", "# of Clusters Per Plane x", 10, 0,10);
    hNbClusterPerPlane_y = new TH1I("hNbClusterPerPlane_y", "# of Clusters Per Plane y", 10, 0,10);
    
    
    
    
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Added by Siyu 
    //
    for (int i=0;i<kNMODULE;i++)
      {
        hStripsADCdist[i] = new TH1F("hStripsADCdist", Form("hStripsADCdist module_%d",i),4095,0,4095);    // X is the ADC value of the each strips Y is the counts
        hStripsADCdistX[i] = new TH1F("hStripsADCdistX", Form("hStripsADCdistX module_%d",i),4095,0,4095);    // X is the ADC value of the each strips Y is the counts
        hStripsADCdistY[i] = new TH1F("hStripsADCdistY", Form("hStripsADCdistY module_%d",i),4095,0,4095);    // X is the ADC value of the each strips Y is the counts
        
      }
    
  }

  //==================================================================================
  // @ save histograms.
  // June 28th 2016, add the input file name and default file name to this function --Siyu
  void PrintHistograms(const char *out_put_fname="results/tracks.root")
  {
    TFile *file = new TFile(out_put_fname,"RECREATE");
    /*    for(int i=0;i<kNMODULE;i++)
      {
	//Hit Time
	hHitTimeX[i]->Write();
	hHitTimeY[i]->Write();
	//APV decay constant
	hAPVConstX[i]->Write();
	hAPVConstY[i]->Write();
	//Max ADC on strip
	hMaxADConStrip[i]->Write();
	//Max ADC Time on strip
	hMaxADConStripTime[i]->Write();
	//Max ADC Time on strip
	hChargeFromFit[i]->Write();  
	//Hit Distribution for all modules

	hReducedChisquare[i]->Write();
      }
    */
    for(int i=0;i<kNMODULE;i++)
      {
	hHitDistX[i]->Write();
	hHitDistY[i]->Write();
	hClusterDistX[i]->Write();
	hClusterDistY[i]->Write();

	hClusterAdcDistX[i]->Write();
	hClusterAdcDistY[i]->Write();

	hhClusterDist[i]->Write();
	//hhClusterDistRatioMethod[i]->Write();
	//Cluster charge ratio
	hhClusterChargeRatio[i]->Write();
	//Cluster charge ratio using ratio method
	//hhClusterChargeRatioRatioMethod[i]->Write();
	hMaxTimeBinXModule[i]->Write();
	hMaxTimeBinYModule[i]->Write();
	hMaxTimeBinClusterXModule[i]->Write();
	hMaxTimeBinClusterYModule[i]->Write();

	Int_t nX = hhClusterDist[i]->GetNbinsX();
	Int_t nY = hhClusterDist[i]->GetNbinsY();
	for(int ii=1;ii<=nX;ii++)
	  {
	    for(int jj=1;jj<=nY;jj++)
	      {
		if(hhClusterDist[i]->GetBinContent(ii,jj) != 0)
		  {
		    //hhClusterDistXMaxBin[i]->SetBinContent(ii,jj,((hhClusterDistXMaxBin[i]->GetBinContent(ii,jj))/(hhClusterDist[i]->GetBinContent(ii,jj))) );
		    //hhClusterDistXStartTime[i]->SetBinContent(ii,jj,((hhClusterDistXStartTime[i]->GetBinContent(ii,jj))/(hhClusterDist[i]->GetBinContent(ii,jj))) );
		    //hhClusterDistXShappingTime[i]->SetBinContent(ii,jj,((hhClusterDistXShappingTime[i]->GetBinContent(ii,jj))/(hhClusterDist[i]->GetBinContent(ii,jj))) );
		  }
		/*
		  else
		  {
		  hhClusterDistXMaxBin[i]->SetBinContent(ii,jj,0 );
		  hhClusterDistXStartTime[i]->SetBinContent(ii,jj,0 );
		  hhClusterDistXShappingTime[i]->SetBinContent(ii,jj,0 );
		  }
		*/
	      }
	  }
	//hhClusterDistXMaxBin[i]->Write();
	hhClusterDistXStartTime[i]->Write();
	hhClusterDistXShappingTime[i]->Write();
	hhClusterStartTimeCorrelation[i]->Write();
	hhClusterShappingTimeCorrelation[i]->Write();
      }
    for(int i=0;i<kNMODULE;i++)
      {
	//Cluster Time
	hHitTimeClusterX[i]->Write();
	hHitTimeClusterY[i]->Write();
	//Cluster APV decay constant
	hAPVConstClusterX[i]->Write();
	hAPVConstClusterY[i]->Write();

	//Chi Square
	//hReducedChisquareCluster[i]->Write();
      }
    //Cluster Size
     hClusterSize->Write();
     hClusterSize_x->Write();    // write the size of cluster in x dimension and y dimension seperatly in the root file 
     hClusterSize_y->Write();
       
    //Cluster Size
     hNbClusterPerPlane->Write();
     hNbClusterPerPlane_x->Write();  // write the number of clusters in x and y dimension seperatly in the root file 
     hNbClusterPerPlane_y->Write();
     
     // added by siyu 
     //used for calculat strips ADC distribution 
  for(int i=0; i<kNMODULE;i++)
    {
     hStripsADCdist[i]->Write();
     hStripsADCdistX[i]->Write();
     hStripsADCdistY[i]->Write();
      
    }
  
  
    file->Write();
    file->Close();
  }

  //==================================================================================
  void PrintHistogramsCut()
  {
    TFile *file = new TFile("tracks_cut.root","RECREATE");
    for(int i=0;i<kNMODULE;i++)
      {
	//Hit Time
	hHitTimeXCut[i]->Write();
	hHitTimeYCut[i]->Write();
	//APV decay constant
	hAPVConstXCut[i]->Write();
	hAPVConstYCut[i]->Write();
	//Max ADC on strip
	hMaxADConStripCut[i]->Write();
	//Max ADC Time on strip
	hMaxADConStripTimeCut[i]->Write();
	//Max ADC Time on strip
	hChargeFromFitCut[i]->Write();

	hReducedChisquareCut[i]->Write();
      }
    //Hit Distribution for all modules
    for(int i=0;i<kNMODULE;i++)
      {
	hHitDistXCut[i]->Write();
	hHitDistYCut[i]->Write();
	hClusterDistXCut[i]->Write();
	hClusterDistYCut[i]->Write();
	hhClusterDistCut[i]->Write();
      }
    file->Write();
    file->Close();
  }
  
  
  //==================================================================================
  void PrintHistogramsBest()
  {
    TFile *file = new TFile("tracks_best.root","RECREATE");
    for(int i=0;i<kNMODULE;i++)
      {
	//Hit Time
	hHitTimeXBest[i]->Write();
	hHitTimeYBest[i]->Write();
	//APV decay constant
	hAPVConstXBest[i]->Write();
	hAPVConstYBest[i]->Write();
	//Max ADC on strip
	hMaxADConStripBest[i]->Write();
	//Max ADC Time on strip
	hMaxADConStripTimeBest[i]->Write();
	//Max ADC Time on strip
	hChargeFromFitBest[i]->Write();

	hReducedChisquareBest[i]->Write();
	
      }
    file->Write();
    file->Close();
  }
  
  
  //==================================================================================
  void PrintHistogramsAPV()
  {
    TFile *file = new TFile("tracks_each_APV.root","RECREATE");
    for(int i=0;i<kNAPV;i++)
      {
	//Hit Time
	if(hHitTimeAPV[i]->GetEntries()!=0)
	  {
	    hHitTimeAPV[i]->SetTitle(Form("APV Hit Time module_%d, plane_%d, APV_%d",((i-i%40)/40)/2,((i-i%40)/40)%2,i%40));
	    //hHitTimeAPV[i]->Fit("gaus","Q","",7.5,15);
	    if(hHitTimeAPV[i]->GetFunction("gaus")!=NULL)
	      {
		hRMSStartTime->Fill(hHitTimeAPV[i]->GetFunction("gaus")->GetParameter(2));
	      }
	    hHitTimeAPV[i]->Write();
	    hhRMSvsMeanTime -> Fill(hHitTimeAPV[i]->GetMean(),hHitTimeAPV[i]->GetRMS());
	  }
     
	//APV decay constant
	if(hAPVConstAPV[i]->GetEntries()!=0)
	  {
	    hAPVConstAPV[i]->SetTitle(Form("APV Const module_%d, plane_%d, APV_%d",((i-i%40)/40)/2,((i-i%40)/40)%2,i%40));
	    //hAPVConstAPV[i]->Fit("gaus","Q","",40,70);
	    hAPVConstAPV[i]->Write();
	    hhRMSvsMeanConst -> Fill(hAPVConstAPV[i]->GetMean(), hAPVConstAPV[i]->GetRMS());
	  }

	//Chi Square
	if(hReducedChisquareAPV[i]->GetEntries()!=0)
	  {
	    hReducedChisquareAPV[i]->SetTitle(Form("Chi Square module_%d, plane_%d, APV_%d",((i-i%40)/40)/2,((i-i%40)/40)%2,i%40));
	    //hAPVConstAPV[i]->Fit("gaus","Q","",40,70);
	    hReducedChisquareAPV[i]->Write();
	  }
      }
    hhRMSvsMeanTime->GetXaxis()->SetTitle("Mean");
    hhRMSvsMeanTime->GetYaxis()->SetTitle("RMS");
    hhRMSvsMeanTime->SetMarkerStyle(5);
    hhRMSvsMeanTime->SetMarkerColor(2);
    hhRMSvsMeanTime->Write();

    hhRMSvsMeanConst->GetXaxis()->SetTitle("Mean");
    hhRMSvsMeanConst->GetYaxis()->SetTitle("RMS");
    hhRMSvsMeanConst->SetMarkerStyle(5);
    hhRMSvsMeanConst->SetMarkerColor(2);
    hhRMSvsMeanConst->Write();
    //Distribution of the width of each Starting time histogram (From Fitting)
    hRMSStartTime->Write();
    file->Write();
    file->Close();
  }
  //==================================================================================
  void PrintHistogramsAPVCut()
  {
    TFile *file = new TFile("tracks_each_APV_cut.root","RECREATE");
    for(int i=0;i<kNAPV;i++)
      {
	//Hit Time
	if(hHitTimeAPVCut[i]->GetEntries()!=0)
	  {
	    hHitTimeAPVCut[i]->SetTitle(Form("APV Hit Time module_%d, plane_%d, APV_%d with cut",((i-i%40)/40)/2,((i-i%40)/40)%2,i%40));
	    //hHitTimeAPVCut[i]->Fit("gaus","Q","",0,30);
	    //hHitTimeAPVCut[i]->Fit("gaus","Q","",7.5,15);
	    if(hHitTimeAPVCut[i]->GetFunction("gaus")!=NULL)
	      {
		hRMSStartTimeCut->Fill(hHitTimeAPVCut[i]->GetFunction("gaus")->GetParameter(2));
	      }
	    hHitTimeAPVCut[i]->Write();
	    hhRMSvsMeanTimeCut -> Fill(hHitTimeAPVCut[i]->GetMean(),hHitTimeAPVCut[i]->GetRMS());
	  }
     
	//APV decay constant
	if(hAPVConstAPVCut[i]->GetEntries()!=0)
	  {
	    hAPVConstAPVCut[i]->SetTitle(Form("APV Const module_%d, plane_%d, APV_%d with cut",((i-i%40)/40)/2,((i-i%40)/40)%2,i%40));
	    //hAPVConstAPVCut[i]->Fit("gaus","Q");
	    hAPVConstAPVCut[i]->Write();
	    hhRMSvsMeanConstCut -> Fill(hAPVConstAPVCut[i]->GetMean(), hAPVConstAPVCut[i]->GetRMS());
	  }

	//Chi Square
	if(hReducedChisquareAPVCut[i]->GetEntries()!=0)
	  {
	    hReducedChisquareAPVCut[i]->SetTitle(Form("Chi Square with cut module_%d, plane_%d, APV_%d",((i-i%40)/40)/2,((i-i%40)/40)%2,i%40));
	    //hAPVConstAPV[i]->Fit("gaus","Q","",40,70);
	    hReducedChisquareAPVCut[i]->Write();
	  }
      }
    hhRMSvsMeanTimeCut->GetXaxis()->SetTitle("Mean");
    hhRMSvsMeanTimeCut->GetYaxis()->SetTitle("RMS");
    hhRMSvsMeanTimeCut->SetMarkerStyle(5);
    hhRMSvsMeanTimeCut->SetMarkerColor(2);
    hhRMSvsMeanTimeCut->Write();

    hhRMSvsMeanConstCut->GetXaxis()->SetTitle("Mean");
    hhRMSvsMeanConstCut->GetYaxis()->SetTitle("RMS");
    hhRMSvsMeanConstCut->SetMarkerStyle(5);
    hhRMSvsMeanConstCut->SetMarkerColor(2);
    hhRMSvsMeanConstCut->Write();

    hRMSStartTimeCut->Write();
    file->Write();
    file->Close();
  }
  //==================================================================================
  void PrintHistogramsAPVBest()
  {
    TFile *file = new TFile("tracks_each_APV_best.root","RECREATE");
    for(int i=0;i<kNAPV;i++)
      {
	//Hit Time
	if(hHitTimeAPVBest[i]->GetEntries()!=0)
	  {
	    hHitTimeAPVBest[i]->SetTitle(Form("APV Hit Time module_%d, plane_%d, APV_%d Best",((i-i%40)/40)/2,((i-i%40)/40)%2,i%40));
	    //hHitTimeAPVBest[i]->Fit("gaus","Q","",0,30);
	    hHitTimeAPVBest[i]->Write();
	    hhRMSvsMeanTimeBest -> Fill(hHitTimeAPVBest[i]->GetMean(),hHitTimeAPVBest[i]->GetRMS());
	  }
     
	//APV decay constant
	if(hAPVConstAPVBest[i]->GetEntries()!=0)
	  {
	    hAPVConstAPVBest[i]->SetTitle(Form("APV Const module_%d, plane_%d, APV_%d Best",((i-i%40)/40)/2,((i-i%40)/40)%2,i%40));
	    //hAPVConstAPVBest[i]->Fit("gaus","Q");
	    hAPVConstAPVBest[i]->Write();
	    hhRMSvsMeanConstBest -> Fill(hAPVConstAPVBest[i]->GetMean(), hAPVConstAPVBest[i]->GetRMS());
	  }

	//Chi Square
	if(hReducedChisquareAPVBest[i]->GetEntries()!=0)
	  {
	    hReducedChisquareAPVBest[i]->SetTitle(Form("Chi Square Best with cut module_%d, plane_%d, APV_%d",((i-i%40)/40)/2,((i-i%40)/40)%2,i%40));
	    //hAPVConstAPV[i]->Fit("gaus","Q","",40,70);
	    hReducedChisquareAPVBest[i]->Write();
	  }
      }
    hhRMSvsMeanTimeBest->GetXaxis()->SetTitle("Mean");
    hhRMSvsMeanTimeBest->GetYaxis()->SetTitle("RMS");
    hhRMSvsMeanTimeBest->SetMarkerStyle(5);
    hhRMSvsMeanTimeBest->SetMarkerColor(2);
    hhRMSvsMeanTimeBest->Write();

    hhRMSvsMeanConstBest->GetXaxis()->SetTitle("Mean");
    hhRMSvsMeanConstBest->GetYaxis()->SetTitle("RMS");
    hhRMSvsMeanConstBest->SetMarkerStyle(5);
    hhRMSvsMeanConstBest->SetMarkerColor(2);
    hhRMSvsMeanConstBest->Write();
    file->Write();
    file->Close();
  }

  //==================================================================================
  void save_cluster_tree()
  {
    TFile f("GEMTCluster.root","RECREATE");
    cluster_tree.GetTree()->Write("",TObject::kOverwrite);
    f.Write();
    f.Close();
  }
  
}

#endif
