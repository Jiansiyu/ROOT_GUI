/*
 * UserGuiMainFrame.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: Siyu Jian
 */

#include "UserGuiMainFrame.h"

#include "TApplication.h"
#include "TGLayout.h"
#include "TG3DLine.h"
#include "TGFrame.h"
#include "TGNumberEntry.h"
#include "TThread.h"
#include "TFile.h"
#include "libgen.h"
#include "string"

/// gem analysis method
#include "../analysis/Config.h"
#include "../analysis/GEMTree.h"
#include "../analysis/GEMTracking.h"
#include "../analysis/GEMFittingShape.h"
///

// main decoder lib
#include "UserGuiGeneralDialogProcess.h"
#include "../GUIDialog/UserGUIMapWizard.h"

// raw decoder
#include "../GEMDetector/MPDDecoder.h"
#include "../GEMDetector/GEMConfigure.h"

#include <libgen.h>

//#include "../DecoderMPD4_VME/GEMDataParserM4V.h"

UserGuiMainFrame::UserGuiMainFrame(const TGWindow *p, UInt_t w, UInt_t h) : TGMainFrame(p, w, h) {
	// TODO Auto-generated constructor stub
	SetCleanup(kDeepCleanup);

	fMenuDock = new TGDockableFrame(this);
	AddFrame(fMenuDock, new TGLayoutHints(kLHintsExpandX, 0, 0, 1, 0));
	fMenuDock->SetWindowName("MainFrame Menu");
	// set the layout of the menu bar
	fMenuBarLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX);
	fMenuBarItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0,0);
	fMenuBarHelpLayout = new TGLayoutHints(kLHintsTop | kLHintsRight);

	// set the "File" menu
	fMenuFile = new TGPopupMenu(fClient->GetRoot());
	SetMenuFile();

	fMenuSet = new TGPopupMenu(fClient->GetRoot());
	SetMenuSet();

	fMenuTool= new TGPopupMenu(fClient->GetRoot());
	SetMenuTool();

	fMenuView = new TGPopupMenu(fClient->GetRoot());
	SetMenuView();

	fMenuHelp = new TGPopupMenu(fClient->GetRoot());
	SetMenuHelp();

	fMenuDock->EnableUndock(kTRUE);
	fMenuDock->EnableHide(kTRUE);

	fMenuBar = new TGMenuBar(fMenuDock,1,1,kHorizontalFrame);
	fMenuBar -> AddPopup("&File",fMenuFile,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&Set",fMenuSet,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&Tool",fMenuTool,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&View",fMenuView,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&Help",fMenuHelp,fMenuBarHelpLayout);
	fMenuDock -> AddFrame(fMenuBar,fMenuBarLayout);

	// add separation line to the menu bar
	TGHorizontal3DLine *menu_seperator=new TGHorizontal3DLine(this);
	AddFrame(menu_seperator,new TGLayoutHints(kLHintsExpandX));

	// add the work zone frame
	fWorkZoneFrame = new TGHorizontalFrame(this);
	fWorkZoneControlFrame = new TGVerticalFrame(fWorkZoneFrame, 10,10);
	SetWorkZoneButton();   												// set the control penal
	//workZone  -> display Sub-zone
	fWorkZoneCanvasFrame  = new TGVerticalFrame(fWorkZoneFrame, 10,10);
	TGVertical3DLine * WorkZoneSeparation= new TGVertical3DLine(fWorkZoneFrame,10,10);
	fWorkZoneTab= new TGTab(fWorkZoneCanvasFrame);

	fWorkZoneCanvasFrame->AddFrame(fWorkZoneTab, new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));

	fWorkZoneFrame ->AddFrame(fWorkZoneControlFrame,new TGLayoutHints(kLHintsLeft |kLHintsExpandY));
	fWorkZoneFrame -> AddFrame(WorkZoneSeparation,new TGLayoutHints(kLHintsLeft|kLHintsExpandY));
	fWorkZoneFrame ->AddFrame(fWorkZoneCanvasFrame, new TGLayoutHints(kLHintsLeft|kLHintsExpandX|kLHintsExpandY));
	AddFrame(fWorkZoneFrame, new TGLayoutHints(kLHintsRight|kLHintsExpandX|kLHintsExpandY));

	// statuts bar menu
	TGHorizontal3DLine *StatusBar_separator= new TGHorizontal3DLine(this);
	AddFrame(StatusBar_separator, new TGLayoutHints(kLHintsExpandX));
	fStatusFrame = new TGCompositeFrame(this, 60, 20, kHorizontalFrame | kSunkenFrame);
	SetStatusBar();
	AddFrame(fStatusFrame , new TGLayoutHints(kLHintsBottom | kLHintsExpandX,0,0,1,0));

	SetWindowName("UVa GEM Analysis Framework");
	MapSubwindows();
	// we need to use GetDefault...() to initialize the layout algorithm...
	Resize();   //resize to default size
	MapWindow();

