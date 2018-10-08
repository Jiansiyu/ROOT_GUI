/*
 * GUIMainFrame.h
 *
 *  Created on: Sep 10, 2018
 *      Author: newdriver
 */

#ifndef GUIMAINFRAME_H_
#define GUIMAINFRAME_H_

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
#include "TGTab.h"           //used for the tab
#include "TGClient.h"        // dialog
#include "TGFileDialog.h"
#include <TGColorSelect.h>   // used for the color wheel
#include <TColor.h>
#include "TBrowser.h"
#include "TDatime.h"
#include "TChain.h"          // usede for GEManalysis program

#include "vector"
#include "string.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TSystemDirectory.h"
#include "TH1F.h"
#include "map"
#include "GUIInformation.h"
class GUIMainFrame: public TGMainFrame {
public:
	GUIMainFrame(){};
	GUIMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
	virtual ~GUIMainFrame();
	virtual void CloseWindow();
	virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);
//	ClassDef(GUIMainFrame,0);

public:
	void SetStatusBarDisplay();
	void SetConfirmButtonColor();
	int64_t GUIWorkZoneControlInputNumber(const int64_t i);
	std::vector<std::string> GUIWorkZoneControlRawInput(const std::vector<std::string>);
	std::string GUIWorkZoneControlPedestalInput(const std::string);
// GUI main interface
protected:
	virtual void gMenuUnitDraw(TGLayoutHints *l );
	virtual void gWorktabUnitDraw(TGLayoutHints *l);
	virtual void gWorktabControlUnitDraw(TGCompositeFrame *p,TGLayoutHints *l);
	virtual void gStatusUnitDraw(TGLayoutHints *l);
	virtual void gSetMenuFile(TGPopupMenu *fMenuFile);
	virtual void gSetMenuSet(TGPopupMenu *fMenuSet);
	virtual void gSetMenuTool(TGPopupMenu *fMenuTool);
	virtual void gSetMenuView(TGPopupMenu  *fMenuView);
	virtual void gSetMenuHelp(TGPopupMenu *fMenuHelp);
	virtual TGFrame *gWorktabDisplayUnitDraw(TGCompositeFrame *p,TGLayoutHints *l);
	virtual TGCompositeFrame *gWorktabControlButtonSetDraw(TGCompositeFrame *p,TGLayoutHints *l);
	virtual TGCompositeFrame *gWorktabControlFileIODraw(TGCompositeFrame *p, TGLayoutHints *l);
	virtual TGCompositeFrame *gWorktabControlOutputDraw(TGCompositeFrame *p, TGLayoutHints *l);
	virtual TGCompositeFrame *gWorktabControlOutputButtonDraw(TGCompositeFrame *p, TGLayoutHints *l);
	virtual TGCompositeFrame *gWorktabControlPedestalInputBox(TGCompositeFrame *p, TGLayoutHints *l);
	virtual TGCompositeFrame *gWorktabControlRawDataInputBox(TGCompositeFrame *p, TGLayoutHints *l);
	virtual TGCompositeFrame *gSysInforTabDraw(TGCompositeFrame *p, TGLayoutHints *l);
	virtual TGCompositeFrame *gWorktabControlFileIORawListBox(TGCompositeFrame *p, TGLayoutHints *l);
	void gSetDefaultInformation();

	// command process
	virtual void gMessageProcessMenu(Long_t msg, Long_t parm);
	virtual void gMessageProcessButton(Long_t msg, Long_t parm);
	virtual void gMessageProcessRadioButton(Long_t msg, Long_t parm);
	virtual void gMessageProcessCheckButton(Long_t msg, Long_t parm);
	virtual void gMessageProcessColorSel(Long_t msg, Long_t parm);

protected:
	//global variables
	GUIInformation *guiinfor=GUIInformation::GetInstance();
	virtual void GUIUpdata();
protected:
	// some manditory global variable that used for interactive with the canvas
	virtual void gWorkingModeRaw();
	virtual void gWorkingModePedestal();
	virtual void gWorkingModeZeroSubtr();
	virtual void gWorkingModeHitDisplay();
	virtual void gWorkingModeHit();
	virtual void gWorkingModeCalibration();
	virtual void gWorkingModeAnalysis();

protected:
	std::map<int64_t,TGCompositeFrame *>    vWorkZoneTabSubFrame;
	std::map<int64_t,TRootEmbeddedCanvas *> vWorkZoneEmbededCanbas;
	std::map<int64_t,TCanvas *>             vWorkZoneTabCanvas;

};

#endif /* GUIMAINFRAME_H_ */
