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
//#include "../GEMDecoder/input_handler.h"
#include "UserGuiGeneralDialogProcess.h"
#include "../GUIDialog/UserGUIMapWizard.h"

// raw decoder
#include "../GEMDetector/MPDDecoder.h"
#include "../GEMDetector/GEMConfigure.h"

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

	SetWorkZoneTab();
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
		fWorkZoneTab->RemoveTab(1);
	};
	fWorkZoneTabDefultFrame = fWorkZoneTab->AddTab("WorkStatus");
	// this is just used for draw the GUI, so efficency is not the first concen
	GEMConfigure *cfg=GEMConfigure::GetInstance();
	cfg->GetMapping().GetMPDList();
	SetWorkZoneTab(cfg->GetMapping().GetMPDList());
	gSystem->ProcessEvents();
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
//		fWorkZoneTabEnbeddedCanvas.push_back(new TRootEmbeddedCanvas("MainCanvas", fWorkZoneTabSubFrame.back(), 600,600));
//		fWorkZoneTabEnbeddedCanvas.back()->GetCanvas()->SetBorderMode(0);
//		fWorkZoneTabEnbeddedCanvas.back()->GetCanvas()->SetGrid();
//		fWorkZoneTabSubFrame.back()->AddFrame(fWorkZoneTabEnbeddedCanvas.back(),new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
//		cfWorkZoneTabCanvas.push_back(fWorkZoneTabEnbeddedCanvas.back()->GetCanvas());
		gSystem->ProcessEvents();
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
						//tNumberEntry->SetState(kFALSE);
						//fZeroSupressionProcess(vEventNumber,vPedestalName.c_str(),vRawDataList[0].c_str());
						fZeroSupressionProcess(vEventNumber,vPedestalName.c_str(),vRawDataList[tRawFileEntry->GetSelected()].c_str());
						//tNumberEntry->SetState(kTRUE);
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
//			canvas->GetCanvas()->Modified();
			canvas->GetCanvas()->Update();
			canvas->GetCanvas()->Draw();
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

		for (auto name :vRawDataList ){
			//rawPaserList[name.c_str()]=new GEMDataParserM4V();
			SetStatusBarDisplay(Form("Reading File"));
//			rawPaserList[name.c_str()]->OpenFileIn(name.c_str());
			SetStatusBarDisplay(Form("Reading File Done!"));
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
	MPDDecoder *decoder=new MPDDecoder(rawfilename.c_str());
	decoder->Connect("GUICanvasTabDraw(GUICanvasDataStream *)","UserGuiMainFrame",this,"fCanvasDraw(GUICanvasDataStream *)");
	decoder->RawDisplay(entries);

//	if(rawPaserList.find(rawfilename.c_str())==rawPaserList.end())
//		{
//			rawPaserList[rawfilename.c_str()]=new GEMDataParserM4V();
//			//rawPaserList[rawfilename.c_str()]->OpenFileIn(rawfilename.c_str());
//		}
//	rawPaserList[rawfilename.c_str()]->Connect("GEMDrawRaw(GEM::EventRawStruct)","UserGuiMainFrame",this,"fCanvasDrawRaw(GEM::EventRawStruct)" );
//	rawPaserList[rawfilename.c_str()]->DrawRawDisplay(entries);

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

}

void UserGuiMainFrame::fAnalysisProcess(std::vector<std::string> Filenames){

	UserGuiGeneralDialogProcess *Filenamecheck=new UserGuiGeneralDialogProcess();
	TChain *fChain = new TChain("GEMHit", "");
	std::vector<std::string>::iterator iter_filename=Filenames.begin();
	while (iter_filename != Filenames.end()) {
		Filenamecheck->CheckAppendix((*iter_filename).c_str(), "root");
		TFile *ff = new TFile((*iter_filename).c_str());
		if (ff->IsOpen()) {
			TTree *theTree = (TTree *) ff->Get("GEMHit");
			if (!(theTree->IsZombie())) {
				fChain->AddFile((*iter_filename).c_str());
			} else
				printf("Tree is not found in the file\n");
		}
		iter_filename++;
	}
	UserGuiGeneralDialogProcess *a= new UserGuiGeneralDialogProcess();

	std::string savename(Form(tOutPutfilePattern->GetTitle(),"_Tracking",a->GetNumberFromFilename(Filenames[0])));
	printf("File will be save as : %s",savename.c_str());
	GEMTracking *pGEMTrack = new GEMTracking(fChain);
	pGEMTrack->Run(-1,savename.c_str());

}