#ifdef __GUI_DEBUG_MODE
	Print();
#endif
}

void UserGuiMainFrame::SetMenuFile() {
	fMenuFile->AddEntry("&Open...", M_FILE_OPEN);
	fMenuFile->AddEntry("&New Canvas", M_FILE_NEWCANVAS);
	fMenuFile->AddEntry("Root &TBrowser", M_FILE_TBROWER);
	fMenuFile->AddEntry("&Save", M_FILE_SAVE);
	fMenuFile->AddEntry("S&ave as...", M_FILE_SAVEAS);
	fMenuFile->AddEntry("&Close", -1);
	fMenuFile->AddSeparator();
	fMenuFile->AddEntry("&Print", M_FILE_PRINT);
	fMenuFile->AddEntry("P&rint setup...", M_FILE_PRINTSETUP);
	fMenuFile->AddSeparator();
	fMenuFile->AddEntry("E&xit", M_FILE_EXIT);
	fMenuFile->DisableEntry(M_FILE_SAVEAS);
	fMenuFile->HideEntry(M_FILE_PRINT);
	fMenuFile->Associate(this);
}
void UserGuiMainFrame::SetMenuSet() {
	fMenuSet->AddEntry("Loading Mapping",M_SET_LOADMAPPING);
	fMenuSet->AddEntry("Load Pedestal(root)...", M_SET_LOADPEDESTAL);
	fMenuSet->AddEntry("&Set...", M_SET_WORKMODE);
	fMenuSet->Associate(this);
}

void UserGuiMainFrame::SetMenuTool(){
	fMenuTool->AddEntry("&APVMapping Wizard...",M_TOOL_APVMAPPINGWIZARD);
	fMenuTool->Associate(this);
}

void UserGuiMainFrame::SetMenuView() {
	fMenuView->AddEntry("&Dock", M_VIEW_DOCK);
	fMenuView->AddEntry("&Undock", M_VIEW_UNDOCK);
	fMenuView->AddSeparator();
	fMenuView->AddEntry("Enable U&ndock", M_VIEW_ENBL_DOCK);
	fMenuView->AddEntry("Enable &Hide", M_VIEW_ENBL_HIDE);

	fMenuView->CheckEntry(M_VIEW_ENBL_DOCK);
	fMenuView->CheckEntry(M_VIEW_ENBL_HIDE);
	fMenuView->Associate(this);
}
void UserGuiMainFrame::SetMenuHelp() {
	fMenuHelp->AddEntry("&Contents", M_HELP_CONTENTS);
	fMenuHelp->AddEntry("&Search...", M_HELP_SEARCH);
	fMenuHelp->AddSeparator();
	fMenuHelp->AddEntry("&About", M_HELP_ABOUT);
	fMenuHelp->Associate(this);
}

void UserGuiMainFrame::SetWorkZone(){
	// TODO
}

/*
 * Used for set the tab automatically by mapping file
 */
void UserGuiMainFrame::SetWorkZoneTab(){
	int tabnumber = fWorkZoneTab->GetCounter();
	for (int i = 0; i < tabnumber; i++) {
		fWorkZoneTab->RemoveTab(0);
	};
	fWorkZoneTabDefultFrame = fWorkZoneTab->AddTab("WorkStatus");
	// this is just used for draw the GUI, so efficiency is not the first concern
	std::map<int,std::string> tablist;
	GEMConfigure *cfg = GEMConfigure::GetInstance();
	if (vWorkMode == 'R') {
		for(auto apv : cfg->GetMapping().GetMPDList()){
			tablist[apv]=Form("crate%d_mpd%d",(GEM::getCrateID(apv)),(GEM::getMPDID(apv)));
		}
	}else if(vWorkMode == 'Z'){
		for(auto module :cfg->GetMapping().GetGEMModuleList()){
			tablist[module]=Form("Module%d",module);
		}
	}
	SetWorkZoneTab(tablist);
	MapSubwindows();
	Resize();   //resize to default size
}

/*
 *
 */
