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

	SetWindowName("UVa GEM analysis");
	MapSubwindows();
	// we need to use GetDefault...() to initialize the layout algorithm...
	Resize();   // resize to default size
	MapWindow();
	Print();
}

void UserGuiMainFrame::SetMenuFile() {
	fMenuFile->AddEntry("&Open...", M_FILE_OPEN);
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
	tPedestalFileEntry->SetTitle("Pededestal(*.root/*.dat)");
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

	fNumberFrame = new TGGroupFrame(fWorkZoneControlFrame, "Event Control");
	fNumberFrame->SetTitlePos(TGGroupFrame::kCenter);
	tNumberEntry = new TGNumberEntry(fNumberFrame);
	tNumberEntry->SetUniqueID(V_NUMBERINPUT);
	tNumberEntry->Associate(this);
	fNumberFrame->AddFrame(tNumberEntry,new TGLayoutHints(kLHintsExpandX));
	fWorkZoneControlFrame->AddFrame(fNumberFrame,new TGLayoutHints(kLHintsExpandX));

	bConfirmButton = new TGTextButton(fWorkZoneControlFrame,"Confirm",C_CONFIRM);
	bConfirmButton->Associate(this);
	fWorkZoneControlFrame->AddFrame(bConfirmButton, new TGLayoutHints(kLHintsBottom|kLHintsExpandX,5,5,5,5));
}

void UserGuiMainFrame::SetStatusBar(){
	TGLayoutHints *StatusBarLayout1=new TGLayoutHints(kLHintsLeft|kLHintsTop|kLHintsExpandY,10,10);
	TDatime *lSystemTime = new TDatime();
	//TGString *time_string=new TGString(Form("%4d:%2d:%2d",lSystemTime->GetYear(),lSystemTime->GetMonth(),lSystemTime->GetDate()));
	nStatusBarTimeLabel = new TGLabel(fStatusFrame,new TGString(Form("%4d : %02d : %02d",lSystemTime->GetYear(),lSystemTime->GetMonth(),lSystemTime->GetDay())));
	nStatusBarTimeLabel->Set3DStyle(0);
	TGLabel *autor_display= new TGLabel(fStatusFrame,"UVa GEM GUI Author: Siyu Jian");
	nStatusBarTimeLabel->Set3DStyle(0);
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
	TGVertical3DLine *lStatusbarSeparation3=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation3, new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY));

	fStatusFrame->AddFrame(nStatusBarTimeLabel,StatusBarLayout1);
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
				printf("open files\n");
				static TString dir(".");
				TGFileInfo fi;
				fi.fFileTypes = filetype;
				fi.fIniDir = StrDup(dir);
				new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);
				printf("Open file: %s (dir: %s)\n", fi.fFilename, fi.fIniDir);
				dir = fi.fIniDir;
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
				static TString dir(".");
				TGFileInfo fi;
				fi.fFileTypes = filetype;
				fi.fIniDir = StrDup(dir);
				new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);
				std::string filename=fi.fFilename;
				vPedestalName=filename;
				printf("Open file: %s \n", vPedestalName.c_str());
				std::string filname=(basename(strdup(vPedestalName.c_str())));
				tPedestalFileEntry->SetTitle(filename.c_str());
			    }
				break;
			case C_RAWFILE_DAT: {
				static TString dir(".");
				TGFileInfo fi;
				fi.fFileTypes = datfiletype;
				fi.fIniDir = StrDup(dir);
				new TGFileDialog(fClient->GetRoot(), this, kFDOpen, &fi);
				//std::string filedir = (fi.fIniDir);
				std::string filename = (fi.fFilename);
				std::string FileFullName = filename;
				//printf("Open file: %s \n", FileFullName.c_str());
				vRawDataList.push_back(FileFullName);
				tRawFileEntry->RemoveAll();
				for(int i =0; i <vRawDataList.size();i++) {
					//sprintf(stderr,"entry %d",vRawDataList.size());
					string raw_filename(basename(strdup(vRawDataList[i].c_str())));

					tRawFileEntry->AddEntry(Form("%s",raw_filename.c_str()),i);
					tRawFileEntry->MapSubwindows();
					tRawFileEntry->Layout();
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
							fRawModeProcess(10,"/home/newdriver/Research/SBS/Remote_Data/MPD_1380.dat");
						}
						break;
					case 'Z':
						printf("zero mode\n");
						break;

					case 'P':
						printf("pedestal mode\n");
						{

							std::ifstream testfile(vPedestalName.c_str());
							if (testfile.good()) {
								printf("start pedestal mode\n");

								InputHandler * decoder = new InputHandler(vPedestalName.c_str());
								decoder->PedProcessAllEvents(vEventNumber,"SBS39_Pedestal.root");
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

								InputHandler * decoder = new InputHandler("/home/newdriver/Research/SBS/Remote_Data/MPD_1380.dat");//vRawDataList[i].c_str());
								string raw_filename(basename(strdup(vRawDataList[i].c_str())));
								string lGenetatedFilename=raw_filename.substr(0,raw_filename.find_last_of("."))+".root";

								decoder->HitProcessAllEvents(1000,"/home/newdriver/Research/SBS/Remote_Data/SBS39_Pedestal.root","test.root");//vPedestalName.c_str(),lGenetatedFilename.c_str());
								//delete decoder;

								Pixel_t green;
								gClient->GetColorByName("green", green);
								nStatusBarInfor->SetBackgroundColor(green);
								nStatusBarInfor->SetText(Form("Processing  %s\n",vRawDataList[i].c_str()));
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
				break;
			case C_WORKMODE_ZEROSUBTRACTION:
				vWorkMode = 'Z';
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
				case -1:
					vEventNumber=tNumberEntry->GetNumberEntry()->GetIntNumber();
					printf("Number of Entries is set to %d\n",vEventNumber);
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

		map<int, map<int, TH1F* > >  thisto;
		map<int, map<int, TH1F* > > mAPVRawHistos = inputHandler->RawProcessAllEvents(10,  thisto);

		map<int, map<int, TH1F *>>::iterator iter_mpd=mAPVRawHistos.begin();
			for(;iter_mpd!=mAPVRawHistos.end();iter_mpd++){
				printf("=>MPD = %d\n",iter_mpd->first);
				map<int, TH1F *>::iterator itter_apv = (iter_mpd->second).begin();
				for(;itter_apv!=(iter_mpd->second).end();itter_apv++){
					printf("=>APV= %d\n", itter_apv->first);
					TH1F *histo_temp=itter_apv->second;
					for(int i=0 ; i < histo_temp->GetMaximumBin(); i ++){
						printf("=> MPD=%d, APV=%d, i=%d  bin = %d\n",iter_mpd->first, itter_apv->first, i, histo_temp->GetBinContent(i));
					}
				}
			}


	TCanvas *RawCanvas = fEmnbeddedCanvas->GetCanvas();
	RawCanvas->Modified();
	RawCanvas->Update();
	gSystem->ProcessEvents();


		//TH1F *test_histo = new TH1F("histo", "histo", 100, 100, 200);
		//test_histo->Draw();

		RawCanvas->Modified();
		RawCanvas->Update();
		gSystem->ProcessEvents();

}
















