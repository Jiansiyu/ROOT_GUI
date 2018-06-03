/*
 * GEMDataParserM4V.cpp
 *
 *  Created on: May 22, 2018
 *      Author: newdriver
 */
#include <TQObject.h>
#include <RQ_OBJECT.h>

#include "GEMDataParserM4V.h"
#include "datastructureM4V.h"
#include "RawDecoderM4V.h"
#include "../src/GEMStructue.h"

#include <string>
#include <bitset>
#include <numeric>

#include "evioUtil.hxx"
#include "evioFileChannel.hxx"

GEMDataParserM4V::GEMDataParserM4V() {
	// TODO Auto-generated constructor stub
}

GEMDataParserM4V::~GEMDataParserM4V() {
	// TODO Auto-generated destructor stub
}

bool GEMDataParserM4V::OpenFileIn(std::string fname) {
	RawDatfileName = fname;
	ParserRawDat();
//	try {
//
//	} catch (evio::evioException e) {
//	 std::cerr <<e.toString()<<std::endl<<std::endl;
//	 exit(EXIT_FAILURE);
//	}
}
bool GEMDataParserM4V::ParserRawDat(){
	try {
		evio::evioFileChannel chan(RawDatfileName.c_str(),"r");
		chan.open();
		int evtID=0;
		while(chan.read() && evtID <=5000){
			evio::evioDOMTree event(chan);
			evio::evioDOMNodeListP mpdEventList = event.getNodeList( evio::isLeaf() );

			for(auto iter = mpdEventList->begin();
					iter != mpdEventList->end(); iter++){
				vector<uint32_t> *block_vec = (*iter)->getVector<uint32_t>();
				if((*iter)->tag == MPD_tag){
					int iend;
					int vec_size=(*block_vec).size();
					//std::cout <<"Vectsize :"<< vec_size<<"  Eventid : "<<evtID << std::endl;
					for(iend =1 ; iend < vec_size; iend++){
						uint32_t tag = (((*block_vec)[iend])>>24)&0xf8;
						if(tag==0x90||tag==0x88) break;
					}
					RawDecoderM4V *decoder=new RawDecoderM4V(*block_vec, 0, iend);
					EventRaw[evtID]=decoder->GetAPVRawData();
					//delete decoder;
					evtID++;
				}
			}
		}
	} catch (evio::evioException e) {
		std::cerr << e.toString() << std::endl << std::endl;
		exit(EXIT_FAILURE);
	}
}