void UserGuiMainFrame::SetWorkZoneTab(std::vector<int>tablist){
	fWorkZoneTabSubFrame.clear();
	fWorkZoneTabEnbeddedCanvas.clear();
	cfWorkZoneTabCanvas.clear();
	for(auto tab:tablist){
		fWorkZoneTabSubFrame[tab]=((fWorkZoneTab->AddTab(Form("crate%d_mpd%d",GEM::getCrateID(tab),GEM::getMPDID(tab)))));
		fWorkZoneTabEnbeddedCanvas[tab]=new TRootEmbeddedCanvas("MainCanvas", fWorkZoneTabSubFrame[tab], 600,600);
		fWorkZoneTabEnbeddedCanvas[tab]->GetCanvas()->SetBorderMode(0);
		fWorkZoneTabEnbeddedCanvas[tab]->GetCanvas()->SetGrid();
		fWorkZoneTabSubFrame[tab]->AddFrame(fWorkZoneTabEnbeddedCanvas[tab],new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
		cfWorkZoneTabCanvas[tab]=fWorkZoneTabEnbeddedCanvas[tab]->GetCanvas();
	}
}

void UserGuiMainFrame::SetWorkZoneTab(std::map<int,std::string> tablist){
	fWorkZoneTabSubFrame.clear();
	fWorkZoneTabEnbeddedCanvas.clear();
	cfWorkZoneTabCanvas.clear();
	for(auto tab = tablist.begin();tab!=tablist.end();tab++){
		fWorkZoneTabSubFrame[tab->first]=fWorkZoneTab->AddTab(tab->second.c_str());
		fWorkZoneTabEnbeddedCanvas[tab->first]=new TRootEmbeddedCanvas("MainCanvas", fWorkZoneTabSubFrame[tab->first], 600,600);
		fWorkZoneTabEnbeddedCanvas[tab->first]->GetCanvas()->SetBorderMode(0);
		fWorkZoneTabEnbeddedCanvas[tab->first]->GetCanvas()->SetGrid();
		fWorkZoneTabSubFrame[tab->first]->AddFrame(fWorkZoneTabEnbeddedCanvas[tab->first],new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
		cfWorkZoneTabCanvas[tab->first]=fWorkZoneTabEnbeddedCanvas[tab->first]->GetCanvas();

	}
}

void UserGuiMainFrame::SetWorkZoneButton(){

	bWorkModeButtonGroup   = new TGButtonGroup(fWorkZoneControlFrame,"Work Mode");
	bWorkModeButtonGroup -> SetTitlePos(TGGroupFrame::kCenter);
	// add the button to the frame

	bWorkModeRAW = new TGRadioButton(bWorkModeButtonGroup,"&Raw",C_WORKMODE_RAW);

	bWorkModeZeroSubtraction = new TGRadioButton(bWorkModeButtonGroup,"&ZeroSub",C_WORKMODE_ZEROSUBTRACTION);

	bWorkModePedestal = new TGRadioButton(bWorkModeButtonGroup,"&Pedestal",C_WORKMODE_PEDESTAL);

	bWorkModeHit = new TGRadioButton(bWorkModeButtonGroup,"&Hit",C_WORKMODE_HIT);

	bWorkModeAnalysis = new TGRadioButton(bWorkModeButtonGroup,"&Analysis",C_WORKMODE_ANALYSIS);

	bWorkModeCalibration = new TGRadioButton(bWorkModeButtonGroup,"&Calibration",C_WORKMODE_CALIBRATION);

	bWorkModeRAW->Associate(this);
	bWorkModeZeroSubtraction ->Associate(this);
	bWorkModePedestal ->Associate(this);
	bWorkModeHit ->Associate(this);
	bWorkModeAnalysis ->Associate(this);
	bWorkModeCalibration->Associate(this);
	fWorkZoneControlFrame->AddFrame(bWorkModeButtonGroup , new TGLayoutHints(kLHintsExpandX));

	TGGroupFrame * fDataInputFrame= new TGGroupFrame(fWorkZoneControlFrame,"Data Input");
	fDataInputFrame->SetTitlePos(TGGroupFrame::kCenter);

	tPedestalFileEntry  =  new TGTextEntry(fDataInputFrame);
	tPedestalFileEntry->SetTitle("Pedestal(*.root/*.dat)");
	bPedestalFileButton = new TGTextButton (fDataInputFrame,"Open..",C_RAWFILE_PEDESTAL);
	bPedestalFileButton->Associate(this);

	fDataInputFrame->AddFrame(tPedestalFileEntry,new TGLayoutHints(kLHintsExpandX));
	fDataInputFrame->AddFrame(bPedestalFileButton,new TGLayoutHints(kLHintsRight));
	tRawFileEntry = new TGListBox(fDataInputFrame);
	tRawFileEntry->Resize(150,80);
	tRawFileEntry->AddEntry("*.dat files to be process",0);
	bRawFileButton = new TGTextButton (fDataInputFrame,"Open..",C_RAWFILE_DAT);
	bRawFileButton -> Associate(this);

	fDataInputFrame->AddFrame(tRawFileEntry,new TGLayoutHints(kLHintsLeft|kLHintsExpandX,5,5,5,5));
	fDataInputFrame->AddFrame(bRawFileButton,new TGLayoutHints(kLHintsRight));
	fWorkZoneControlFrame->AddFrame(fDataInputFrame,new TGLayoutHints(kLHintsCenterX));

	// out put name control
	TGGroupFrame * fDataOutPutFrame= new TGGroupFrame(fWorkZoneControlFrame,"Out Put Pattern");
	fDataOutPutFrame->SetTitlePos(TGGroupFrame::kCenter);
	tOutPutfilePattern = new TGTextEntry(fDataOutPutFrame);
	tOutPutfilePattern ->SetTitle("~/Research/SBS/SBS_GEM_labtest/SBS38_39_41_40%s_temp%04d.root");
	fDataOutPutFrame->AddFrame(tOutPutfilePattern, new TGLayoutHints(kLHintsExpandX));
	fWorkZoneControlFrame->AddFrame(fDataOutPutFrame, new TGLayoutHints(kLHintsExpandX));

	// number control
	fNumberFrame = new TGGroupFrame(fWorkZoneControlFrame, "Event Control");
	fNumberFrame->SetTitlePos(TGGroupFrame::kCenter);
	tNumberEntry = new TGNumberEntry(fNumberFrame);
	tNumberEntry->SetUniqueID(V_NUMBERINPUT);
	tNumberEntry->SetNumber(0);
	tNumberEntry->Associate(this);
	fNumberFrame->AddFrame(tNumberEntry,new TGLayoutHints(kLHintsExpandX));
	fWorkZoneControlFrame->AddFrame(fNumberFrame,new TGLayoutHints(kLHintsExpandX));

	//confirm button
	bConfirmButton = new TGTextButton(fWorkZoneControlFrame,"Confirm",C_CONFIRM);
	bConfirmButton->Associate(this);
	fWorkZoneControlFrame->AddFrame(bConfirmButton, new TGLayoutHints(kLHintsBottom|kLHintsExpandX,5,5,5,5));
}

void UserGuiMainFrame::SetStatusBar(){
	Pixel_t yellow;
	fClient->GetColorByName("yellow", yellow);
	fColorSel = new TGColorSelect(fStatusFrame, yellow, COLORSEL);
	fColorSel->Associate(this);
	fStatusFrame->AddFrame(fColorSel,new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY,5,5,0,0));
	TGVertical3DLine *lStatusbarSeparation0=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation0, new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY));

	TGLabel *autor_display= new TGLabel(fStatusFrame,"UVa GEM Analysis Framework @Siyu");
	fStatusFrame->AddFrame(autor_display,new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY,5,5,0,0));
	TGVertical3DLine *lStatusbarSeparation1=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation1, new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY));


	pStatusBarProcessBar = new TGHProgressBar(fStatusFrame, TGProgressBar::kFancy,300);
	pStatusBarProcessBar->SetBarColor("lightblue");
	pStatusBarProcessBar->Increment(100);
	pStatusBarProcessBar->ShowPosition(kTRUE,kFALSE,"%.0f events");
	fStatusFrame->AddFrame(pStatusBarProcessBar,new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY,5,5,0,0));

	TGVertical3DLine *lStatusbarSeparation2=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation2, new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY));

	nStatusBarInfor = new TGLabel(fStatusFrame,"Work mode/ pedestal / raw dat");
	fStatusFrame->AddFrame(nStatusBarInfor, new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY,15,15,0,0));

	// set the time display
	TGLayoutHints *StatusBarLayout1=new TGLayoutHints(kLHintsLeft|kLHintsTop|kLHintsExpandY,10,10);
	TDatime *lSystemTime = new TDatime();
	//TGString *time_string=new TGString(Form("%4d:%2d:%2d",lSystemTime->GetYear(),lSystemTime->GetMonth(),lSystemTime->GetDate()));
	nStatusBarTimeLabel = new TGLabel(fStatusFrame,new TGString(Form("%4d : %02d : %02d",lSystemTime->GetYear(),lSystemTime->GetMonth(),lSystemTime->GetDay())));
	nStatusBarTimeLabel->Set3DStyle(0);
	fStatusFrame->AddFrame(nStatusBarTimeLabel,StatusBarLayout1);
	TGVertical3DLine *lStatusbarSeparation3=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation3, new TGLayoutHints(kLHintsLeft|kLHintsTop|kLHintsExpandY));
}

