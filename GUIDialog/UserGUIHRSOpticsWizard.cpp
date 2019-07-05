/*
 * UserGUIHRSOpticsWizard.cpp
 *
 *  Created on: Jul 4, 2019
 *      Author: newdriver
 */

#include "UserGUIHRSOpticsWizard.h"

UserGUIHRSOpticsWizard::UserGUIHRSOpticsWizard(const TGWindow *p, const TGWindow *main, UInt_t w,
        UInt_t h, UInt_t options) : TGTransientFrame(p, main, w, h, options) {
	// TODO Auto-generated constructor stub
	SetCleanup(kDeepCleanup);
	opticsMainFrame= new TGHorizontalFrame(this,600,400,kFixedWidth);
		//split the fram into two, the left side will be used for input, the right side will be used for the canvas
	opticsInforFrame=new TGVerticalFrame(opticsMainFrame);
	opticsCanvasFrame=new TGVerticalFrame(opticsMainFrame);

	opticsBeamInforUnit(opticsInforFrame,new TGLayoutHints(kLHintsTop));
	opticsTargetInforUnit(opticsInforFrame,new TGLayoutHints(kLHintsTop));

	opticsMainFrame ->AddFrame(opticsInforFrame,new TGLayoutHints(kLHintsLeft|kLHintsTop|kLHintsTop,2,2,2,2));
	opticsMainFrame ->AddFrame( new TGVertical3DLine(opticsMainFrame),new TGLayoutHints(kLHintsTop|kLHintsCenterX|kLHintsExpandY,2,2));
	opticsMainFrame ->AddFrame(opticsCanvasFrame,new TGLayoutHints(kLHintsBottom|kLHintsTop|kLHintsCenterX|kLHintsExpandX,2,2,2,2));

	//redraw the main windows
	AddFrame(opticsMainFrame,new TGLayoutHints(kLHintsTop | kLHintsRight|kLHintsExpandX|kLHintsExpandY, 2, 2, 5, 1));
	SetWindowName("HRS Optics Wizard");
	MapSubwindows();
	Resize();   // resize to default size
	MapWindow();
	// position relative to the parent's window
	CenterOnParent();
	Print();
}

// plot the Input informations
void UserGUIHRSOpticsWizard::opticsBeamInforUnit(TGCompositeFrame *p,TGLayoutHints *l){
	TGGroupFrame *fBeamInforFram=new TGGroupFrame(p,"Beam Information");
    //fBeamInforFram->SetLayoutManager(new TGMatrixLayout(fBeamInforFram,0,2,15));

    // add the beam X dimension
	TGHorizontalFrame *fBeamXFrame=new TGHorizontalFrame(fBeamInforFram);
	fBeamXFrame->AddFrame(new TGLabel(fBeamXFrame,new TGHotString("Beam position x:  ")));
	TGTextEntry *tBeamInforX_input=new TGTextEntry(fBeamXFrame);
	fBeamXFrame->AddFrame(tBeamInforX_input,new TGLayoutHints(kLHintsTop));

	TGHorizontalFrame *fBeamYFrame=new TGHorizontalFrame(fBeamInforFram);
	fBeamYFrame->AddFrame(new TGLabel(fBeamYFrame,new TGHotString("Beam position y:  ")));
	TGTextEntry *tBeamInforY_input=new TGTextEntry(fBeamYFrame);
	fBeamYFrame->AddFrame(tBeamInforY_input,new TGLayoutHints(kLHintsTop));

	fBeamInforFram->AddFrame(fBeamXFrame,new TGLayoutHints(kLHintsTop));
	fBeamInforFram->AddFrame(fBeamYFrame,new TGLayoutHints(kLHintsTop));

	p->AddFrame(fBeamInforFram,new TGLayoutHints(kLHintsTop|kLHintsCenterX));

}

void UserGUIHRSOpticsWizard::opticsTargetInforUnit(TGCompositeFrame *p,TGLayoutHints *l){
	TGGroupFrame *fTargetInforFram=new TGGroupFrame(p,"Target Information");
    //fTargetInforFram->SetLayoutManager(new TGMatrixLayout(fBeamInforFram,0,2,15));

	//add the target matrials
	TGHorizontalFrame *targetMaterialFrame=new TGHorizontalFrame(fTargetInforFram);
	targetMaterialFrame->AddFrame(new TGLabel(targetMaterialFrame,TGHotString("Target Type:   ")));
    tOpticsTargetMaterial=new TGTextEntry(targetMaterialFrame);
    targetMaterialFrame->AddFrame(tOpticsTargetMaterial,new TGLayoutHints(kLHintsTop));
    fTargetInforFram->AddFrame(targetMaterialFrame,new TGLayoutHints(kLHintsTop|kLHintsExpandX));

    // add the tatget position informations
    // add multi target foil support for the optics




	p->AddFrame(fTargetInforFram,new TGLayoutHints(kLHintsTop));
}


UserGUIHRSOpticsWizard::~UserGUIHRSOpticsWizard() {
	// TODO Auto-generated destructor stub
}

// signal process unit
Bool_t UserGUIHRSOpticsWizard::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2) {

}

// close  the windows
void UserGUIHRSOpticsWizard::CloseWindow() {

	delete this;
//	 DeleteWindow();
}
