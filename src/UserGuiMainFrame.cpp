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
#include "../GEMDecoder/input_handler.h"
#include "UserGuiGeneralDialogProcess.h"
#include "../GUIDialog/UserGUIMapWizard.h"
#include "../DecoderMPD4_VME/GEMDataParserM4V.h"

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

	SetWorkZoneTab(NTabs);
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

}

void UserGuiMainFrame::SetWorkZoneTab(unsigned int NTabs) {
	fWorkZoneTabDefultFrame = fWorkZoneTab->AddTab("WorkStatus");
	int counter=0;
	for(auto mpdname : gemInfor->GetGEMdetectorMap().GetMPDNameList()){
		fWorkZoneTabSubFrame[counter]=fWorkZoneTab->AddTab(mpdname.c_str());
		rawCanvasMPDTabCorrolation[mpdname.c_str()]=counter;
		// attach the embeded canvas
		fWorkZoneTabEnbeddedCanvas[counter] = new TRootEmbeddedCanvas("MainCanvas", fWorkZoneTabSubFrame[counter], 600,600);
		fWorkZoneTabEnbeddedCanvas[counter]->GetCanvas()->SetBorderMode(0);
		fWorkZoneTabEnbeddedCanvas[counter]->GetCanvas()->SetGrid();
		fWorkZoneTabSubFrame[counter]->AddFrame(fWorkZoneTabEnbeddedCanvas[counter],new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
		cfWorkZoneTabCanvas[counter]= fWorkZoneTabEnbeddedCanvas[counter]->GetCanvas();
		counter++;
	}
}

void UserGuiMainFrame::SetWorkZoneTab(unsigned int NTabs,std::vector<std::string> TabName){
	fWorkZoneTabDefultFrame = fWorkZoneTab->AddTab("WorkStatus");
	if(TabName.size()>=NTabs){
		int counter=0;
		for(auto mpdname : gemInfor->GetGEMdetectorMap().GetMPDNameList()){
			fWorkZoneTabSubFrame[counter]=fWorkZoneTab->AddTab(mpdname.c_str());
			rawCanvasMPDTabCorrolation[mpdname.c_str()]=counter;
			// attach the embeded canvas
			fWorkZoneTabEnbeddedCanvas[counter] = new TRootEmbeddedCanvas("MainCanvas", fWorkZoneTabSubFrame[counter], 600,600);
			fWorkZoneTabEnbeddedCanvas[counter]->GetCanvas()->SetBorderMode(0);
			fWorkZoneTabEnbeddedCanvas[counter]->GetCanvas()->SetGrid();
			fWorkZoneTabSubFrame[counter]->AddFrame(fWorkZoneTabEnbeddedCanvas[counter],new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
			cfWorkZoneTabCanvas[counter]= fWorkZoneTabEnbeddedCanvas[counter]->GetCanvas();
			counter++;
		}
	}else{
		exit(EXIT_FAILURE);
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
		for(int i =0; i <NTabs; i++){
			fWorkZoneTabEnbeddedCanvas[i]->GetCanvas()->SetFillColor(TColor::GetColor(fColorSel->GetColor()));
			fWorkZoneTabEnbeddedCanvas[i]->GetCanvas()->Modified();
			fWorkZoneTabEnbeddedCanvas[i]->GetCanvas()->Update();
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

//ooooooooooooooooooooooooooo00000000000000000000000oooooooooooooooooooooooooooooooooooo
void UserGuiMainFrame::fRawModeProcess(int entries, string rawfilename){
	if(rawPaserList.find(rawfilename.c_str())==rawPaserList.end())
		{
			rawPaserList[rawfilename.c_str()]=new GEMDataParserM4V();
			rawPaserList[rawfilename.c_str()]->OpenFileIn(rawfilename.c_str());
		}

	//GEMDataParserM4V *paser=new GEMDataParserM4V();
	//SetStatusBarDisplay(Form("Reading File"));
	//paser->OpenFileIn("/home/newdriver/Research/Eclipse_Workspace/oxygen/ROOT_GUI/Data/mpd_ssp_2726.dat.0");
	//SetStatusBarDisplay(Form("Read done! ready for select the event..."));
	rawPaserList[rawfilename.c_str()]->Connect("GEMDrawRaw(GEM::EventRawStruct)","UserGuiMainFrame",this,"fCanvasDrawRaw(GEM::EventRawStruct)" );
	rawPaserList[rawfilename.c_str()]->DrawRawDisplay(entries);
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
			rawPaserList[name.c_str()]=new GEMDataParserM4V();
			SetStatusBarDisplay(Form("Reading File"));
			rawPaserList[name.c_str()]->OpenFileIn(name.c_str());
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
		printf("%s\n",vMappingName.c_str());
	}
	delete dialog;
}


void  UserGuiMainFrame::SetStatusBarDisplay(std::string infor){
	nStatusBarInfor->SetText(infor.c_str());
	gSystem->ProcessEvents();
}



void UserGuiMainFrame::fCanvasDrawRaw(std::map<int, std::map<int,std::vector<int>>>  &event){
	TH1F *h;
	for (auto iter = rawCanvasMPDTabCorrolation.begin();iter != rawCanvasMPDTabCorrolation.end();iter++){
		std::cout<<iter->first<<" "<<iter->second<<std::endl;
	}
	for (auto iter_mpd = event.begin();
		iter_mpd!=event.end();iter_mpd++){
		if(rawCanvasMPDTabCorrolation.find(Form("Crate0_MPD%d",iter_mpd->first))!=rawCanvasMPDTabCorrolation.end()){
			int Canvas_counter=rawCanvasMPDTabCorrolation[Form("Crate0_MPD%d",iter_mpd->first)];
			cfWorkZoneTabCanvas[Canvas_counter]->Clear();
			cfWorkZoneTabCanvas[Canvas_counter]->ResetAttPad();
			cfWorkZoneTabCanvas[Canvas_counter]->Divide(5, 5);
			int histo_counter=1;
			for(auto iter_apv : iter_mpd->second){
				cfWorkZoneTabCanvas[Canvas_counter]->cd(histo_counter++);
				h= new TH1F(Form("MPD%d_ADC%d",iter_mpd->first,iter_apv.first),Form("MPD%d_ADC%d",iter_mpd->first,iter_apv.first),779,0,780);
				int i=1 ;
				for (auto channel : iter_apv.second){
					h->Fill(i++,channel);
				}
				h->Draw();

				std::cout<<"CanvasID:"<<Canvas_counter<<"  MPD"<< iter_mpd->first<<"  "<< iter_apv.first<<std::endl;
			}
			cfWorkZoneTabCanvas[Canvas_counter]->Modified();
			cfWorkZoneTabCanvas[Canvas_counter]->Update();
		}
	}
	gSystem->ProcessEvents();
}


void UserGuiMainFrame::generalCanvasDraw(std::map<int, std::map<int,TH1F *>> histos,int CanvasID){
	cfWorkZoneTabCanvas[CanvasID]->Clear();
	int lCanvasDiv_x=0,lCanvasDiv_y=0;
	for(auto iter_y = histos.begin();iter_y!=histos.end();iter_y++){
		if(iter_y->first > lCanvasDiv_y)lCanvasDiv_y=iter_y->first;
		for(auto iter_x=iter_y->second.begin();iter_x!=iter_y->second.end();iter_x++){
			if(iter_x->first > lCanvasDiv_x)lCanvasDiv_x=iter_x->first;
		}
	}
	cfWorkZoneTabCanvas[CanvasID]->ResetAttPad();
	cfWorkZoneTabCanvas[CanvasID]->Divide(lCanvasDiv_x,lCanvasDiv_y);

	int subcanvas_counter=1;
	for (auto iter_y = histos.begin(); iter_y != histos.end(); iter_y++) {
		for (auto iter_x = iter_y->second.begin();
				iter_x != iter_y->second.end(); iter_x++) {
			cfWorkZoneTabCanvas[CanvasID]->cd(subcanvas_counter++);
			iter_x->second->Draw();
		}
	}
	cfWorkZoneTabCanvas[CanvasID]->Modified();
	cfWorkZoneTabCanvas[CanvasID]->Update();
	gSystem->ProcessEvents();
};

void UserGuiMainFrame::fCanvasDrawRaw(GEM::EventRawStruct event){
#ifdef __SLOT_DEBUG_MODE
	std::cout<<"catch signal :"<<event.raw.size()<<std::endl;
#endif
	fCanvasDrawRaw(event.raw);
}

//void UserGuiMainFrame::fRawModeProcess(int entries, string rawfilename){
//	if (!rawfilename.empty()) {
//		printf("Filename: %s\n", rawfilename.c_str());
//		InputHandler *inputHandler = new InputHandler(rawfilename.c_str());
//		if (!vMappingName.empty())
//			inputHandler->SetMapping(vMappingName.c_str());
//		map<int, map<int, map<int, std::vector<int> > > > a;
//		map<int, map<int, map<int, std::vector<int> > > > dMultiGEMSingleEvent =
//				inputHandler->RawProcessAllEvents(entries, a);
//		for (std::map<int, std::map<int, std::map<int, TH1F*>>>::iterator iter_detectorID=dMultiGEMHistoBuffer.begin();iter_detectorID!=dMultiGEMHistoBuffer.end();iter_detectorID++) {
//			for(std::map<int,std::map<int,TH1F*>>::iterator itter_mpd=iter_detectorID->second.begin();itter_mpd!=iter_detectorID->second.end();itter_mpd++) {
//				for(std::map<int,TH1F*>::iterator ittter_apv=itter_mpd->second.begin();ittter_apv!=itter_mpd->second.end();ittter_apv++) {
//					delete dMultiGEMHistoBuffer[iter_detectorID->first][itter_mpd->first][ittter_apv->first];
//				}
//			}
//		}
//		dMultiGEMHistoBuffer.clear();
//		// test functions
//		for (map<int, map<int, map<int, std::vector<int> > > >::iterator iter_detectorID =
//				dMultiGEMSingleEvent.begin();
//				iter_detectorID != dMultiGEMSingleEvent.end();
//				iter_detectorID++) {
//			for (map<int, map<int, std::vector<int> > >::iterator itter_mpd =
//					iter_detectorID->second.begin();
//					itter_mpd != iter_detectorID->second.end(); itter_mpd++) {
//				for (map<int, std::vector<int> >::iterator ittter_apv =
//						itter_mpd->second.begin();
//						ittter_apv != itter_mpd->second.end(); ittter_apv++) {
//					dMultiGEMHistoBuffer[iter_detectorID->first][itter_mpd->first][ittter_apv->first] =
//							new TH1F(
//									Form("MPD%d_ADC%d", itter_mpd->first,
//											ittter_apv->first),
//									Form("MPD%d_ADC%d", itter_mpd->first,
//											ittter_apv->first), 800, 0, 800);
//					for (unsigned int i = 0; i < (ittter_apv->second).size();
//							i++) {
//						dMultiGEMHistoBuffer[iter_detectorID->first][itter_mpd->first][ittter_apv->first]->Fill(
//								i, (ittter_apv->second)[i]);
//					}
//				}
//			}
//		}
//		unsigned int Canvas_counter = 0;
//		for (std::map<int, std::map<int, std::map<int, TH1F*>>>::iterator iter_detectorid=dMultiGEMHistoBuffer.begin();iter_detectorid!=dMultiGEMHistoBuffer.end();iter_detectorid++) {
//			cfWorkZoneTabCanvas[Canvas_counter]->Clear();
//			cfWorkZoneTabCanvas[Canvas_counter]->ResetAttPad();
//			cfWorkZoneTabCanvas[Canvas_counter]->Divide(5,5);
//			int canvaspad_counter=0;
//			for(std::map<int,std::map<int,TH1F*>>::iterator itter_mpd=iter_detectorid->second.begin();itter_mpd!=iter_detectorid->second.end();itter_mpd++) {
//				for(std::map<int,TH1F*>::iterator ittter_apv=itter_mpd->second.begin();ittter_apv!=itter_mpd->second.end();ittter_apv++) {
//					cfWorkZoneTabCanvas[Canvas_counter]->cd(canvaspad_counter+1);
//					ittter_apv->second->SetMaximum(2500);
//					ittter_apv->second->SetMinimum(0);
//					ittter_apv->second->Draw();
//					canvaspad_counter++;
//				}
//			}
//			cfWorkZoneTabCanvas[Canvas_counter]->Modified();
//			cfWorkZoneTabCanvas[Canvas_counter]->Update();
//			Canvas_counter++;
//		}
//		gSystem->ProcessEvents();
//		delete inputHandler;
//	} else {
//		printf("No input file detected\n");
//	}
//}

void UserGuiMainFrame::fZeroSupressionProcess(int entries,string Pedestal_name, string rawfilename){

	/*std::map<int,std::map<int,TH1F*> > ZeroSubHistoBuffer;
	if(Pedestal_name.substr(Pedestal_name.find_last_of(".")+1)=="root"){

			InputHandler *inputHandler= new InputHandler(rawfilename.c_str());
			if(!vMappingName.empty()) inputHandler->SetMapping(vMappingName.c_str());
			std::map<int,std::map<int,std::map<int,int> > > a;
			std::map<int,std::map<int,std::map<int,int> > > ZeroSubRawBuffer=inputHandler->ZeroSProcessSingleEvents(vEventNumber,a,Pedestal_name.c_str(),-1);

			for(std::map<int,std::map<int,std::map<int,int> > > ::iterator iter_detectorID=ZeroSubRawBuffer.begin(); iter_detectorID!=ZeroSubRawBuffer.end();iter_detectorID++){
				ZeroSubHistoBuffer[iter_detectorID->first][1]=new TH1F("BeforeZeroSupression_X",Form("GEM_%d_BeforeZeroSupression_X",iter_detectorID->first),1600,0,1600);
				ZeroSubHistoBuffer[iter_detectorID->first][11]=new TH1F("BeforeZeroSupression_Y",Form("GEM_%d_BeforeZeroSupression_Y",iter_detectorID->first),1600,0,1600);

				ZeroSubHistoBuffer[iter_detectorID->first][2]=new TH1F("AfterZeroSupression_X",Form("GEM_%d_AfterZeroSupression_X",iter_detectorID->first),1600,0,1600);
				ZeroSubHistoBuffer[iter_detectorID->first][12]=new TH1F("AfterZeroSupression_Y",Form("GEM_%d_AfterZeroSupression_Y",iter_detectorID->first),1600,0,1600);

				ZeroSubHistoBuffer[iter_detectorID->first][3]=new TH1F("AfterRemoveCrosstalk_X",Form("GEM_%d_AfterRemoveCrosstalk_X",iter_detectorID->first),1600,0,1600);
				ZeroSubHistoBuffer[iter_detectorID->first][13]=new TH1F("AfterRemoveCrosstalk_Y",Form("GEM_%d_AfterRemoveCrosstalk_Y",iter_detectorID->first),1600,0,1600);

				ZeroSubHistoBuffer[iter_detectorID->first][4]=new TH1F("CrossTalk_X",Form("GEM_%d_CrossTalk_X",iter_detectorID->first),1600,0,1600);
				ZeroSubHistoBuffer[iter_detectorID->first][14]=new TH1F("CrossTalk_Y",Form("GEM_%d_CrossTalk_Y",iter_detectorID->first),1600,0,1600);

				for(std::map<int,std::map<int,int> >::iterator itter_dimension=iter_detectorID->second.begin();itter_dimension!=iter_detectorID->second.end();itter_dimension++){
					for(std::map<int,int>::iterator ittter_strips=itter_dimension->second.begin(); ittter_strips!=itter_dimension->second.end();ittter_strips++) {
						ZeroSubHistoBuffer[iter_detectorID->first][itter_dimension->first]->Fill(ittter_strips->first,ittter_strips->second);
					}
				}
			}
		unsigned int Canvas_counter = 0;
		for(std::map<int,std::map<int,TH1F *>>::iterator iter_detectorID=ZeroSubHistoBuffer.begin();iter_detectorID!=ZeroSubHistoBuffer.end();iter_detectorID++){
			cfWorkZoneTabCanvas[Canvas_counter]->Clear();
			cfWorkZoneTabCanvas[Canvas_counter]->ResetAttPad();
			cfWorkZoneTabCanvas[Canvas_counter]->Divide(2, 3);
			cfWorkZoneTabCanvas[Canvas_counter]->cd(1);
			iter_detectorID->second[1]->Draw();
			cfWorkZoneTabCanvas[Canvas_counter]->cd(3);
			iter_detectorID->second[2]->Draw();
			cfWorkZoneTabCanvas[Canvas_counter]->cd(5);
			iter_detectorID->second[3]->Draw();
			iter_detectorID->second[4]->SetLineColor(2);
			iter_detectorID->second[4]->Draw("same");

			cfWorkZoneTabCanvas[Canvas_counter]->cd(2);
			iter_detectorID->second[11]->Draw();
			cfWorkZoneTabCanvas[Canvas_counter]->cd(4);
			iter_detectorID->second[12]->Draw();
			cfWorkZoneTabCanvas[Canvas_counter]->cd(6);
			iter_detectorID->second[13]->Draw();
			iter_detectorID->second[14]->SetLineColor(2);
			iter_detectorID->second[14]->Draw("same");

			cfWorkZoneTabCanvas[Canvas_counter]->Modified();
			cfWorkZoneTabCanvas[Canvas_counter]->Update();
			gSystem->ProcessEvents();
			Canvas_counter++;
		}
	}else{
		printf("Pedestal File structure unrecgnized, only .root allowed\n");
	}
*/
}

void UserGuiMainFrame::fAnalysisProcess(std::vector<std::string> Filenames){

/*	UserGuiGeneralDialogProcess *Filenamecheck=new UserGuiGeneralDialogProcess();
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
	*/
}

void UserGuiMainFrame::fPedestalModeProcess(int entries,std::string rawfilename){
/*	std::ifstream testfile(rawfilename.c_str());
	string Pedestal_name= rawfilename;
	if ((!Pedestal_name.empty())&&testfile.good()&&(Pedestal_name.substr(Pedestal_name.find_last_of(".")+1)=="dat")) {
		printf("start pedestal mode\n");

		string raw_filename(basename(strdup(vPedestalName.c_str()))); // get the basename
		string filename_noappendix=raw_filename.substr(0,raw_filename.find_last_of("."));
		string number_index=filename_noappendix.substr(filename_noappendix.find_last_not_of("0123456789")+1);
		std::string Pedestal_outname(Form(tOutPutfilePattern->GetTitle(),"_Pedestal",atoi(number_index.c_str())));

		InputHandler * decoder = new InputHandler(vPedestalName.c_str());
		if(!vMappingName.empty()) decoder->SetMapping(vMappingName.c_str());
		if (entries > 1) {
			decoder->PedProcessAllEvents(entries, Pedestal_outname.c_str());
		} else {
			decoder->PedProcessAllEvents(Pedestal_outname.c_str());
		}
		printf("Pedestal file is saved as: %s\n",Pedestal_outname.c_str());
		delete decoder;
	} else {
		printf("Please input the Pedestal file\n");
		// set the color
		Pixel_t red;
		gClient->GetColorByName("red", red);
		nStatusBarInfor->SetBackgroundColor(red);
		nStatusBarInfor->SetText(
				"Please input the Pedestal file");
	}*/
}

void UserGuiMainFrame::fHitModeProcess(int entries,string Pedestal_name,vector<string> rawfilename){
	/*UserGuiGeneralDialogProcess *dialog = new UserGuiGeneralDialogProcess();
	if (rawfilename.size() != 0) {
		std::ifstream testfile(Pedestal_name.c_str());
		if ((!testfile.good())|| (!(dialog->CheckAppendix(Pedestal_name, "root")))) {
			printf("Please input the Pedestal file\n");
			nStatusBarInfor->SetText("Please input the Pedestal file");
		} else {
				for (unsigned int i = 0; i < rawfilename.size(); i++) {
					std::ifstream testfile(rawfilename[i].c_str());
					if (testfile.good()) {
						printf("Processing  %s\n", rawfilename[i].c_str());
						nStatusBarInfor->SetText(
								Form("Processing  %s\n",
										rawfilename[i].c_str()));
						InputHandler * decoder = new InputHandler(
								rawfilename[i].c_str());
						if (!vMappingName.empty())
							decoder->SetMapping(vMappingName.c_str());

						string raw_filename(
								basename(strdup(rawfilename[i].c_str()))); // get the basename
						string filename_noappendix = raw_filename.substr(0,
								raw_filename.find_last_of("."));
						string number_index = filename_noappendix.substr(
								filename_noappendix.find_last_not_of(
										"0123456789") + 1);
						std::string Hit_outname(
								Form(tOutPutfilePattern->GetTitle(), "",
										atoi(number_index.c_str())));
						if (entries > 2) {
							decoder->HitProcessAllEvents(entries,
									vPedestalName.c_str(), Hit_outname.c_str());
						} else {
							decoder->HitProcessAllEvents(vPedestalName.c_str(),
									Hit_outname.c_str());
						}
						printf("OutPut file is save as %s\n",
								Hit_outname.c_str());
						nStatusBarInfor->SetText(
								Form("OutPut file is save as %s\n",
										Hit_outname.c_str()));
						//delete decoder;
						delete decoder;
					}
			}
		}
	}*/
}

void UserGuiMainFrame::fCalibrationProcess(std::vector<std::string> Filenames){
/*
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
	std::string savename(Form(tOutPutfilePattern->GetTitle(),"_Calibration",a->GetNumberFromFilename(Filenames[0])));
	GEMTracking *pGEMTrack = new GEMTracking(fChain);
	pGEMTrack->Calibration(-1,savename.c_str());
	*/
}
