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
//#include "../GEMDetector/GEMIDGenerator.h"

struct GUICanvasDataStream{
public:
	GUICanvasDataStream(){
	};
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
public:
	ClassDef(GUIInforCenter,0);
};


#endif /* GUIINFORCENTER_H_ */