void UserGuiMainFrame::fHitModeProcess(int entries,string Pedestal_name,vector<string> rawfilename){
	std::string fname("/home/newdriver/Storage/mpdssp_data/mpd_ssp_test_3119.dat.0");
	std::string pedestalfname("/home/newdriver/Research/Eclipse_Workspace/photon/ROOT_GUI/pedestal.root");
	MPDDecoder *decoder=new MPDDecoder(fname.c_str());
	decoder->HitMode(pedestalfname.c_str(),"test_hit.root","");
}

void UserGuiMainFrame::fCanvasDraw(GUICanvasDataStream *data){

	std::map<int/**/,std::vector<int>> rawdata = data->GetRaw();
	std::map<int/*tab*/,std::vector<std::vector<int>>> tabrawdata;
	for(auto apv = rawdata.begin();apv!=rawdata.end();apv++){
		int crateid=GEM::getCrateID(apv->first);
		int mpdid=GEM::getMPDID(apv->first);
		int id=GEM::GetUID(crateid,mpdid,0,0);
		tabrawdata[id].push_back(apv->second);
	}
	// solve the issues that if there is no data, it would show the previous data
	if (tabrawdata.size() == 0) {
		for (auto i = cfWorkZoneTabCanvas.begin();
				i != cfWorkZoneTabCanvas.end(); i++) {
			i->second->Clear();
			i->second->Update();
			i->second->Draw();
		}
	}
	for(auto mpd_iter = tabrawdata.begin();mpd_iter!=tabrawdata.end();mpd_iter++){
		int tabcanvasid=mpd_iter->first;
		if(cfWorkZoneTabCanvas.find(tabcanvasid)!=cfWorkZoneTabCanvas.end()){
			cfWorkZoneTabCanvas[tabcanvasid]->Clear();
			cfWorkZoneTabCanvas[tabcanvasid]->ResetAttPad();
			cfWorkZoneTabCanvas[tabcanvasid]->Divide(4,4);
			int canvas_counter=1;
			for(auto apv : mpd_iter->second){
				TH1F *h = new TH1F(
						Form("crate%d_mpd%", GEM::getCrateID(tabcanvasid),
								GEM::getMPDID(tabcanvasid)),
						Form("crate%d_mpd%", GEM::getCrateID(tabcanvasid),
								GEM::getMPDID(tabcanvasid)), 800, 0, 800);
				for(int i = 0; i <apv.size();i++){
					h->Fill(i+1,apv.at(i));
					h->SetYTitle("ADC");
					h->SetXTitle("channel");

				}
				cfWorkZoneTabCanvas[tabcanvasid]->cd(canvas_counter++);
//				/h->SetMarkerStyle(21);
				h->Draw("HIST");
			}

		}else{
			std::cout<<"cannot find"<<GEM::getCrateID(tabcanvasid)<<" "<<GEM::getMPDID(tabcanvasid)<<std::endl;
		}
		//cfWorkZoneTabCanvas[tabcanvasid]->Modified();
		cfWorkZoneTabCanvas[tabcanvasid]->Update();
		cfWorkZoneTabCanvas[tabcanvasid]->Draw();
	}
	gSystem->ProcessEvents();

}

void UserGuiMainFrame::fCalibrationProcess(std::vector<std::string> Filenames){
}
