/*
 * GEMCalibration.cpp
 *
 *  Created on: Mar 5, 2017
 *      Author: newdriver
 */

#include "GEMCalibration.h"

// used for the test functions
#include "TCanvas.h"
#include "TH2D.h"
#include  "TH3D.h"
#include "TLine.h"
#include "TProfile.h"

GEMCalibration::GEMCalibration(std::vector<GEMCluster>Cluster_input) {
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
		vNCluster[detector_index]=(vxCluster[detector_index].size()<vyCluster[detector_index].size())? vxCluster[detector_index].size() : vyCluster[detector_index].size();
  }

	for(unsigned int i =0; i <kNMODULE; i ++){
		cDetectorRangeMaxX[i]=kNbXAPVModule[i] * 128 * kStripPitchX[i]/ 2;
		cDetectorRangeMaxY[i]=kNbYAPVModule[i] * 128 * kStripPitchY[i]/ 2;
		cDetectorRangeMinX[i]=(-1.0) * kNbXAPVModule[i] * 128 * kStripPitchX[i]/ 2;
		cDetectorRangeMinY[i]=(-1.0) * kNbYAPVModule[i] * 128 * kStripPitchY[i]/ 2;
	}
}


void GEMCalibration::CosmicCalibrate(){
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
	if((lNEventAllChamber>=NFIREDCHAMBERS_THR)&&lSingleTrackFlag){
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
			vOriginalPosZ[i]=kZStartModule[i];
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
		//printf("\n\n[%s]  Position ", __FUNCTION__);
		for(int i = 0; i <kNMODULE; i ++) {
			if( vNCluster[i]){
				lAverageX+=vCorrectedPosX[i];//vxCluster[i][j].Pos;
				lAverageY+=vCorrectedPosY[i];//vyCluster[i][j].Pos;
				lAverageZ+=vCorrectedPosZ[i];//kZStartModule[i];
				lCounter++;
				//printf(" [%d ==>0]  (%5.5f, %5.5f, %5.5f)  ",i,vCorrectedPosX[i], vCorrectedPosY[i], vCorrectedPosZ[i]);
			}
		}
		//printf("\n\n");
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

		// select the good tracking
		double_t lChiSquareXZ = lFitFunctionXZ->GetChisquare() > lFitFunctionZX->GetChisquare() ? lFitFunctionZX->GetChisquare():lFitFunctionXZ->GetChisquare();
		double_t lChiSquareYZ = lFitFunctionYZ->GetChisquare() > lFitFunctionZY->GetChisquare() ? lFitFunctionZY->GetChisquare():lFitFunctionYZ->GetChisquare();
		//printf("%f,   %f, \n\n\n ",lChiSquareXZ, lChiSquareYZ);
		double_t lChiSquareTHRD=DETECTOR_RESOLUTION_SIGMA*DETECTOR_RESOLUTION_SIGMA*10*lNEventAllChamber/3;
		if((lChiSquareXZ<lChiSquareTHRD)&&(lChiSquareYZ<lChiSquareTHRD)) {

			//+++++++++++++++++++++++++++++++++
			// generate the predicted position for each chamber
			if(lFitFunctionXZ->GetChisquare() < lFitFunctionZX->GetChisquare()){
				for(unsigned int i =0; i <kNMODULE; i ++){   // loop on all the chamber and generate the predicted position
					vPredictedPosX[i]=(vCorrectedPosZ[i]-lFitFunctionXZ->GetParameter(0))/(lFitFunctionXZ->GetParameter(1));
				}
			}else{
				for(unsigned int i =0; i <kNMODULE; i ++){
					vPredictedPosX[i]=lFitFunctionZX->GetParameter(0)+lFitFunctionZX->GetParameter(1)*vCorrectedPosZ[i];
				}
			}
			if(lFitFunctionYZ->GetChisquare() < lFitFunctionZY->GetChisquare()){

				for(unsigned int i =0; i<kNMODULE; i ++){
					vPredictedPosY[i]=(vCorrectedPosZ[i]-lFitFunctionYZ->GetParameter(0))/(lFitFunctionYZ->GetParameter(1));
				}
			}else {
				for(unsigned int i =0; i <kNMODULE; i ++) {
					vPredictedPosY[i]=lFitFunctionZY->GetParameter(0)+ lFitFunctionZY->GetParameter(1)*vCorrectedPosZ[i];
				}
			}

			for(unsigned int i =0; i <kNMODULE; i ++){
				vPredictedPosZ[i]=vCorrectedPosZ[i];
			}
			//++++++++++++++++++++++++++++++++

/*			//=====================================
			double_t error_XZ;
			double_t error_ZX;
			double_t error_YZ;
			double_t error_ZY;
			for(unsigned int i =0; i<kNMODULE; i ++){
				if(vNCluster[i]){
				vPredictedPosX[i]=(vCorrectedPosZ[i]-lFitFunctionXZ->GetParameter(0))/(lFitFunctionXZ->GetParameter(1));
				vPredictedPosY[i]=(vCorrectedPosZ[i]-lFitFunctionYZ->GetParameter(0))/(lFitFunctionYZ->GetParameter(1));
				error_XZ+=(vPredictedPosX[i]-vCorrectedPosX[i])*(vPredictedPosX[i]-vCorrectedPosX[i]);
				error_YZ+=(vPredictedPosY[i]-vCorrectedPosY[i])*(vPredictedPosY[i]-vCorrectedPosY[i]);
				printf("[%s] XZ and YZ error x = %5.5f, y = %5.5f, e2=%5.5f\n",__FUNCTION__, fabs(vPredictedPosX[i]-vCorrectedPosX[i]),fabs(vPredictedPosY[i]-vCorrectedPosY[i]),(vPredictedPosY[i]-vCorrectedPosY[i])*(vPredictedPosY[i]-vCorrectedPosY[i])+(vPredictedPosX[i]-vCorrectedPosX[i])*(vPredictedPosX[i]-vCorrectedPosX[i]));
				vPredictedPosX[i]=lFitFunctionZX->GetParameter(0)+lFitFunctionZX->GetParameter(1)*vCorrectedPosZ[i];
				vPredictedPosY[i]=lFitFunctionZY->GetParameter(0)+ lFitFunctionZY->GetParameter(1)*vCorrectedPosZ[i];

				error_ZX+=(vPredictedPosX[i]-vCorrectedPosX[i])*(vPredictedPosX[i]-vCorrectedPosX[i]);
				error_ZY+=(vPredictedPosY[i]-vCorrectedPosY[i])*(vPredictedPosY[i]-vCorrectedPosY[i]);
				printf("[%s] ZX and ZY error x = %5.5f, y = %5.5f, e2=%5.5f\n\n",__FUNCTION__, fabs(vPredictedPosX[i]-vCorrectedPosX[i]),fabs(vPredictedPosY[i]-vCorrectedPosY[i]),(vPredictedPosY[i]-vCorrectedPosY[i])*(vPredictedPosY[i]-vCorrectedPosY[i])+(vPredictedPosX[i]-vCorrectedPosX[i])*(vPredictedPosX[i]-vCorrectedPosX[i]));
			}
			}
			printf("error for XZ %5.5f    ZX  %5.5f   YZ  %5.5f  ZY  %5.5f\n",error_XZ,error_ZX,error_YZ,error_ZY);
			//======================================
			*/


			// set the flags
			//+++++++++++++++++++++++++++++++++
			// make sure the this is a good tracking

			for (unsigned int i = 0; i < kNMODULE; i++) {
				if ((vPredictedPosX[i] >= cDetectorRangeMinX[i])
						&& (vPredictedPosX[i] <= cDetectorRangeMaxX[i])
						&& (vPredictedPosY[i] >= cDetectorRangeMinY[i])
						&& (vPredictedPosY[i] <= cDetectorRangeMaxY[i])) {
					vGoodTrackingFlag[i]=1;
				}
			}

			nFiredChamber=lNEventAllChamber;

			//+++++++++++++++++++++++++++++++++
		}
		//Test_report();
		// debug flags
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

		delete vxzHisto;
		delete vzxHisto;
		delete vyzHisto;
		delete vzyHisto;
		delete vxyHisto;
		delete vyxHisto;
		delete histo_3d;
	}
}

