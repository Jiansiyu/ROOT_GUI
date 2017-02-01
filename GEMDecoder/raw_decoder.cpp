
// DEBUG flag used to disable the display
#define __INFORMATION_Decoder_DISPLAY_FLAG__      // display the event disabled

#include <arpa/inet.h>
#include <assert.h>
#include <utility>

#include "raw_decoder.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>

#include <TH1F.h>
#include <TCanvas.h>
using namespace std;

//==========================================================================
RawDecoder::RawDecoder(unsigned int * buffer, int n)
{
  mAPVRawSingleEvent.clear();
  mAPVRawHisto.clear();

  fBuf = n;
  buf = new unsigned int[fBuf];
  for(int i=0;i<fBuf;i++)
  {
    buf[i] = buffer[i];
  }
  Decode();
}

//==========================================================================
RawDecoder::RawDecoder(vector<int> buffer)
{
  mAPVRawSingleEvent.clear();
  mAPVRawHisto.clear();

  fBuf = buffer.size();
  buf = new unsigned int[fBuf];
  for(int i=0;i<fBuf;i++)
  {
    buf[i] = buffer[i];
  }
  if(fBuf <= 0)
  {
    cout<<"empty vector passed in..."<<endl;
    return;
  }
  Decode();
};

//==========================================================================
RawDecoder::~RawDecoder()
{
  //free buf
  delete[] buf;

  //clear maps
  map<int, map<int, vector<int> > >::iterator it;
  for(it=mAPVRawSingleEvent.begin(); it!=mAPVRawSingleEvent.end(); ++it)
  {
    map<int, vector<int> > temp = it->second;
    map<int, vector<int> >::iterator it1;
    for(it1=temp.begin(); it1 != temp.end(); ++it1)
    {
      it1->second.clear();
    }
    it->second.clear();
  }
  mAPVRawSingleEvent.clear();

  //clear APV raw histos
  map<int, map<int, TH1F*> >::iterator it_apv;
  for(it_apv=mAPVRawHisto.begin(); it_apv!=mAPVRawHisto.end(); ++it_apv)
  {
    map<int, TH1F*> temp = it_apv->second;
    map<int, TH1F*>::iterator itt_apv;

    for(itt_apv=temp.begin(); itt_apv!=temp.end(); ++itt_apv)
    {
      TH1F *h = itt_apv->second;
      h->Delete();
    }
    temp.clear();
  }
  mAPVRawHisto.clear();
  //
}

//==========================================================================
void RawDecoder::Decode()
{
  unsigned int word32bit;

  int mpdid;
  int adc_ch;

  //map<int, vector<int> > mpd_event;
  vector<int> apv_event;
  vector<int> apv_margin;
  vector<int> mpd_margin;

  mpd_margin.clear();
  apv_margin.clear();

  //find apv margin
  for(int i=0;i<fBuf;i++)
  {
    if( ( buf[i] & 0xff000000 ) == 0x30000000 ) //ADC_TAG     0x30000000
      {
	//cout<<"margin buf:"<<(buf[i] & 0x000000ff)<<endl;
	apv_margin.push_back(i-1);
      }
    if( i==(fBuf-1))apv_margin.push_back(fBuf);//push end of file as final margin
  }

  int nMargin = apv_margin.size();

  #ifndef __INFORMATION_Decoder_DISPLAY_FLAG__
  cout<<"Number of ADCs:"<<nMargin-1<<endl;
  #endif

  for(int i=0;i<nMargin;i++)
  {
    if(apv_margin[i]!=fBuf)
      {
	apv_event.clear();
	mpdid = ( buf[ apv_margin[i] ] )&0xff;//cout<<"MPDID:"<<mpdid;
	adc_ch = buf[ apv_margin[i]+1 ] & 0xff;//cout<<"  ADCID:"<<adc_ch<<endl;//ADC_TAG     0x30000000
	for(int k=apv_margin[i]+2; k<apv_margin[i+1]; k++)
	  {
	    if((k-apv_margin[i]-2)%130!=0)
	      {
		word32bit = buf[k]&0xffff;
		apv_event.push_back( (int)word32bit );
	      }
	  }
      }
    mAPVRawSingleEvent[mpdid][adc_ch] = apv_event;
  }
}



