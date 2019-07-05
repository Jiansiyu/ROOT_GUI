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

private:
	void opticsBeamInforUnit(TGCompositeFrame *p,TGLayoutHints *l);
	void opticsTargetInforUnit(TGCompositeFrame *p,TGLayoutHints *l);

};

#endif /* USERGUIHRSOPTICSWIZARD_H_ */
