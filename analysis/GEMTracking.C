#include "GEMTracking.h"

#include "GEMHistoManager.h"

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
	      //FindCluster(j,k,vHit_cut,1);
	    }
	}
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
  //++++++++++++++++++++++++
  //TFile f("tree1.root","recreate");
  TTree GEMCluster_tree("GEMCluster","HAHAHAHA");
  
  Int_t evtID_t;
  Int_t Module_t;
  Int_t Plane_t;
  Int_t StripIndex_t;
  Int_t adc0_t;
  Int_t adc1_t;
  Int_t adc2_t;
  Int_t adc3_t;
  Int_t adc4_t;
  Int_t adc5_t;
  
  GEMCluster_tree.Branch("evtID_t",&evtID_t,"evtID_t/I");
  GEMCluster_tree.Branch("Module_t",&Module_t,"Module_t/I");
  GEMCluster_tree.Branch("Plane_t",&Plane_t,"Plane_t/I");
  GEMCluster_tree.Branch("StripIndex_t",&StripIndex_t,"StripIndex_t/I");
  GEMCluster_tree.Branch("adc0_t",&adc0_t,"adc0_t/I");
  GEMCluster_tree.Branch("adc1_t",&adc1_t,"adc1_t/I");
  GEMCluster_tree.Branch("adc2_t",&adc2_t,"adc2_t/I");
  GEMCluster_tree.Branch("adc3_t",&adc3_t,"adc3_t/I");
  GEMCluster_tree.Branch("adc4_t",&adc4_t,"adc4_t/I");
  GEMCluster_tree.Branch("adc5_t",&adc5_t,"adc5_t/I");
  //++++++++++++++++++++++++
  
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
	  for(int k=0;k<2;k++)      // two dimension
	    {
	      Decode(j,k);
	      
	      
	      //+++++++++++++++++++++++++
	      
	      Int_t nch = digi_gem_nch; //number of strips fired
	      for(int m=0; m<nch; m++)       //strips index loop on all the channels
	        {
	          // cout<<digi_gem_plane[i]<<endl;
	           if(digi_gem_chamber[m] == j && digi_gem_plane[m] == k)
	           {
	      	    evtID_t=evtID;
	            Module_t=j;
	            Plane_t=k;
	            StripIndex_t=digi_gem_strip[m];
	            //digi_gem_adc[j][i] // 6 tinme sample, strips index 
	            adc0_t=digi_gem_adc[0][m];
	            adc1_t=digi_gem_adc[1][m];
	            adc2_t=digi_gem_adc[2][m];
	            adc3_t=digi_gem_adc[3][m];
	            adc4_t=digi_gem_adc[4][m];
	            adc5_t=digi_gem_adc[5][m];
	            GEMCluster_tree.Fill();
	            //printf("eventID=%d, plane=%d, strips=%d, adc0=%d,adc1=%d,adc2%d",evtID_t,);
       		    //module chamber index   plane : dimension x or y 
      		    //if(digi_gem_chamber[m] == module && digi_gem_plane[m] == plane)   // where is this comes from
			//{
	     		//}
	            }
	            
	     	 }
	      //+++++++++++++++++++++++++
	      
	    }
	}
      
      for(int j=0;j<kNMODULE;j++)
	{
	  for(int k=0;k<2;k++)
	    {
	      FindCluster(j,k,vHit,0);
	      //FindCluster(j,k,vHit_cut,1);
	    }
	}
      FillHistograms(); //up to here, all hits and clusters in one entry have been filled to vHit and vCluster.
    }

  PrintHistograms(filename);
  
  PrintHistogramsAPV();
  //PrintHistogramsCut();
  //PrintHistogramsAPVCut();
  //PrintHistogramsBest();
  //PrintHistogramsAPVBest();
  save_cluster_tree();
  //++++++++++++++++++++++++++++
      TFile *cluster_rootfile = new TFile(Form("Cluster_%s",filename),"RECREATE");
      GEMCluster_tree.Write();
      cluster_rootfile->Write();
      cluster_rootfile->Close();
      //GEMCluster_tree.Write();
      
      //++++++++++++++++++++++++++++
}


//==================================================================================
// module, how many deectors(chamvers, chamber index), 
void GEMTracking::Decode(Int_t module, Int_t plane)
{

  Int_t nch = digi_gem_nch; //number of strips fired
  //cout<<"number of strips fired"<<nch<<endl;

  for(int i=0; i<nch; i++)       //strips index 
     {
       //module chamber index   plane : dimension x or y 
      if(digi_gem_chamber[i] == module && digi_gem_plane[i] == plane)   // where is this comes from
	{
	  //cout<<"GEMID:"<<digi_gem_chamber[i]<<endl;
	  vector<Double_t> vamp;

	  for(int j = 0; j< kMAXADC ; j++)
	    {
	      vamp.push_back(digi_gem_adc[j][i]);
	      //if(i>100)
      
	    }
	  //cout<<endl;
	  GEMHit gem_hit(module, plane, digi_gem_strip[i], vamp);
	  gem_hit.SetTiming();

	  //make sure all events are reasonable and apply cuts here
	  //Xinzhan: if you want to put some cuts, the following line would be a good place to start
	  //if( (gem_hit.Charge < kMaxSignalADC) && (gem_hit.Charge > 0) && (gem_hit.MaxTsAdc < kMaxStripADC) && ( gem_hit.MaxTsAdc > 0) )
	 //if(IsBestHit(gem_hit,0,0,0))
	  {//cout<<"ANY?"<<endl;
	      vHit.push_back(gem_hit);
	      if(gem_hit.StartTime>kStartingTimeLow && gem_hit.StartTime < kStartingTimeHigh)
		{
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
	  //cout<<"Find cluster:  "<<it->PedSigma.size()<<endl;
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
	  		hNbClusterPerPlane->Fill(nbclustperplane);
	  		if(plane==0)
	  		  {
	  		     hNbClusterPerPlane_x->Fill(nbclustperplane);
	  		   }else
	  		    {
	  		       if(plane==1)
	  		          {
	  		          
	  		             hNbClusterPerPlane_y->Fill(nbclustperplane);
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

  for(int i=0;i<kNMODULE;i++)
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
		  hClusterSize->Fill((*iter_cluster).Size);
		  hClusterSize_x->Fill((*iter_cluster).Size);
		  hClusterAdcDistX[i]->Fill((*iter_cluster).Charge);
		}
	      else if((*iter_cluster).Module == i && (*iter_cluster).Plane == 1)
		{
		  hClusterDistY[i]->Fill((*iter_cluster).Pos);
		  hClusterSize->Fill((*iter_cluster).Size);
		  hClusterSize_y->Fill((*iter_cluster).Size);
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
      //_______________________________________________________
    }
  assert(nhits == nhits_count);
}
