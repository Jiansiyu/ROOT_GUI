#include "GEMTracking.h"
#include "Config.h"
#include "GEMHistoManager.h"
#include "GEMTrackConstrcution.h"
#include "GEMCalibration.h"
#include "time.h"
using namespace GEMHistoManager;
using namespace std;

//==================================================================================
GEMTracking::GEMTracking(TChain* chain) : GEMTree(chain)
{
  //fTimer = new TTimer("gSystem->ProcessEvents();", 50, 0);
  
  gStyle->SetOptFit(1111);
  InitHistograms();

  evtID = 0;
}

//==================================================================================
GEMTracking::~GEMTracking()
{
  cout<<"GEMTracking Destructor called..."<<endl;
}

//==================================================================================
void GEMTracking::Reset()
{
  vCluster.clear();
  vHit.clear();
  vHit_cut.clear();
  vCluster_cut.clear();
}

//==================================================================================
Int_t GEMTracking::APVIndex(Int_t module, Int_t plane, Int_t strip)
{
  //maximum 40 APVs on each plane
  return (2*module+plane)*40+strip/128;
}

//==================================================================================
Int_t GEMTracking::IsBestHit(GEMHit hit)
{
  //criterion:
  //Max ADC at 3rd or 4th Time Bin, ADC values increase and decrease smoothly
  Double_t a[kMAXADC];
  vector<Double_t>::iterator it = hit.vTsAdc.begin();
  Int_t i=0;
  for(;it!=hit.vTsAdc.end();++it)
    {
      a[i]=*it;
      i++;
    }
  //maximum at the 3rd time bin
  Int_t maxbin=0;
  i=0;
  while(i<kMAXADC-1)
    {
      if(i<3)
	{
	  if(a[i]<a[i+1])
	    {
	      i++;
	      continue;
	    }
	  else
	    {
	      break;
	    }
	}
      else
	{
	  if(a[i]>a[i+1])
	    {
	      i++;
	      continue;
	    }
	  else
	    {
	      break;
	    }
	}
    }
  if(i==kMAXADC-1)
    maxbin=3;

  //maximum at the 4th time bin
  i=0;
  while(i<kMAXADC && maxbin!=3)
    {
      if(i<4)
	{
	  if(a[i]<a[i+1])
	    {
	      i++;
	      continue;
	    }
	  else
	    {
	      break;
	    }
	}
      else
	{
	  if(a[i]>a[i+1])
	    {
	      i++;
	      continue;
	    }
	  else
	    {
	      break;
	    }
	}
    }
  if(i==kMAXADC-1)
    maxbin=4;

  if(maxbin==3 || maxbin==4)
    return 1;
  else
    return 0;
}

//==================================================================================
// Sept 15th 2016    Siyu
// select the maximum position  
// To be improved
// 
Int_t GEMTracking::IsBestHit(GEMHit hit,int max_sample_1,int max_sample_2,int max_sample_3)
{
  //criterion:
  //Max ADC at 3rd or 4th Time Bin, ADC values increase and decrease smoothly
  Double_t a[kMAXADC];
  vector<Double_t>::iterator it = hit.vTsAdc.begin();
  Int_t i=0;
  for(;it!=hit.vTsAdc.end();++it)
    {
      a[i]=*it;
      i++;
    }
    
  //maximum at the 3rd time bin
  Int_t maxbin=0;
  i=0;
  
  while(i<kMAXADC-1)
    {
      if(i<2)
	{
	  if(a[i]<a[i+1])
	    {
	      i++;
	      continue;
	    }
	  else
	    {
	      break;
	    }
	}
      else
	{
	  if(a[i]>a[i+1])
	    {
	      i++;
	      continue;
	    }
	  else
	    {
	      break;
	    }
	}
    }
  // make sure all the samples have effective data  
  if(i==kMAXADC-1)
    maxbin=3;
    
  if(maxbin==3 || maxbin==4)
    return 1;
  else
    return 0;
}


