/*
 * GUIMainFrame.cpp
 *
 *  Created on: Sep 10, 2018
 *  Author: newdriver
 */

#include "GUIMainFrame.h"
#include "TApplication.h"
#include "TGLayout.h"
#include "TG3DLine.h"
#include "TGFrame.h"
#include "TGNumberEntry.h"
#include "TThread.h"
#include <TGSimpleTable.h>

#include "TFile.h"
#include "libgen.h"
#include "string"
#include "iostream"

//#include "GUIStructure.h"
#include "GUISysGeneral.h"


//#include "../GEMDetector/MPDDecoder.h"

#include "TTree.h"


GUIMainFrame::GUIMainFrame(const TGWindow *p, UInt_t w, UInt_t h) : TGMainFrame(p, w, h) {
	SetCleanup(kDeepCleanup);
	gMenuUnitDraw(new TGLayoutHints(kLHintsExpandX,0,0,1,0));
	gWorktabUnitDraw(new TGLayoutHints(kLHintsRight|kLHintsExpandX|kLHintsExpandY));
	gStatusUnitDraw(new TGLayoutHints(kLHintsLeft|kLHintsTop|kLHintsExpandY));
	SetWindowName("UVa GEM Analysis Framework");
	MapSubwindows();
	Resize();   //resize to default size
	MapWindow();
}

GUIMainFrame::~GUIMainFrame() {
	// TODO Auto-generated destructor stub
}

Bool_t GUIMainFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t) {
	switch (GET_MSG(msg)) {
		case kC_COMMAND:             // button command
			{
				switch (GET_SUBMSG(msg)) {
					case kCM_MENU:
						gMessageProcessMenu(msg,parm1);
						break;
					case kCM_BUTTON:
						gMessageProcessButton(msg,parm1);
						break;
					case kCM_RADIOBUTTON:
						gMessageProcessRadioButton(msg,parm1);
					default:
						break;
				}
			};
			break;
		case kCM_CHECKBUTTON:         // number entry
			gMessageProcessCheckButton(msg,parm1);
			break;
		case kC_COLORSEL:			  // color ring
			gMessageProcessColorSel(msg,parm1);
			break;
		default:
			break;
	}
	return kTRUE;
};

// button process
void GUIMainFrame::gMessageProcessButton(Long_t msg, Long_t parm){
	GUISysGeneral *openDialog=new GUISysGeneral();
	switch (parm) {
		case B_PEDESTALFILE_OPEN:
			{
				std::vector<std::string> files=openDialog->FilesBrowser();
				if(files.size()==0) break;
				if(files.size()>1) std::cout<<"Pedestal file only support one file, only "<<files.front().c_str()<<"  will be saved"<<std::endl;
				guiinfor->SetPedestalInputFile(files.front().c_str());
				cUpdateWorkTabControlPedestalInputBox();
			}
			break;
		case B_RAWFILE_OPEN:
			{
				std::vector<std::string> files=openDialog->FilesBrowser();
				if(files.size()==0) break;
				guiinfor->SetRawFileInputList_add(files);
				cUpdateWorkTabControlRawInputBox();

			}
			break;
		case B_RAWFILE_DELETE:
			{
				std::cout<<"Raw delete Button"<<std::endl;
				guiinfor->SetRawFileInputList_remove(cGetSelectedWorkTabControlRawInputBox().c_str());
				cUpdateWorkTabControlRawInputBox();
			}
			break;
		case B_CONFIRM:
			{
				std::cout<<"Confirm Button"<<std::endl;
				switch (guiinfor->GetRunMode()) {
					case WORKMODE_PEDESTAL:
						{
							std::cout<<"Pedestal mode process switch function"<<std::endl;
							gWorkingModePedestal();
						}
						break;
					case WORKMODE_ANALYSIS:{
						gWorkingModeAnalysis();
					}
					break;
					case WORKMODE_CALIBRATION:{

						gWorkingModeCalibration();
					}
					break;
					default:
						std::cout<<guiinfor->GetRunMode()<<std::endl;
						break;
				}
			}
			break;
		default:
			break;
	}

}

