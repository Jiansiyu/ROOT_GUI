/*
 * UserGuiMainFrame.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: newdriver
 */

#include "UserGuiMainFrame.h"
#include "TApplication.h"
#include "TGLayout.h"

UserGuiMainFrame::UserGuiMainFrame(const TGWindow *p, UInt_t w, UInt_t h) :
		TGMainFrame(p, w, h) {
	// TODO Auto-generated constructor stub
	SetCleanup(kDeepCleanup);

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
}
void UserGuiMainFrame::SetMenuSet() {
	fMenuSet->AddEntry("Load Pedestal(root)...", M_SET_LOADPEDESTAL);
	fMenuSet->AddEntry("&Set...", M_SET_WORKMODE);
}

void UserGuiMainFrame::SetMenuView() {
	fMenuView->AddEntry("&Dock", M_VIEW_DOCK);
	fMenuView->AddEntry("&Undock", M_VIEW_UNDOCK);
	fMenuView->AddSeparator();
	fMenuView->AddEntry("Enable U&ndock", M_VIEW_ENBL_DOCK);
	fMenuView->AddEntry("Enable &Hide", M_VIEW_ENBL_HIDE);

	fMenuView->CheckEntry(M_VIEW_ENBL_DOCK);
	fMenuView->CheckEntry(M_VIEW_ENBL_HIDE);
}
void UserGuiMainFrame::SetMenuHelp() {
	fMenuHelp->AddEntry("&Contents", M_HELP_CONTENTS);
	fMenuHelp->AddEntry("&Search...", M_HELP_SEARCH);
	fMenuHelp->AddSeparator();
	fMenuHelp->AddEntry("&About", M_HELP_ABOUT);
}

UserGuiMainFrame::~UserGuiMainFrame() {
// TODO Auto-generated destructor stub
}

void UserGuiMainFrame::CloseWindow() {

	gApplication->Terminate(); // the end of the program
}

Bool_t UserGuiMainFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t) {
	return kTRUE;
}