//==================================================================================
void GEMTracking::Run(Int_t event)
{
  if(!fChain)
    {
      Error("run","No Tree to analyze.");
    }

  Int_t entries = (Int_t) fChain->GetEntries();
  cout<<"Number of events: "<<entries<<endl;
  if(event > 0)
    entries = event; //for decoding
  cout<<"    Will analyze  "<<entries<<"  event..."<<endl;

  for(int i=0;i<entries;i++)
    {
      //Progress bar
      Double_t ratio = i/(Double_t)entries;
      cout<<setw(8)<<(int)(ratio*100)<<"%\r"<<flush;
      fChain->GetEntry(i);
      Reset();
      evtID = i;
      //cout<<"Event: "<<evtID<<endl;
      for(int j=0;j<kNMODULE;j++)
      {
    	  for(int k=0;k<2;k++)
    	  	  {
    		  	  Decode(j,k);
    	  	  }
      	 }
	
      for(int j=0;j<kNMODULE;j++)
	{
	  for(int k=0;k<2;k++)
	    {
	      FindCluster(j,k,vHit,0);
	    }
	}

     // finish the cluster searching
     // start doing the cluster matching and the layer matching

      // in this step , it do the cluster and event matching
      FillHistograms(); //up to here, all hits and clusters in one entry have been filled to vHit and vCluster.
    }

  PrintHistograms();
  //PrintHistogramsAPV();
  //PrintHistogramsCut();
  //PrintHistogramsAPVCut();
  //PrintHistogramsBest();
  //PrintHistogramsAPVBest();
  save_cluster_tree();
}
//==================================================================================
//June 28th modified so as to change the output file name  --siyu 
void GEMTracking::Run(Int_t event, const char *filename)
{
	// used for the efficency test
	Long_t nEvents[kNMODULE];
	Long_t nEffEvents[kNMODULE];
	float_t nEfficiency[kNMODULE];

	for(int i = 0 ; i <kNMODULE; i ++){
		nEvents[i]=0;
		nEffEvents[i]=0;
		nEfficiency[i]=0;
	}

	if (!fChain) {
		Error("run", "No Tree to analyze.");
	}

	Int_t entries = (Int_t) fChain->GetEntries();
	cout << "Number of events: " << entries << endl;
	if (event > 0)
		entries = event; //for decoding
	cout << "    Will analyze  " << entries << "  event..." << endl;


	for (int i =0; i < entries; i++)    // each entries is one event
	{
		//Progress barss
		Double_t ratio = i / (Double_t) entries;
		cout<<setw(8)<<(int)(ratio*100)<<"%\r"<<flush;


		fChain->GetEntry(i);
		Reset();
		evtID = i;

		// make sure the maximum fired strips is with the limit range
		if(digi_gem_nch > kMAXNCH) {
			printf("WORNING : the maximum fired strips is %d, beyound the seted range(%d)",digi_gem_nch,kMAXNCH);
			continue;
		}

		// start decode the data
		for (int j = 0; j < kNMODULE; j++)	// loop on detector and dimension
				{
			for (int k = 0; k < 2; k++)      // two dimension
					{
				Decode(j, k);
			}
		}
		for (int j = 0; j < kNMODULE; j++) {
			for (int k = 0; k < 2; k++) {
				FindCluster(j, k, vHit, 0);
				//FindCluster(j,k,vHit_cut,1);
			}
		}

		FillHistograms(); //up to here, all hits and clusters in one entry have been filled to vHit and vCluster.
	}

	PrintHistograms(filename);
	//PrintHistogramsAPV();
	//PrintHistogramsCut();
	//PrintHistogramsAPVCut();
	//PrintHistogramsBest();
	//PrintHistogramsAPVBest();
	save_cluster_tree();

	cout<<"Result"<<endl;
}