void PedestalMode(std::string fname,std::string savename){
	try {
		evio::evioFileChannel chan(fname.c_str(),"r");
		chan.open();
		int evtID=0;
		//        MPD          APV         stripID histo
		std::map<int,std::map<int,std::map<int,TH1F*>>> histo_temp;
		while(chan.read()){
			evio::evioDOMTree event(chan);
			evio::evioDOMNodeListP mpdEventList = event.getNodeList( evio::isLeaf() );

			for(auto iter = mpdEventList->begin();
					iter != mpdEventList->end(); iter++){
				vector<uint32_t> *block_vec = (*iter)->getVector<uint32_t>();
				if((*iter)->tag == MPD_tag){
					int iend;
					int vec_size=(*block_vec).size();
					//std::cout <<"Vectsize :"<< vec_size<<"  Eventid : "<<evtID << std::endl;
					for(iend =1 ; iend < vec_size; iend++){
						uint32_t tag = (((*block_vec)[iend])>>24)&0xf8;
						if(tag==0x90||tag==0x88) break;
					}
					RawDecoderM4V *decoder=new RawDecoderM4V(*block_vec, 0, iend);
					 //      MPD          APV             strip  time samples
					std::map<int,std::map<int,std::map<int,std::vector<int>>>> eventdata=decoder->GetStripTsAdcMap();

					for(auto iter_mpd = eventdata.begin();iter_mpd!=eventdata.end();iter_mpd++){
						for(auto iter_apv=iter_mpd->second.begin();iter_apv!=iter_mpd->second.end();iter_apv++){
							for(auto iter_strip=iter_apv->second.begin();iter_strip!=iter_apv->second.end();iter_strip++){
								int average=accumulate(iter_strip->second.begin(),iter_strip->second.end(),0)/(iter_strip->second.size());
								// initialize
								if((histo_temp.find(iter_mpd->first)==histo_temp.end())||
										(histo_temp[iter_mpd->first].find(iter_apv->first)==histo_temp[iter_mpd->first].end())||
										(histo_temp[iter_mpd->first][iter_apv->first].find(iter_strip->first)==histo_temp[iter_mpd->first][iter_apv->first].end())){
									histo_temp[iter_mpd->first][iter_apv->first][iter_strip->first] =
											new TH1F(
													Form(
															"MPD%d_apv%d_strip%d_histo",
															iter_mpd->first,
															iter_apv->first,
															iter_strip->first),
													Form(
															"MPD%d_apv%d_strip%d_histo",
															iter_mpd->first,
															iter_apv->first,
															iter_strip->first),
													4096, 0, 4096);
								}

								histo_temp[iter_mpd->first][iter_apv->first][iter_strip->first]->Fill(average);
							}
						}
					}
					//delete decoder;
					evtID++;
				}
			}

		}
		// finish the raw histo
		// get the pedestal value
		// mpd    apv		strips histo
		std::map<int,std::map<int,TH1F*>> PedetalMeanHisto;
		std::map<int,std::map<int,TH1F*>> PedetalRMSHisto;
		for(auto iter_mpd=histo_temp.begin();iter_mpd!=histo_temp.end();iter_mpd++){
			for(auto iter_apv=iter_mpd->second.begin();iter_apv!=iter_mpd->second.end();iter_apv++){
				if((PedetalMeanHisto.find(iter_mpd->first)==PedetalMeanHisto.end())||
						(PedetalMeanHisto[iter_mpd->first].find(iter_apv->first)==PedetalMeanHisto[iter_mpd->first].end())){
					PedetalMeanHisto[iter_mpd->first][iter_apv->first] =
							new TH1F(
									Form("PedestalMean(offset)_mpd_%d_ch_%d",
											iter_mpd->first, iter_apv->first),
									Form("PedestalMean(offset)_mpd_%d_ch_%d",
											iter_mpd->first, iter_apv->first),
									128, 0, 128);
					PedetalRMSHisto[iter_mpd->first][iter_apv->first] =
							new TH1F(
									Form("PedestalRMS_mpd_%d_ch_%d",
											iter_mpd->first, iter_apv->first),
									Form("PedestalRMS_mpd_%d_ch_%d",
											iter_mpd->first, iter_apv->first),
									128, 0, 128);
				}
				for(auto iter_strip=iter_apv->second.begin();iter_strip!=iter_apv->second.end();iter_strip++){
					PedetalMeanHisto[iter_mpd->first][iter_apv->first]->Fill(iter_strip->first,iter_strip->second->GetMean());
					PedetalRMSHisto[iter_mpd->first][iter_apv->first]->Fill(iter_strip->first,iter_strip->second->GetRMS());
				}

			}
		}
		// save the RMS
		TFile *fileio=new TFile(savename.c_str(),"RECREATE");
		for(auto iter_mpd=PedetalMeanHisto.begin();iter_mpd!=PedetalMeanHisto.end();iter_mpd++){
			for(auto iter_apv=iter_mpd->second.begin();iter_apv!=iter_mpd->second.end();iter_apv++){
				iter_apv->second->Write();
			}
		}
		for(auto iter_mpd=PedetalRMSHisto.begin();iter_mpd!=PedetalRMSHisto.end();iter_mpd++){
			for(auto iter_apv=iter_mpd->second.begin();iter_apv!=iter_mpd->second.end();iter_apv++){
				iter_apv->second->Write();
			}
		}
		fileio->Write();
		fileio->Close();

	} catch (evio::evioException e) {
		std::cerr << e.toString() << std::endl << std::endl;
		exit(EXIT_FAILURE);
	}
}

// decode hit mode
void GEMDataParserM4V::HitMode(std::string fname, std::string outfile){
	try {
		evio::evioFileChannel chan(RawDatfileName.c_str(),"r");
		chan.open();
		int evtID=0;
		while(chan.read()){
			evio::evioDOMTree event(chan);
			evio::evioDOMNodeListP mpdEventList = event.getNodeList( evio::isLeaf() );

			for(auto iter = mpdEventList->begin();
					iter != mpdEventList->end(); iter++){
				vector<uint32_t> *block_vec = (*iter)->getVector<uint32_t>();
				if((*iter)->tag == MPD_tag){
					int iend;
					int vec_size=(*block_vec).size();
					//std::cout <<"Vectsize :"<< vec_size<<"  Eventid : "<<evtID << std::endl;
					for(iend =1 ; iend < vec_size; iend++){
						uint32_t tag = (((*block_vec)[iend])>>24)&0xf8;
						if(tag==0x90||tag==0x88) break;
					}
					RawDecoderM4V *decoder=new RawDecoderM4V(*block_vec, 0, iend);
					//TODO


					evtID++;
				}
			}
		}
	} catch (evio::evioException e) {
		std::cerr << e.toString() << std::endl << std::endl;
		exit(EXIT_FAILURE);
	}
}
