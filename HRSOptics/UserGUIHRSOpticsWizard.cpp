/*
 * UserGUIHRSOpticsWizard.cpp
 *
 *  Created on: Jul 4, 2019
 *      Author: newdriver
 */
#include "TGDimension.h"
#include "iostream"
#include "UserGUIHRSOpticsWizard.h"

UserGUIHRSOpticsWizard::UserGUIHRSOpticsWizard(const TGWindow *p, const TGWindow *main, UInt_t w,
        UInt_t h, UInt_t options) : TGTransientFrame(p, main, w, h, options) {
	// TODO Auto-generated constructor stub
	SetCleanup(kDeepCleanup);
	opticsMainFrame= new TGHorizontalFrame(this,400,800);
	opticsInforFrame=new TGVerticalFrame(opticsMainFrame);
	opticsCanvasFrame=new TGVerticalFrame(opticsMainFrame);

	// add the main functionnal frame
	opticsBeamInforUnit(opticsInforFrame,new TGLayoutHints(kLHintsTop));
	opticsTargetInforUnit(opticsInforFrame,new TGLayoutHints(kLHintsTop));
	opticsSieveInforUnit(opticsInforFrame,new TGLayoutHints(kLHintsTop));
	opticsCutInforUnit(opticsInforFrame,new TGLayoutHints(kLHintsTop));

	opticsMainFrame ->AddFrame(opticsInforFrame,new TGLayoutHints(kLHintsLeft|kLHintsTop,2,2,2,2));
	opticsMainFrame ->AddFrame( new TGVertical3DLine(opticsMainFrame),new TGLayoutHints(kLHintsExpandY|kLHintsLeft,2,2));
	opticsMainFrame ->AddFrame(opticsCanvasFrame,new TGLayoutHints(kLHintsBottom|kLHintsTop|kLHintsLeft,2,2,2,2));

	//redraw the main windows
	AddFrame(opticsMainFrame,new TGLayoutHints(kLHintsTop | kLHintsRight|kLHintsExpandX|kLHintsExpandY, 2, 2, 5, 1));
	SetWindowName("HRS Optics Wizard");
	MapSubwindows();
	Resize();   // resize to default size
	MapWindow();
	// position relative to the parent's window
	CenterOnParent();
}

