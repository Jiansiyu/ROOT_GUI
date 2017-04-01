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
	SingleTrackFilter();
/*
	if (Detector_ID >= 0) {
		SingleTrackFilter(Detector_ID);
	} else {
		for (int i = 0; i < kNMODULE; i++) {
			SingleTrackFilter(i);
		}
	}
*/
	//ResultReport();
}


void GEMTrackConstrcution::Clean() {
	vGEMCluster.clear();
	for(unsigned int i=0; i<kNMODULE;i++){
		vNCluster[i]=0;
		vxCluster[i].clear();
		vyCluster[i].clear();
		vChiSquareFlag[i]=0;
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
void GEMTrackConstrcution::Init() {
	for(unsigned int i =0; i <kNMODULE; i++) {
		vOriginalPosX[i]=0;
		vOriginalPosY[i]=0;
		vOriginalPosZ[i]=0;

		vCorrectedPosX[i]=0;
		vCorrectedPosY[i]=0;
		vCorrectedPosZ[i]=0;
	}
}



void GEMTrackConstrcution::SingleTrackFilter() {
	Init();
	// make sure all the detector have no more than one event, in this case only select the single tracking event
	Bool_t lSingleTrackFlag=1;
	unsigned lNEventAllChamber=0;
	for(unsigned int i = 0; i<kNMODULE; i++){
		if(vNCluster[i]<=1){
			lNEventAllChamber+=vNCluster[i];
		}else{
			lSingleTrackFlag=0; //
		}
	}
	//if at least 3 chamber are fired, we consider this is a effective tracking
	if((lNEventAllChamber>=NFIREDCHAMBERS)&&lSingleTrackFlag){
		//Initialize the histograms
		TH2D *vxzHisto=new TH2D("x-z plane","x-z plane",160,(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],160,-10,120);
		vxzHisto->GetXaxis()->SetTitle("x dimension");
		vxzHisto->GetYaxis()->SetTitle("z dimension");
		vxzHisto->SetMarkerSize(0.5);
		vxzHisto->SetMarkerStyle(21);
		vxzHisto->SetMarkerColor(4);

		TH2D *vyzHisto=new TH2D("y-z plane","y-z plane",160,(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],160,-10,120);
		vyzHisto->GetXaxis()->SetTitle("y dimension");
		vyzHisto->GetYaxis()->SetTitle("z dimension");
		vyzHisto->SetMarkerSize(0.6);
		vyzHisto->SetMarkerStyle(21);
		vyzHisto->SetMarkerColor(4);

		TH2D *vxyHisto= new TH2D("x-y plane","x-y plane",160,(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0]-20,(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0]+20,160,(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0]);
		vxyHisto->GetXaxis()->SetTitle("x dimension");
		vxyHisto->GetYaxis()->SetTitle("y dimension");
		vxyHisto->SetMarkerSize(0.6);
		vxyHisto->SetMarkerStyle(21);
		vxyHisto->SetMarkerColor(4);

		// inverse the histogram and apply fit, so as to improve the fit result when the slop is t
		TH2D *vzxHisto=new TH2D("z-x plane","z-x plane",160,-10,120,160,(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0]);
		vzxHisto->GetXaxis()->SetTitle("z dimension");
		vzxHisto->GetYaxis()->SetTitle("x dimension");
		vzxHisto->SetMarkerSize(0.5);
		vzxHisto->SetMarkerStyle(21);
		vzxHisto->SetMarkerColor(4);

		TH2D *vzyHisto=new TH2D("z-y plane","z-y plane",160,-10,120,160,(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0]);
		vzyHisto->GetXaxis()->SetTitle("z dimension");
		vzyHisto->GetYaxis()->SetTitle("y dimension");
		vzyHisto->SetMarkerSize(0.6);
		vzyHisto->SetMarkerStyle(21);
		vzyHisto->SetMarkerColor(4);

		TH2D *vyxHisto= new TH2D("y-x plane","y-x plane",160,(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],160,(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0]-20,(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0]+20);
		vyxHisto->GetXaxis()->SetTitle("y dimension");
		vyxHisto->GetYaxis()->SetTitle("x dimension");
		vyxHisto->SetMarkerSize(0.6);
		vyxHisto->SetMarkerStyle(21);
		vyxHisto->SetMarkerColor(4);

		// 3-D tracking
		TH3D *histo_3d = new TH3D("GEM 3D tracking", "GEM 3D tracking", 160,-80, 80, 160, -80, 80, 160, -80, 80);
		histo_3d->SetMarkerSize(0.6);
		histo_3d->SetMarkerStyle(21);
		histo_3d->SetMarkerColor(4);

		//++++++++++++++++++++++++++++++++++++++++++++
		// load the original data into the histogram
		// if need to change to the corrected position change from the fowlling code
		//++++++++++++++++++++++++++++++++++++++++++

		for (int i = 0; i < kNMODULE; i++) {
			for (unsigned int j = 0; j < vNCluster[i]; j++) {
				// load the inital data into to the orignal data buffer, used for calculate the residue
				vOriginalPosX[i]=vxCluster[i][j].Pos;
				vOriginalPosY[i]=vyCluster[i][j].Pos;
				vOriginalPosZ[i]=kZStartModule[i];
/*
				vxzHisto->Fill(vxCluster[i][j].Pos, kZStartModule[i]);
				vyzHisto->Fill(vyCluster[i][j].Pos, kZStartModule[i]);
				vxyHisto->Fill(vxCluster[i][j].Pos, vyCluster[i][j].Pos);
				histo_3d->Fill(vxCluster[i][j].Pos, vyCluster[i][j].Pos,kZStartModule[i]);

				// the inverse histogram
				vzxHisto->Fill(kZStartModule[i],vxCluster[i][j].Pos);
				vzyHisto->Fill(kZStartModule[i],vyCluster[i][j].Pos);
				vyxHisto->Fill(vyCluster[i][j].Pos,vxCluster[i][j].Pos);
*/
			}
		}
		for(int i =0; i<kNMODULE; i ++){
			vCorrectedPosX[i]=DistortionMx[i][0]*vOriginalPosX[i]+DistortionMx[i][1]*vOriginalPosY[i]+DistortionMx[i][2]*vOriginalPosZ[i]+DistortionMx[i][3];
			vCorrectedPosY[i]=DistortionMx[i][4]*vOriginalPosX[i]+DistortionMx[i][5]*vOriginalPosY[i]+DistortionMx[i][6]*vOriginalPosZ[i]+DistortionMx[i][7];
			vCorrectedPosZ[i]=DistortionMx[i][8]*vOriginalPosX[i]+DistortionMx[i][9]*vOriginalPosY[i]+DistortionMx[i][10]*vOriginalPosZ[i]+DistortionMx[i][11];
		}


		for (int i = 0; i < kNMODULE; i++) {
			if (vNCluster[i]) {
				vxzHisto->Fill(vCorrectedPosX[i], vCorrectedPosZ[i]);
				vyzHisto->Fill(vCorrectedPosY[i], vCorrectedPosZ[i]);
				vxyHisto->Fill(vCorrectedPosX[i], vCorrectedPosY[i]);
				histo_3d->Fill(vCorrectedPosX[i], vCorrectedPosY[i] , vCorrectedPosZ[i]);

				// the inverse histogram
				vzxHisto->Fill(vCorrectedPosZ[i], vCorrectedPosX[i]);
				vzyHisto->Fill(vCorrectedPosZ[i], vCorrectedPosY[i]);
				vyxHisto->Fill(vCorrectedPosX[i], vCorrectedPosY[i]);
			}
		}


		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//calculate the correlation coefficient
		// 1. calculate the average
		Double_t lAverageX=0;
		Double_t lAverageY=0;
		double_t lAverageZ=0;
		unsigned int lCounter=0;
		printf("\n\n[%s]  Position ", __FUNCTION__);
		for(int i = 0; i <kNMODULE; i ++) {
			if( vNCluster[i]){
				lAverageX+=vCorrectedPosX[i];//vxCluster[i][j].Pos;
				lAverageY+=vCorrectedPosY[i];//vyCluster[i][j].Pos;
				lAverageZ+=vCorrectedPosZ[i];//kZStartModule[i];
				lCounter++;
				printf(" [%d ==>0]  (%5.5f, %5.5f, %5.5f)  ",i,vCorrectedPosX[i], vCorrectedPosY[i], vCorrectedPosZ[i]);
			}
		}
		printf("\n\n");
		lAverageX=lAverageX/lCounter;
		lAverageY=lAverageY/lCounter;
		lAverageZ=lAverageZ/lCounter;

		double_t lCorreNumXZ=0;
		double_t lCorreNumYZ=0;
		double_t lCorreNumXY=0;
		double_t lCorreDomx2 = 0;
		double_t lCorreDomy2 = 0;
		double_t lCorreDomz2 = 0;

		for(int i = 0; i<kNMODULE ; i++) {
			if( vNCluster[i]){
				lCorreNumXY +=(lAverageX-vCorrectedPosX[i])*(lAverageY-vCorrectedPosY[i]);
				lCorreNumXZ +=(lAverageX-vCorrectedPosX[i])*(lAverageZ-vCorrectedPosZ[i]);
				lCorreNumYZ +=(lAverageY-vCorrectedPosY[i])*(lAverageZ-vCorrectedPosZ[i]);

				lCorreDomx2 +=(lAverageX-vCorrectedPosX[i])*(lAverageX-vCorrectedPosX[i]);
				lCorreDomy2 +=(lAverageY-vCorrectedPosY[i])*(lAverageY-vCorrectedPosY[i]);
				lCorreDomz2 +=(lAverageZ-vCorrectedPosZ[i])*(lAverageZ-vCorrectedPosZ[i]);
			}
		}

		double_t lCorrelationXY=lCorreNumXY/(sqrt(lCorreDomx2)*sqrt(lCorreDomy2));
		double_t lCorrelationXZ=lCorreNumXZ/(sqrt(lCorreDomx2)*sqrt(lCorreDomz2));
		double_t lCorrelationYZ=lCorreNumYZ/(sqrt(lCorreDomy2)*sqrt(lCorreDomz2));

		// loading the correlation eff
		vCorrelationEffXY=fabs(lCorrelationXY);
		vCorrelationEffXZ=fabs(lCorrelationXZ);
		vCorrelationEffYZ=fabs(lCorrelationYZ);

		//if((fabs(lCorrelationXZ)+fabs(lCorrelationYZ))>1.8) vCorrelationEfficiencyFlag=1;
		printf("[%s]    Correlation   xz=%5.5f;  yz=%5.5f;   xy=%5.5f; \n ",__FUNCTION__,fabs(lCorrelationXZ),fabs(lCorrelationYZ), fabs(lCorrelationXY) );
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// fit the lines in xz, yz and zx, zy 2d histograms, because when the angle is larger than 45, the fit line is not good, here switch the x and y dimension
		// to solve this problems
		// here us the chi square and the coefficiency to make sure this is a good fit, sometimes when the line is vertical, the fit result is terrible,
		// with this double and coefficency check, hope to solve this issues
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// start fit 2D graph
		vxzHisto->Fit("pol1","WMQE");
		vzxHisto->Fit("pol1","WMQE");
		vyzHisto->Fit("pol1","WMQE");
		vzyHisto->Fit("pol1","WMQE");

		TF1 *lFitFunctionXZ=vxzHisto->GetFunction("pol1");
		TF1 *lFitFunctionZX=vzxHisto->GetFunction("pol1");
		TF1 *lFitFunctionYZ=vyzHisto->GetFunction("pol1");
		TF1 *lFitFunctionZY=vzyHisto->GetFunction("pol1");

/*		for(unsigned int i =0; i<kNMODULE ; i++) {
			for(unsigned int j = 0 ; j<16;j++){
				cout<< DistortionMx[i][j]<<", ";
				if((j+1)%4==0) cout<<endl;
			}
			cout<<endl;
		}*/

		// select the good tracking
		double_t lChiSquareXZ = lFitFunctionXZ->GetChisquare() > lFitFunctionZX->GetChisquare() ? lFitFunctionZX->GetChisquare():lFitFunctionXZ->GetChisquare();
		double_t lChiSquareYZ = lFitFunctionYZ->GetChisquare() > lFitFunctionZY->GetChisquare() ? lFitFunctionZY->GetChisquare():lFitFunctionYZ->GetChisquare();
		printf("%f,   %f, \n\n\n ",lChiSquareXZ, lChiSquareYZ);
		double_t lChiSquareTHRD=DETECTOR_RESOLUTION_SIGMA*DETECTOR_RESOLUTION_SIGMA*10*lNEventAllChamber/3;
		if((lChiSquareXZ<lChiSquareTHRD)&&(lChiSquareYZ<lChiSquareTHRD)) {

			//+++++++++++++++++++++++++++++++++
			// generate the predicted position for each chamber
			if(lFitFunctionXZ->GetChisquare() < lFitFunctionZX->GetChisquare()){

			}else{

			}
			if(lFitFunctionYZ->GetChisquare() < lFitFunctionZY->GetChisquare()){

			}else {

			}
			//++++++++++++++++++++++++++++++++

			// set the flags and write the positions



		}

		if (Display_flag) {

			// the reference lines that used for label the detector Z position in the  histo z direction
			TLine * detectorX[kNMODULE];
			TLine * detectorY[kNMODULE];

			for (int i = 0; i < kNMODULE; i++) {
				detectorX[i] = new TLine((-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0], kZStartModule[i], (1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],kZStartModule[i]);
				detectorY[i] = new TLine((-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0], kZStartModule[i], (1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],kZStartModule[i]);
			}

			TCanvas *canvas = new TCanvas("CANVAS", "alala", 900, 900);
			canvas->Divide(3, 2);

			// x-z histo
			canvas->cd(1);
			vxzHisto->Draw("");
			for (int i = 0; i < kNMODULE; i++) {
				detectorX[i]->SetLineColor(30);
				detectorX[i]->Draw("same");
			}
			//tPredictedPosx->Draw("same");

			// y-z histo
			canvas->cd(2);
			vyzHisto->Draw("");
			for (int i = 0; i < kNMODULE; i++) {
				detectorY[i]->SetLineColor(30);
				detectorY[i]->Draw("same");
			}
			//tPredictedPosy->Draw("same");

			//  x-y histo
			canvas->cd(3);
			vxyHisto->Fit("pol1","FMQ");
			vxyHisto->Draw();

			canvas->cd(4);
			vzxHisto->Draw();

			canvas->cd(5);
			vzyHisto->Draw();

			canvas->cd(6);
			vyxHisto->Fit("pol1","WMQE");
			vyxHisto->Draw();

			canvas->Modified();
			canvas->Update();
			canvas->Draw();
			//if(vCorrelationEfficiencyFlag)canvas->SaveAs(Form("%d_%f.jpg",ChamberID,temp_lalal));
			getchar();
			delete canvas;
			for (int i = 0; i < kNMODULE; i++) {
				delete detectorX[i];
				delete detectorY[i];
			}
		}

	}


}
void GEMTrackConstrcution::SingleTrackFilter(unsigned int ChamberID) {

	// Initialize the variables
	for (unsigned int i = 0; i < kNMODULE; i++) {
		vChiSquareFlag[i]=0;
	}


	// check all the rest chamber have effective signals
	Bool_t SingleTrack_Flag = 1;
	for (unsigned int i = 0; i < kNMODULE; i++) {
		if ((i != ChamberID)) {
			if (vNCluster[i] != 1)    //make sure all the rest chamber have and only have one effective signals
				SingleTrack_Flag = 0;
		}
	}

	if (SingleTrack_Flag) {

		//Initialize the histograms

		TH2D *vxzHisto=new TH2D("x-z plane","x-z plane",160,(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],160,-10,120);
		vxzHisto->GetXaxis()->SetTitle("x dimension");
		vxzHisto->GetYaxis()->SetTitle("z dimension");
		vxzHisto->SetMarkerSize(0.5);
		vxzHisto->SetMarkerStyle(21);
		vxzHisto->SetMarkerColor(4);

		TH2D *vyzHisto=new TH2D("y-z plane","y-z plane",160,(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],160,-10,120);
		vyzHisto->GetXaxis()->SetTitle("y dimension");
		vyzHisto->GetYaxis()->SetTitle("z dimension");
		vyzHisto->SetMarkerSize(0.6);
		vyzHisto->SetMarkerStyle(21);
		vyzHisto->SetMarkerColor(4);

		TH2D *vxyHisto= new TH2D("x-y plane","x-y plane",160,(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0]-20,(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0]+20,160,(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0]);
		vxyHisto->GetXaxis()->SetTitle("x dimension");
		vxyHisto->GetYaxis()->SetTitle("y dimension");
		vxyHisto->SetMarkerSize(0.6);
		vxyHisto->SetMarkerStyle(21);
		vxyHisto->SetMarkerColor(4);


		// inverse the histogram and apply fit, so as to improve the fit result when the slop is t
		TH2D *vzxHisto=new TH2D("z-x plane","z-x plane",160,-10,120,160,(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0]);
		vzxHisto->GetXaxis()->SetTitle("z dimension");
		vzxHisto->GetYaxis()->SetTitle("x dimension");
		vzxHisto->SetMarkerSize(0.5);
		vzxHisto->SetMarkerStyle(21);
		vzxHisto->SetMarkerColor(4);

		TH2D *vzyHisto=new TH2D("z-y plane","z-y plane",160,-10,120,160,(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0]);
		vzyHisto->GetXaxis()->SetTitle("z dimension");
		vzyHisto->GetYaxis()->SetTitle("y dimension");
		vzyHisto->SetMarkerSize(0.6);
		vzyHisto->SetMarkerStyle(21);
		vzyHisto->SetMarkerColor(4);

		TH2D *vyxHisto= new TH2D("y-x plane","y-x plane",160,(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],160,(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0]-20,(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0]+20);
		vyxHisto->GetXaxis()->SetTitle("y dimension");
		vyxHisto->GetYaxis()->SetTitle("x dimension");
		vyxHisto->SetMarkerSize(0.6);
		vyxHisto->SetMarkerStyle(21);
		vyxHisto->SetMarkerColor(4);


		// 3-D tracking
		TH3D *histo_3d = new TH3D("GEM 3D tracking", "GEM 3D tracking", 160,-80, 80, 160, -80, 80, 160, -80, 80);
		histo_3d->SetMarkerSize(0.6);
		histo_3d->SetMarkerStyle(21);
		histo_3d->SetMarkerColor(4);


		//++++++++++++++++++++++++++++++++++++++++++++
		// load the original data into the histogram
		// if need to change to the corrected position change from the fowlling code
		//++++++++++++++++++++++++++++++++++++++++++
		for (int i = 0; i < kNMODULE; i++) {

			for (unsigned int j = 0; j < vNCluster[i]; j++) {

				// calculate the corrected position for the detector
				vCorrectedPosX[i]=DistortionMx[i][0]*vxCluster[i][j].Pos+DistortionMx[i][1]*vyCluster[i][j].Pos+DistortionMx[i][1]*kZStartModule[i];
				vCorrectedPosY[i]=DistortionMx[i][4]*vxCluster[i][j].Pos+DistortionMx[i][5]*vyCluster[i][j].Pos+DistortionMx[i][6]*kZStartModule[i];
				vCorrectedPosZ[i]=DistortionMx[i][8]*vxCluster[i][j].Pos+DistortionMx[i][9]*vyCluster[i][j].Pos+DistortionMx[i][10]*kZStartModule[i];

				// load the inital data into to the orignal data buffer, used for calculate the residue
				vOriginalPosX[i]=vxCluster[i][j].Pos;
				vOriginalPosY[i]=vyCluster[i][j].Pos;
				vOriginalPosZ[i]=kZStartModule[i];


				vxzHisto->Fill(vxCluster[i][j].Pos, kZStartModule[i]);
				vyzHisto->Fill(vyCluster[i][j].Pos, kZStartModule[i]);
				vxyHisto->Fill(vxCluster[i][j].Pos, vyCluster[i][j].Pos);
				histo_3d->Fill(vxCluster[i][j].Pos, vyCluster[i][j].Pos,kZStartModule[i]);

				// the inverse histogram
				vzxHisto->Fill(kZStartModule[i],vxCluster[i][j].Pos);
				vzyHisto->Fill(kZStartModule[i],vyCluster[i][j].Pos);
				vyxHisto->Fill(vyCluster[i][j].Pos,vxCluster[i][j].Pos);

			}
		}

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//calculate the correlation coefficient
		// 1. calculate the average
		Double_t lAverageX=0;
		Double_t lAverageY=0;
		double_t lAverageZ=0;

		unsigned int lCounter=0;

		printf("\n\n[%s]  Position ", __FUNCTION__);
		for(int i = 0; i <kNMODULE; i ++) {
			for(unsigned int j = 0; j <vNCluster[i]; j ++) {
				lAverageX+=vxCluster[i][j].Pos;
				lAverageY+=vyCluster[i][j].Pos;
				lAverageZ+=kZStartModule[i];
				lCounter++;

				printf(" [%d ==>%d]  (%5.5f, %5.5f, %5.5f)  ",i,j, vxCluster[i][j].Pos, vyCluster[i][j].Pos, kZStartModule[i] );
			}
		}
		printf("\n\n");
		lAverageX=lAverageX/lCounter;
		lAverageY=lAverageY/lCounter;
		lAverageZ=lAverageZ/lCounter;

		//
		double_t lCorreNumXZ=0;
		double_t lCorreNumYZ=0;
		double_t lCorreNumXY=0;

		double_t lCorreDomx2 = 0;
		double_t lCorreDomy2 = 0;
		double_t lCorreDomz2 = 0;

		for(int i = 0; i<kNMODULE ; i++) {
			for(unsigned int j = 0 ; j<vNCluster[i]; j++) {
				lCorreNumXY +=(lAverageX-vxCluster[i][j].Pos)*(lAverageY-vyCluster[i][j].Pos);
				lCorreNumXZ +=(lAverageX-vxCluster[i][j].Pos)*(lAverageZ-kZStartModule[i]);
				lCorreNumYZ +=(lAverageY-vyCluster[i][j].Pos)*(lAverageZ-kZStartModule[i]);

				lCorreDomx2 +=(lAverageX-vxCluster[i][j].Pos)*(lAverageX-vxCluster[i][j].Pos);
				lCorreDomy2 +=(lAverageY-vyCluster[i][j].Pos)*(lAverageY-vyCluster[i][j].Pos);
				lCorreDomz2 +=(lAverageZ-kZStartModule[i])*(lAverageZ-kZStartModule[i]);
			}
		}

		double_t lCorrelationXY=lCorreNumXY/(sqrt(lCorreDomx2)*sqrt(lCorreDomy2));
		double_t lCorrelationXZ=lCorreNumXZ/(sqrt(lCorreDomx2)*sqrt(lCorreDomz2));
		double_t lCorrelationYZ=lCorreNumYZ/(sqrt(lCorreDomy2)*sqrt(lCorreDomz2));
		if((fabs(lCorrelationXZ)+fabs(lCorrelationYZ))>1.8) vCorrelationEfficiencyFlag=1;
		printf("[%s]    Correlation   xz=%5.5f;  yz=%5.5f;   xy=%5.5f; \n ",__FUNCTION__,fabs(lCorrelationXZ),fabs(lCorrelationYZ), fabs(lCorrelationXY) );
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


		Double_t FitRangeMinX=(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0];
		Double_t FitRangeMaxX=(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0];
		Double_t FitRangeMinY=(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0];
		Double_t FitRangeMaxY=(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0];

		vxzHisto->Fit("pol1","WMQE","",FitRangeMinX-10,FitRangeMaxX+10);
		vyzHisto->Fit("pol1","WMQE","",FitRangeMinY-10,FitRangeMaxY+10);

		//vzxHisto->Fit("pol1","WMQE");

		TH2D *tPredictedPosx= new TH2D("","",160,(-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],(1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],160,-10,120);
		TH2D *tPredictedPosy= new TH2D("","",160,(-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],(1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],160,-10,120);


		if(!vNCluster[ChamberID]) {
			tPredictedPosx->SetMarkerColor(2);
		}else tPredictedPosx->SetMarkerColor(3);

		tPredictedPosx->SetMarkerSize(0.8);
		tPredictedPosx->SetMarkerStyle(21);

		if(!vNCluster[ChamberID]) {
			tPredictedPosy->SetMarkerColor(2);
		}else {
			tPredictedPosy->SetMarkerColor(3);
		}
		tPredictedPosy->SetMarkerSize(0.8);
		tPredictedPosy->SetMarkerStyle(21);

		//get the fit function and check whether this is a good fit
		TF1 *FitFunctionx=vxzHisto->GetFunction("pol1");
		TF1 *FitFunctiony=vyzHisto->GetFunction("pol1");
		//vxHisto->Fit(FitFunctionx,"F");
		//vyHisto->Fit(FitFunctiony,"F");
		if((FitFunctionx->GetChisquare()<=ChiSquareTHRD)&&(FitFunctiony->GetChisquare()<=ChiSquareTHRD)){   // make sure this a good fit

			//generated all the predicted position on each chamber
			for(int i = 0 ; i <kNMODULE; i++){
				vChiSquareFlag[i]=1;  // to be improve

				// generate the predicted data for all the chambers
				vPredictedPosX[i]=(kZStartModule[i]-FitFunctionx->GetParameter(0))/(FitFunctionx->GetParameter(1));
				vPredictedPosY[i]=(kZStartModule[i]-FitFunctiony->GetParameter(0))/(FitFunctiony->GetParameter(1));
				vPredictedPosZ[i]=kZStartModule[i];

				//fill the histo data
//				tPredictedPosx->Fill(vPredictedPosX[i],vPredictedPosZ[i]);
//				tPredictedPosy->Fill(vPredictedPosY[i],vPredictedPosZ[i]);
				/*if(vNCluster[i]){
				printf("[%s] Detector[%d] Predicted Data x=>%5.5f  y= %5.5f, realData x=%5.5f y=%5.5f Difference Dx=>%5.5f Dy=%5.5f\n",__FUNCTION__,i,

										vPredictedPosX[i],
										vPredictedPosY[i],

										vOriginalPosX[i],
										vOriginalPosY[i],
										vPredictedPosX[i]-vOriginalPosX[i],
										vPredictedPosY[i]-vOriginalPosY[i]
								);
				}*/
			}

			tPredictedPosx->Fill(vPredictedPosX[ChamberID],vPredictedPosZ[ChamberID]);
			tPredictedPosy->Fill(vPredictedPosY[ChamberID],vPredictedPosZ[ChamberID]);

			// make sure the projected point is located in  the chamber
			double_t  tRange_MaxX=kNbXAPVModule[ChamberID] * 128 / 2* kStripPitchX[ChamberID];
			double_t  tRange_MinX=(-1.0) * kNbXAPVModule[ChamberID]*128/2*kStripPitchX[ChamberID];
			double_t  tRange_MaxY=kNbYAPVModule[ChamberID] * 128 / 2* kStripPitchY[ChamberID];
			Double_t  tRange_MinY=(-1.0) * kNbYAPVModule[ChamberID] * 128 / 2* kStripPitchY[ChamberID];
		/*	if ((vPredictedPosX[ChamberID]> (-1.0) * kNbXAPVModule[ChamberID]*128/2*kStripPitchX[ChamberID])
				&& (vPredictedPosX[ChamberID]< kNbXAPVModule[ChamberID] * 128 / 2* kStripPitchX[ChamberID])
				&& (vPredictedPosY[ChamberID]> (-1.0) * kNbYAPVModule[ChamberID] * 128 / 2* kStripPitchY[ChamberID])
				&& (vPredictedPosY[ChamberID]< kNbYAPVModule[ChamberID] * 128 / 2* kStripPitchY[ChamberID]))
*/
			if ((vPredictedPosX[ChamberID]> tRange_MinX)&&(vPredictedPosX[ChamberID]<tRange_MaxX)&& (vPredictedPosY[ChamberID]>tRange_MinY)&& (vPredictedPosY[ChamberID]<tRange_MaxY))
			{
				//vWithInRange[ChamberID] = 1;
				vGoodTrackingFlag[ChamberID] = 1;     // this is a good hit
				if (vNCluster[ChamberID]) {
					vEventDetected[ChamberID] = 1;
				} else {
					vEventDetected[ChamberID] = 0;
				}
			}
			else
			{
					vGoodTrackingFlag[ChamberID]=0;
			}
		} else {
			// if the chisquare is too large, this is not a good fit
			vGoodTrackingFlag[ChamberID] = 0;
			//vEventDetected[ChamberID] = 0;
		}


		// debug mode
		// display if needed, this is used for debug
		if (Display_flag) {

			// the reference lines that used for label the detector Z position in the  histo z direction
			TLine * detectorX[kNMODULE];
			TLine * detectorY[kNMODULE];

			for (int i = 0; i < kNMODULE; i++) {
				detectorX[i] = new TLine((-1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0], kZStartModule[i], (1.0)* kNbXAPVModule[0]*128/2*kStripPitchX[0],kZStartModule[i]);
				detectorY[i] = new TLine((-1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0], kZStartModule[i], (1.0)* kNbYAPVModule[0]*128/2*kStripPitchY[0],kZStartModule[i]);
			}

			TCanvas *canvas = new TCanvas("CANVAS", "alala", 900, 900);
			canvas->Divide(3, 2);

			// x-z histo
			canvas->cd(1);
			vxzHisto->Draw("");
			for (int i = 0; i < kNMODULE; i++) {
				detectorX[i]->SetLineColor(30);
				detectorX[i]->Draw("same");
			}
			tPredictedPosx->Draw("same");

			// y-z histo
			canvas->cd(2);
			vyzHisto->Draw("");
			for (int i = 0; i < kNMODULE; i++) {
				detectorY[i]->SetLineColor(30);
				detectorY[i]->Draw("same");
			}
			tPredictedPosy->Draw("same");

			//  x-y histo
			canvas->cd(3);
			vxyHisto->Fit("pol1","FMQ");
			vxyHisto->Draw();

			canvas->cd(4);
			vzxHisto->Fit("pol1","WMQE");
			vzxHisto->Draw();

			canvas->cd(5);
			vzyHisto->Fit("pol1","WMQE");
			vzyHisto->Draw();

			canvas->cd(6);
			vyxHisto->Fit("pol1","WMQE");
			vyxHisto->Draw();

			canvas->Modified();
			canvas->Update();
			canvas->Draw();
			//if(vCorrelationEfficiencyFlag)canvas->SaveAs(Form("%d_%f.jpg",ChamberID,temp_lalal));
			getchar();
			delete canvas;
			for (int i = 0; i < kNMODULE; i++) {
				delete detectorX[i];
				delete detectorY[i];
			}
		}

		delete tPredictedPosx;
		delete tPredictedPosy;
		delete vxzHisto;
		delete vyzHisto;
		delete vxyHisto;

		delete vzxHisto;
		delete vzyHisto;
		delete vyxHisto;

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