//==============================================================
// used for extract the single cosmic ray and calculate the distortion matrix
// to be improve to make it a general program used for any number of layers of chambers
void GEMTracking::Calibration(Int_t event,const char *filename) {

	std::vector<unsigned int> cKnownDetector;
	cKnownDetector.push_back(0);
	cKnownDetector.push_back(3);
	std::vector<unsigned int> cUnknownDetector;
	cUnknownDetector.push_back(1);
	cUnknownDetector.push_back(2);

	// used for generate the X-Z and Y-Z 2d histo
	TH2D *hZCalibrateX2D[cUnknownDetector.size()];
	TH2D *hZCalibrateY2D[cUnknownDetector.size()];

	TH1D *hCorrelationEffHitoXZ;
	TH1D *hCorrelationEffHitoYZ;

	TH1D *DistortionX[cUnknownDetector.size()];
	TH1D *DistortionY[cUnknownDetector.size()];

	hCorrelationEffHitoXZ = new TH1D("CorrelationEffXZ","CorrelationEffXZ",100,0,1.2);
	hCorrelationEffHitoYZ = new TH1D("CorrelationEffYZ","CorrelationEffYZ",100,0,1.2);

	unsigned int Counter_temp=0;
	for(unsigned int i : cUnknownDetector){
		hZCalibrateX2D[Counter_temp]=new TH2D(Form("CalibrationModule%d_X",i),Form("CalibrationModule%d_X",i),1000,-10000,10000,2000,-100000,100000);
		hZCalibrateY2D[Counter_temp]=new TH2D(Form("CalibrationModule%d_Y",i),Form("CalibrationModule%d_Y",i),1000,-10000,10000,2000,-100000,100000);

		DistortionX[Counter_temp] = new TH1D(Form("DistortionX_module%d",i),Form("DistortionX_module%d",i),1000,-500,500);
		DistortionX[Counter_temp]->GetXaxis()->SetTitle("Dx/100um");
		DistortionX[Counter_temp]->GetYaxis()->SetTitle("Counts");
		DistortionY[Counter_temp] = new TH1D(Form("DistortionY_module%d",i),Form("DistortionY_module%d",i),1000,-500,500);
		DistortionY[Counter_temp]->GetXaxis()->SetTitle("Dy/100um");
		DistortionY[Counter_temp]->GetYaxis()->SetTitle("Counts");

		hZCalibrateX2D[Counter_temp]->GetXaxis()->SetTitle(Form("x%d-x%d",0,3));
		hZCalibrateX2D[Counter_temp]->GetYaxis()->SetTitle(Form("(z%d-z%d)*(x%d-x%d)",0,3,i,3));
		hZCalibrateY2D[Counter_temp]->GetXaxis()->SetTitle(Form("y%d-y%d",0,3));
		hZCalibrateY2D[Counter_temp]->GetYaxis()->SetTitle(Form("(z%d-z%d)*(y%d-y%d)",0,3,i,3));

		Counter_temp++;
	}

	if (!fChain) {
		Error("run", "No Tree to analyze.");
	}

	Int_t entries = (Int_t) fChain->GetEntries();
	cout << "Number of events: " << entries << endl;
	if (event > 0)entries = event; //for decoding
	cout << "    Will analyze  " << entries << "  event..." << endl;

	time_t starttime;
	time(&starttime);
	for (int i = 0; i < entries; i++)   { // each entries is one event

		if (i % 100 == 1) {           //Progress bar

			time_t endtime;

			time(&endtime);
			double_t timeperiod=difftime(endtime,starttime);

			int time_remins=(entries-i)*timeperiod/i;
			Double_t ratio = i / (Double_t) entries;
			cout << setw(8) << (int) (ratio * 100) << setw(8)
					<< "%  time remains : " << (int) (time_remins / 60)
					<< "mins " << (int) (time_remins % 60)
					<< "s / total time : "
					<< ((int) ((entries) * timeperiod / i) / 60) << " mins "
					<< ((int) ((entries) * timeperiod / i) % 60) << "s  "
					<< (int) (i / timeperiod) << " events/s \r" << flush;
		}

		fChain->GetEntry(i);
		Reset();
		evtID = i;

		if (digi_gem_nch > kMAXNCH) {// make sure the maximum fired strips is with the limit range
			printf("WORNING : the maximum fired strips is %d, beyound the seted range(%d)",digi_gem_nch, kMAXNCH);
			continue;
		}

		// start decode the data
		for (int j = 0; j < kNMODULE; j++)	// loop on detector and dimension
				{
			for (int k = 0; k < 2; k++)      // two dimension
					{
				Decode(j, k);
			}
		}
		for (int j = 0; j < kNMODULE; j++) {
			for (int k = 0; k < 2; k++) {
				FindCluster(j, k, vHit, 0);
				//FindCluster(j,k,vHit_cut,1);
			}
		}

		GEMCalibration *calibration = new GEMCalibration(vCluster);
		calibration->CosmicCalibrate();
		if(     (calibration->vGoodTrackingFlag[0])&&
				(calibration->vGoodTrackingFlag[1])&&
				(calibration->vGoodTrackingFlag[2])&&
				(calibration->vGoodTrackingFlag[3])&&
				(calibration->vNCluster[0]==1)&&
				(calibration->vNCluster[1]==1)&&
				(calibration->vNCluster[2]==1)&&
				(calibration->vNCluster[3]==1)) {

		 hZCalibrateX2D[0]->Fill((calibration->vOriginalPosX[0]-calibration->vOriginalPosX[3])*100, ((calibration->vOriginalPosZ[0]-calibration->vOriginalPosZ[3])*(calibration->vOriginalPosX[1]-calibration->vOriginalPosX[3]))*100);
		 hZCalibrateY2D[0]->Fill((calibration->vOriginalPosY[0]-calibration->vOriginalPosY[3])*100, ((calibration->vOriginalPosZ[0]-calibration->vOriginalPosZ[3])*(calibration->vOriginalPosY[1]-calibration->vOriginalPosY[3]))*100);

		 hZCalibrateX2D[1]->Fill((calibration->vOriginalPosX[0]-calibration->vOriginalPosX[3])*100, ((calibration->vOriginalPosZ[0]-calibration->vOriginalPosZ[3])*(calibration->vOriginalPosX[2]-calibration->vOriginalPosX[3]))*100);
		 hZCalibrateY2D[1]->Fill((calibration->vOriginalPosY[0]-calibration->vOriginalPosY[3])*100, ((calibration->vOriginalPosZ[0]-calibration->vOriginalPosZ[3])*(calibration->vOriginalPosY[2]-calibration->vOriginalPosY[3]))*100);

		 DistortionX[0]->Fill(100*((calibration->vCorrectedPosZ[1]-calibration->vCorrectedPosZ[3])*(calibration->vOriginalPosX[0]-calibration->vOriginalPosX[3])/(calibration->vCorrectedPosZ[0]-calibration->vCorrectedPosZ[3])+calibration->vOriginalPosX[3]-calibration->vOriginalPosX[1]));
		 DistortionY[0]->Fill(100*((calibration->vCorrectedPosZ[1]-calibration->vCorrectedPosZ[3])*(calibration->vOriginalPosY[0]-calibration->vOriginalPosY[3])/(calibration->vCorrectedPosZ[0]-calibration->vCorrectedPosZ[3])+calibration->vOriginalPosY[3]-calibration->vOriginalPosY[1]));

		 DistortionX[1]->Fill(100*((calibration->vCorrectedPosZ[2]-calibration->vCorrectedPosZ[3])*(calibration->vOriginalPosX[0]-calibration->vOriginalPosX[3])/(calibration->vCorrectedPosZ[0]-calibration->vCorrectedPosZ[3])+calibration->vOriginalPosX[3]-calibration->vOriginalPosX[2]));
		 DistortionY[1]->Fill(100*((calibration->vCorrectedPosZ[2]-calibration->vCorrectedPosZ[3])*(calibration->vOriginalPosY[0]-calibration->vOriginalPosY[3])/(calibration->vCorrectedPosZ[0]-calibration->vCorrectedPosZ[3])+calibration->vOriginalPosY[3]-calibration->vOriginalPosY[2]));

		 hCorrelationEffHitoXZ->Fill(calibration->vCorrelationEffXZ);
		 hCorrelationEffHitoYZ->Fill(calibration->vCorrelationEffYZ);
		}

		delete calibration;
	}
	TFile *file = new TFile(filename,"RECREATE");
	hZCalibrateX2D[0]->Write();
	hZCalibrateY2D[0]->Write();
	hZCalibrateX2D[1]->Write();
	hZCalibrateY2D[1]->Write();

	DistortionX[0]->Write();
	DistortionY[0]->Write();
	DistortionX[1]->Write();
	DistortionY[1]->Write();

	hCorrelationEffHitoXZ->Write();
	hCorrelationEffHitoYZ->Write();
	file->Write();
	file->Close();
}