// menu process
void GUIMainFrame::gMessageProcessMenu(Long_t msg, Long_t parm){
	switch (parm) {
		case M_FILE_OPEN:
			{
				std::cout<<"Open file dialog" <<std::endl;
			}
			break;
		case M_FILE_NEWCANVAS:
			{
				new TCanvas("UVa GEM Analysis Framework--NewCanvas",
				"UVa GEM Analysis Framework--NewCanvas", 400, 400);
			}
			break;
		case M_FILE_TBROWER:
			{
				new TBrowser("UVa GEM Analysis Framework--TBrowser",
				"UVa GEM Analysis Framework--Root Tree Browser");
			}
			break;
		case M_FILE_SAVE:
			std::cout<<"Save dialog"<<std::endl;
			break;
		case M_FILE_SAVEAS:
			std::cout<<"Save as dialog"<<std::endl;
			break;
		case M_FILE_PRINT:
			std::cout<<"Print dialog"<<std::endl;
			break;
		case M_FILE_EXIT:
			CloseWindow();
			break;
		case M_SET_LOADMAPPING:
			std::cout<<"load mapping as diaglog"<<std::endl;
			break;
		case M_SET_LOADPEDESTAL:
			std::cout<<"Load Pedestal diaglog"<<std::endl;
			break;
		case M_TOOL_APVMAPPINGWIZARD:
			std::cout<<"APV mapping wizard diaglog"<<std::endl;
			break;
		default:
			std::cout<<"Command Currently not support"<<std::endl;
			break;
	}
}

// radio button used for select the work mode
void GUIMainFrame::gMessageProcessRadioButton(Long_t msg, Long_t parm){
	switch (parm) {
		case C_WORKMODE_RAW:
			guiinfor->SetRunMode(WORKMODE_RAW);
			std::cout<<"Raw mode selected"<<std::endl;
			GUIUpdata();
			break;
		case C_WORKMODE_ZEROSUBTRACTION:
			std::cout<<"Zero Subtraction mode selected"<<std::endl;
			guiinfor->SetRunMode(WORKMODE_ZERSUBTRACTION);

			break;
		case C_WORKMODE_PEDESTAL:
			std::cout<<"Pedestal Mode selected"<<std::endl;
			guiinfor->SetRunMode(WORKMODE_PEDESTAL);

			break;
		case C_WORKMODE_HIT:
			std::cout<<"Pedestal Mode selected"<<std::endl;
			guiinfor->SetRunMode(WORKMODE_HIT);
			break;
		case C_WORKMODE_ANALYSIS:
			std::cout<<"Analysis mode selected" <<std::endl;
			guiinfor->SetRunMode(WORKMODE_ANALYSIS);
			break;
		case C_WORKMODE_CALIBRATION:
			std::cout<<"Calibration mode selected" <<std::endl;
			guiinfor->SetRunMode(WORKMODE_CALIBRATION);
			break;
		default:
			std::cout<<"Command currently not support"<<std::endl;
			break;
	}
}

// check button used for control the number
void GUIMainFrame::gMessageProcessCheckButton(Long_t msg, Long_t parm){
	switch (parm) {
		case -1:
			std::cout<<"Event Number is changed"<<std::endl;
			break;
		default:
			std::cout<<parm<<std::endl;
			break;
	}
}
//
void GUIMainFrame::gMessageProcessColorSel(Long_t msg, Long_t parm){
	std::cout<<"Color Sel changed"<<std::endl;
}

void GUIMainFrame::CloseWindow() {
	printf("Program Terminated\n");
	gApplication->Terminate(); // the end of the program
}

