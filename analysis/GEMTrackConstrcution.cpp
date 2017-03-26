/*
 * GEMTrackConstrcution.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: newdriver
 */
#include "GEMTrackConstrcution.h"

// used for the test functions
#include "TCanvas.h"
#include "TH2D.h"
#include  "TH3D.h"
#include "TLine.h"
#include "TProfile.h"

GEMTrackConstrcution::GEMTrackConstrcution(std::vector<GEMCluster> Cluster_input) {
	// TODO Auto-generated constructor stub
	vGEMCluster=Cluster_input;   // load the cluster
	// load the position of the cluster in the buffer
	for(int detector_index=0;detector_index<kNMODULE;detector_index++){
		if(vGEMCluster.size()>0){
			std::vector<GEMCluster>::iterator iter_cluster=vGEMCluster.begin();
			while(iter_cluster!=vGEMCluster.end()){

				if((*iter_cluster).Module==detector_index && (*iter_cluster).Plane==0){
					vxCluster[detector_index].push_back(*iter_cluster);
				}
				else if ((*iter_cluster).Module==detector_index && (*iter_cluster).Plane==1){
					vyCluster[detector_index].push_back(*iter_cluster);
				}
				iter_cluster++;
			}
		}
		Sort(vxCluster[detector_index]);
		Sort(vyCluster[detector_index]);
		// load the number of clusters for each plane
		vNCluster[detector_index]=(vxCluster[detector_index].size()<vyCluster[detector_index].size())? vxCluster[detector_index].size() : vyCluster[detector_index].size();
	}

}

// used for Calculate the comic run detector efficiency
void GEMTrackConstrcution::CosmicEff(int Detector_ID){
	if (Detector_ID >= 0) {
		SingleTrackFilter(Detector_ID);
	} else {
		for (int i = 0; i < kNMODULE; i++) {
			SingleTrackFilter(i);
		}
	}
	//ResultReport();
}


void GEMTrackConstrcution::Clean() {
	vGEMCluster.clear();
	for(unsigned int i=0; i<kNMODULE;i++){
		vNCluster[i]=0;
		vxCluster[i].clear();
		vyCluster[i].clear();
	}
}

// Sort According
void GEMTrackConstrcution::Sort(vector<GEMCluster> &vC)
{
  vector<GEMCluster>::iterator it = vC.begin();
  vector<GEMCluster>::iterator it1;
  for(;it!=vC.end();++it)
    {
      for(it1=it+1; it1 != vC.end(); ++it1)
	{
	  if((*it).Charge < (*it1).Charge)
	    {
	      GEMCluster gem_cluster;
	      (*it).Clone(gem_cluster);
	      (*it1).Clone( (*it) );
	      gem_cluster.Clone(*it1);
	    }
	}
    }
}