//==========================================================================
map<int, map<int, vector<int> > >  RawDecoder::GetDecoded()
{
  return mAPVRawSingleEvent;
}

//===========================================================================
map<int, map<int, TH1F* > > RawDecoder::GetAPVRawHisto()
{
  int mpd_id=0;
  int adc_ch=0;

  map<int, TH1F*> ch_apv;
  map<int, map<int, vector<int> > >::iterator it;
  for(it = mAPVRawSingleEvent.begin(); it!=mAPVRawSingleEvent.end(); ++it)
  {
    mpd_id = it->first;
    map<int, vector<int> > temp = it->second;

    map<int, vector<int> >::iterator itt;
    for(itt=temp.begin(); itt!=temp.end(); ++itt)
    {
      adc_ch = itt->first;
      vector<int> adc_temp = itt->second;

      int N = adc_temp.size();
      //cout<<"adc_tempsize:"<<N<<endl;
      //cout<<"mpdid: "<<mpd_id<<"  adcCh: "<<adc_ch<<endl;
      TH1F* h = new TH1F(Form("mpd_%d_ch_%d",mpd_id, adc_ch), Form("mpd_%d_ch_%d_raw_data",mpd_id, adc_ch), N+128, 0, N+127);
      for(int i=0;i<N;i++)
      {
        h->Fill(i+1, (Float_t) adc_temp[i]);
      }
      mAPVRawHisto[mpd_id][adc_ch] = h;
    }
  }
  return mAPVRawHisto;
}


map<int, map<int, map<int, vector<int> > > > RawDecoder::GetStripTsAdcMap()
{
  int mpd_id=0;
  int adc_ch=0;

  map<int, TH1F*> ch_apv;
  map<int, map<int, vector<int> > >::iterator it;
  for(it = mAPVRawSingleEvent.begin(); it!=mAPVRawSingleEvent.end(); ++it)
  {
    mpd_id = it->first;
    map<int, vector<int> > temp = it->second;

    map<int, vector<int> >::iterator itt;
    for(itt=temp.begin(); itt!=temp.end(); ++itt)
    {
      adc_ch = itt->first;
      vector<int> adc_temp = itt->second;
      //cout<<adc_temp.size()<<endl;//774
      int TSsize=adc_temp.size()/129;
      //cout<<TSsize<<endl;//774
      for(int i=0; i<TSsize;i++)
	{
	  vector<int> singleTSadc_temp;
	  singleTSadc_temp.insert(singleTSadc_temp.end(),&adc_temp[129*i],&adc_temp[129*(i+1)]);
	  //cout<<"singleTSADCSIZE: "<<singleTSadc_temp.size()<<"     ";
	  vector<int> singleTSadc_temp_sorting;
	  singleTSadc_temp_sorting.insert(singleTSadc_temp_sorting.end(),singleTSadc_temp.begin(),singleTSadc_temp.end());
	  sort(singleTSadc_temp_sorting.begin(),singleTSadc_temp_sorting.end()-1);
	  TH1F* h = new TH1F("lala", "lalalla", 3000, 0, 3000);
	  for ( int k=0; k <singleTSadc_temp_sorting.size()-5; k++) //omitting largest 4 channels for common mode, necessary to calculate this from the middle
	     {
	       h->Fill(singleTSadc_temp_sorting[k]);
	      //cout <<singleTSadc_temp[k] << " ";if(k==singleTSadc_temp.size()-1){cout<<endl;}
	    }
	   int iCommonMode = h->GetMean(); //cout<<"commonmode: "<<iCommonMode<<endl;
	   for ( int k=0; k <singleTSadc_temp.size()-1; k++)
	     {
	       singleTSadc_temp[k]-=iCommonMode;
	       //cout<<"commonmode: "<<singleTSadc_temp[k]<<endl;
	     }

	   h->Delete();
	  for(int j=0; j<128;j++)
	    {
	      mPedestalTsAdc[mpd_id][adc_ch][j].push_back(singleTSadc_temp[j]);//singleTSadc_temp[j];
	    }

	}

    }
  }//
  return mPedestalTsAdc;
}