void UserGuiMainFrame::SetDataFileName(){

}

UserGuiMainFrame::~UserGuiMainFrame() {
// TODO Auto-generated destructor stub
}

void UserGuiMainFrame::CloseWindow() {
	printf("Program Terminated\n");
	gApplication->Terminate(); // the end of the program
}


Bool_t UserGuiMainFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t) {
	//printf("mcg: %d  expect %d\n",GET_MSG(msg),kC_COLORSEL);
	switch (GET_MSG(msg)) {
	case kC_COMMAND:
		switch (GET_SUBMSG(msg)) {
		case kCM_MENU:
			gKCMMenuConfirmProcess(parm1);
			break;
		case kCM_BUTTON:
			switch (parm1) {
			case C_RAWFILE_PEDESTAL: {
				dButtonPedestalOpenFileDialog();
			}
				break;
			case C_RAWFILE_DAT: {
				dButtonRawOpenFileDialog();
			}
				break;
			case C_CONFIRM: {
				bButtonConfirmProcess();
			}
				break;
			default:
				break;
			}
			break;
		case kCM_RADIOBUTTON:
			switch (parm1) {
			case C_WORKMODE_RAW:
				vWorkMode = 'R';
				SetStatusBarDisplay("Raw mode selected ");
				break;
			case C_WORKMODE_ZEROSUBTRACTION:
				vWorkMode = 'Z';
				SetWorkZoneTab();
				SetStatusBarDisplay("ZERO mode selected");
				break;
			case C_WORKMODE_PEDESTAL:
				vWorkMode = 'P';
				SetStatusBarDisplay("PEDESTAL mode selected");
				break;
			case C_WORKMODE_HIT:
				vWorkMode = 'H';
				SetStatusBarDisplay("Histo mode selected");
				break;
			case C_WORKMODE_ANALYSIS:
				vWorkMode = 'A';
				SetStatusBarDisplay("Analysis mode selected");
				break;
			case C_WORKMODE_CALIBRATION:
				vWorkMode = 'C';
				SetStatusBarDisplay("Calibration mode selected");
				break;
			default:
				break;
			}
			SetWorkZoneTab();
			break;
		default:
			break;
		}
		break;
	case kCM_CHECKBUTTON:
			switch (parm1) {         //when changed the number control
				case -1:{
					vEventNumber=tNumberEntry->GetNumberEntry()->GetIntNumber();
					if((vWorkMode=='R')&&(vRawDataList.size()!=0)){
						//tNumberEntry->SetState(kFALSE);
						fRawModeProcess(vEventNumber,vRawDataList[tRawFileEntry->GetSelected()].c_str());
						//tNumberEntry->SetState(kTRUE);
						gSystem->ProcessEvents();
					}
					string Pedestal_name= vPedestalName;
					if((vWorkMode=='Z')&&(vRawDataList.size()!=0)&&(Pedestal_name.substr(Pedestal_name.find_last_of(".")+1)=="root")){
						fZeroSupressionProcess(vEventNumber,vPedestalName.c_str(),vRawDataList[tRawFileEntry->GetSelected()].c_str());
					};
					}
					break;
				default:
					break;
			}
			break;
	case kC_COLORSEL:
		for(auto canvas_iter = fWorkZoneTabEnbeddedCanvas.begin();canvas_iter != fWorkZoneTabEnbeddedCanvas.end(); canvas_iter++ ){
			auto canvas=canvas_iter->second;
			canvas->GetCanvas()->SetFillColor(TColor::GetColor(fColorSel->GetColor()));
			canvas->GetCanvas()->Modified();
			canvas->GetCanvas()->Update();
			gSystem->ProcessEvents();
		}
		break;
	default:
		break;
	}
	return kTRUE;
}