void GEMTrackConstrcution::SingleTrackFilter(unsigned int ChamberID) {

	Bool_t SingleTrack_Flag = 1;
	for (unsigned int i = 0; i < kNMODULE; i++) {
		if ((i != ChamberID)) {
			if (vNCluster[i] != 1)
				SingleTrack_Flag = 0;
		}
	}
	if (SingleTrack_Flag) {
		TLine * detectorX[kNMODULE];
		TLine * detectorY[kNMODULE];
		TH2D *vxHisto=new TH2D("x-z plane","x-z plane",160,(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],160,-10,120);
		vxHisto->GetXaxis()->SetTitle("x dimension");
		vxHisto->GetYaxis()->SetTitle("z dimension");
		vxHisto->SetMarkerSize(0.5);
		vxHisto->SetMarkerStyle(21);
		vxHisto->SetMarkerColor(4);

		//vxHisto->GetYaxis()->SetRangeUser(-10,60);
		TH2D *vyHisto=new TH2D("y-z plane","y-z plane",160,(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],160,-10,120);
		vyHisto->GetXaxis()->SetTitle("y dimension");
		vyHisto->GetYaxis()->SetTitle("z dimension");
		vyHisto->SetMarkerSize(0.6);
		vyHisto->SetMarkerStyle(21);
		vyHisto->SetMarkerColor(4);

		TH2D *vzHisto= new TH2D("x-y plane","x-y plane",160,(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0]-20,(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0]+20,160,(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0]);
		vzHisto->GetXaxis()->SetTitle("x dimension");
		vzHisto->GetYaxis()->SetTitle("y dimension");
		vzHisto->SetMarkerSize(0.6);
		vzHisto->SetMarkerStyle(21);
		vzHisto->SetMarkerColor(4);

		TH3D *histo_3d = new TH3D("GEM 3D tracking", "GEM 3D tracking", 160,
				-80, 80, 160, -80, 80, 160, -80, 80);
		histo_3d->SetMarkerSize(0.6);
		histo_3d->SetMarkerStyle(21);
		histo_3d->SetMarkerColor(4);

		for (int i = 0; i < kNMODULE; i++) {
			//printf("\n\n Detector: %d\n",i);
			for (unsigned int j = 0; j < vNCluster[i]; j++) {

				vCorrectedPosX[i]=DistortionMx[i][0]*vxCluster[i][j].Pos+DistortionMx[i][1]*vyCluster[i][j].Pos+DistortionMx[i][1]*kZStartModule[i];
				vCorrectedPosY[i]=DistortionMx[i][4]*vxCluster[i][j].Pos+DistortionMx[i][5]*vyCluster[i][j].Pos+DistortionMx[i][6]*kZStartModule[i];
				vCorrectedPosZ[i]=DistortionMx[i][8]*vxCluster[i][j].Pos+DistortionMx[i][9]*vyCluster[i][j].Pos+DistortionMx[i][10]*kZStartModule[i];

				vOriginalPosX[i]=vxCluster[i][j].Pos;
				vOriginalPosY[i]=vyCluster[i][j].Pos;
				vOriginalPosZ[i]=kZStartModule[i];

				vxHisto->Fill(vxCluster[i][j].Pos, kZStartModule[i]);
				//vxHisto->Fill(0.0,0.0);
				vyHisto->Fill(vyCluster[i][j].Pos, kZStartModule[i]);
				vzHisto->Fill(vxCluster[i][j].Pos, vyCluster[i][j].Pos);
				histo_3d->Fill(vxCluster[i][j].Pos, vyCluster[i][j].Pos,
						kZStartModule[i]);
			}
			detectorX[i] = new TLine((-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0], kZStartModule[i], (1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],
					kZStartModule[i]);
			detectorY[i] = new TLine((-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0], kZStartModule[i], (1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],
								kZStartModule[i]);
		}
		Double_t FitRangeMinX=(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0];
		Double_t FitRangeMaxX=(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0];
		Double_t FitRangeMinY=(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0];
		Double_t FitRangeMaxY=(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0];

		vxHisto->Fit("pol1","FMQ","",FitRangeMinX-10,FitRangeMaxX+10);
		vyHisto->Fit("pol1","FMQ","",FitRangeMinY-10,FitRangeMaxY+10);
		vzHisto->Fit("pol1","FMQ");

		TF1 *FitFunctionx=vxHisto->GetFunction("pol1");
		TF1 *FitFunctiony=vyHisto->GetFunction("pol1");
		TH2D *PredictedPosx= new TH2D("","",160,(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],160,-10,120);
		TH2D *PredictedPosy= new TH2D("","",160,(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],160,-10,120);

		if(!vNCluster[ChamberID]) {
			PredictedPosx->SetMarkerColor(2);
		}else PredictedPosx->SetMarkerColor(3);

		PredictedPosx->SetMarkerSize(0.8);
		PredictedPosx->SetMarkerStyle(21);

		if(!vNCluster[ChamberID]) {
			PredictedPosy->SetMarkerColor(2);
		}else {
			PredictedPosy->SetMarkerColor(3);
		}
		PredictedPosy->SetMarkerSize(0.8);
		PredictedPosy->SetMarkerStyle(21);

		// chech whether this is a good fit
		if((FitFunctionx->GetChisquare()<=ChiSquareTHRD)&&(FitFunctiony->GetChisquare()<=ChiSquareTHRD)){   // make sure this a good fit

			vChiSquareFlag[ChamberID]=1;  // to be inprove

			//generated all the predicted position on each chamber
			vPredictedPosX[ChamberID]=(kZStartModule[ChamberID]-FitFunctionx->GetParameter(0))/(FitFunctionx->GetParameter(1));
			vPredictedPosY[ChamberID]=(kZStartModule[ChamberID]-FitFunctiony->GetParameter(0))/(FitFunctiony->GetParameter(1));
			vPredictedPosZ[ChamberID]=kZStartModule[ChamberID];

			//fill the histo data
			PredictedPosx->Fill(vPredictedPosX[ChamberID],vPredictedPosZ[ChamberID]);
			PredictedPosy->Fill(vPredictedPosY[ChamberID],vPredictedPosZ[ChamberID]);

			// make sure the projected point is located in  the chamber
			if ((vPredictedPosX[ChamberID]> (-1.0) * kNbXAPVModule[ChamberID]*128/2*kStripPitchX[ChamberID])
				&& (vPredictedPosX[ChamberID]< kNbXAPVModule[ChamberID] * 128 / 2* kStripPitchX[ChamberID])
				&& (vPredictedPosY[ChamberID]> (-1.0) * kNbYAPVModule[ChamberID] * 128 / 2* kStripPitchY[ChamberID])
				&& (vPredictedPosY[ChamberID]< kNbYAPVModule[ChamberID] * 128 / 2* kStripPitchY[ChamberID]))
				{
					vWithInRange[ChamberID]=1;
					vGoodTrackingFlag[ChamberID]=1;     // this is a good hit
					//cout<<"[G] good tracking";
					if(vNCluster[ChamberID]) {
						vEventDetected[ChamberID]=1;
					}
					else{
						vEventDetected[ChamberID]=0;
					}
				}else {
					vGoodTrackingFlag[ChamberID]=0;     // this is a good hit
			}
		}
		else{  // if this is not a good fit, this is not a good tracking
			vGoodTrackingFlag[ChamberID]=0;
			vEventDetected[ChamberID]=0;
		}
		// display if needed
		if (Display_flag) {
			TCanvas *canvas = new TCanvas("CANVAS", "alala", 900, 900);
			canvas->Divide(2, 2);
			canvas->cd(1);
			vxHisto->Draw("");
			//vxCorrHisto->Draw("same");
			// detector refence lines
			for (int i = 0; i < kNMODULE; i++) {
				detectorX[i]->SetLineColor(30);
				detectorX[i]->Draw("same");
			}
			PredictedPosx->Draw("same");
			// y dimensions
			canvas->cd(2);
			vyHisto->Draw("");
			//vyCorrHisto->Draw("same");

			for (int i = 0; i < kNMODULE; i++) {
				detectorY[i]->SetLineColor(30);
				detectorY[i]->Draw("same");
			}
			PredictedPosy->Draw("same");
			//  draw the three dimension graph
			canvas->cd(3);
			vzHisto->Draw();
			//vzCorrHisto->Draw("same");
			canvas->cd(4);
			histo_3d->Draw();
			canvas->Modified();
			canvas->Update();
			canvas->Draw();
			getchar();
			delete canvas;
		}

		delete PredictedPosx;
		delete PredictedPosy;
		delete vxHisto;
		delete vyHisto;
		delete vzHisto;
		delete histo_3d;

	}else vGoodTrackingFlag[ChamberID]=0;
}

