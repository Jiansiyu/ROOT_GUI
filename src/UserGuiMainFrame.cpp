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

#include "libgen.h"
#include "string"

// main decoder lib
#include "../GEMDecoder/input_handler.h"
#include "UserGuiGeneralDialogProcess.h"
const char *filetype[] = {
			"ROOT files", "*.root",
			"Data files", "*.dat",
			"All files", "*",
			0, 0
	};
const char *datfiletype[]={
		"Data files", "*.dat",
		"All files", "*",
					0, 0
};
UserGuiMainFrame::UserGuiMainFrame(const TGWindow *p, UInt_t w, UInt_t h) :
		TGMainFrame(p, w, h) {
	// TODO Auto-generated constructor stub
	SetCleanup(kDeepCleanup);

	// oooooooooooOOOOOOOOOOO000000000000OOOOOOOOOooooooooooooooooo
	// user variable initialize
	vWorkMode=NULL;
	// oooooooooooOOOOOOOOOOO000000000000OOOOOOOOOooooooooooooooooo


	fMenuDock = new TGDockableFrame(this);
	AddFrame(fMenuDock, new TGLayoutHints(kLHintsExpandX, 0, 0, 1, 0));
	fMenuDock->SetWindowName("MainFrame Menu");

	// set the layout of the menu bar
	fMenuBarLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX);
	fMenuBarItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0,
			0);
	fMenuBarHelpLayout = new TGLayoutHints(kLHintsTop | kLHintsRight);

	// set the "File" menu
	fMenuFile = new TGPopupMenu(fClient->GetRoot());
	SetMenuFile();

	fMenuSet = new TGPopupMenu(fClient->GetRoot());
	SetMenuSet();


	fMenuView = new TGPopupMenu(fClient->GetRoot());
	SetMenuView();

	fMenuHelp = new TGPopupMenu(fClient->GetRoot());
	SetMenuHelp();


	fMenuDock->EnableUndock(kTRUE);
	fMenuDock->EnableHide(kTRUE);

	// Menu button messages are handled by the main frame (i.e. "this")
	// ProcessMessage() method.
	fMenuFile->Associate(this);
	fMenuSet->Associate(this);
	fMenuView->Associate(this);
	fMenuHelp->Associate(this);

	fMenuBar = new TGMenuBar(fMenuDock,1,1,kHorizontalFrame);
	fMenuBar -> AddPopup("&File",fMenuFile,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&Set",fMenuSet,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&View",fMenuView,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&Help",fMenuHelp,fMenuBarHelpLayout);
	fMenuDock -> AddFrame(fMenuBar,fMenuBarLayout);
	// add separation line to the menu bar
	TGHorizontal3DLine *menu_seperator=new TGHorizontal3DLine(this);
	AddFrame(menu_seperator,new TGLayoutHints(kLHintsExpandX));


	// add the work zone frame
	fWorkZoneFrame = new TGHorizontalFrame(this);
	fWorkZoneControlFrame = new TGVerticalFrame(fWorkZoneFrame, 10,10);
	SetWorkZoneButton();   // set the control penal


	//fWorkZoneControlFrame->SetBackgroundColor(33);
	fWorkZoneCanvasFrame  = new TGVerticalFrame(fWorkZoneFrame, 10,10);
	//fWorkZoneCanvasFrame -> SetBackgroundColor(1);
	TGVertical3DLine * WorkZoneSeparation= new TGVertical3DLine(fWorkZoneFrame,10,10);
	fEmnbeddedCanvas = new TRootEmbeddedCanvas("MainCanvas", fWorkZoneCanvasFrame, 600,600);
	fEmnbeddedCanvas->GetCanvas()->SetBorderMode(0);
	fEmnbeddedCanvas->GetCanvas()->SetFillColor(0);
	fEmnbeddedCanvas->GetCanvas()->SetFrameFillColor(41);
	fEmnbeddedCanvas->GetCanvas()->SetGrid();
	fWorkZoneCanvasFrame->AddFrame(fEmnbeddedCanvas,new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
	cRawCanvas=fEmnbeddedCanvas->GetCanvas();

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
	Resize();   // resize to default size
	MapWindow();
	Print();
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

void UserGuiMainFrame::SetWorkZoneButton(){

	bWorkModeButtonGroup   = new TGButtonGroup(fWorkZoneControlFrame,"Work Mode");
	bWorkModeButtonGroup -> SetTitlePos(TGGroupFrame::kCenter);
	// add the button to the frame
	TGLayoutHints *bWorkModeLayout= new TGLayoutHints(kLHintsTop);
	bWorkModeRAW = new TGRadioButton(bWorkModeButtonGroup,"&Raw",C_WORKMODE_RAW);
	bWorkModeZeroSubtraction = new TGRadioButton(bWorkModeButtonGroup,"&ZeroSub",C_WORKMODE_ZEROSUBTRACTION);
	bWorkModePedestal = new TGRadioButton(bWorkModeButtonGroup,"&Pedestal",C_WORKMODE_PEDESTAL);
	bWorkModeHit = new TGRadioButton(bWorkModeButtonGroup,"&Hit",C_WORKMODE_HIT);
	bWorkModeAnalysis = new TGRadioButton(bWorkModeButtonGroup,"&Analysis",C_WORKMODE_ANALYSIS);
	bWorkModeRAW->Associate(this);
	bWorkModeZeroSubtraction ->Associate(this);
	bWorkModePedestal ->Associate(this);
	bWorkModeHit ->Associate(this);
	bWorkModeAnalysis ->Associate(this);

	fWorkZoneControlFrame->AddFrame(bWorkModeButtonGroup , new TGLayoutHints(kLHintsExpandX));

	TGGroupFrame * fDataInputFrame= new TGGroupFrame(fWorkZoneControlFrame,"Data Input");
	fDataInputFrame->SetTitlePos(TGGroupFrame::kCenter);

	tPedestalFileEntry  =  new TGTextEntry(fDataInputFrame);
	tPedestalFileEntry->SetTitle("Pedestal(*.root/*.dat)");
	bPedestalFileButton = new TGTextButton (fDataInputFrame,"Pedestal..",C_RAWFILE_PEDESTAL);
	bPedestalFileButton->Associate(this);

	fDataInputFrame->AddFrame(tPedestalFileEntry,new TGLayoutHints(kLHintsExpandX));
	fDataInputFrame->AddFrame(bPedestalFileButton,new TGLayoutHints(kLHintsRight));
	tRawFileEntry = new TGListBox(fDataInputFrame);
	tRawFileEntry->Resize(150,80);
	tRawFileEntry->AddEntry("*.dat files to be process",0);
	bRawFileButton = new TGTextButton (fDataInputFrame,"Data..",C_RAWFILE_DAT);
	bRawFileButton -> Associate(this);

	fDataInputFrame->AddFrame(tRawFileEntry,new TGLayoutHints(kLHintsLeft|kLHintsExpandX,5,5,5,5));
	fDataInputFrame->AddFrame(bRawFileButton,new TGLayoutHints(kLHintsRight));
	fWorkZoneControlFrame->AddFrame(fDataInputFrame,new TGLayoutHints(kLHintsCenterX));

	// out put name control
	TGGroupFrame * fDataOutPutFrame= new TGGroupFrame(fWorkZoneControlFrame,"Out Put Pattern");
	fDataOutPutFrame->SetTitlePos(TGGroupFrame::kCenter);
	tOutPutfilePattern = new TGTextEntry(fDataOutPutFrame);
	tOutPutfilePattern ->SetTitle("~/SBS39%s_temp%04d.root");
	fDataOutPutFrame->AddFrame(tOutPutfilePattern, new TGLayoutHints(kLHintsExpandX));
	fWorkZoneControlFrame->AddFrame(fDataOutPutFrame, new TGLayoutHints(kLHintsExpandX));

	// number control
	fNumberFrame = new TGGroupFrame(fWorkZoneControlFrame, "Event Control");
	fNumberFrame->SetTitlePos(TGGroupFrame::kCenter);
	tNumberEntry = new TGNumberEntry(fNumberFrame);
	tNumberEntry->SetUniqueID(V_NUMBERINPUT);
	tNumberEntry->Associate(this);
	fNumberFrame->AddFrame(tNumberEntry,new TGLayoutHints(kLHintsExpandX));
	fWorkZoneControlFrame->AddFrame(fNumberFrame,new TGLayoutHints(kLHintsExpandX));


	//confirm button
	bConfirmButton = new TGTextButton(fWorkZoneControlFrame,"Confirm",C_CONFIRM);
	bConfirmButton->Associate(this);
	fWorkZoneControlFrame->AddFrame(bConfirmButton, new TGLayoutHints(kLHintsBottom|kLHintsExpandX,5,5,5,5));
}

void UserGuiMainFrame::SetStatusBar(){

	//TGVertical3DLine *lStatusbarSeparation0=new TGVertical3DLine(fStatusFrame);
	//fStatusFrame->AddFrame(lStatusbarSeparation0, new TGLayoutHints(kLHintsLeft|kLHintsTop|kLHintsExpandY));

	TGLabel *autor_display= new TGLabel(fStatusFrame,"UVa GEM Analysis Framework Author: Siyu Jian");
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
	nStatusBarTimeLabel = new TGLabel(fStatusFrame,new TGString(Form("%4d : %02d : %02d",lSystemTime->GetYear(),lSystemTime->GetMonth(),lSystemTime->GetDay(),lSystemTime->GetSecond())));
	nStatusBarTimeLabel->Set3DStyle(0);
	fStatusFrame->AddFrame(nStatusBarTimeLabel,StatusBarLayout1);
	TGVertical3DLine *lStatusbarSeparation3=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation3, new TGLayoutHints(kLHintsLeft|kLHintsTop|kLHintsExpandY));
}
void UserGuiMainFrame::fFileBrowse() {
	printf("where am I \n");
	static TString dir(".");
	TGFileInfo file_infor;
	file_infor.fFileTypes = filetype;
	file_infor.fIniDir = StrDup(dir);
	new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &file_infor);
	printf(" try to open file %s\n", file_infor.fFilename);
}

