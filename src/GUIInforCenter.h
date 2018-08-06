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
		TVector2 size(histo_1h_arry.begin()->second.size(),histo_1h_arry.begin()->second.begin()->second.size());
		return size;
	}
	const unsigned int GetCanvasNumber(){
		return histo_1h_arry.size();
	}
	std::map<int/*tab*/,std::vector<TH1F*>> GetHisto1D(){
		generateHisto(data);
		return histo_1h;
	}
	std::map<int/**/,std::vector<int>> GetRaw(){
		return data;
	}
	void LoadData(const std::map<int/**/,std::vector<int>> &data){
		this->data=data;
	}
private:
	void generateHisto(const std::map<int/**/,std::vector<int>> &data){
		// tab should be the same number of tabs
		for (auto apv = data.begin(); apv != data.end(); apv++) {
			int crateid = GEM::getCrateID(apv->first);
			int mpdid = GEM::getMPDID(apv->first);
			int adcid = GEM::getADCID(apv->first);

			histo_1h[GEM::GetUID(crateid, mpdid, 0, 0)].push_back(new TH1F(Form("mpd%d_apv%d", mpdid, adcid),
					Form("mpd%d_apv%d", mpdid, adcid), 800, 0, 800));
			std::vector<int> data = apv->second;
			for (int i = 0; i < data.size(); i++) {
				histo_1h[GEM::GetUID(crateid, mpdid, 0, 0)].back()->Fill(i + 1, data.at(i));
			}
		}
		// fill into the map
		for(auto mpd  = histo_1h.begin(); mpd!=histo_1h.end();mpd++){
			for(int histo_id=0;histo_id<(mpd->second.size());histo_id++){
				histo_1h_arry[mpd->first][histo_id/4][histo_id%4]=(mpd->second.at(histo_id));
			}
		}
	}
private:

	std::map<int/**/,std::vector<int>> data;
	std::map<int/*tab*/,std::vector<TH1F*>> histo_1h;
	std::map<int/*tab*/,std::vector<TH2F*>> histo_2h;
	std::map<int/*tab*/,std::map<int /*x*/,std::map<int/*y*/,TH1F *>>> histo_1h_arry;
	std::map<int/*tab*/,std::map<int/*x*/,std::map<int/*y*/,TH1F*>>> histo_2h_arry;

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
