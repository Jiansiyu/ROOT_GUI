/*
 * UserRootGUI.cpp
 *
 *  Created on: Sep 20, 2018
 *      Author: newdriver
 */
#include <algorithm>
#include "UserRootGUI.h"
#include "../root_gui/GUIMainFrame.h"
#include "../root_gui/ThreadPool.h"
#include "../GEMDetector/MPDDecoder.h"
#include "../analysis/GEMAnalysis.h"
//#include "../analysis/GEMAnalysis.h"
UserRootGUI::UserRootGUI(const TGWindow *p, UInt_t w, UInt_t h):GUIMainFrame()
{
	// TODO Auto-generated constructor stub
	SetCleanup(kDeepCleanup);
	gMenuUnitDraw(new TGLayoutHints(kLHintsExpandX,0,0,1,0));
	gWorktabUnitDraw(new TGLayoutHints(kLHintsRight|kLHintsExpandX|kLHintsExpandY));
	gStatusUnitDraw(new TGLayoutHints(kLHintsLeft|kLHintsTop|kLHintsExpandY));
	SetWindowName("UVa GEM Analysis Framework");
	MapSubwindows();
	Resize();   //resize to default size
	MapWindow();
}

UserRootGUI::~UserRootGUI() {
	// TODO Auto-generated destructor stub
}

TGFrame *UserRootGUI::gWorktabDisplayUnitDraw(TGCompositeFrame *p,TGLayoutHints *l){
	TGTab *tWorkZoneTab=new TGTab(p);
	p->AddFrame(UserWorkTabDisplayTabDraw(tWorkZoneTab,l),new TGLayoutHints(kLHintsTop|kLHintsExpandY|kLHintsExpandX));
	return p;
}

TGFrame *UserRootGUI::UserWorkTabDisplayTabDraw(TGTab *p,TGLayoutHints *l){
	fWorkZoneTabDefultFrame=p->AddTab("WorkStatus");
	fWorkZoneTabDefultFrame->AddFrame(gSysInforTabDraw(fWorkZoneTabDefultFrame,new TGLayoutHints(kLHintsExpandY|kLHintsExpandX)),l);
	p->AddFrame(fWorkZoneTabDefultFrame,l);
	for(auto tabid:guiinfor->GetWorkZoneTabList()){
		fWorkZoneTabSubFrame[tabid]=p->AddTab(tabid.c_str());
		fWorkZoneTabEnbeddedCanvas[tabid]=new TRootEmbeddedCanvas(tabid.c_str(),fWorkZoneTabSubFrame[tabid],600,600);
		fWorkZoneTabEnbeddedCanvas[tabid]->GetCanvas()->SetBorderMode(0);
		fWorkZoneTabEnbeddedCanvas[tabid]->GetCanvas()->SetGrid();
		fWorkZoneTabSubFrame[tabid]->AddFrame(fWorkZoneTabEnbeddedCanvas[tabid],new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
		p->AddFrame(fWorkZoneTabSubFrame[tabid],new TGLayoutHints(kLHintsTop|kLHintsExpandY|kLHintsExpandX));
	}
	return p;
}

void UserRootGUI::UserGUICanvasDraw(GUIInformation *infor){

}

void UserRootGUI::gWorkingModePedestal(void){
	MPDDecoder *decoder=new MPDDecoder();
	std::vector<std::string> flist=guiinfor->GetRawFileInputList();
	std::string fname="test.root";
	std::cout<<"Main thread :"<<std::this_thread::get_id()<<std::endl;
	//decoder->PedestalMode(flist,fname);
	try{
	using func_type=void (MPDDecoder::*)(std::vector<std::string>,std::string);
	func_type func=&MPDDecoder::PedestalMode;
	THpool->enqueue(func,decoder,flist,fname);
	}catch (exception e) {
		std::cout<<e.what()<<std::endl;
	}
}

void UserRootGUI::gWorkingModeAnalysis(){
	for(auto fname: guiinfor->GetRawFileInputList()){
		GEMAnalysis *ana=new GEMAnalysis();
		ana->Analysis(fname.c_str());
		delete ana;
	}
}

void UserRootGUI::gWorkingModeCalibration(){
	for(auto fname: guiinfor->GetRawFileInputList()){
			GEMAnalysis *ana=new GEMAnalysis();
			ana->Analysis(fname.c_str());
			delete ana;
		}
}
