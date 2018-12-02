/*
 * UserRootGUI.cpp
 *
 *  Created on: Sep 20, 2018
 *      Author: newdriver
 */
#include <algorithm>
#include "stdio.h"
#include "UserRootGUI.h"
#include "TThread.h"
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

void UserRootGUI::gMessageProcessRadioButton(Long_t msg, Long_t parm){
	switch (parm) {
		case C_WORKMODE_RAW:
			guiinfor->SetRunMode(WORKMODE_RAW);
			std::cout<<"Raw mode selected"<<std::endl;
			UserWorkTabDisplayTabDraw();
			GUIUpdata();
			break;
		case C_WORKMODE_ZEROSUBTRACTION:
			std::cout<<"Zero Subtraction mode selected"<<std::endl;
			guiinfor->SetRunMode(WORKMODE_ZERSUBTRACTION);
			UserWorkTabDisplayTabDraw();
			break;
		case C_WORKMODE_PEDESTAL:
			std::cout<<"Pedestal Mode selected"<<std::endl;
			guiinfor->SetRunMode(WORKMODE_PEDESTAL);
			UserWorkTabDisplayTabDraw();
			break;
		case C_WORKMODE_HIT:
			std::cout<<"Pedestal Mode selected"<<std::endl;
			guiinfor->SetRunMode(WORKMODE_HIT);
			UserWorkTabDisplayTabDraw();
			break;
		case C_WORKMODE_ANALYSIS:
			std::cout<<"Analysis mode selected" <<std::endl;
			guiinfor->SetRunMode(WORKMODE_ANALYSIS);
			UserWorkTabDisplayTabDraw();
			break;
		case C_WORKMODE_CALIBRATION:
			std::cout<<"Calibration mode selected" <<std::endl;
			guiinfor->SetRunMode(WORKMODE_CALIBRATION);
			UserWorkTabDisplayTabDraw();
			break;
		default:
			std::cout<<"Command currently not support"<<std::endl;
			break;
	}

}

TGFrame *UserRootGUI::gWorktabDisplayUnitDraw(TGCompositeFrame *p,TGLayoutHints *l){
	tWorkZoneTab=new TGTab(p);
	UserWorkTabDisplayTabDraw();
	p->AddFrame(tWorkZoneTab,new TGLayoutHints(kLHintsTop|kLHintsExpandY|kLHintsExpandX));
	return p;
}
TGCompositeFrame *UserRootGUI::gSysInforTabDraw(TGCompositeFrame *p, TGLayoutHints *l){
	TGVerticalFrame *gWorkZoneMainFrame=new TGVerticalFrame(p);
	TGHorizontalFrame *gWorkStatusSubFrame=new TGHorizontalFrame(gWorkZoneMainFrame);
	TGHorizontal3DLine *gSeperateLine=new TGHorizontal3DLine(gWorkZoneMainFrame);
	gWorkZoneMainFrame->AddFrame(gWorkStatusSubFrame,new TGLayoutHints(kLHintsTop|kLHintsCenterY|kLHintsRight|kLHintsCenterX));
	gWorkZoneMainFrame->AddFrame(gSeperateLine,new TGLayoutHints(kLHintsTop|kLHintsExpandX));
	return gWorkZoneMainFrame;
}
TGFrame *UserRootGUI::UserWorkTabDisplayTabDraw(TGTab *p,TGLayoutHints *l){

}

TGFrame *UserRootGUI::UserWorkTabDisplayTabDraw(){
	fWorkZoneTabSubFrame.clear();
	fWorkZoneTabEnbeddedCanvas.clear();
	cfWorkZoneTabCanvas.clear();
	auto tabNumber=tWorkZoneTab->GetNumberOfTabs();
	for(int i = 0 ; i < tabNumber ; i ++){
		tWorkZoneTab->RemoveTab(0);
	}
	fWorkZoneTabDefultFrame=tWorkZoneTab->AddTab("Status");
	fWorkZoneTabDefultFrame->AddFrame(gSysInforTabDraw(fWorkZoneTabDefultFrame,new TGLayoutHints(kLHintsTop|kLHintsExpandY|kLHintsExpandX)),new TGLayoutHints(kLHintsTop|kLHintsExpandY|kLHintsExpandX));
	auto tablist=guiinfor->GetWorkZoneTabList(guiinfor->GetRunMode());
	for(auto tabid=tablist.begin();tabid!=tablist.end();tabid++){
		fWorkZoneTabSubFrame[tabid->first]=tWorkZoneTab->AddTab((tabid->second).c_str());
		fWorkZoneTabEnbeddedCanvas[tabid->first]=new TRootEmbeddedCanvas("Canvas", fWorkZoneTabSubFrame[tabid->first], 600,600);
		fWorkZoneTabEnbeddedCanvas[tabid->first]->GetCanvas()->SetBorderMode(0);
		fWorkZoneTabEnbeddedCanvas[tabid->first]->GetCanvas()->SetGrid();
		fWorkZoneTabSubFrame[tabid->first]->AddFrame(fWorkZoneTabEnbeddedCanvas[tabid->first],new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
		cfWorkZoneTabCanvas[tabid->first]=fWorkZoneTabEnbeddedCanvas[tabid->first]->GetCanvas();
	}
	GUIUpdata();
	return tWorkZoneTab;
}


void UserRootGUI::UserGUICanvasDraw(GUIInformation *infor){

}

void UserRootGUI::gWorkingModePedestal(void){
/*	MPDDecoder *decoder=new MPDDecoder();
	std::vector<std::string> flist=guiinfor->GetRawFileInputList();
	std::string fname="test.root";
	std::cout<<"Main thread :"<<std::this_thread::get_id()<<std::endl;

	try{
	using func_type=void (MPDDecoder::*)(std::vector<std::string>,std::string);
	func_type func=&MPDDecoder::PedestalMode;
	THpool->enqueue(func,decoder,flist,fname);
	}catch (exception e) {
		std::cout<<e.what()<<std::endl;
	}*/
}

void UserRootGUI::gWorkingModeAnalysis(){
/*	for(auto fname: guiinfor->GetRawFileInputList()){
		GEMAnalysis *ana=new GEMAnalysis();
		ana->Analysis(fname.c_str());
		delete ana;
	}*/
}

void UserRootGUI::gWorkingModeCalibration(){
	/*for(auto fname: guiinfor->GetRawFileInputList()){
			GEMAnalysis *ana=new GEMAnalysis();
			ana->TrainingData(fname.c_str());
			delete ana;
		}*/
}