void UserGuiMainFrame::SetDataFileName(){
	printf("where am I \n");
}

UserGuiMainFrame::~UserGuiMainFrame() {
// TODO Auto-generated destructor stub
}

void UserGuiMainFrame::CloseWindow() {

	gApplication->Terminate(); // the end of the program
}

Bool_t UserGuiMainFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t) {
	//printf("transfer key pressed %d, %d, %d\n",kCM_CHECKBUTTON,GET_MSG(msg),parm1);
	switch (GET_MSG(msg)) {

	case kC_COMMAND:
		switch (GET_SUBMSG(msg)) {
		case kCM_MENU:
			switch (parm1) {
			case M_FILE_OPEN: {
				dMenuOpenFileDialog();
			}
				break;
			case M_FILE_NEWCANVAS:{
				TCanvas *Canvas= new TCanvas("UVa GEM Analysis Framework--NewCanvas","UVa GEM Analysis Framework--NewCanvas",400,400);
			}
			break;

			case M_FILE_TBROWER: {
				TBrowser *tBrowser=new TBrowser("UVa GEM Analysis Framework--TBrowser","UVa GEM Analysis Framework--Root Tree Browser");
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
			default:
				break;
			}
			break;
		case kCM_BUTTON:
			switch (parm1) {
			case C_RAWFILE_PEDESTAL: {
				dButtonPedestalOpenFileDialog();
//				static TString dir(".");
//				TGFileInfo fi;
//				fi.fFileTypes = filetype;
//				fi.fIniDir = StrDup(dir);
//				new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);
//				if (fi.fFilename != NULL) {
//					std::string filename = fi.fFilename;
//					vPedestalName = filename;
//					printf("Open file: %s \n", vPedestalName.c_str());
//					std::string filname = (basename(
//							strdup(vPedestalName.c_str())));
//					tPedestalFileEntry->SetTitle(filename.c_str());
//				}
			}
				break;
			case C_RAWFILE_DAT: {
				static TString dir(".");
				TGFileInfo fi;
				fi.fFileTypes = datfiletype;
				fi.fIniDir = StrDup(dir);
				new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);
				if (fi.fMultipleSelection && fi.fFileNamesList) {
					TObjString *el;
					TIter next(fi.fFileNamesList);
					while ((el = (TObjString *) next())) {
						string filename(el->GetString().Data());
						vRawDataList.push_back(filename);
						tRawFileEntry->RemoveAll();
						for (int i = 0; i < vRawDataList.size(); i++) {
							string raw_filename(
							basename(strdup(vRawDataList[i].c_str())));
							tRawFileEntry->AddEntry(
									Form("%s", raw_filename.c_str()), i);
							tRawFileEntry->MapSubwindows();
							tRawFileEntry->Layout();
						}
					}
				} else if (fi.fFilename != NULL) {
					std::string filename = (fi.fFilename);
					std::string FileFullName = filename;
					vRawDataList.push_back(FileFullName);
					tRawFileEntry->RemoveAll();
					for (int i = 0; i < vRawDataList.size(); i++) {
						string raw_filename(
						basename(strdup(vRawDataList[i].c_str())));
						tRawFileEntry->AddEntry(
								Form("%s", raw_filename.c_str()), i);
						tRawFileEntry->MapSubwindows();
						tRawFileEntry->Layout();
					}
				}
			}
				break;
			case C_CONFIRM:
				{
					if (vWorkMode==NULL) {
						printf("Please Set the work Mode\n");
						// set the color
						Pixel_t red;
						gClient->GetColorByName("red",red);
						nStatusBarInfor->SetBackgroundColor(red);
						nStatusBarInfor->SetText("Please Set the work Mode");
					}else {
					switch (vWorkMode) {
					case 'R':
						printf("raw mode\n");
						{
							fRawModeProcess(vEventNumber,vRawDataList[0].c_str());
						}
						break;
					case 'Z':
						printf("zero mode\n");
						{
							//InputHandler *inputHandler= new InputHandler("/home/newdriver/Research/SBS/Remote_Data/MPD_1380.dat");
							//	map<int,map<int,int>> sSingleEventData=inputHandler->ZeroSProcessAllEvents(10,"gui","/home/newdriver/SBS39_Pedestal_temp1356.root");
							fZeroSupressionProcess(vEventNumber,vPedestalName.c_str(),vRawDataList[0].c_str());
						}
						break;

					case 'P':
						printf("pedestal mode\n");
						{

							std::ifstream testfile(vPedestalName.c_str());
							string Pedestal_name= vPedestalName;
							if (testfile.good()&&(Pedestal_name.substr(Pedestal_name.find_last_of(".")+1)=="dat")) {
								printf("start pedestal mode\n");

								string raw_filename(basename(strdup(vPedestalName.c_str()))); // get the basename
								string filename_noappendix=raw_filename.substr(0,raw_filename.find_last_of("."));
								string number_index=filename_noappendix.substr(filename_noappendix.find_last_not_of("0123456789")+1);
								std::string Pedestal_outname(Form(tOutPutfilePattern->GetTitle(),"_Pedestal",atoi(number_index.c_str())));

								InputHandler * decoder = new InputHandler(vPedestalName.c_str());
								decoder->PedProcessAllEvents(vEventNumber,Pedestal_outname.c_str());
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
								break;
							}

						}
						break;

					case 'H':
						printf("histo mode\n");
						if(vRawDataList.size()!=0){
							std::ifstream testfile(vPedestalName.c_str());
							if (!testfile.good()) {
								printf("Please input the Pedestal file\n");
								// set the color
								Pixel_t red;
								gClient->GetColorByName("red", red);
								nStatusBarInfor->SetBackgroundColor(red);
								nStatusBarInfor->SetText(
										"Please input the Pedestal file");
								break;
							}
							for(int i=0;i<vRawDataList.size();i++ ){
								printf("Processing  %s\n", vRawDataList[i].c_str());
								// set the color
								Pixel_t yellow;
								gClient->GetColorByName("yellow", yellow);
								nStatusBarInfor->SetBackgroundColor(yellow);
								nStatusBarInfor->SetText(Form("Processing  %s\n", vRawDataList[i].c_str()));

								InputHandler * decoder = new InputHandler(vRawDataList[i].c_str());
								string raw_filename(basename(strdup(vRawDataList[i].c_str())));// get the basename
								string filename_noappendix=raw_filename.substr(0,raw_filename.find_last_of("."));
								string number_index=filename_noappendix.substr(filename_noappendix.find_last_not_of("0123456789")+1);
								std::string Hit_outname(Form(tOutPutfilePattern->GetTitle(),"",atoi(number_index.c_str())));
								decoder->HitProcessAllEvents(1000,vPedestalName.c_str(),Hit_outname.c_str());
								printf("OutPut file is save as %s\n",Hit_outname.c_str());
								//delete decoder;
								delete decoder;
//								Pixel_t green;
//								gClient->GetColorByName("green", green);
//								nStatusBarInfor->SetBackgroundColor(green);
//								nStatusBarInfor->SetText(Form("Processing  %s\n",vRawDataList[i].c_str()));
							}
						}
						break;

					case 'A':
						printf("analysis mode\n");
						break;

					default:
						break;
						}
					}
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
				//delete cRawCanvas;
				cRawCanvas->Clear();
				cRawCanvas->ResetAttPad();
				cRawCanvas->Divide(5,5);

				break;
			case C_WORKMODE_ZEROSUBTRACTION:
				vWorkMode = 'Z';
				cRawCanvas->Clear();
				cRawCanvas->ResetAttPad();
				cRawCanvas->Divide(2,3);
				printf("ZERO mode selected \n");
				break;
			case C_WORKMODE_PEDESTAL:
				vWorkMode = 'P';
				printf("PEDESTAL mode selected \n");
				break;
			case C_WORKMODE_HIT:
				vWorkMode = 'H';
				printf("HISTO mode selected \n");
				break;
			case C_WORKMODE_ANALYSIS:
				vWorkMode = 'A';
				printf("Analysis mode selected \n");
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
			switch (parm1) {
				case -1:{
					vEventNumber=tNumberEntry->GetNumberEntry()->GetIntNumber();
					if((vWorkMode=='R')&&(vRawDataList.size()!=0)){
						fRawModeProcess(vEventNumber,vRawDataList[0].c_str());
					}
					string Pedestal_name= vPedestalName;
					if((vWorkMode=='Z')&&(vRawDataList.size()!=0)&&(Pedestal_name.substr(Pedestal_name.find_last_of(".")+1)=="root")){
						fZeroSupressionProcess(vEventNumber,vPedestalName.c_str(),vRawDataList[0].c_str());
					};
					//printf("Number of Entries is set to %d\n",vEventNumber);
					}
					break;
				default:
					break;
			}
			break;

	default:
		break;
	}
	return kTRUE;
}

//ooooooooooooooooooooooooooo00000000000000000000000oooooooooooooooooooooooooooooooooooo
void UserGuiMainFrame::fRawModeProcess(int entries, string rawfilename){

		printf("Filename: %s\n", rawfilename.c_str());
		InputHandler *inputHandler= new InputHandler(rawfilename.c_str());
		map<int,map<int,vector<int>>> dSingleEventData=inputHandler->RawProcessAllEvents(entries,"gui");

			for (map<int, map<int, vector<int>>> ::iterator iter_mpd=dSingleEventData.begin();iter_mpd!=dSingleEventData.end();iter_mpd++) {
				map<int,vector<int>>::iterator itter_apv=iter_mpd->second.begin();
				for(;itter_apv!=iter_mpd->second.end();itter_apv++) {
					delete dRawHistoBuffer[iter_mpd->first][itter_apv->first];
				}
			}

		dRawHistoBuffer.clear();
//		map<int,map<int,vector<int>>>::iterator iter_mpd=dSingleEventData.begin();
		for(map<int,map<int,vector<int>>>::iterator iter_mpd=dSingleEventData.begin();iter_mpd!=dSingleEventData.end();iter_mpd++){
			map<int,vector<int>>::iterator itter_apv=iter_mpd->second.begin();
			for(;itter_apv!=iter_mpd->second.end();itter_apv++){
				//printf("%d.\n",itter_apv->second.size());
				dRawHistoBuffer[iter_mpd->first][itter_apv->first]=new TH1F(Form("MPD%d_APV%d",iter_mpd->first,itter_apv->first),Form("MPD%d_APV%d",iter_mpd->first,itter_apv->first),800,0,800);
				for(unsigned int i =0; i<(itter_apv->second.size());i++){
					//printf("strips=%d, ADC=%d\n",i,(itter_apv->second)[i]);
					dRawHistoBuffer[iter_mpd->first][itter_apv->first]->Fill(i,(itter_apv->second)[i]);
				}
			}
		}

	int canvas_counter=1;
	for(map<int,map<int,TH1F*>>::iterator iter_mpd=dRawHistoBuffer.begin();iter_mpd!=dRawHistoBuffer.end();iter_mpd++){

		for(map<int,TH1F*>::iterator itter_apv=iter_mpd->second.begin();itter_apv!=iter_mpd->second.end();itter_apv++){
			cRawCanvas->cd(canvas_counter++);
			itter_apv->second->SetMaximum(2000);
			itter_apv->second->SetMinimum(0);
			itter_apv->second->Draw();
		}
	}
	cRawCanvas->Modified();
	cRawCanvas->Update();
	gSystem->ProcessEvents();


}

void UserGuiMainFrame::fZeroSupressionProcess(int entries,string Pedestal_name, string rawfilename){

	printf("=>Raw Filename: %s\n", rawfilename.c_str());
	printf("=>Pedestal Filename: %s\n",Pedestal_name.c_str());
	TH1F *histox=new TH1F("BeforeZeroSupression_X","BeforeZeroSupression_X",1600,0,1600);
	TH1F *histoy=new TH1F("BeforeZeroSupression_Y","BeforeZeroSupression_Y",1600,0,1600);

	TH1F *histo_zerox=new TH1F("AfterZeroSupression_X","AfterZeroSupression_X",1600,0,1600);
	TH1F *histo_zeroy=new TH1F("AfterZeroSupression_Y","AfterZeroSupression_Y",1600,0,1600);

	TH1F *histo_rmtakx=new TH1F("AfterRemoveCrosstalk_X","AfterRemoveCrosstalkX",1600,0,1600);
	TH1F *histo_rmtaky=new TH1F("AfterRemoveCrosstalk_Y","AfterRemoveCrosstalk_Y",1600,0,1600);

	TH1F *histo_talkx=new TH1F("CrossTalk_X","CrossTalk_X",1600,0,1600);
	histo_talkx->SetLineColor(2);
	TH1F *histo_talky=new TH1F("CrossTalk_Y","CrossTalk_Y",1600,0,1600);
	histo_talky->SetLineColor(2);



		if(Pedestal_name.substr(Pedestal_name.find_last_of(".")+1)=="root"){

		InputHandler *inputHandler= new InputHandler(rawfilename.c_str());
		map<int,map<int,int>> sSingleEventData=inputHandler->ZeroSProcessAllEvents(vEventNumber,"gui",Pedestal_name.c_str());
		//home/newdriver/SBS39_Pedestal_temp1356.root
	// LOAD THE DAta
		for(map<int,int>::iterator iter=sSingleEventData[1].begin(); iter!= sSingleEventData[1].end();iter++){
			histox->Fill(iter->first,iter->second);
		}
		for(map<int,int>::iterator iter=sSingleEventData[11].begin(); iter!= sSingleEventData[11].end();iter++){
				histoy->Fill(iter->first,iter->second);
		}

		for(map<int,int>::iterator iter=sSingleEventData[2].begin(); iter!= sSingleEventData[2].end();iter++){
			histo_zerox->Fill(iter->first,iter->second);
		}
		for(map<int,int>::iterator iter=sSingleEventData[12].begin(); iter!= sSingleEventData[12].end();iter++){
			histo_zeroy->Fill(iter->first,iter->second);
		}

		for(map<int,int>::iterator iter=sSingleEventData[3].begin(); iter!= sSingleEventData[3].end();iter++){
			histo_rmtakx->Fill(iter->first,iter->second);
		}
		for(map<int,int>::iterator iter=sSingleEventData[13].begin(); iter!= sSingleEventData[13].end();iter++){
			histo_rmtaky->Fill(iter->first,iter->second);
		}

		for(map<int,int>::iterator iter=sSingleEventData[4].begin(); iter!= sSingleEventData[4].end();iter++){
			histo_talkx->Fill(iter->first,iter->second);
		}
		for(map<int,int>::iterator iter=sSingleEventData[14].begin(); iter!= sSingleEventData[14].end();iter++){
			histo_talky->Fill(iter->first,iter->second);
		}
		cRawCanvas->cd(1);
		histox->Draw();
		cRawCanvas->cd(2);
		histoy->Draw();
		cRawCanvas->cd(3);
		histo_zerox->Draw();
		cRawCanvas->cd(4);
		histo_zeroy->Draw();
		cRawCanvas->cd(5);
		histo_rmtakx->Draw();
		histo_talkx->Draw("same");
		cRawCanvas->cd(6);
		histo_rmtaky->Draw();
		histo_talky->Draw("same");

		cRawCanvas->Modified();
		cRawCanvas->Update();
		gSystem->ProcessEvents();

	/*	delete histox;
		delete histoy;
		delete histo_zerox;
		delete histo_zeroy;
		delete histo_rmtakx;
		delete histo_rmtaky;
		delete histo_talkx;
		delete histo_talky;*/

	}else{
		printf("Pedestal File structure unrecgnized, only .root allowed\n");
	}

}


void UserGuiMainFrame::dMenuOpenFileDialog(){
	try {
		UserGuiGeneralDialogProcess *dialog=new UserGuiGeneralDialogProcess();
		std::string inputfilename=dialog->Browser_file();
		if(inputfilename!=NULL){
		printf("OPEN file  %s\n",inputfilename.c_str());
		}else{
			printf("on file inputed");
		}
	} catch (exception e){

	}
}

void UserGuiMainFrame::dButtonPedestalOpenFileDialog(){
	try {
		UserGuiGeneralDialogProcess *dialog=new UserGuiGeneralDialogProcess();
		std::string inputfilename=dialog->Browser_file();
		if(inputfilename!=NULL){
			if(dialog->CheckAppendix(inputfilename,"root")){
				vPedestalROOTFileName=inputfilename;
				vPedestalDataFileName.clear();
				vPedestalName=inputfilename;

			}else if(dialog->CheckAppendix(inputfilename,"dat")){
				vPedestalDataFileName=inputfilename;
				vPedestalROOTFileName.clear();
				vPedestalName=inputfilename;
			}else{
				printf("Unkown data format\n");
			}
			std::string filebasename=basename(strdup(vPedestalName.c_str()));
			tPedestalFileEntry->SetTitle(filebasename.c_str());
		}
	} catch (exception e) {
	}
}


//std::string filname = (basename(
//							strdup(vPedestalName.c_str())));
//					tPedestalFileEntry->SetTitle(filename.c_str());





