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
//#include "TGPo"
#include "TGCanvas.h"

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

	M_CASCADE_1,
	M_CASCADE_2,
	M_CASCADE_3,

	M_NEW_REMOVEMENU,

	VId1,
	HId1,
	VId2,
	HId2,

	VSId1,
	HSId1,
	VSId2,
	HSId2,

	ColorSel
};

class UserGuiMainFrame: public TGMainFrame {
public:
	UserGuiMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
	virtual ~UserGuiMainFrame();

	virtual void CloseWindow();
	virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);

private:
	TGDockableFrame *fMenuDock;
	TGCompositeFrame *fStatusFrame;
	TGCanvas *fCanvasWindow;

	TGMenuBar *fMenuBar;
	TGPopupMenu *fMenuFile, *fMenuSet, *fMenuView, *fMenuHelp;
	TGLayoutHints *fMenuBarLayout, *fMenuBarItemLayout, *fMenuBarHelpLayout;

private:
	// functions that used for the the menu
	void SetMenuFile();
	void SetMenuSet();
	void SetMenuView();
	void SetMenuHelp();

};

#endif /* USERGUIMAINFRAME_H_ */