//ooooooooooooooooooooooooooo00000000000000000000000oooooooooooooooooooooooooooooooooooo
void UserGuiMainFrame::gKCMMenuConfirmProcess(Long_t parm1){
	switch (parm1) {

	case M_FILE_OPEN: {
		dMenuOpenFileDialog();
	}
		break;
	case M_FILE_NEWCANVAS: {
		new TCanvas("UVa GEM Analysis Framework--NewCanvas",
				"UVa GEM Analysis Framework--NewCanvas", 400, 400);
	}
		break;

	case M_FILE_TBROWER: {
		new TBrowser("UVa GEM Analysis Framework--TBrowser",
				"UVa GEM Analysis Framework--Root Tree Browser");
	}
		break;

	case M_FILE_SAVE:
		printf("file->save \n");
		break;
	case M_FILE_SAVEAS:
		printf("file->saveas\n");
		break;
	case M_FILE_PRINT:
		printf("file->print\n");
		break;
	case M_FILE_EXIT:
		CloseWindow();
		break;
	case C_RAWFILE_PEDESTAL:
		printf("pdestal detected\n");
		break;

	case M_SET_LOADMAPPING:
		dMenuSetLoadMapping();
		break;
	case COLORSEL:
		printf("color color \n");
		break;

	case M_TOOL_APVMAPPINGWIZARD:
		printf("APV mapping wizard\n");
		new UserGUIMapWizard(fClient->GetRoot(), this, 400, 800);
		break;

	default:
		break;
	}
}