//==================================================================================
// module, how many detectors(chambers, chamber index),
void GEMTracking::Decode(Int_t module, Int_t plane)
{

  Int_t nch = digi_gem_nch; //number of strips fired
  for (int i = 0; i < nch; i++)       //strips index
	{
		//module chamber index   plane : dimension x or y
		if (digi_gem_chamber[i] == module && digi_gem_plane[i] == plane) {
			vector<Double_t> vamp;
			for (int j = 0; j < kMAXADC; j++) {
				vamp.push_back(digi_gem_adc[j][i]);     // six time samples
			}
			GEMHit gem_hit(module, plane, digi_gem_strip[i], vamp);
			gem_hit.SetTiming();

			//make sure all events are reasonable and apply cuts here
			//Xinzhan: if you want to put some cuts, the following line would be a good place to start
			//if( (gem_hit.Charge < kMaxSignalADC) && (gem_hit.Charge > 0) && (gem_hit.MaxTsAdc < kMaxStripADC) && ( gem_hit.MaxTsAdc > 0) )
			{
				vHit.push_back(gem_hit);
				if (gem_hit.StartTime > kStartingTimeLow
						&& gem_hit.StartTime < kStartingTimeHigh) {
					vHit_cut.push_back(gem_hit);
				}
			}
		}
	}
}

