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
#include "TGFrame.h"
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

// dialog
#include "TGClient.h"
#include "TGFileDialog.h"

#include "TDatime.h"

#include "vector"
#include "string.h"

#include "TSystem.h"
#include "TSystemDirectory.h"
enum ETestCommandIdentifiers {
	M_FILE_OPEN,
	M_FILE_SAVE,
	M_FILE_SAVEAS,
	M_FILE_PRINT,
	M_FILE_PRINTSETUP,
	M_FILE_EXIT,

	M_SET_WORKMODE,
	M_SET_LOADPEDESTAL,
	M_SET_BATCH,


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

	C_RAWFILE_PEDESTAL,
	C_RAWFILE_DAT,

	V_NUMBERINPUT,
	C_CONFIRM
};

class UserGuiMainFrame: public TGMainFrame {
public:
	UserGuiMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
	virtual ~UserGuiMainFrame();

	virtual void CloseWindow();
	virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);

///oooooooooooooo00000000000000000000000000000000000ooooooooooooooooooooooooooooo
//UVa input handler mode

public:
	// Interaction functions
	char GetWorkMode(char & WorkMode);
	std::vector<std::string > GetProcessList();  // return the raw data list
	std::string GetPedestalFileName();
	int GetCurrentEventID();                      // Get the current Evnt id
	int SetCurrentEventID();

private:
	std::string vPedestalName;
	std::vector<std::string> vRawDataList;
	char vWorkMode;
	long int vEventNumber;

private :
	void fRawModeProcess(int entries, std::string rawfilename);

	///oooooooooooooo00000000000000000000000000000000000ooooooooooooooooooooooooooooo

private:
	TGDockableFrame *fMenuDock;

	// set the menu layout
	TGMenuBar *fMenuBar;
	TGPopupMenu *fMenuFile, *fMenuSet, *fMenuView, *fMenuHelp;
	TGLayoutHints *fMenuBarLayout, *fMenuBarItemLayout, *fMenuBarHelpLayout;


	// set the work zone variables
	TGHorizontalFrame *fWorkZoneFrame;
	TGLayoutHints *fWorkZoneLayout;//, *fWorkZoneItemLayout;
	TRootEmbeddedCanvas *fEmnbeddedCanvas;

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

	// address and file control
	TGGroupFrame *fDataInputFrame;

	// file input control
	TGTextEntry *tPedestalFileEntry;
	TGTextButton *bPedestalFileButton;
	TGListBox *tRawFileEntry;
	TGTextButton *bRawFileButton;


	//number input button
	TGGroupFrame *fNumberFrame;
	TGNumberEntry *tNumberEntry;

	// Confirm button
	TGTextButton *bConfirmButton;

	// status bar display
	TGLabel *nStatusBarTimeLabel;
	TGLabel *nStatusBarInfor;
	TGHProgressBar * pStatusBarProcessBar;
private:
	// menu
	void SetMenuFile();
	void SetMenuSet();
	void SetMenuView();
	void SetMenuHelp();

private:
	void SetWorkZone();    // set the workspace
	void SetWorkZoneButton();
	void SetWorkZoneDataInput();
	void SetStatusBar();


public:
	void fFileBrowse();
	void SetDataFileName();

};

#endif /* USERGUIMAINFRAME_H_ */
