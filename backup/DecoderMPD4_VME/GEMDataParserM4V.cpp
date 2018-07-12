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
#include "../src/GEMInforCenter.h"
#include "../src/Benchmark.h"

#include <string>
#include <bitset>
#include <numeric>
#include <unordered_map>
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



void GEMDataParserM4V::PedestalMode(std::string fname,std::string savename){

	try {
		evio::evioFileChannel chan(fname.c_str(),"r");
		chan.open();
		int evtID=0;
		//        MPD          APV         stripID histo
		std::map<int,std::map<int,std::map<int,TH1F*>>> histo_temp;

		RawDecoderM4V *decoder=new RawDecoderM4V();

		while(chan.read()){

			Benchmark* time_elapse=new Benchmark();

			evio::evioDOMTree event(chan);
			evio::evioDOMNodeListP mpdEventList = event.getNodeList( evio::isLeaf() );
			for(auto iter = mpdEventList->begin();
					iter != mpdEventList->end(); iter++){

				vector<uint32_t> *block_vec = (*iter)->getVector<uint32_t>();

				if((*iter)->tag == MPD_tag){
					int iend;
					int vec_size=(*block_vec).size();

					for(iend =1 ; iend < vec_size; iend++){
						uint32_t tag = (((*block_vec)[iend])>>24)&0xf8;
						if(tag==0x90||tag==0x88) break;
					}

					//RawDecoderM4V *decoder=new RawDecoderM4V(*block_vec, 0, iend);
					decoder->RegistBuffer(*block_vec, 0, iend);
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

					evtID++;
					if(evtID%100==0) std::cout<<"Event :"<<evtID<<" Time spend(ms/event):"<<(time_elapse->GetElapaedTime()/100)<<std::endl;
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

// load the pedestal file, and return the pedestals
std::map<int,std::map<int,std::map<std::string,std::vector<int>>>> GEMDataParserM4V::LoadPedestal(std::string fname){
	std::map<int,std::map<int,std::map<std::string,std::vector<int>>>> data_return;

	TFile *fileio=new TFile(fname.c_str(),"READ");
	// searching for the mapping list, and check whether the histogram have the corresponding data
	GEMInforCenter *geminfor=GEMInforCenter::GetInstance();
	for(auto mpd : geminfor->GetGEMdetectorMap().GetMPDList()){
		for(auto apv : mpd.GetAPVs()){
			// check whether the MPD and apv

			if((fileio->GetListOfKeys()->Contains(Form("PedestalMean(offset)_mpd_%d_ch_%d",mpd.GetMPDID(),apv.GetADCid())))&&
					(fileio->GetListOfKeys()->Contains(Form("PedestalRMS_mpd_%d_ch_%d",mpd.GetMPDID(),apv.GetADCid())))){
				TH1F *hmean=(TH1F*) fileio->Get(Form("PedestalMean(offset)_mpd_%d_ch_%d",mpd.GetMPDID(),apv.GetADCid()));
				TH1F *hrms =(TH1F*) fileio->Get(Form("PedestalRMS_mpd_%d_ch_%d",mpd.GetMPDID(),apv.GetADCid()));
				for(int i =0; i < 128 ; i++){
					data_return[mpd.GetMPDID()][apv.GetADCid()]["mean"].push_back(hmean->GetBinContent(i+1));
					data_return[mpd.GetMPDID()][apv.GetADCid()]["rms"].push_back(hrms->GetBinContent(i+1));
				}
			} else {
				std::cout << "[WORNING] " << __FUNCTION__ << " " << __LINE__
						<< "MPD_" << mpd.GetMPDID() << " APV_" << apv.GetADCid()
						<< " is declared in the map, but cannot file in the pedestal file"
						<< std::endl;
			}
		}
	}
	return data_return;
}

// decode hit mode
void GEMDataParserM4V::HitMode(std::string fname, std::string pedestal_name,std::string outfile){
	RawDatfileName=fname;
	//       MPD         APV           iterm/ mean/rms        value
	std::map<int,std::map<int,std::map<std::string,std::vector<int>>>> pedestal=LoadPedestal(pedestal_name.c_str());

	for(auto iter_mpd=pedestal.begin();iter_mpd!=pedestal.end();iter_mpd++){
		for(auto iter_apv=iter_mpd->second.begin();iter_apv!=iter_mpd->second.end();iter_apv++){
			std::cout<<"Map file Enabled :"<<std::endl
					<<"MPD		APV"<<std::endl
					<<iter_mpd->first<<"		"<<iter_apv->first<<std::endl;
		}
	}

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

					//      MPD          APV             strip  time samples
					// this is the data after common mode subtraction
					std::map<int,std::map<int,std::map<int,std::vector<int>>>> eventdata=decoder->GetStripTsAdcMap();

					std::map<int,std::map<int,std::map<int,std::vector<int>>>> data_zeroSubtr; // used for save the data after zero subtraction

					// zero subtraction algorithm
					for(auto iter_mpd = eventdata.begin(); iter_mpd!=eventdata.end();iter_mpd++){
						for(auto iter_apv = iter_mpd->second.begin();iter_apv!=iter_mpd->second.end();iter_apv++){
#ifdef __DEBUG_FLAG
							if(iter_apv->second.size()==128){
								std::cout<<"[Debug]"<<__FUNCTION__<<" The frame size match the expectation"<<std::endl;
							}else{
								std::cout<<"[Debug]"<<__FUNCTION__<<" The frame size DOES NOT match the expectation"<<std::endl;
							}
#endif
							// TODO
							// compare each strips with the pedestal
							if (pedestal.find(iter_mpd->first) != pedestal.end()
									&& (pedestal[iter_mpd->first].find(
											iter_apv->first)
											!= pedestal[iter_mpd->first].end())) {
								// subtract the mean
								// loop on the strips
								for(auto iter_strip=iter_apv->second.begin();iter_strip!=iter_apv->second.end();iter_strip++){
									if(iter_strip->first > pedestal[iter_mpd->first][iter_apv->first]["mean"].size()){
										std::cerr<<"[Fatal]  : Can NOT find Channel "<<iter_strip->first<<" in the pedestal file !!!"<<std::endl;
										exit(EXIT_FAILURE);
									}
									int channel_sum=0;
									for(auto tsample : iter_strip->second){
										channel_sum=channel_sum+tsample-pedestal[iter_mpd->first][iter_apv->first]["mean"][iter_strip->first];
									}

									//5 sigma cut
									if((channel_sum/(iter_strip->second.size()))>=5 * pedestal[iter_mpd->first][iter_apv->first]["mean"][iter_strip->first]){
										// feed the data after zero subtraction into the out data buffer
										data_zeroSubtr[iter_mpd->first][iter_apv->first][iter_strip->first]=iter_strip->second;
									}
								}

							} else {
								if(evtID%100==0)

								std::cout << "[Worning] MPD." << iter_mpd->first
										<< " APV." << iter_apv->first
										<< " Can NOT find pedestals, skip this cards"
										<<"	Working on event "<<evtID<< std::endl;
							}
						}
					}
					// after zero subtraction
					// TODO this is the right place to add the display mode of the zero subtraction
					evtID++;
				}
			}
		}
	} catch (evio::evioException e) {
		std::cerr << e.toString() << std::endl << std::endl;
		exit(EXIT_FAILURE);
	}


}



void GEMDataParserM4V::InitRootFile(){
	  Vstrip   = new Int_t[20000];
	  VdetID   = new Int_t[20000];
	  VplaneID = new Int_t[20000];
	  adc0     = new Int_t[20000];
	  adc1     = new Int_t[20000];
	  adc2     = new Int_t[20000];
	  adc3     = new Int_t[20000];
	  adc4     = new Int_t[20000];
	  adc5     = new Int_t[20000];
	  fCH      = new Int_t[100];
	  ftiming  = new Int_t[100];
	  fadc     = new Int_t[100];
	  tCH      = new Int_t[100];
	  ttiming  = new Double_t [100];

	  Hit      = new TTree("GEMHit","Hit list");

	  //-------------GEM----------------------
	  Hit->Branch("evtID",&EvtID,"evtID/I");
	  Hit->Branch("nch",&nch,"nch/I");
	  Hit->Branch("strip",Vstrip,"strip[nch]/I");
	  Hit->Branch("detID",VdetID,"detID[nch]/I");
	  Hit->Branch("planeID",VplaneID,"planeID[nch]/I");
	  Hit->Branch("adc0",adc0,"adc0[nch]/I");
	  Hit->Branch("adc1",adc1,"adc1[nch]/I");
	  Hit->Branch("adc2",adc2,"adc2[nch]/I");
	  Hit->Branch("adc3",adc3,"adc3[nch]/I");
	  Hit->Branch("adc4",adc4,"adc4[nch]/I");
	  Hit->Branch("adc5",adc5,"adc5[nch]/I");
	}