//ooooooooooooooooooooooooooo00000000000000000000000oooooooooooooooooooooooooooooooooooo
void UserGuiMainFrame::bButtonConfirmProcess(){
	Pixel_t red;
	fClient->GetColorByName("red", red);
	bConfirmButton->SetBackgroundColor(red);
	bConfirmButton->SetText("Processing...");
	bConfirmButton->SetEnabled(kFALSE);
	if (vWorkMode == '\0') {
		printf("Please Set the work Mode\n");
		nStatusBarInfor->SetText("Please Set the work Mode");
	} else {
		switch (vWorkMode) {
		case 'R':
			{
				if (!tRawFileEntry->GetNumberOfEntries())
				fRawModeProcess(vEventNumber,vRawDataList[tRawFileEntry->GetSelected()].c_str());
			}
			break;
		case 'Z':
			printf("zero mode\n");
			{
				if (!tRawFileEntry->GetNumberOfEntries())
					fZeroSupressionProcess(vEventNumber, vPedestalName.c_str(),
							vRawDataList[tRawFileEntry->GetSelected()].c_str());
			}
			break;

		case 'P':
			printf("pedestal mode\n");
			fPedestalModeProcess(vEventNumber, vPedestalDataFileName);
			break;

		case 'H':
			printf("histo mode\n");
			fHitModeProcess(vEventNumber, vPedestalROOTFileName, vRawDataList);
			break;

		case 'A': {
			printf("analysis mode\n");
			fAnalysisProcess(vRootDataList);
		}
			break;
		case 'C': {
			printf("Calibration mode\n");
			fCalibrationProcess(vRootDataList);

		}
			break;

		default:
			break;
		}
	}
	Pixel_t green;
	fClient->GetColorByName("green", green);
	bConfirmButton->SetBackgroundColor(green);
	bConfirmButton->SetText("Confirm");
	bConfirmButton->SetEnabled(kTRUE);
}


void UserGuiMainFrame::dMenuOpenFileDialog(){
		UserGuiGeneralDialogProcess *dialog=new UserGuiGeneralDialogProcess();
		std::string inputfilename=dialog->Browser_file();
		if(inputfilename!=NULL){
		printf("OPEN file  %s\n",inputfilename.c_str());
		}else{
			printf("on file inputed");
		}
		delete dialog;
}

void UserGuiMainFrame::dButtonPedestalOpenFileDialog(){
		UserGuiGeneralDialogProcess *dialog=new UserGuiGeneralDialogProcess();
		std::string inputfilename=dialog->Browser_file();
		if(inputfilename!=NULL){
			if(dialog->CheckAppendix(inputfilename,"root")){
				vPedestalROOTFileName=inputfilename;
				vPedestalDataFileName.clear();
				vPedestalName=inputfilename;

			}else
				//if(dialog->CheckAppendix(inputfilename,"dat"))
			{
				vPedestalDataFileName=inputfilename;
				vPedestalROOTFileName.clear();
				vPedestalName=inputfilename;
			}
			std::string filebasename=basename(strdup(vPedestalName.c_str()));
			tPedestalFileEntry->SetTitle(filebasename.c_str());
		}
		delete dialog;
}

void UserGuiMainFrame::dButtonRawOpenFileDialog(){
	UserGuiGeneralDialogProcess *dialog=new UserGuiGeneralDialogProcess();
	std::vector<std::string> RawFilenames=dialog->Browser_files();
	if(RawFilenames.size()){
		for(unsigned int file_counter=0; file_counter<RawFilenames.size();file_counter++){
			if(dialog->CheckAppendix(RawFilenames[file_counter],"root")){
				vRootDataList.push_back(RawFilenames[file_counter]);
			}else{
					vRawDataList.push_back(RawFilenames[file_counter]);
			}
		}
	}
	if (vRawDataList.size() || vRootDataList.size()) {
		tRawFileEntry->RemoveAll();
		for (unsigned int file_counter = 0; file_counter < vRawDataList.size();
				file_counter++) {
			tRawFileEntry->AddEntry(
					(dialog->GetBaseFileName(vRawDataList[file_counter])).c_str(),
					file_counter);//dialog->GetNumberFromFilename(vRawDataList[file_counter]));
		}
		for (unsigned int file_counter = 0; file_counter < vRootDataList.size();
				file_counter++) {
			tRawFileEntry->AddEntry(
					(dialog->GetBaseFileName(vRootDataList[file_counter])).c_str(),
					vRawDataList.size()+file_counter);//dialog->GetNumberFromFilename(vRootDataList[file_counter]));
		}
		tRawFileEntry->Select(0);
		tRawFileEntry->MapSubwindows();
		tRawFileEntry->Layout();
	}
}

