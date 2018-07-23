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
#include <string>
#include <math.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TVector2.h>
#include <TVector3.h>
struct GUICanvasDataStream{
public:
	GUICanvasDataStream(){
	};
	TVector2 GetCanvasDivided(){

	};
	void LoadData(TH1F ***(*data)){

	}
	void LoadData(TH2F ***(*data)){

	}
	void LoadData(std::vector<int/**/,std::vector<TH1F *>> data){

	}
	void LoadData(std::vector<int/**/,std::vector<TH2F *>> data){

	}
	void LoadData(std::map<int/*tab*/,std::map<int /*x*/,std::map<int/*y*/,TH1F *>>> data){

	}
	void LoadData(std::map<int/*tab*/,std::map<int /*x*/,std::map<int/*y*/,TH2F *>>> data){

	}

private:
	std::map<int/*tab*/,std::vector<TH1F*>> histo_1h;
	std::map<int/*tab*/,std::vector<TH2F*>> histo_2h;
	std::map<int/*tab*/,std::map<int /*x*/,std::map<int/*y*/,TH1F *>>> histo_1d_arry;
	std::map<int/*tab*/,std::map<int/*x*/,std::map<int/*y*/,TH1F*>>> histo_2d_arry;
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
	void StatusBarDisplay(GUIStatusBarDataStream *);
	void CanvasTabDisplay(GUICanvasDataStream *);
	void MainTabDisplay(GUIMainTabDataStream *);

private:
	void GUICanvasTabDraw(GUICanvasDataStream *); //*SIGNAL*
	void GUIMainTabDraw(GUIMainTabDataStream *);	  //*SIGNAL*
	void GUIStatusBarDraw(GUIStatusBarDataStream *); //*SIGNAL*
};


#endif /* GUIINFORCENTER_H_ */