void GUIMainFrame::gMenuUnitDraw(TGLayoutHints *l ){
	TGDockableFrame *fMenuDock = new TGDockableFrame(this);
	AddFrame(fMenuDock,l);
	fMenuDock->SetWindowName("UVa GEM Analysis Framework");
	TGLayoutHints *fMenuBarLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX);
	TGLayoutHints *fMenuBarItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0,0);
	TGLayoutHints *fMenuBarHelpLayout = new TGLayoutHints(kLHintsTop | kLHintsRight);
	TGPopupMenu *fMenuFile = new TGPopupMenu(fClient->GetRoot());
	TGPopupMenu *fMenuSet  = new TGPopupMenu(fClient->GetRoot());
	TGPopupMenu *fMenuTool  = new TGPopupMenu(fClient->GetRoot());
	TGPopupMenu *fMenuView  = new TGPopupMenu(fClient->GetRoot());
	TGPopupMenu *fMenuHelp  = new TGPopupMenu(fClient->GetRoot());
	gSetMenuFile(fMenuFile);
	gSetMenuSet(fMenuSet);
	gSetMenuTool(fMenuTool);
	gSetMenuView(fMenuView);
	gSetMenuHelp(fMenuHelp);
	fMenuDock->EnableUndock(kTRUE);
	fMenuDock->EnableHide(kTRUE);
	TGMenuBar *fMenuBar= new TGMenuBar(fMenuDock,1,1,kHorizontalFrame);
	fMenuBar -> AddPopup("&File",fMenuFile,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&Set",fMenuSet,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&Tool",fMenuTool,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&View",fMenuView,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&Help",fMenuHelp,fMenuBarHelpLayout);
	fMenuDock -> AddFrame(fMenuBar,fMenuBarLayout);
	TGHorizontal3DLine *menu_seperator=new TGHorizontal3DLine(this);
	AddFrame(menu_seperator,new TGLayoutHints(kLHintsExpandX));
}

void GUIMainFrame::gWorktabUnitDraw(TGLayoutHints *l){
	TGHorizontalFrame *fWorkZoneFrame= new TGHorizontalFrame(this);
	TGVerticalFrame *fWorkZoneControlFrame = new TGVerticalFrame(fWorkZoneFrame,10,10);
	TGVerticalFrame *fWorkZoneCanvasFrame  = new TGVerticalFrame(fWorkZoneFrame, 10,10);
	TGVertical3DLine * WorkZoneSeparation= new TGVertical3DLine(fWorkZoneFrame,10,10);

	gWorktabControlUnitDraw(fWorkZoneControlFrame,new TGLayoutHints(kLHintsLeft |kLHintsExpandY));
	gWorktabDisplayUnitDraw(fWorkZoneCanvasFrame,new TGLayoutHints(kLHintsLeft|kLHintsExpandX|kLHintsExpandY));

	fWorkZoneFrame->AddFrame(fWorkZoneControlFrame,new TGLayoutHints(kLHintsLeft |kLHintsExpandY));
	fWorkZoneFrame->AddFrame(WorkZoneSeparation,new TGLayoutHints(kLHintsLeft|kLHintsExpandY));
	fWorkZoneFrame->AddFrame(fWorkZoneCanvasFrame,new TGLayoutHints(kLHintsLeft|kLHintsExpandX|kLHintsExpandY));
	AddFrame(fWorkZoneFrame, l);
}

void GUIMainFrame::gWorktabControlUnitDraw(TGCompositeFrame *p,TGLayoutHints *l){
	p->AddFrame(gWorktabControlButtonSetDraw(p,l),new TGLayoutHints(kLHintsTop|kLHintsExpandX));
	p->AddFrame(gWorktabControlFileIODraw(p,l),new TGLayoutHints(kLHintsTop|kLHintsExpandX));
	p->AddFrame(gWorktabControlOutputDraw(p,l),new TGLayoutHints(kLHintsTop|kLHintsExpandX));
	p->AddFrame(gWorktabControlOutputButtonDraw(p,l),new TGLayoutHints(kLHintsBottom|kLHintsExpandX));
}