// plot the Input informations
void UserGUIHRSOpticsWizard::opticsBeamInforUnit(TGCompositeFrame *p,TGLayoutHints *l){
	TGGroupFrame *fBeamInforFram=new TGGroupFrame(p,"Beam Information");
//  fBeamInforFram->SetBackgroundColor(3);
	//fBeamInforFram->SetLayoutManager(new TGMatrixLayout(fBeamInforFram,0,2,15));
	std::cout<<(fBeamInforFram->GetWidth())<<"  "<<(fBeamInforFram->GetSize().fWidth)<<std::endl;
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
	TGVerticalFrame *inforFrame=new TGVerticalFrame(fTargetInforFram);
	TGHorizontalFrame *targetInputFrame=new TGHorizontalFrame(fTargetInforFram);

	TGVerticalFrame *targetNumericalInput=new TGVerticalFrame(targetInputFrame);
	TGHorizontalFrame *targetMaterialFrame=new TGHorizontalFrame(targetNumericalInput);
	targetMaterialFrame->AddFrame(new TGLabel(targetMaterialFrame,TGHotString("TargetType:  ")),new TGLayoutHints(kLHintsLeft));
	tOpticsTargetMaterial=new TGTextEntry(targetMaterialFrame);
	tOpticsTargetMaterial->SetWidth(50);
	targetMaterialFrame->AddFrame(tOpticsTargetMaterial,new TGLayoutHints(kLHintsRight|kLHintsTop,5));

	TGHorizontalFrame *targetPosXFrame=  new TGHorizontalFrame(targetNumericalInput);
	targetPosXFrame->AddFrame(new TGLabel(targetPosXFrame,TGHotString("PositionX:    ")),new TGLayoutHints(kLHintsLeft));
	nOpticsTargetPosEntry_x=new TGNumberEntry(targetPosXFrame);
	targetPosXFrame->AddFrame(nOpticsTargetPosEntry_x, new TGLayoutHints(kLHintsRight|kLHintsTop));

	TGHorizontalFrame *targetPosYFrame=  new TGHorizontalFrame(targetNumericalInput);
	targetPosYFrame->AddFrame(new TGLabel(targetPosYFrame,TGHotString("PositionY:    ")),new TGLayoutHints(kLHintsLeft));
	nOpticsTargetPosEntry_y=new  TGNumberEntry(targetPosYFrame);
	targetPosYFrame->AddFrame(nOpticsTargetPosEntry_y,new TGLayoutHints(kLHintsRight|kLHintsTop));

	TGHorizontalFrame *targetPosZFrame=  new TGHorizontalFrame(targetNumericalInput);
	targetPosZFrame->AddFrame(new TGLabel(targetPosZFrame,TGHotString("PositionZ:   ")),new TGLayoutHints(kLHintsLeft));
	nOpticsTargetPosEntry_z=new TGNumberEntry(targetPosZFrame);
	targetPosZFrame->AddFrame(nOpticsTargetPosEntry_z,new TGLayoutHints(kLHintsRight|kLHintsTop));


	// the butten frame
	TGVerticalFrame *targetButtControlFrame=new TGVerticalFrame(targetInputFrame);

	TGTextButton *opticsTargetInforCancelButt=new TGTextButton(targetButtControlFrame,"Cancel ");
	TGTextButton *opticsTargetInforAddButt=new TGTextButton(targetButtControlFrame,"  Add   ");
	TGTextButton *opticsTargetInforSaveAsButt=new TGTextButton(targetButtControlFrame,"SaveAs");
	TGTextButton *opticsTargetInforLoadButt=new TGTextButton(targetButtControlFrame," Load....");

	opticsTargetInforCancelButt->Resize(40,80);
	opticsTargetInforAddButt->Resize(40,80);
	opticsTargetInforSaveAsButt->Resize(40,80);
	opticsTargetInforLoadButt->Resize(40,80);

	TGLayoutHints *butt_layout=new TGLayoutHints(kLHintsBottom|kLHintsRight,1,1,1,1);
	targetButtControlFrame->AddFrame(opticsTargetInforLoadButt,butt_layout);
	targetButtControlFrame->AddFrame(opticsTargetInforSaveAsButt,butt_layout);
	targetButtControlFrame->AddFrame(opticsTargetInforAddButt,butt_layout);
	targetButtControlFrame->AddFrame(opticsTargetInforCancelButt,butt_layout);

	targetNumericalInput->AddFrame(targetMaterialFrame,new TGLayoutHints(kLHintsRight|kLHintsTop));
	targetNumericalInput->AddFrame(new TGHorizontal3DLine(targetNumericalInput),new TGLayoutHints(kLHintsTop|kLHintsExpandX,5,5,5,5));
	targetNumericalInput->AddFrame(targetPosXFrame,new TGLayoutHints(kLHintsRight|kLHintsTop));
	targetNumericalInput->AddFrame(targetPosYFrame,new TGLayoutHints(kLHintsRight|kLHintsTop));
	targetNumericalInput->AddFrame(targetPosZFrame,new TGLayoutHints(kLHintsRight|kLHintsTop));
	targetNumericalInput->AddFrame(new TGHorizontal3DLine(targetNumericalInput),new TGLayoutHints(kLHintsTop|kLHintsExpandX));

	targetInputFrame->AddFrame(targetNumericalInput,new TGLayoutHints(kLHintsLeft|kLHintsCenterX));
	targetInputFrame->AddFrame(new TGVertical3DLine(targetInputFrame),new TGLayoutHints(kLHintsLeft|kLHintsExpandY,5,5,5,5));
	targetInputFrame->AddFrame(targetButtControlFrame,new TGLayoutHints(kLHintsTop|kLHintsRight));
	fTargetInforFram->AddFrame(inforFrame,new TGLayoutHints(kLHintsCenterX|kLHintsTop));
	fTargetInforFram->AddFrame(targetInputFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));



	p->AddFrame(fTargetInforFram,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));
}