void GEMCalibration::Test_report(){
	for(unsigned int i =0; i <kNMODULE; i++){
		printf("[%s]  %d  origanl=>  (%5.5f, %5.5f, %5.5f) predicted is => %d\n", __FUNCTION__, vNCluster[i],vOriginalPosX[i],vOriginalPosY[i],vOriginalPosZ[i],vGoodTrackingFlag[i]);
		printf("[%s]  %d  corect =>  (%5.5f, %5.5f, %5.5f) predicted is => %d\n", __FUNCTION__, vNCluster[i],vCorrectedPosX[i],vCorrectedPosY[i],vCorrectedPosZ[i],vGoodTrackingFlag[i]);
		printf("[%s]  %d  predict=>  (%5.5f, %5.5f, %5.5f) predicted is => %d\n", __FUNCTION__, vNCluster[i],vPredictedPosX[i],vPredictedPosY[i],vPredictedPosZ[i],vGoodTrackingFlag[i]);
	}
	printf("[%s]     eff=>  (%5.5f, %5.5f, %5.5f) predicted is \n", __FUNCTION__, vCorrelationEffXY,vCorrelationEffXZ,vCorrelationEffYZ);
}

///===================================================
void GEMCalibration::Init() {
	for(unsigned int i =0; i <kNMODULE; i++) {
		vOriginalPosX[i]=0;
		vOriginalPosY[i]=0;
		vOriginalPosZ[i]=0;

		vCorrectedPosX[i]=0;
		vCorrectedPosY[i]=0;
		vCorrectedPosZ[i]=0;
		vGoodTrackingFlag[i]=0;
	}
}

//=================================================
// Sort According
void GEMCalibration::Sort(vector<GEMCluster> &vC) {

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


GEMCalibration::~GEMCalibration() {
	// TODO Auto-generated destructor stub
}