TGCompositeFrame *GUIMainFrame::gWorktabControlButtonSetDraw(TGCompositeFrame *p,TGLayoutHints *l){
	TGGroupFrame *fWorkControlButtonFrame=new TGGroupFrame(p,"Run Mode");
	TGTab *tWorkModeTab=new TGTab(fWorkControlButtonFrame);
	TGCompositeFrame *fWorkButtonSelectionFrame=tWorkModeTab->AddTab("Mode");
	TGCompositeFrame *fWorkButtonSettingFrame=tWorkModeTab->AddTab("Setting");
	//set the button
	TGButtonGroup *fWorkModeButtonGroup=new TGButtonGroup(fWorkButtonSelectionFrame,5,2);
	fWorkModeButtonGroup->DrawBorder();
	fWorkModeButtonGroup->SetLayoutHints(new TGLayoutHints(kLHintsCenterX));
	// add the button
	TGRadioButton *bWorkModeRAW = new TGRadioButton(fWorkModeButtonGroup,"&Raw*",C_WORKMODE_RAW);
	TGRadioButton *bWorkModeZeroSubtraction = new TGRadioButton(fWorkModeButtonGroup,"&ZeroSub*",C_WORKMODE_ZEROSUBTRACTION);
	TGRadioButton *bWorkModePedestal = new TGRadioButton(fWorkModeButtonGroup,"&Pedestal",C_WORKMODE_PEDESTAL);
	TGRadioButton *bWorkModeHit = new TGRadioButton(fWorkModeButtonGroup,"&Hit",C_WORKMODE_HIT);
	TGRadioButton *bWorkModeAnalysis = new TGRadioButton(fWorkModeButtonGroup,"&Analysis",C_WORKMODE_ANALYSIS);
	TGRadioButton *bWorkModeCalibration = new TGRadioButton(fWorkModeButtonGroup,"&Calibration",C_WORKMODE_CALIBRATION);
	TGRadioButton *bWorkModeOnlineMonitor = new TGRadioButton(fWorkModeButtonGroup,"&Monitor",C_WORKMODE_ONLINEMONITOR);
	TGRadioButton *bWorkModeTracking = new TGRadioButton(fWorkModeButtonGroup,"&Tracking",C_WORKMODE_TRACKING);
	TGRadioButton *bWorkModetest = new TGRadioButton(fWorkModeButtonGroup,"&Test",C_WORKMODE_TEST);

	bWorkModeRAW->Associate(this);
	bWorkModeZeroSubtraction->Associate(this);
	bWorkModePedestal->Associate(this);
	bWorkModeHit->Associate(this);
	bWorkModeAnalysis->Associate(this);
	bWorkModeCalibration->Associate(this);
	bWorkModeOnlineMonitor->Associate(this);
	bWorkModeTracking->Associate(this);
	bWorkModetest->Associate(this);

	//bWorkModeOnlineMonitor->Associate(this);
	fWorkButtonSelectionFrame->AddFrame(fWorkModeButtonGroup,new TGLayoutHints(kLHintsTop|kLHintsExpandX,10,10,0,10));
	fWorkControlButtonFrame->AddFrame(tWorkModeTab,new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
	return fWorkControlButtonFrame;
}

TGCompositeFrame * GUIMainFrame::gWorktabControlFileIODraw(TGCompositeFrame *p, TGLayoutHints *l){
	TGGroupFrame *fDataInputFrame=new TGGroupFrame(p,"Data Input");
	fDataInputFrame->AddFrame(gWorktabControlPedestalInputBox(fDataInputFrame,new TGLayoutHints(kLHintsExpandX)),new TGLayoutHints(kLHintsExpandX));
	fDataInputFrame->AddFrame(new TGHorizontal3DLine(fDataInputFrame),new TGLayoutHints(kLHintsTop | kLHintsExpandX));
	fDataInputFrame->AddFrame(gWorktabControlRawDataInputBox(fDataInputFrame,new TGLayoutHints(kLHintsExpandX)),new TGLayoutHints(kLHintsTop | kLHintsExpandX));
	return fDataInputFrame;
}


TGCompositeFrame *GUIMainFrame::gWorktabControlPedestalInputBox(TGCompositeFrame *p, TGLayoutHints *l){
	TGHorizontalFrame *fPedestalInputFrame=new TGHorizontalFrame(p);
	gfPedestalFileEntry= new TGTextEntry(fPedestalInputFrame);
	gfPedestalFileEntry->SetTitle("Pedestal *.root");
	gfPedestalFileEntry->Resize(150,30);
	TGTextButton *bPedestalFileButton=new TGTextButton (fPedestalInputFrame,"Open..",B_PEDESTALFILE_OPEN);
	bPedestalFileButton->Associate(this);
	fPedestalInputFrame->AddFrame(gfPedestalFileEntry,new TGLayoutHints(kLHintsLeft));
	fPedestalInputFrame->AddFrame(bPedestalFileButton,new TGLayoutHints(kLHintsLeft|kLHintsBottom,10,10,10,10));
	return fPedestalInputFrame;
}

TGCompositeFrame *GUIMainFrame::gWorktabControlRawDataInputBox(TGCompositeFrame *p, TGLayoutHints *l){
	TGHorizontalFrame *fRawFileInputFrame =new TGHorizontalFrame(p);
	TGVerticalFrame   *fRawFileButtomFrame=new TGVerticalFrame(fRawFileInputFrame);

	// raw file input layout
	gtRawDataEntry=new TGListBox(fRawFileInputFrame);
	gtRawDataEntry->Resize(150,80);
	gtRawDataEntry->AddEntry("*.dat files to be process",0);


	TGTextButton *bRawDataFileOpenButton= new TGTextButton(fRawFileButtomFrame,"Open..",B_RAWFILE_OPEN);
	TGTextButton *bRawDataFileDeleteButton= new TGTextButton(fRawFileButtomFrame,"Delete",B_RAWFILE_DELETE);
	bRawDataFileOpenButton->Associate(this);
	bRawDataFileDeleteButton->Associate(this);

	fRawFileButtomFrame->AddFrame(bRawDataFileDeleteButton,new TGLayoutHints(kLHintsCenterX|kLHintsBottom,10,10));
	fRawFileButtomFrame->AddFrame(bRawDataFileOpenButton,new TGLayoutHints(kLHintsCenterX|kLHintsBottom,10,10,10,10));
	fRawFileInputFrame->AddFrame(gtRawDataEntry,new TGLayoutHints(kLHintsLeft));
	fRawFileInputFrame->AddFrame(fRawFileButtomFrame,new TGLayoutHints(kLHintsLeft));
	return fRawFileInputFrame;
}

TGCompositeFrame *GUIMainFrame::gWorktabControlFileIORawListBox(TGCompositeFrame *p, TGLayoutHints *l){
	TGListBox *tRawDataEntry=new TGListBox(p);
	tRawDataEntry->Resize(150,80);
	tRawDataEntry->AddEntry("*.dat files to be process",0);
	if(guiinfor->GetRawFileInputList().size()!=0)tRawDataEntry->Clear();
	for(auto fname : guiinfor->GetRawFileInputList()){
		tRawDataEntry->AddEntry(fname.c_str(),10);
	}
	return tRawDataEntry;
}

TGCompositeFrame *GUIMainFrame::gWorktabControlOutputDraw(TGCompositeFrame *p, TGLayoutHints *l){

	TGGroupFrame *fOutputIOGroupFrame=new TGGroupFrame(p,"Output Control",kVerticalFrame);
	fOutputIOGroupFrame->SetLayoutManager(new TGMatrixLayout(fOutputIOGroupFrame,0,2,15));
	fOutputIOGroupFrame->AddFrame(new TGLabel(fOutputIOGroupFrame,new TGHotString("Out File pattern:")));
	TGTextEntry *tOutPutfilePattern=new TGTextEntry(fOutputIOGroupFrame);
	tOutPutfilePattern->Resize(100,tOutPutfilePattern->GetDefaultHeight());
	fOutputIOGroupFrame->AddFrame(tOutPutfilePattern);

	fOutputIOGroupFrame->AddFrame(new TGLabel(fOutputIOGroupFrame,new TGHotString("Event Number:")));
	TGNumberEntry *tWorkControlNumberEntry= new TGNumberEntry(fOutputIOGroupFrame);
	tWorkControlNumberEntry->SetUniqueID(V_NUMBERINPUT);
	tWorkControlNumberEntry->SetName(0);
	tWorkControlNumberEntry->Associate(this);
	tWorkControlNumberEntry->Resize(100,tOutPutfilePattern->GetDefaultHeight());
	fOutputIOGroupFrame->AddFrame(tWorkControlNumberEntry);

	return fOutputIOGroupFrame;
}

TGCompositeFrame *GUIMainFrame::gWorktabControlOutputButtonDraw(TGCompositeFrame *p, TGLayoutHints *l){
	TGHorizontalFrame *fWorkZoneConfirmFrame=new TGHorizontalFrame(p);
	TGTextButton *bWorkZoneConfirm=new TGTextButton(fWorkZoneConfirmFrame,"Confirm",B_CONFIRM);
	bWorkZoneConfirm->Associate(this);
	fWorkZoneConfirmFrame->AddFrame(bWorkZoneConfirm,new TGLayoutHints(kLHintsExpandX,10,10,10,10));
	return fWorkZoneConfirmFrame;
}

TGFrame *GUIMainFrame::gWorktabDisplayUnitDraw(TGCompositeFrame *p,TGLayoutHints *l){
	TGTab *tWorkZoneTab=new TGTab(p);
	TGCompositeFrame *test = tWorkZoneTab->AddTab(Form("test"));
	TRootEmbeddedCanvas *canvas =new TRootEmbeddedCanvas("MainCanvas", test, 600,600);
	canvas->GetCanvas()->SetBorderMode(0);
	canvas->GetCanvas()->SetGrid();
	test->AddFrame(canvas,new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
	p->AddFrame(tWorkZoneTab,new TGLayoutHints(kLHintsTop|kLHintsExpandY|kLHintsExpandX));
	return tWorkZoneTab;
}

void GUIMainFrame::gStatusUnitDraw(TGLayoutHints *l){
	TGCompositeFrame *fStatusFrame = new TGCompositeFrame(this, 60,20,kHorizontalFrame|kSunkenFrame);

	Pixel_t yellow,green;
	fClient->GetColorByName("yellow",yellow);
	fClient->GetColorByName("green",green);
	TGColorSelect *fColorSel =  new TGColorSelect(fStatusFrame, green, C_COLORSEL);
	fColorSel->Associate(this);
	fStatusFrame->AddFrame(fColorSel, new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY,5,5,0,0));

	TGVertical3DLine *lStatusbarSeparation0=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation0,new TGLayoutHints(kLHintsRight | kLHintsTop |kLHintsExpandY));

	TGLabel *author_display=new TGLabel(fStatusFrame,"UVa GEM Analysis Framework @Siyu");
	fStatusFrame->AddFrame(author_display,new TGLayoutHints(kLHintsRight | kLHintsTop | kLHintsExpandY,5,5,0,0));
	TGVertical3DLine *lStatusbarSeparation1=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation1, new TGLayoutHints(kLHintsRight | kLHintsTop | kLHintsExpandY));

	TGHProgressBar *pStatusBarProcessBar = new TGHProgressBar(fStatusFrame, TGProgressBar::kFancy,300);
	pStatusBarProcessBar->SetBarColor("lightblue");
	pStatusBarProcessBar->Increment(100);
	pStatusBarProcessBar->ShowPosition(kTRUE,kFALSE,"%.0f events");
	fStatusFrame->AddFrame(pStatusBarProcessBar,new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY,5,5,0,0));

	TGVertical3DLine *lStatusbarSeparation2=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation2, new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY));

	TGLabel *nStatusBarInfor = new TGLabel(fStatusFrame,"Work mode/ pedestal / raw dat");
	fStatusFrame->AddFrame(nStatusBarInfor, new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY,15,15,0,0));

	// set the time display
	TGLayoutHints *StatusBarLayout1=new TGLayoutHints(kLHintsLeft|kLHintsTop|kLHintsExpandY,10,10);
	TDatime *lSystemTime = new TDatime();

	TGLabel *nStatusBarTimeLabel = new TGLabel(fStatusFrame,new TGString(Form("%4d : %02d : %02d",lSystemTime->GetYear(),lSystemTime->GetMonth(),lSystemTime->GetDay())));
	nStatusBarTimeLabel->Set3DStyle(0);
	fStatusFrame->AddFrame(nStatusBarTimeLabel,StatusBarLayout1);

	TGVertical3DLine *lStatusbarSeparation3=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation3, new TGLayoutHints(kLHintsLeft|kLHintsTop|kLHintsExpandY));

	AddFrame(fStatusFrame,new TGLayoutHints(kLHintsBottom|kLHintsExpandX,0,0,1,0));
}