void GEMTrackConstrcution::TestFunctions() {

	printf("\n\n\n\n***************\n");
	TLine * detector[kNMODULE];
	TH2D *vxHisto=new TH2D("x-z plane","x-z plane",160,(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],160,-10,120);
	vxHisto->GetXaxis()->SetTitle("x dimension");
	vxHisto->GetYaxis()->SetTitle("z dimension");
	vxHisto->SetMarkerSize(0.5);
	vxHisto->SetMarkerStyle(21);
	vxHisto->SetMarkerColor(4);
	//vxHisto->GetYaxis()->SetRangeUser(-10,60);
	TH2D *vyHisto=new TH2D("y-z plane","y-z plane",160,(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],160,-10,120);
	vyHisto->GetXaxis()->SetTitle("y dimension");
	vyHisto->GetYaxis()->SetTitle("z dimension");
	vyHisto->SetMarkerSize(0.6);
	vyHisto->SetMarkerStyle(21);
	vyHisto->SetMarkerColor(4);
	//vyHisto->GetYaxis()->SetRangeUser(-10,60);
	TH3D *histo_3d=new TH3D("GEM 3D tracking","GEM 3D tracking",160,-80,80,160,-80,80,160,-80,80);
	histo_3d->SetMarkerSize(0.6);
	histo_3d->SetMarkerStyle(21);
	histo_3d->SetMarkerColor(4);

	for(int i =0; i < kNMODULE;i++){
		//printf("\n\n Detector: %d\n",i);
		for(unsigned int j=0;j<vNCluster[i];j++ ){
			printf("DetectorID=%d\n Number of Cluster   = %d, cluster=%5d, x=%5d  @%4d, y=%5d  @%4d, z=%5d\n ",i,vNCluster[i],j,(int)vxCluster[i][j].Pos,(int)vxCluster[i][j].StartTime,(int)vyCluster[i][j].Pos,(int)vyCluster[i][j].StartTime,(int)kZStartModule[i]);
			vxHisto->Fill(vxCluster[i][j].Pos,kZStartModule[i]);
			//vxHisto->Fill(0.0,0.0);
			vyHisto->Fill(vyCluster[i][j].Pos,kZStartModule[i]);
			histo_3d->Fill(vxCluster[i][j].Pos,vyCluster[i][j].Pos,kZStartModule[i]);

		}
		detector[i]=new TLine(-80,kZStartModule[i],80,kZStartModule[i]);
	}
	TCanvas *canvas= new TCanvas("CANVAS","alala",800,1100);
	canvas->Divide(1,3);
	canvas->cd(1);
	vxHisto->Draw("");
	for(int i =0;i<kNMODULE;i++){
		detector[i]->SetLineColor(30);
		detector[i]->Draw("same");
	}
	vxHisto->Fit("pol1");
	canvas->cd(2);
	vyHisto->Draw("");
	for (int i = 0; i < kNMODULE; i++) {
		detector[i]->SetLineColor(30);
		detector[i]->Draw("same");
	}
	vyHisto->Fit("pol1");
	canvas->cd(3);
	histo_3d->Fit("");
	histo_3d->Draw();
	histo_3d->Fit("pol3");
	canvas->Modified();
	canvas->Update();
	canvas->Draw();
	getchar();
	delete vxHisto;
	delete vyHisto;
	delete canvas;
}

void GEMTrackConstrcution::ResultReport() {
	for (int chamberid = 0; chamberid < kNMODULE; ++ chamberid) {
	if (vGoodTrackingFlag[chamberid]) {
			printf("\n\n^_^.......^_^ GOOD EVENT  ^_^.......^_^\n");
			for (int i = 0; i < kNMODULE; ++i) {
				if (vEventDetected[i]) {
					printf(
							"=> \" o \"  on chamber %d , event detected, match the prediction\n",
							i);
				} else {
					printf("=> \" x \"  on chamber %d , No event Detected   \n",
							i);
				}
			}
		} else {
			printf("\n\n==> This is NOT a good Event    @Chamber %d\n\n",chamberid);
		}
	}
}

// used for Set the display flag
Bool_t GEMTrackConstrcution::SetDisplay(Bool_t Enable_flag) {
	return Display_flag=Enable_flag;
}

GEMTrackConstrcution::~GEMTrackConstrcution() {
	// TODO Auto-generated destructor stub
}

