/*******************************************************************************************
 * Usage:
 *
 * Input:  1), A buffer contains purely SRS data, and the buffer size in [int] unit
 *
 *         2), Or, SRS data filled in a vector<int>
 *
 * Output: 1), map<mpd_id, map<ch_id, vector<int> > >, vector: adc values (all time samples)
 *                map<int, map<int, vector<int> > > GetDecoded();
 *
 *         2), map<mpd_id, map<ch_id, TH1F* > > , TH1F*: adc values filled in histograms
 *             No need to worry memory leakage, this class owns every object it produced.
 *                map<int, map<int, TH1F* > > GetAPVRawHisto();
 * *****************************************************************************************/

#ifndef __RAW_DECODER_H__
#define __RAW_DECODER_H__

#include <map>
#include <vector>

#include <TH1F.h>

using namespace std;

class RawDecoder
{
public:
  RawDecoder( vector<int> );
  RawDecoder( unsigned int *, int);
  ~RawDecoder();

private:
  unsigned int * buf;
  int fBuf;
  map<int, map<int, vector<int> > > mAPVRawSingleEvent;
  map<int, map<int, TH1F* > > mAPVRawHisto;


public:
  void Decode();
  map<int, map<int, map<int, vector<int> > > > GetStripTsAdcMap();
  map<int, map<int, vector<int> > > GetDecoded();
  map<int, map<int, map<int, vector<int> > > > mPedestalTsAdc;
  map<int, map<int, TH1F* > > GetAPVRawHisto();

};

#endif