void UserGUIHRSOpticsWizard::opticsCanvasFrameUnit(TGCompositeFrame *p,TGLayoutHints *l){
	TGVerticalFrame *opticsCanvasFrame=new TGVerticalFrame(p);

	// add the canvas,
	opticsMainCanvas=new TRootEmbeddedCanvas("opticsCanvas",opticsCanvasFrame,600,400);


	opticsCanvasFrame->AddFrame(opticsMainCanvas,new TGLayoutHints(kLHintsCenterX|kLHintsTop));
	// add selection buttern used for control the canvas display
	p->AddFrame(opticsCanvasFrame,new TGLayoutHints(kLHintsExpandX|kLHintsCenterY));
}



void UserGUIHRSOpticsWizard::opticsSieveInforUnit(TGCompositeFrame *p,TGLayoutHints *l){

	//----------------------------------
	//  Sieve Size x, y
	//-----------------------------------
	//  Sieve Row, Sieve Col, Sieve UID[auto Generated]
	//  Sieve Position X,Y
	//  [entrys] Add Cancel Load Update
	//
	// Sieve Print Out Informations
	// Sieve Centrol
	// How many Rows
	// How many Col
	//----------------------------------
	TGGroupFrame *opticsSieveFrameGroup=new TGGroupFrame(p,"Sieve Information");

	TGVerticalFrame *opticsSieveFrame=new TGVerticalFrame(opticsSieveFrameGroup);

	TGHorizontalFrame *SieveSizeFrame=new TGHorizontalFrame(opticsSieveFrame);
	SieveSizeFrame->AddFrame(new TGLabel(SieveSizeFrame,new TGHotString("Sieve Size:      X: ")),new TGLayoutHints(kLHintsLeft,2,2,2,2));
	nOpticsSieveSizeX= new TGNumberEntry(SieveSizeFrame);
	nOpticsSieveSizeX->SetWidth(50);
	nOpticsSieveSizeX->SetText("0.0");
	SieveSizeFrame->AddFrame(nOpticsSieveSizeX,new TGLayoutHints(kLHintsLeft,2,2,2,2));

	SieveSizeFrame->AddFrame(new TGLabel(SieveSizeFrame,new TGHotString("   Y: ")),new TGLayoutHints(kLHintsLeft,2,2,2,2));
	nOpticsSieveSizeY= new TGNumberEntry(SieveSizeFrame);
	nOpticsSieveSizeY->SetWidth(50);
	nOpticsSieveSizeY->SetText("0.0");
	SieveSizeFrame->AddFrame(nOpticsSieveSizeY,new TGLayoutHints(kLHintsLeft,2,2,2,2));

	// add the sieve informations

	TGHorizontalFrame *opticsSieveIDFrame=new TGHorizontalFrame(opticsSieveFrame);
	opticsSieveIDFrame->AddFrame(new TGLabel(opticsSieveIDFrame,new TGHotString("Sieve Holes:  Row: ")));
	nOpticsSieveRow= new TGNumberEntry(opticsSieveIDFrame);
	nOpticsSieveRow->SetWidth(50);
	opticsSieveIDFrame->AddFrame(nOpticsSieveRow);

	opticsSieveIDFrame->AddFrame(new TGLabel(opticsSieveIDFrame,new TGHotString("  Col: ")));
	nOpticsSieveCol= new TGNumberEntry(opticsSieveIDFrame);
	nOpticsSieveCol->SetWidth(50);
	opticsSieveIDFrame->AddFrame(nOpticsSieveCol);


	TGHorizontalFrame *SieveHolePosFrame=new TGHorizontalFrame(opticsSieveFrame);
	SieveHolePosFrame->AddFrame(new TGLabel(SieveHolePosFrame,new TGHotString("Position:        Row: ")));
	nOpticsSieveRowPos= new TGNumberEntry(SieveHolePosFrame);
	nOpticsSieveRowPos->SetWidth(50);
	SieveHolePosFrame->AddFrame(nOpticsSieveRowPos);

	SieveHolePosFrame->AddFrame(new TGLabel(SieveHolePosFrame,new TGHotString("  Col: ")));
	nOpticsSieveColPos=new TGNumberEntry(SieveHolePosFrame);
	nOpticsSieveColPos->SetWidth(50);
	SieveHolePosFrame->AddFrame(nOpticsSieveColPos);


	// add the control butt
	TGHorizontalFrame *SieveControlButtFrame=new TGHorizontalFrame(opticsSieveFrame);
	TGTextButton *SieveControlAddButt=new TGTextButton(SieveControlButtFrame,"Add   ");
	TGTextButton *SieveControlDeletButt=new TGTextButton(SieveControlButtFrame,"Delet ");
	TGTextButton *SieveControlLoadButt=new TGTextButton(SieveControlButtFrame,"Load..");
	TGTextButton *SieveControlUpdateButt=new TGTextButton(SieveControlButtFrame,"Update");

	TGLayoutHints *butt_layout=new TGLayoutHints(kLHintsRight,4,4,2,2);
	SieveControlButtFrame->AddFrame(SieveControlAddButt,butt_layout);
	SieveControlButtFrame->AddFrame(SieveControlDeletButt,butt_layout);
	SieveControlButtFrame->AddFrame(SieveControlLoadButt,butt_layout);
	SieveControlButtFrame->AddFrame(SieveControlUpdateButt,butt_layout);


	nOpticsSievePrintOut=new TGTextButton(opticsSieveFrame);
	nOpticsSievePrintOut->SetText("Sieve Informations:\nSieve Size: 10cm x 10 cm\nTotal Number of Sieve Holes: 10\nCentrol Hole: (6,3)");
	opticsSieveFrame->AddFrame(SieveSizeFrame,new TGLayoutHints(kLHintsLeft|kLHintsExpandX));
	opticsSieveFrame->AddFrame(new TGHorizontal3DLine(opticsSieveFrame),new TGLayoutHints(kLHintsLeft|kLHintsExpandX,2,2,2,2));
	opticsSieveFrame->AddFrame(opticsSieveIDFrame,new TGLayoutHints(kLHintsLeft|kLHintsExpandX));
	opticsSieveFrame->AddFrame(SieveHolePosFrame,new TGLayoutHints(kLHintsLeft|kLHintsExpandX));
	opticsSieveFrame->AddFrame(new TGHorizontal3DLine(opticsSieveFrame),new TGLayoutHints(kLHintsLeft|kLHintsExpandX,2,2,2,2));
	opticsSieveFrame->AddFrame(SieveControlButtFrame,new TGLayoutHints(kLHintsLeft|kLHintsExpandX,2,2,2,2));
	opticsSieveFrame->AddFrame(nOpticsSievePrintOut,new TGLayoutHints(kLHintsLeft|kLHintsExpandX,2,2,2,2));

	opticsSieveFrameGroup->AddFrame(opticsSieveFrame,new TGLayoutHints(kLHintsLeft|kLHintsExpandX|kLHintsTop|kLHintsCenterY,2,2,2,2));
	p->AddFrame(opticsSieveFrameGroup,new TGLayoutHints(kLHintsExpandX|kLHintsCenterY));
}


void UserGUIHRSOpticsWizard::opticsCutInforUnit(TGCompositeFrame *p,TGLayoutHints *l){
	//------------------------------
	//  Enabled Cut
	//  List of Cut
	//  Print Out
	//------------------------------
	TGGroupFrame *opticsCutInforGroup=new TGGroupFrame(p,"Cut Informations");
	TGVerticalFrame *opticsCutInforFrame=new TGVerticalFrame(opticsCutInforGroup);

	TGTab *opticsCutTab=new TGTab(opticsCutInforFrame);
	TGCompositeFrame *opticsCutTabFrameDp=opticsCutTab->AddTab("Dp Cut");
	TGCompositeFrame *opticsCutTabFrameSieve=opticsCutTab->AddTab("Sieve Cut");
	TGCompositeFrame *opticsCutTabFrameCustomize=opticsCutTab->AddTab("Customize");

	opticsCutInforFrame->AddFrame(opticsCutTab,new TGLayoutHints(kLHintsLeft|kLHintsExpandX));

	opticsCutInforGroup->AddFrame(opticsCutInforFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));
	p->AddFrame(opticsCutInforGroup,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));

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
