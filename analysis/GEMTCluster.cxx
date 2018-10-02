#include <TTree.h>
#include <Riostream.h>
#include "GEMTCluster.h"
#include "GEMCluster.h"
#include "Config.h"

//==================================================================================
GEMTCluster::GEMTCluster(const char* name, const char* title)
{
  evtID = 0;
  adc0=0;
  adc1=0;
  adc2=0;
  adc3=0;
  adc4=0;
  adc5=0;
  adc6=0;
  adc7=0;
  adc8=0;

  GEMTreeCluster = new TTree(name, title);

  GEMTreeCluster->Branch("evtID",&evtID,"evtID/I");
  GEMTreeCluster->Branch("Module",&Module,"Module/I");
  GEMTreeCluster->Branch("Plane",&Plane,"Plane/I");
  GEMTreeCluster->Branch("Size",&Size,"Size/I");
  GEMTreeCluster->Branch("StripIndex",StripIndex,"StripIndex[Size]/I");
  GEMTreeCluster->Branch("Pos",&Pos,"Pos/D");
  GEMTreeCluster->Branch("Charge",&Charge,"Charge/D");
  GEMTreeCluster->Branch("StartTime",&StartTime,"StartTime/D");
  GEMTreeCluster->Branch("ShappingTime",&ShappingTime,"ShappingTime/D");
  GEMTreeCluster->Branch("MaxTimeBin",&MaxTimeBin,"MaxTimeBin/D");
  GEMTreeCluster->Branch("adc0",&adc0,"adc0/I");
  GEMTreeCluster->Branch("adc1",&adc1,"adc1/I");
  GEMTreeCluster->Branch("adc2",&adc2,"adc2/I");
  GEMTreeCluster->Branch("adc3",&adc3,"adc3/I");
  GEMTreeCluster->Branch("adc4",&adc4,"adc4/I");
  GEMTreeCluster->Branch("adc5",&adc5,"adc5/I");
  GEMTreeCluster->Branch("adc6",&adc6,"adc6/I");
  GEMTreeCluster->Branch("adc7",&adc7,"adc7/I");
  GEMTreeCluster->Branch("adc8",&adc8,"adc8/I");
 
}

//==================================================================================
GEMTCluster::~GEMTCluster()
{
}

//==================================================================================
void GEMTCluster::Fill(GEMCluster cluster, Int_t id)
{
  evtID = id;
  Pos = cluster.Pos;
  Charge = cluster.Charge;
  Size = cluster.Size;
  Plane = cluster.Plane;
  Module = cluster.Module;
  for(int i=0;i<Size;i++)
    {
      StripIndex[i] = ((GEMHit*)cluster.HitIndex[i])->Strip;
    }
  StartTime = cluster.StartTime;
  ShappingTime = cluster.ShappingTime;
  MaxTimeBin = cluster.MaxTimeBin;

  Int_t adc[16];
  for(int i=0;i<16;i++)
    {
      adc[i]=0;
    }
  vector<Double_t>::iterator it = cluster.vTsAdc.begin();
  Int_t ii=0;
  for(;it!=cluster.vTsAdc.end();++it)
    {
      adc[ii]=cluster.vTsAdc[ii];
      ii++;
    }
  adc0=cluster.vTsAdc[0];
  adc1=cluster.vTsAdc[1];
  adc2=cluster.vTsAdc[2];
  adc3=cluster.vTsAdc[3];
  adc4=cluster.vTsAdc[4];
  adc5=cluster.vTsAdc[5];
  adc6=cluster.vTsAdc[6];
  adc7=cluster.vTsAdc[7];
  adc8=cluster.vTsAdc[8];

  GEMTreeCluster->Fill();
}

//==================================================================================
TTree* GEMTCluster::GetTree()
{
  return GEMTreeCluster;
}
