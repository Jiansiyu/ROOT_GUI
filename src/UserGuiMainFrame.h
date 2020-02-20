/*
 * UserGuiMainFrame.h
 *
 *  Created on: Jan 25, 2017
 *      Author: newdriver
 */

#ifndef USERGUIMAINFRAME_H_
#define USERGUIMAINFRAME_H_

#include "TGFrame.h"
#include "TGWindow.h"
#include "TGDockableFrame.h"
#include "TGMenu.h"
#include "TRootEmbeddedCanvas.h"
#include "TGCanvas.h"
#include "TCanvas.h"
#include "TG3DLine.h"

#include "TGButton.h"
#include "TGButtonGroup.h"

#include "TGSplitter.h"
#include "TGCanvas.h"

#include "TGLabel.h"

#include "TGTextEntry.h"
#include "TGFileBrowser.h"
#include "TGFileDialog.h"

#include "TGListBox.h"
#include "TGString.h"
#include "TGNumberEntry.h"
#include "TGProgressBar.h"

//used for the tab
#include "TGTab.h"

// dialog
#include "TGClient.h"
#include "TGFileDialog.h"

// used for the color wheel
#include <TGColorSelect.h>
#include <TColor.h>

//
#include "TBrowser.h"

#include "TDatime.h"

// usede for GEManalysis program
#include "TChain.h"

#include "vector"
#include "string.h"
#include "TObjString.h"

#include "TSystem.h"
#include "TSystemDirectory.h"
#include "TH1F.h"
#include "map"
//#include "GEMInforCenter.h"
#include "UserGUIDataStructue.h"
//#include "../DecoderMPD4_VME/GEMDataParserM4V.h"
#include "GUIInforCenter.h"
#include "MPDDecoder.h"

enum ETestCommandIdentifiers {
	M_FILE_OPEN,
	M_FILE_NEWCANVAS,
	M_FILE_TBROWER,
	M_FILE_SAVE,
	M_FILE_SAVEAS,
	M_FILE_PRINT,
	M_FILE_PRINTSETUP,
	M_FILE_EXIT,

	M_SET_WORKMODE,
	M_SET_LOADMAPPING,
	M_SET_LOADPEDESTAL,
	M_SET_BATCH,

	M_TOOL_APVMAPPINGWIZARD,

	M_VIEW_ENBL_DOCK,
	M_VIEW_ENBL_HIDE,
	M_VIEW_DOCK,
	M_VIEW_UNDOCK,

	M_HELP_CONTENTS,
	M_HELP_SEARCH,
	M_HELP_ABOUT,

	// workzone button key value

	C_WORKMODE_RAW,
	C_WORKMODE_ZEROSUBTRACTION,
	C_WORKMODE_PEDESTAL,
	C_WORKMODE_HIT,
	C_WORKMODE_ANALYSIS,
	C_WORKMODE_CALIBRATION,

	C_RAWFILE_PEDESTAL,
	C_RAWFILE_DAT,

	V_NUMBERINPUT,
	C_CONFIRM,

	COLORSEL
};

class UserGuiMainFrame: public TGMainFrame {

public:
UserGuiMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
	virtual ~UserGuiMainFrame();

	virtual void CloseWindow();
	virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);

///oooooooooooooo00000000000000000000000000000000000ooooooooooooooooooooooooooooo
//UVa input handler mode
   ClassDef(UserGuiMainFrame,0);


// dialog process
private:
	void dMenuOpenFileDialog();
	void dButtonPedestalOpenFileDialog();
	void dButtonRawOpenFileDialog();
	void dMenuSetLoadMapping();


private:
	void gKCMMenuConfirmProcess(Long_t parm1);
	void gKCMButtonConfirmProcess(Long_t parm1);
	void bButtonConfirmProcess();

private:
	TGDockableFrame *fMenuDock;

	// set the menu layout
	TGMenuBar *fMenuBar;
	TGPopupMenu *fMenuFile, *fMenuSet, *fMenuTool, *fMenuView, *fMenuHelp;
	TGLayoutHints *fMenuBarLayout, *fMenuBarItemLayout, *fMenuBarHelpLayout;

	// set the work zone variables
	TGHorizontalFrame *fWorkZoneFrame;
	TGLayoutHints *fWorkZoneLayout;//, *fWorkZoneItemLayout;

// tab related
private:
        
        TGTab *fWorkZoneTab;
	TGCompositeFrame *fWorkZoneTabDefultFrame;
	// id 1 refers to top level tab.
	// id 2 refers to the second (i.e. nested) tab level
	std::map<int /*tab id 1*/, TGCompositeFrame *> fWorkZoneTabSubFrame;
	std::map<int /*tab id 1*/, std::map<int /*tab id 2*/, TRootEmbeddedCanvas *>> fWorkZoneTabEnbeddedCanvas;
	std::map<int /*tab id 1*/, std::map<int /*tab id 2*/, TCanvas *>> cfWorkZoneTabCanvas;
	std::map<int /*tab id 2*/, TGTab *> fNestedTab;
	std::map<int, std::map<int, TGCompositeFrame*>> fWorkZoneTabSubSubFrame;
	

