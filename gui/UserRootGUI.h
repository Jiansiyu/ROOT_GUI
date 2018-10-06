/*
 * UserRootGUI.h
 *
 *  Created on: Sep 20, 2018
 *      Author: newdriver
 */

#ifndef USERROOTGUI_H_
#define USERROOTGUI_H_

#include "TApplication.h"
#include "TGLayout.h"
#include "TG3DLine.h"
#include "TGFrame.h"
#include "TGNumberEntry.h"
#include "TThread.h"
#include "TFile.h"
#include "libgen.h"
#include "string"
#include "iostream"
#include <vector>
#include <algorithm>
#include <chrono>
#include "../root_gui/GUIMainFrame.h"
#include "../root_gui/GUIInformation.h"
#include "../root_gui/ThreadPool.h"
class UserRootGUI: public GUIMainFrame {
public:
	UserRootGUI(const TGWindow *p, UInt_t w, UInt_t h);
	virtual ~UserRootGUI();

protected:
	virtual TGFrame *gWorktabDisplayUnitDraw(TGCompositeFrame *p,TGLayoutHints *l);
	virtual TGFrame *UserWorkTabDisplayTabDraw(TGTab *p,TGLayoutHints *l);
	virtual void gWorkingModePedestal();
	virtual void gWorkingModeAnalysis();
	virtual void gWorkingModeCalibration();
	virtual void UserGUICanvasDraw(GUIInformation *);
	GUIInformation *guiinfor=GUIInformation::GetInstance();
private:
	TGCompositeFrame *fWorkZoneTabDefultFrame;
	std::map<std::string /*tab id*/,TGCompositeFrame *>fWorkZoneTabSubFrame;
	std::map<std::string /*tab id*/,TRootEmbeddedCanvas *>fWorkZoneTabEnbeddedCanvas;
	std::map<std::string ,TCanvas *> cfWorkZoneTabCanvas;
private:
	ThreadPool *THpool=new ThreadPool(std::min((std::thread::hardware_concurrency()!=0?std::thread::hardware_concurrency():12),(unsigned int)12));
};

#endif /* USERROOTGUI_H_ */