void UserGuiMainFrame::dMenuSetLoadMapping(){
	UserGuiGeneralDialogProcess *dialog=new UserGuiGeneralDialogProcess();
	std::string inputfilename=dialog->Browser_file("");
	if((!inputfilename.empty()&&(dialog->CheckAppendix(inputfilename,"cfg")))){
		vMappingName=inputfilename;
		GEMConfigure *cfg=GEMConfigure::GetInstance();
		cfg->LoadNewMapping(vMappingName.c_str());
		SetWorkZoneTab();
		printf("%s\n",vMappingName.c_str());
	}
	delete dialog;
}


void  UserGuiMainFrame::SetStatusBarDisplay(std::string infor){
	nStatusBarInfor->SetText(infor.c_str());
	gSystem->ProcessEvents();
}

//ooooooooooooooooooooooooooo00000000000000000000000oooooooooooooooooooooooooooooooooooo
void UserGuiMainFrame::fRawModeProcess(int entries, string rawfilename){
#ifdef __DECODER_DEBUG_MODE
	rawfilename="/home/newdriver/Storage/Server/JLabFarm/mpd_ssp_3300.dat.0";
#endif
	MPDDecoder *decoder=new MPDDecoder(rawfilename.c_str());
	decoder->Connect("GUICanvasTabDraw(GUICanvasDataStream *)","UserGuiMainFrame",this,"fCanvasDraw(GUICanvasDataStream *)");
	decoder->RawDisplay(entries);

}

void UserGuiMainFrame::fPedestalModeProcess(int entries,std::string rawfilename){
	UserGuiGeneralDialogProcess *generalprocess=new UserGuiGeneralDialogProcess();
	std::string name=generalprocess->GetBaseFileName(rawfilename.c_str());
	GEMConfigure *cfg=GEMConfigure::GetInstance();
	std::cout<<"Working on "<<rawfilename.c_str()<<std::endl;
	std::string savefilename=Form(cfg->GetSysCondfig().Analysis_cfg.PedestalSavePattern.c_str(),
			generalprocess->GetNumberFromFilename(
					generalprocess->GetAppendixLess_FileName(
							name.c_str())));
	std::cout<<"File will save as "<<savefilename.c_str()<<std::endl;
	MPDDecoder *decoder= new MPDDecoder(rawfilename.c_str());
	decoder->PedestalMode(savefilename.c_str());

}


void UserGuiMainFrame::fZeroSupressionProcess(int entries,string Pedestal_name, string rawfilename){
#ifdef __DECODER_DEBUG_MODE
	Pedestal_name="/home/newdriver/Research/Eclipse_Workspace/photon/ROOT_GUI/results/Pedestal_run3300.root";
	rawfilename=("/home/newdriver/Research/Eclipse_Workspace/photon/ROOT_GUI/mpd_ssp_3300.dat.0");
#endif
	MPDDecoder *decoder=new MPDDecoder(rawfilename.c_str());
	decoder->Connect("GUICanvasTabDraw(GUICanvasDataStream *)","UserGuiMainFrame",this,"fCanvasDraw(GUICanvasDataStream *)");
	decoder->HitDisplay(Pedestal_name.c_str(),entries);
}

void UserGuiMainFrame::fAnalysisProcess(std::vector<std::string> Filenames){
	for(auto file : Filenames){
		fAnalysisProcess(file.c_str());
	}
}

void UserGuiMainFrame::fAnalysisProcess(std::string fname){
	UserGuiGeneralDialogProcess *Filenamecheck=new UserGuiGeneralDialogProcess();
	TChain *fChain = new TChain("GEMHit", "");
	Filenamecheck->CheckAppendix((fname).c_str(), "root");
	TFile *ff = new TFile((fname).c_str());
	if (ff->IsOpen()) {
		TTree *theTree = (TTree *) ff->Get("GEMHit");
		if (!(theTree->IsZombie())) {
			fChain->AddFile((fname).c_str());
		} else
			printf("Tree is not found in the file\n");
	}
	UserGuiGeneralDialogProcess *a= new UserGuiGeneralDialogProcess();
	std::string file=fname;
	GEMConfigure *cfg=GEMConfigure::GetInstance();
	UserGuiGeneralDialogProcess *generalprocess=new UserGuiGeneralDialogProcess();

	//extract the number in the string
	std::string filebasename=basename(strdup(fname.c_str()));
	filebasename.substr(0,filebasename.find_first_of("."));
	std::string filename_temp=filebasename;
	int rundividednumber=atoi(filename_temp.substr(filename_temp.find_last_of("_")+1).c_str());

	std::string test = filebasename.substr(0,filebasename.find_last_of("_"));
	std::cout<<"Test :"<< test.c_str()<<std::endl;
	std::string savefilename=Form("Tracking_run%d_%d.root",
				generalprocess->GetNumberFromFilename(
						test.c_str()),rundividednumber);
	std::cout<<"Working on file : "<< file.c_str()<<std::endl
			<<"Save file name   : "<<savefilename.c_str()<<std::endl;
	std::string savename=savefilename;
	printf("File will be save as : %s",savename.c_str());
	GEMTracking *pGEMTrack = new GEMTracking(fChain);
	pGEMTrack->Run(-1,savename.c_str());
}

