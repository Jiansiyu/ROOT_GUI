/*
 * UserGUIHRSOpticsWizard.h
 *
 *  Created on: Jul 4, 2019
 *      Author: newdriver
 */

#ifndef USERGUIHRSOPTICSWIZARD_H_
#define USERGUIHRSOPTICSWIZARD_H_
#include <TROOT.h>
#include <TGFrame.h>
#include <TGClient.h>

#include <TCanvas.h>
#include <TRootEmbeddedCanvas.h>
#include <TColor.h>

#include <TGFrame.h>
#include <TGButton.h>
#include <TG3DLine.h>
#include <TGButtonGroup.h>
#include <TGListBox.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TGTab.h>
#include <TGTextView.h>
#include <TGClient.h>
//#include <TPi>
//#include "TGFrame.h"
//#include
#include <map>
#include <vector>
#include <TApplication.h>

class UserGUIHRSOpticsWizard : public TGTransientFrame{
public:
	UserGUIHRSOpticsWizard(const TGWindow *p, const TGWindow *main, UInt_t w, UInt_t h,
            UInt_t options = kVerticalFrame);
	virtual void CloseWindow();
	virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

	virtual ~UserGUIHRSOpticsWizard();

private:
	TGCompositeFrame *opticsMainFrame;

	TGVerticalFrame *opticsInforFrame;  // used for the input the informations
	TGVerticalFrame *opticsCanvasFrame;

	TGVerticalFrame *OpticsBeamInfor;
	TGVerticalFrame *OpticsTargetInfor;
	TGVerticalFrame *OpticsSieveInfor;


	// beam information text input
private:

	// target information input
private:
	TGTextEntry   *tOpticsTargetMaterial;
	TGNumberEntry *nOpticsTargetPosEntry_x;
	TGNumberEntry *nOpticsTargetPosEntry_y;
	TGNumberEntry *nOpticsTargetPosEntry_z;

	// sieve information input
private:
	TGNumberEntry *nOpticsSieveSizeX;
	TGNumberEntry *nOpticsSieveSizeY;
	TGNumberEntry *nOpticsSieveRow;
	TGNumberEntry *nOpticsSieveCol;
	TGNumberEntry *nOpticsSieveRowPos;
	TGNumberEntry *nOpticsSieveColPos;
	TGTextButton *nOpticsSievePrintOut;

	// sieve cut
private:
	TGCheckButton *SieveEnableButt;
	TGTextEntry *SieveCutplotCommandInput;
	TGTextEntry *SieveCutplotCutCommandInput;
	TGCheckButton *SievePatternAutoRecEnableButt;
	TGTextEntry *SieveCutRowRange;
	TGTextEntry *SieveCutColRange;

	TGTextButton *SieveCutSkip;
	TGTextButton *SieveCutNext;
	TGTextButton *SieveCutStartCut;



	//Dp Cut informations
private:

	// custmize cut
private:



	// canvas frame informtions
private:
	TRootEmbeddedCanvas *opticsMainCanvas;

	// Left Information display
private:
	std::string displayInfor;
	TGTextView *opticsTextDisplay;


private:
	void gMenuUnitDraw(TGCompositeFrame *p,TGLayoutHints *l);
	void gStatusUnitDraw(TGCompositeFrame *p,TGLayoutHints *l);

private:
	// the main canvas unit
	void opticsMainCanvasUnit(TGCompositeFrame *p,TGLayoutHints *l);

	void opticsBeamInforUnit(TGCompositeFrame *p,TGLayoutHints *l);
	void opticsTargetInforUnit(TGCompositeFrame *p,TGLayoutHints *l);
	void opticsCutInforUnit(TGCompositeFrame *p,TGLayoutHints *l);
	void opticsCanvasFrameUnit(TGCompositeFrame *p,TGLayoutHints *l);
	void opticsSieveInforUnit(TGCompositeFrame *p,TGLayoutHints *l);

	//cut unit
	void opticsDpCutUnit(TGCompositeFrame *p,TGLayoutHints *l);
	void opticsSieveCutUnit(TGCompositeFrame *p,TGLayoutHints *l);
	void opticsCustmizeCutUnit(TGCompositeFrame *p,TGLayoutHints *l);
	void opticsTextPrintOutUnit(TGCompositeFrame *p,TGLayoutHints *l);

};

#endif /* USERGUIHRSOPTICSWIZARD_H_ */