private:
	TGVerticalFrame *fWorkZoneControlFrame;
	TGLayoutHints *fWorkZoneControlFrameLayout;
	TGVerticalFrame *fWorkZoneCanvasFrame;
	TGLayoutHints *fWorkZoneCanvasFrameLayout;
	TGVertical3DLine *fWorkZoneSeparation;


	// set the status bar variables
	TGCompositeFrame *fStatusFrame;
	TGLayoutHints *fStatusFrameLayout;

	// Control zone buttom layout
	TGButtonGroup *bWorkModeButtonGroup;

	// work mode control button
	TGRadioButton *bWorkModeRAW;
	TGRadioButton *bWorkModeZeroSubtraction;
	TGRadioButton *bWorkModePedestal;
	TGRadioButton *bWorkModeHit;
	TGRadioButton *bWorkModeAnalysis;
	TGRadioButton *bWorkModeCalibration;
	// address and file control
	TGGroupFrame *fDataInputFrame;

	// file input control
	TGTextEntry *tPedestalFileEntry;
	TGTextButton *bPedestalFileButton;
	TGListBox *tRawFileEntry;
	TGTextButton *bRawFileButton;

	// the out put file pattern
	TGTextEntry *tOutPutfilePattern;

	//number input button
	TGGroupFrame *fNumberFrame;
	TGNumberEntry *tNumberEntry;

	// Confirm button
	TGTextButton *bConfirmButton;

	// status bar display
	TGLabel *nStatusBarTimeLabel;
	TGLabel *nStatusBarInfor;
	TGHProgressBar * pStatusBarProcessBar;
	TGColorSelect      *fColorSel;

private:
	// menu
	void SetMenuFile();
	void SetMenuSet();
	void SetMenuTool();
	void SetMenuView();
	void SetMenuHelp();

private:
	void SetWorkZone();    // set the workspace
	// set the tab
	void SetWorkZoneTab();
	// void SetWorkZoneTab(std::vector<int>);
	void SetWorkZoneTab(std::map<int,std::map<int,std::string>>);

	void SetWorkZoneButton();
	void SetWorkZoneDataInput();
	void SetStatusBar();

	// read the configure file, and set the default variables
	void SetDefaultPath();
	void SaveDefaultPath();   // save the default value
	//void thrHitRun(std::string rawfilename,std::string pedestalname, std::string mappingfilename,int entries);
//	void TestFunctions(int i, std::string a);
public:
	void fFileBrowse();
	void SetDataFileName();

///oooooooooooooo00000000000000000000000000000000000ooooooooooooooooooooooooooooo
/// Main Function
///oooooooooooooo00000000000000000000000000000000000ooooooooooooooooooooooooooooo
private:
	void generalCanvasDraw(std::map<int, std::map<int,TH1F *>>,int CanvasID);  //< Canvas Y Canvas X, Histo

	std::map<std::string,int> GetCanvasNameList();

public:
	// Interaction functions
	char GetWorkMode(char & WorkMode);
	std::vector<std::string > GetProcessList();  // return the raw data list
	std::string GetPedestalFileName();
	int GetCurrentEventID();                      // Get the current Evnt id
	int SetCurrentEventID();
public:
	// *SIGNAL CATCH FUNCTION
	void fCanvasDraw( GUICanvasDataStream *);
	void fCanvasDrawHit( GUICanvasDataStream *);
	void fStatusBarDraw( GUIStatusBarDataStream *);
	void fMainTabDraw( GUIMainTabDataStream *);

	// status bar control functions
private:
	void SetStatusBarDisplay(std::string);
	void SetStatusBarDisplay(std::string,UserGUI::WorningLevel);

private:
	MPDDecoder *decoder;
	std::string vPedestalName;
	std::string vPedestalDataFileName;
	std::string vPedestalROOTFileName;
	std::vector<std::string> vRawDataList;
	std::vector<std::string> vRootDataList;
	
	char vWorkMode;
	long int vEventNumber;
	// display buffer
	std::map<int,std::map<int,TH1F*>> dRawHistoBuffer;
	std::map<int,std::map<int,std::map<int,TH1F*>>> dMultiGEMHistoBuffer;
	std::string vMappingName;

//private:
	//std::map<std::string,GEMDataParserM4V *> rawPaserList;
private :
	void fRawModeProcess(int entries, std::string rawfilename);
	void fPedestalModeProcess(int entries, std::string rawfilename);
	void fZeroSupressionProcess(int entries,std::string Pedestal_name, std::string rawfilename);
	void fHitModeProcess(int entries,std::string Pedestal_name, std::vector<std::string> rawfilename );
	void fHitModeThreadProcess(int entries,std::string Pedestal_name, std::vector<std::string> rawfilename );
	void fAnalysisProcess(std::vector<std::string> Filenames); // analysis mode process
	void fAnalysisProcess(std::string Filenames); // analysis mode process
	void fCalibrationProcess(std::vector<std::string> Filenames); // analysis mode process




};

#endif /* USERGUIMAINFRAME_H_ */