void GUIMainFrame::gSetMenuFile(TGPopupMenu *fMenuFile){
	fMenuFile->AddEntry("&Open...", M_FILE_OPEN);
	fMenuFile->AddEntry("&New Canvas", M_FILE_NEWCANVAS);
	fMenuFile->AddEntry("Root &TBrowser", M_FILE_TBROWER);
	fMenuFile->AddEntry("&Save", M_FILE_SAVE);
	fMenuFile->AddEntry("S&ave as...", M_FILE_SAVEAS);
	//fMenuFile->AddEntry("&Close", -1);
	fMenuFile->AddSeparator();
	fMenuFile->AddEntry("&Print", M_FILE_PRINT);
	fMenuFile->AddEntry("P&rint setup...", M_FILE_PRINTSETUP);
	fMenuFile->AddSeparator();
	fMenuFile->AddEntry("E&xit", M_FILE_EXIT);
	fMenuFile->DisableEntry(M_FILE_SAVEAS);
	fMenuFile->DisableEntry(M_FILE_PRINTSETUP);
	//fMenuFile->HideEntry(M_FILE_PRINT);
	fMenuFile->Associate(this);
}

void GUIMainFrame::gSetMenuSet(TGPopupMenu *fMenuSet){
	fMenuSet->AddEntry("Loading Mapping",M_SET_LOADMAPPING);
	fMenuSet->AddEntry("Load Pedestal(root)...", M_SET_LOADPEDESTAL);
	fMenuSet->AddEntry("&Set...", M_SET_WORKMODE);
	fMenuSet->Associate(this);
}