//==================================================================================
void GEMTracking::FindCluster(Int_t module, Int_t plane, vector<GEMHit> vHit, Int_t cut)
{
  //Xinzhan: new FindCluster() function
  if( vHit.size()!=0 ) //Make sure not empty
    {
      vector<GEMHit> v_hit;
      vector<GEMHit>::iterator it = vHit.begin();
      for(;it!=vHit.end();++it) //get all hits in this module plane
	{
	  if( ((*it).Module == module) && ((*it).Plane == plane))
	    v_hit.push_back(*it);                                // select only certain plane and modules to process
	}

      Int_t nbclustperplane=0; //number of clusters per plane
      if( v_hit.size()!=0 ) //look for clusters
	{
	  TObjArray hitindex;
	  hitindex.SetOwner();//necessary to avoid memory leak
	  vector<GEMHit>::iterator next = v_hit.begin();
	  vector<GEMHit>::iterator cur=next;
	  vector<GEMHit>::iterator end = v_hit.end();
	  Int_t cluster_size = 0;
	  while( next!=end )
	    {
	      GEMHit* gem_hit = new GEMHit();
	      cur->Clone(*gem_hit);
	      hitindex.Add( gem_hit );
	      ++cluster_size;
	      ++next;
	      if( ( next->Strip - cur->Strip) == 1)
		{
		  cur=next;
		}
	      else
		{
		  if(cluster_size<=kMaxStripsinCluster && cluster_size>=kMinStripsinCluster)
		    {
		      nbclustperplane++; // number of clusters per plane
		      GEMCluster cluster(module, plane, cluster_size, hitindex);
		      cluster.SetTiming();
		      if(cut)
			{
			  vCluster_cut.push_back(cluster);
			}
		      else
			{
			  vCluster.push_back(cluster);//cout<<"cluster?"<<endl;
			}
		      cluster_size=0;
		      hitindex.Clear();
		      cur=next;
		      //Store cluster Tree
		      cluster_tree.Fill(cluster, evtID);
		    }
		  else
		    {
		      cluster_size=0;
		      hitindex.Clear();
		      cur=next;
		    } // only store clusters with size < kMaxStripsinCluster
		}
	    }
	  if(nbclustperplane!=0) 
	  	{
	  		hNbClusterPerPlane[module]->Fill(nbclustperplane);
	  		if(plane==0)
	  		  {
	  		     hNbClusterPerPlane_x[module]->Fill(nbclustperplane);
	  		   }else
	  		    {
	  		       if(plane==1)
	  		          {
	  		          
	  		             hNbClusterPerPlane_y[module]->Fill(nbclustperplane);
	  		            }
	  		      }
	  	}
	}
    }
}

//==================================================================================
void GEMTracking::Sort(vector<GEMCluster> &vC)
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

//==================================================================================
void GEMTracking::Sort(vector<Struct_MaxTimeBin> &vC)
{
  vector<Struct_MaxTimeBin>::iterator it = vC.begin();
  vector<Struct_MaxTimeBin>::iterator it1;
  for(;it!=vC.end();++it)
    {
      for(it1=it+1; it1!=vC.end(); ++it1)
	{
	  if( (*it).charge < (*it1).charge )
	    {
	      Int_t n_section = (*it1).section;
	      Double_t f_charge = (*it1).charge;
	      Double_t f_timebin = (*it1).timebin;

	      (*it1).section = (*it).section;
	      (*it1).charge = (*it).charge;
	      (*it1).timebin = (*it).timebin;

	      (*it).section = n_section;
	      (*it).charge = f_charge;
	      (*it).timebin = f_timebin;
	    }
	}
    }
}

