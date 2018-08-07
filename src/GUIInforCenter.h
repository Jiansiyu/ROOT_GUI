/*
 * GUIInforCenter.h
 *
 *  Created on: May 25, 2018
 *      Author: newdriver
 */

#ifndef GUIINFORCENTER_H_
#define GUIINFORCENTER_H_
#include <TQObject.h>
#include <RQ_OBJECT.h>

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <math.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TVector2.h>
#include <TVector3.h>
#include "../GEMDetector/GEMIDGenerator.h"
struct GUICanvasDataStream{
public:
	GUICanvasDataStream(){
	};
	TVector2 GetCanvasDivied(){

		return size;
	}
	const unsigned int GetCanvasNumber(){
		return histo_1h_arry.size();
	}
	std::map<int/*tab*/,std::vector<TH1F*>> GetHisto1D(){
		generateHisto();
		return histo_1h;
	}
	const std::map<int/*tab*/,std::map<int /*x*/,std::map<int/*y*/,TH1F *>>> GetHisto1dArray(){
		return histo_1h_arry;
	}
	std::map<int/**/,std::vector<int>> GetRaw(){
		return data;
	}
	void LoadData(const std::map<int/**/,std::vector<int>> &data){
		clear();
		this->data=data;
		//generateHisto(data);
	}
	void LoadData(std::map<int, std::map<int,std::vector<TH1F *>>> const data){
		clear();
		this->canvasedDisplay=data;
	}

	void generateHisto(){
		if(data.size()!=0) RawGenerateHisto();
		if(canvasedDisplay.size())ZeroSubGenerateHisto();
	}
private:
	void RawGenerateHisto(){
		std::map<int,std::vector<int>> rawdata = this->data;
		std::map<int,std::vector<std::vector<int>>> tabrawdata;
		for(auto apv = rawdata.begin();apv!=rawdata.end();apv++){
			int crateid=GEM::getCrateID(apv->first);
			int mpdid=GEM::getMPDID(apv->first);
			int id=GEM::GetUID(crateid,mpdid,0,0);
			tabrawdata[id].push_back(apv->second);
		}

		std::map<int, std::vector<TH1F *>> tabHistoArray;

		for(auto mpd_iter = tabrawdata.begin();mpd_iter!=tabrawdata.end();mpd_iter++){
				int tabcanvasid=mpd_iter->first;
				{
					int i =0;
					for(auto apv : mpd_iter->second){
									TH1F *h = new TH1F(
											Form("crate%d_mpd%", GEM::getCrateID(tabcanvasid),
													GEM::getMPDID(tabcanvasid)),
											Form("crate%d_mpd%", GEM::getCrateID(tabcanvasid),
													GEM::getMPDID(tabcanvasid)), 800, 0, 800);
									for(int i = 0; i <apv.size();i++){
											h->Fill(i+1,apv.at(i));
											h->GetYaxis()->SetRangeUser(0,3000);
											h->SetYTitle("ADC");
											h->SetXTitle("channel");
													}
									histo_1h[tabcanvasid].push_back(h);
									histo_1h_arry[tabcanvasid][i%4][i/4]=h;//->Clone("test");
									histo_1h_arry[tabcanvasid][i%4][i/4]->SetTitle(Form("crate%d_mpd%", GEM::getCrateID(tabcanvasid),
											GEM::getMPDID(tabcanvasid)));
									i++;
					}
				}
		}
		size.Set(4.0,4.0);
	}
	void ZeroSubGenerateHisto(){
		histo_1h_arry.clear();
		for (auto tab = canvasedDisplay.begin(); tab != canvasedDisplay.end(); tab++) {
			for (auto dimension = tab->second.begin();
					dimension != tab->second.end(); dimension++) {
				for (unsigned int i = 0; i < (dimension->second).size(); i++) {
					histo_1h_arry[tab->first][dimension->first][i] =
							dimension->second.at(i);
				}
			}
		}
		size.Set(2., 4.);
	}
private:
	TVector2 size;
	std::map<int/**/,std::vector<int>> data;
	std::map<int/*tab*/,std::vector<TH1F*>> histo_1h;
	std::map<int/*tab*/,std::map<int /*x*/,std::map<int/*y*/,TH1F *>>> histo_1h_arry;
	std::map<int, std::map<int,std::vector<TH1F *>>> canvasedDisplay;

	void clear(){
		data.clear();
		histo_1h.clear();
		histo_1h_arry.clear();
		canvasedDisplay.clear();

	}
};

struct GUIStatusBarDataStream{
	int processbar=0;
	std::string tab2;
};

struct GUIMainTabDataStream{

};

class GUIInforCenter {

	RQ_OBJECT("GUIInforCenter");

public:
	GUIInforCenter();
	virtual ~GUIInforCenter();
	void SetTab();            // redraw the tab according to the mapping
	void StatusBarDisplay(GUIStatusBarDataStream *stream){
		Emit("GUIStatusBarDraw(GUIStatusBarDataStream *)",stream);

	}
	void CanvasTabDisplay(GUICanvasDataStream *stream){
		Emit("GUICanvasTabDraw(GUICanvasDataStream *)",stream);

	}
	void MainTabDisplay(GUIMainTabDataStream *stream){
		Emit("GUIMainTabDraw(GUIMainTabDataStream *)",stream);
	}

private:
	void GUICanvasTabDraw(GUICanvasDataStream *);     //*SIGNAL*
	void GUIMainTabDraw(GUIMainTabDataStream *);	  //*SIGNAL*
	void GUIStatusBarDraw(GUIStatusBarDataStream *);  //*SIGNAL*
//	ClassDef(GUIInforCenter,0);
};


#endif /* GUIINFORCENTER_H_ */