void GUIMainFrame::gSetMenuTool(TGPopupMenu * fMenuTool){
	fMenuTool->AddEntry("&APVMapping Wizard...",M_TOOL_APVMAPPINGWIZARD);
	fMenuTool->Associate(this);
}

void GUIMainFrame::gSetMenuView(TGPopupMenu  *fMenuView){
	fMenuView->AddEntry("&Dock", M_VIEW_DOCK);
	fMenuView->AddEntry("&Undock", M_VIEW_UNDOCK);
	fMenuView->AddSeparator();
	fMenuView->AddEntry("Enable U&ndock", M_VIEW_ENBL_DOCK);
	fMenuView->AddEntry("Enable &Hide", M_VIEW_ENBL_HIDE);
	fMenuView->CheckEntry(M_VIEW_ENBL_DOCK);
	fMenuView->CheckEntry(M_VIEW_ENBL_HIDE);
	fMenuView->Associate(this);
}

void GUIMainFrame::gSetMenuHelp(TGPopupMenu *fMenuHelp){
	fMenuHelp->AddEntry("&Contents", M_HELP_CONTENTS);
	fMenuHelp->AddEntry("&Search...", M_HELP_SEARCH);
	fMenuHelp->AddSeparator();
	fMenuHelp->AddEntry("&About", M_HELP_ABOUT);
	fMenuHelp->Associate(this);
}