//==================================================================================
void GEMTracking::FillHistograms()
{
  //After each entry, call this function to fill histograms.
  //Input of this fuction is vHit, vHit_cut, and vCluster
  //**************************************************************************
  //Histograms to be filled:
  //***********************                          hhClusterDist[module]
  //***********************       hHitDistX[module]  hHitDistY[module]
  //***********************   hClusterDistX[module]  hClusterDistY[module]
  //***********************                          hhClusterDistCut[module]
  //***********************    hHitDistXCut[module]  hHitDistYCut[module]
  //***********************hClusterDistXCut[module]  hClusterDistYCut[module]
  //***********************            hClusterSize  hhClusterChargeRatio[module]
 
  Int_t nhits = vHit.size();
  Int_t nhits_count = 0;

  for(int i=0;i<kNMODULE;i++)    // loop on the detectors
    {
      //_____________________________________________________
      //Fill 1D hit distribution
      if(vHit.size() > 0)
	{
	  vector<GEMHit>::iterator iter= vHit.begin();
	  while(iter != vHit.end())
	    {
	      if( (*iter).Module == i  )
		{
		  hMaxADConStrip[i]->Fill( (*iter).MaxFromFitting ); //Fill hMaxADConStrip[module]
		  hMaxADConStripAPV[APVIndex( (*iter).Module, (*iter).Plane, (*iter).Strip )]->Fill((*iter).MaxFromFitting);
		  hMaxADConStripTime[i]->Fill( (*iter).MaxTimeFromFitting );
		  hMaxADConStripTimeAPV[APVIndex( (*iter).Module, (*iter).Plane, (*iter).Strip )]->Fill( (*iter).MaxTimeFromFitting );
		  hChargeFromFit[i]->Fill((*iter).Charge);
		  hChargeFromFitAPV[APVIndex((*iter).Module, (*iter).Plane, (*iter).Strip)]->Fill((*iter).Charge);
		  hHitTimeAPV[APVIndex((*iter).Module, (*iter).Plane, (*iter).Strip)]->Fill( (*iter).StartTime );
		  hAPVConstAPV[APVIndex((*iter).Module, (*iter).Plane, (*iter).Strip)]->Fill( (*iter).ShappingTime );
		  hReducedChisquare[i]->Fill((*iter).ReducedChisquareFromFitting);
		  hReducedChisquareAPV[APVIndex((*iter).Module, (*iter).Plane, (*iter).Strip)]->Fill((*iter).ReducedChisquareFromFitting);
		  if( (*iter).Plane == 0 )
		    {
		      //hHitTimeX[i]->Fill( (*iter).StartTime );
		      //hAPVConstX[i]->Fill( (*iter).ShappingTime );
		      hHitDistX[i]->Fill((*iter).Strip);
		      //hMaxTimeBinXModule[i]->Fill((*iter).MaxTimeBin);
		    }
		  if( (*iter).Plane == 1 )
		    {
		      hHitTimeY[i]->Fill( (*iter).StartTime );
		      hAPVConstY[i]->Fill( (*iter).ShappingTime );
		      hHitDistY[i]->Fill((*iter).Strip);
		      hMaxTimeBinYModule[i]->Fill((*iter).MaxTimeBin);
		    }
		   
		  //++++++++++++++++++++++++++++++++++++++++++++++++++
		  // Added by Siyu 
		  // used for Draw strips ADC distribution
		  vector<Double_t>::iterator iter_fired_strips=((*iter).vTsAdc).begin();
		  for(;iter_fired_strips!=((*iter).vTsAdc).end();iter_fired_strips++)
		    {
		       hStripsADCdist[i]->Fill((*iter_fired_strips));
		       
		       if( (*iter).Plane == 0 )    // in x dimension
		         {
		           hStripsADCdistX[i]->Fill((*iter_fired_strips));		         
		         }
		       if( (*iter).Plane == 1 )    // in y dimension
		         {
		           hStripsADCdistY[i]->Fill((*iter_fired_strips));
		         }
		    
		    }
		  
		  //++++++++++++++++++++++++++++++++++++++++++++++++++  
		  
		  
		  
		  //Signal with best shape Fill 
		  if(IsBestHit(*iter,0,0,0))
		    {
		    
		      hMaxADConStripBest[i]->Fill( (*iter).MaxFromFitting );
		      hMaxADConStripAPVBest[APVIndex((*iter).Module, (*iter).Plane, (*iter).Strip)]->Fill( (*iter).MaxFromFitting );
		      hMaxADConStripTimeBest[i]->Fill( (*iter).MaxTimeFromFitting );
		      hMaxADConStripTimeAPVBest[APVIndex((*iter).Module, (*iter).Plane, (*iter).Strip)]->Fill( (*iter).MaxTimeFromFitting );
		      
		      if( (*iter).Plane == 0 )
			{
			  hHitTimeXBest[i]->Fill( (*iter).StartTime );
			  hAPVConstXBest[i]->Fill( (*iter).ShappingTime );
			}
			
		      if( (*iter).Plane == 1 )
			{
			  hHitTimeYBest[i]->Fill( (*iter).StartTime );
			  hAPVConstYBest[i]->Fill( (*iter).ShappingTime );
			}
			
		      hHitTimeAPVBest[APVIndex((*iter).Module, (*iter).Plane, (*iter).Strip)]->Fill( (*iter).StartTime );
		      hAPVConstAPVBest[APVIndex((*iter).Module, (*iter).Plane, (*iter).Strip)]->Fill( (*iter).ShappingTime );
		      hReducedChisquareBest[i]->Fill((*iter).ReducedChisquareFromFitting);
		      hReducedChisquareAPVBest[APVIndex((*iter).Module, (*iter).Plane, (*iter).Strip)]->Fill((*iter).ReducedChisquareFromFitting);
		     //hChargeFromFitBest[i]->Fill((*iter).Charge);
		     //hChargeFromFitAPVBest[APVIndex((*iter).Module, (*iter).Plane, (*iter).Strip )]->Fill((*iter).Charge);
		    }
		    
		  nhits_count ++;
		}
	      iter++;
	    }
	}
      
      //_______________________________________________________
      //Fill 1D hit distribution with cut
      if(vHit_cut.size()>0)
	{
	  vector<GEMHit>::iterator iter_cut= vHit_cut.begin();
	  while(iter_cut != vHit_cut.end())
	    {
	      if( (*iter_cut).Module == i )
		{
		  hChargeFromFitCut[i]->Fill((*iter_cut).Charge);
		  hChargeFromFitAPVCut[APVIndex((*iter_cut).Module,(*iter_cut).Plane,(*iter_cut).Strip)]->Fill((*iter_cut).Charge);
		  hMaxADConStripCut[i]->Fill( (*iter_cut).MaxFromFitting );
		  hMaxADConStripAPVCut[APVIndex((*iter_cut).Module,(*iter_cut).Plane,(*iter_cut).Strip)]->Fill( (*iter_cut).MaxFromFitting );
		  hMaxADConStripTimeCut[i]->Fill( (*iter_cut).MaxTimeFromFitting );
		  hMaxADConStripTimeAPVCut[APVIndex((*iter_cut).Module,(*iter_cut).Plane,(*iter_cut).Strip)]->Fill( (*iter_cut).MaxTimeFromFitting );

		  hHitTimeAPVCut[APVIndex((*iter_cut).Module,(*iter_cut).Plane,(*iter_cut).Strip)]->Fill((*iter_cut).StartTime);
		  hAPVConstAPVCut[APVIndex((*iter_cut).Module,(*iter_cut).Plane,(*iter_cut).Strip)]->Fill((*iter_cut).ShappingTime);
		  hReducedChisquareCut[i]->Fill((*iter_cut).ReducedChisquareFromFitting);
		  hReducedChisquareAPVCut[APVIndex((*iter_cut).Module,(*iter_cut).Plane,(*iter_cut).Strip)]->Fill((*iter_cut).ReducedChisquareFromFitting);
  
		  if((*iter_cut).Plane == 0)
		    {
		      hHitTimeXCut[i]->Fill((*iter_cut).StartTime);
		      hAPVConstXCut[i]->Fill((*iter_cut).ShappingTime);
		      hHitDistXCut[i]->Fill((*iter_cut).Strip);
		    }
		  if((*iter_cut).Plane == 1)
		    {
		      hHitTimeYCut[i]->Fill((*iter_cut).StartTime);
		      hAPVConstYCut[i]->Fill((*iter_cut).ShappingTime);
		      hHitDistYCut[i]->Fill((*iter_cut).Strip);
		    }
		}
	      iter_cut++;
	    }
	}
      
      //_______________________________________________________
      //Fill 1D cluster Distribution and cluster size distribution
      if(vCluster.size() > 0)
	{
	  vector<GEMCluster>::iterator iter_cluster= vCluster.begin();
	  while(iter_cluster != vCluster.end())
	    {
	      if((*iter_cluster).Module == i && (*iter_cluster).Plane == 0)
		{
		  hClusterDistX[i]->Fill((*iter_cluster).Pos);
		  hClusterSize[i]->Fill((*iter_cluster).Size);
		  hClusterSize_x[i]->Fill((*iter_cluster).Size);
		  hClusterAdcDistX[i]->Fill((*iter_cluster).Charge);
		}
	      else if((*iter_cluster).Module == i && (*iter_cluster).Plane == 1)
		{
		  hClusterDistY[i]->Fill((*iter_cluster).Pos);
		  hClusterSize[i]->Fill((*iter_cluster).Size);
		  hClusterSize_y[i]->Fill((*iter_cluster).Size);
		  hClusterAdcDistY[i]->Fill((*iter_cluster).Charge);
		}
	      iter_cluster++;
	    }
	}
      
      //_______________________________________________________
      //Fill 1D cluster Distribution with cut
      if(vCluster_cut.size() > 0)
	{
	  vector<GEMCluster>::iterator iter_cluster_cut= vCluster_cut.begin();
	  while(iter_cluster_cut != vCluster_cut.end())
	    {
	      if((*iter_cluster_cut).Module == i && (*iter_cluster_cut).Plane == 0)
		{
		  hClusterDistXCut[i]->Fill((*iter_cluster_cut).Pos);
		}
	      else if((*iter_cluster_cut).Module == i && (*iter_cluster_cut).Plane == 1)
		{
		  hClusterDistYCut[i]->Fill((*iter_cluster_cut).Pos);
		}
	      iter_cluster_cut++;
	    }
	}
      

      //_______________________________________________________
      //Fill 2D cluster charge sharing and 2D cluster position distribution
      if(vCluster.size() > 0)
	{
	  vector<GEMCluster> vxcluster;
	  vector<GEMCluster> vycluster;
	  vector<GEMCluster>::iterator it = vCluster.begin();
	  while(it != vCluster.end())
	    {
	      if( (*it).Module == i && (*it).Plane == 0)
		{
		  vxcluster.push_back((*it));
		}
	      else if( (*it).Module == i && (*it).Plane == 1)
		{
		  vycluster.push_back((*it));
		}
	      it++;
	    }
	  // sort the cluster and match the particle in x and y dimension
	  if(vxcluster.size()>0 && vycluster.size()>0)
	    {//cout<<"clusterrrr?"<<endl;
	      Sort(vxcluster);
	      Sort(vycluster);
	      Int_t N = (vxcluster.size() < vycluster.size())? vxcluster.size() : vycluster.size();
	      
	      for(int m =0; m<N; m++)
		{
		  hhClusterChargeRatio[i]->Fill( (vxcluster[m]).Charge, (vycluster[m]).Charge);
		  hhClusterDist[i]->Fill( (vxcluster[m]).Pos, (vycluster[m]).Pos);
		 
		  //cout<<"X:"<<(vxcluster[m]).Pos<<" Y:"<<(vycluster[m]).Pos<<endl;
		  hhClusterDistXMaxBin[i]->Fill( (vxcluster[m]).Pos, (vycluster[m]).Pos, (vycluster[m]).MaxTimeBin );
		  hhClusterDistXStartTime[i]->Fill( (vxcluster[m]).Pos, (vycluster[m]).Pos, (vycluster[m]).StartTime );
		  hhClusterDistXShappingTime[i]->Fill( (vxcluster[m]).Pos, (vycluster[m]).Pos, (vycluster[m]).ShappingTime );
		  hhClusterStartTimeCorrelation[i]->Fill( (vxcluster[m]).StartTime, (vycluster[m]).StartTime );
		  hhClusterShappingTimeCorrelation[i]->Fill( (vxcluster[m]).ShappingTime, (vycluster[m]).ShappingTime);

		  hMaxTimeBinClusterXModule[i]->Fill((vxcluster[m]).MaxTimeBin);
		  hMaxTimeBinClusterYModule[i]->Fill((vycluster[m]).MaxTimeBin);

		  hHitTimeClusterX[i]->Fill((vxcluster[m]).StartTime);
		  hAPVConstClusterX[i]->Fill((vxcluster[m]).ShappingTime);
		  hHitTimeClusterY[i]->Fill((vycluster[m]).StartTime);
		  hAPVConstClusterY[i]->Fill((vycluster[m]).ShappingTime);

		  hReducedChisquareCluster[i]->Fill((vxcluster[m]).Fitting_ReducedChisquare);
		  hReducedChisquareCluster[i]->Fill((vycluster[m]).Fitting_ReducedChisquare);
		}
	    }
	}
      
      //____________________________________________________________________________
      //Fill 2D cluster charge sharing and 2D cluster position distribution with cut
      if(vCluster_cut.size()>0)
	{
	  vector<GEMCluster> vxcluster_cut;
	  vector<GEMCluster> vycluster_cut;
	  vector<GEMCluster>::iterator it_cut = vCluster_cut.begin();
	  while(it_cut != vCluster_cut.end())
	    {
	      if( (*it_cut).Module == i && (*it_cut).Plane == 0)
		{
		  vxcluster_cut.push_back((*it_cut));
		}
	      else if( (*it_cut).Module == i && (*it_cut).Plane == 1)
		{
		  vycluster_cut.push_back((*it_cut));
		}
	      it_cut++;
	    }
	  if(vxcluster_cut.size()>0 && vycluster_cut.size()>0)
	    {
	      Sort(vxcluster_cut);
	      Sort(vycluster_cut);
	      Int_t N = (vxcluster_cut.size() < vycluster_cut.size())? vxcluster_cut.size() : vycluster_cut.size();
	      for(int m =0; m<N; m++)
		{
		  hhClusterDistCut[i]->Fill( (vxcluster_cut[m]).Pos, (vycluster_cut[m]).Pos);
		}
	    }
	}
      //_______________________________________________________
      //Using the Ratio Method to Fill 2D cluster charge sharing and 2D cluster position distribution //Charge Ratio Method
      if(vCluster.size() > 0)
	{
	  vector<GEMCluster> vxcluster;
	  vector<GEMCluster> vycluster;
	  vector<GEMCluster>::iterator it = vCluster.begin();
	  while(it != vCluster.end())
	    {
	      if( (*it).Module == i && (*it).Plane == 0)
		{
		  vxcluster.push_back((*it));
		}
	      else if( (*it).Module == i && (*it).Plane == 1)
		{
		  vycluster.push_back((*it));
		}
	      it++;
	    }
	  if(vxcluster.size()>0 && vycluster.size()>0)
	    {
	      vector<GEMCluster>::iterator itx = vxcluster.begin();
	      vector<GEMCluster>::iterator ity = vycluster.begin();
	      for(;itx != vxcluster.end(); ++itx)
		{
		  for(;ity != vycluster.end(); ++ity)
		    {
		      Double_t ratio = ((*itx).Charge)/((*ity).Charge);
		      if(ratio>kMinChargeRatio && ratio<kMaxChargeRatio)
			{
			  hhClusterChargeRatioRatioMethod[i]->Fill( (*itx).Charge, (*ity).Charge);
			  hhClusterDistRatioMethod[i]->Fill( (*itx).Pos, (*ity).Pos);
			}
		    }
		}
	    }
	}


      // fill the residue
      for(int detectorid=0; detectorid<kNMODULE; detectorid++){
    		hTrackingResidueX[detectorid]->Fill(vResiduex[detectorid]);
    	    hTrackingResidueY[detectorid]->Fill(vResiduey[detectorid]);
      }

      //_______________________________________________________
    }
  assert(nhits == nhits_count);
}