void UserGuiMainFrame::fHitModeProcess(int entries,string Pedestal_name,vector<string> rawfilename){

#ifdef __DECODER_DEBUG_MODE
	Pedestal_name="/home/newdriver/Research/Eclipse_Workspace/oxygen/MPD4_vme_Decoder_infn/Pedestal/pedestal_3510.root";
	rawfilename.push_back("/media/newdriver/data/daq/data/mpd_ssp_3512.dat.0");
	rawfilename.push_back("/media/newdriver/data/daq/data/mpd_ssp_3512.dat.1");
	rawfilename.push_back("/media/newdriver/data/daq/data/mpd_ssp_3512.dat.2");
	rawfilename.push_back("/media/newdriver/data/daq/data/mpd_ssp_3512.dat.3");
	rawfilename.push_back("/media/newdriver/data/daq/data/mpd_ssp_3512.dat.4");
	rawfilename.push_back("/media/newdriver/data/daq/data/mpd_ssp_3512.dat.5");
#endif

	UserGuiGeneralDialogProcess *generalprocess=new UserGuiGeneralDialogProcess();
	GEMConfigure *cfg=GEMConfigure::GetInstance();
	for(auto file : rawfilename){
		std::string fname(file);
		std::string pedestalfname(Pedestal_name.c_str());
		MPDDecoder *decoder=new MPDDecoder(file.c_str());

		std::string savefilename=Form(cfg->GetSysCondfig().Analysis_cfg.HitSavePattern.c_str(),
					generalprocess->GetNumberFromFilename(
							generalprocess->GetAppendixLess_FileName(
									file.c_str())),generalprocess->GetDividedNumber(file.c_str()));
		std::cout<<"Working on file : "<< file.c_str()<<std::endl
				<<"Pedestal file    : "<<pedestalfname.c_str()<<std::endl
				<<"Save file name   : "<<savefilename.c_str()<<std::endl;
//		decoder->HitMode(pedestalfname.c_str(),savefilename.c_str());
		decoder->HiModeTest(pedestalfname.c_str(),savefilename.c_str());
	}
}

void UserGuiMainFrame::fCanvasDraw(GUICanvasDataStream *data){
	// new canvas functions
	data->generateHisto();
	std::map<int/*tab*/,std::map<int /*x*/,std::map<int/*y*/,TH1F *>>> histArr=data->GetHisto1dArray();
	int x_divide=data->GetCanvasDivied().X(),y_divide=data->GetCanvasDivied().Y();
	for(auto tab = histArr.begin(); tab!=histArr.end();tab++){
		int tabid=tab->first;
		if(cfWorkZoneTabCanvas.find(tabid)!=cfWorkZoneTabCanvas.end()){

			cfWorkZoneTabCanvas[tabid]->Clear();
			cfWorkZoneTabCanvas[tabid]->ResetAttPad();
			// TODO
			cfWorkZoneTabCanvas[tabid]->Divide(x_divide,y_divide);
			for(auto x_canvas = (tab->second).begin(); x_canvas!=(tab->second).end();x_canvas++){
				for(auto y_canvas = (x_canvas->second).begin();y_canvas!=(x_canvas->second).end();y_canvas++){
					int canvasid=(y_canvas->first)*int(x_divide)+x_canvas->first;
					cfWorkZoneTabCanvas[tabid]->cd(canvasid+1);
					y_canvas->second->Draw("HIST");
				}
			}
			cfWorkZoneTabCanvas[tabid]->Modified();
			cfWorkZoneTabCanvas[tabid]->Update();
		}else{
			std::cout<<__FUNCTION__<<__LINE__<< " [WORNING] "<<"Can NOT find "<< tab ->first<<" in the canvas"<<std::endl;
		}
	}
	gSystem->ProcessEvents();
}

void UserGuiMainFrame::fCalibrationProcess(std::vector<std::string> Filenames){
}