TGCompositeFrame *GUIMainFrame::gSysInforTabDraw(TGCompositeFrame *p, TGLayoutHints *l){
	TGVerticalFrame *gWorkZoneMainFrame=new TGVerticalFrame(p);
	TGHorizontalFrame *gWorkStatusSubFrame=new TGHorizontalFrame(gWorkZoneMainFrame);
	TGHorizontal3DLine *gSeperateLine=new TGHorizontal3DLine(gWorkZoneMainFrame);

	gWorkZoneMainFrame->AddFrame(gWorkStatusSubFrame,new TGLayoutHints(kLHintsTop|kLHintsCenterY|kLHintsRight|kLHintsCenterX));
	gWorkZoneMainFrame->AddFrame(gSeperateLine,new TGLayoutHints(kLHintsTop|kLHintsExpandX));
	p->AddFrame(gWorkZoneMainFrame,new TGLayoutHints(kLHintsExpandY|kLHintsExpandX));
	return gWorkZoneMainFrame;
}

void GUIMainFrame::GUIUpdata(){
	MapSubwindows();
	Resize();   //resize to default size

}

// working command process
void GUIMainFrame::gWorkingModeRaw(){

}

void GUIMainFrame::gWorkingModePedestal(){
	std::cout<<"Pedestal mode running"<<std::endl;
	if(guiinfor->GetRunMode()==WORKMODE_PEDESTAL){
		std::cout<<"Woring mode checked right"<<std::endl;
	}
//	MPDDecoder *decoder=new MPDDecoder();
//	decoder->PedestalMode(guiinfor->GetRawFileInputList(),"tpedestal_test.root");
//	delete decoder;
}

void GUIMainFrame::gWorkingModeHit(){
	// loading the work mode
}

void GUIMainFrame::gWorkingModeZeroSubtr(){

}

void GUIMainFrame::gWorkingModeHitDisplay(){

}

void GUIMainFrame::gWorkingModeCalibration(){

}

void GUIMainFrame::gWorkingModeAnalysis(){

}

void GUIMainFrame::cUpdateWorkTabControlPedestalInputBox(){
	std::string file=*guiinfor->GetPedestalInputFile();
	std::string name=file.substr(file.find_last_of("/")+1);
	gfPedestalFileEntry->SetText(name.c_str());
	gfPedestalFileEntry->MapSubwindows();
	gfPedestalFileEntry->Layout();
}

void GUIMainFrame::cUpdateWorkTabControlRawInputBox(){

	gtRawDataEntry->RemoveAll();
	gRawDataEntryMap.clear();
	int id=0;
	for(auto file : guiinfor->GetRawFileInputList()){

		std::string name=file.substr(file.find_last_of("/")+1);
		gtRawDataEntry->AddEntry(name.c_str(),id);

		gRawDataEntryMap[id]=file.c_str();
		id++;
	}
	gtRawDataEntry->MapSubwindows();
	gtRawDataEntry->Layout();
}

std::string GUIMainFrame::cGetSelectedWorkTabControlRawInputBox(){
	auto entry = gtRawDataEntry->GetSelected();
	if(gRawDataEntryMap.find(entry)!=gRawDataEntryMap.end()){
		return gRawDataEntryMap[entry];
	}else{
	    std::string a;
		return a;
	}
}
