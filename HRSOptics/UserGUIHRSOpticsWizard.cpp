/*
 * UserGUIHRSOpticsWizard.cpp
 *
 *  Created on: Jul 4, 2019
 *      Author: newdriver
 */
#include "TGDimension.h"
#include "iostream"
#include "TGDockableFrame.h"
#include "TGMenu.h"
#include "UserGUIHRSOpticsWizard.h"

UserGUIHRSOpticsWizard::UserGUIHRSOpticsWizard(const TGWindow *p, const TGWindow *main, UInt_t w,
        UInt_t h, UInt_t options) : TGTransientFrame(p, main, w, h, options) {
	// TODO Auto-generated constructor stub
	SetCleanup(kDeepCleanup);
	gMenuUnitDraw(this,new TGLayoutHints(kLHintsLeft|kLHintsTop,2,2,2,2));
	opticsMainFrame= new TGHorizontalFrame(this,400,800);
	opticsInforFrame=new TGVerticalFrame(opticsMainFrame);
	opticsCanvasFrame=new TGVerticalFrame(opticsMainFrame);

	// add the main functionnal frame
	opticsBeamInforUnit(opticsInforFrame,new TGLayoutHints(kLHintsTop));
	opticsTargetInforUnit(opticsInforFrame,new TGLayoutHints(kLHintsTop));
	opticsSieveInforUnit(opticsInforFrame,new TGLayoutHints(kLHintsTop));
	opticsCutInforUnit(opticsInforFrame,new TGLayoutHints(kLHintsTop));
	opticsTextPrintOutUnit(opticsInforFrame,new TGLayoutHints(kLHintsBottom|kLHintsLeft|kLHintsExpandX));


	// add the canvas frame

	opticsCanvasFrameUnit(opticsCanvasFrame,new TGLayoutHints(kLHintsTop));
	opticsMainFrame ->AddFrame(opticsInforFrame,new TGLayoutHints(kLHintsLeft|kLHintsTop,2,2,2,2));
	opticsMainFrame ->AddFrame( new TGVertical3DLine(opticsMainFrame),new TGLayoutHints(kLHintsExpandY|kLHintsLeft,2,2));
	opticsMainFrame ->AddFrame(opticsCanvasFrame,new TGLayoutHints(kLHintsBottom|kLHintsTop|kLHintsLeft,2,2,2,2));

	gStatusUnitDraw(this,new TGLayoutHints(kLHintsLeft|kLHintsTop,2,2,2,2));
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

	p->AddFrame(fBeamInforFram,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));

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
	//-------------------------------
	//
	//[ Main Canvas]
	//[Sub Canvas 1]  [Sub Canvas 2]
	//--------------------------------


	TGVerticalFrame *opticsCanvasFrame=new TGVerticalFrame(p);
	TGHorizontalFrame *opticsMainCanvasFrame=new TGHorizontalFrame(opticsCanvasFrame);

	// add the main canvas control code
	opticsMainCanvas=new TRootEmbeddedCanvas("mainCanvas",opticsMainCanvasFrame,600,600);

	opticsMainCanvasFrame->AddFrame(opticsMainCanvas,new TGLayoutHints(kLHintsLeft|kLHintsExpandX|kLHintsTop|kLHintsExpandY));
	// add the sub canvas frame
	TGHorizontalFrame *opticsSubCanvasFrame=new TGHorizontalFrame(opticsCanvasFrame);
	TGVerticalFrame *opticsSubCanvasFrame_Left=new TGVerticalFrame(opticsSubCanvasFrame);
	TGVerticalFrame *opticsSubCanvasFrame_right=new TGVerticalFrame(opticsSubCanvasFrame);

	opticsSubCanvas_left=new TRootEmbeddedCanvas("SubLeft",opticsSubCanvasFrame_Left,300,300);
	opticsSubCanvas_right=new TRootEmbeddedCanvas("SubRight",opticsSubCanvasFrame_right,300,300);

	opticsSubCanvasFrame_Left->AddFrame(opticsSubCanvas_left,new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));
	opticsSubCanvasFrame_right->AddFrame(opticsSubCanvas_right,new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));

	opticsSubCanvasFrame->AddFrame(opticsSubCanvasFrame_Left,new TGLayoutHints(kLHintsLeft|kLHintsCenterX|kLHintsTop|kLHintsExpandY));
	opticsSubCanvasFrame->AddFrame(new TGVertical3DLine(opticsSubCanvasFrame),new TGLayoutHints(kLHintsTop|kLHintsExpandY));
	opticsSubCanvasFrame->AddFrame(opticsSubCanvasFrame_right,new TGLayoutHints(kLHintsCenterX|kLHintsRight|kLHintsTop|kLHintsExpandY));


	opticsCanvasFrame->AddFrame(opticsMainCanvasFrame,new TGLayoutHints(kLHintsTop|kLHintsExpandY|kLHintsLeft|kLHintsExpandX,5,5,5,5));
	opticsCanvasFrame->AddFrame(new TGHorizontal3DLine(opticsCanvasFrame),new TGLayoutHints(kLHintsLeft|kLHintsExpandX,5,5,5,5));
	opticsCanvasFrame->AddFrame(opticsSubCanvasFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX,5,5,5,5));

	p->AddFrame(opticsCanvasFrame,new TGLayoutHints(kLHintsTop|kLHintsExpandY|kLHintsLeft|kLHintsExpandX));
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

	opticsSieveCutUnit(opticsCutTabFrameSieve,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));
	opticsDpCutUnit(opticsCutTabFrameDp,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));
	opticsCustmizeCutUnit(opticsCutTabFrameCustomize,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));

	opticsCutInforFrame->AddFrame(opticsCutTab,new TGLayoutHints(kLHintsLeft|kLHintsExpandX));
	opticsCutInforGroup->AddFrame(opticsCutInforFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));
	p->AddFrame(opticsCutInforGroup,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));

}


void UserGUIHRSOpticsWizard::opticsSieveCutUnit(TGCompositeFrame *p,TGLayoutHints *l){
	//----------------------
	// Enable []
	//----------------------
	//Plot Coomand
	//Plot Cut
	//Auto recginize[*]
	//Select Range you want to apply cut
	//Raw Range[]    Col Range[]
	//---------------------------------
	//[Start Cut] [Skip] [Next]
	//[Print Out Informations]
	//[Print Out Informations]
	//----------------------
	//----------------------
	//[Sieve Design] [Cutted] []
	//
	//[                      ]
	//-------------------------

	TGVerticalFrame *SieveCutFrame=new TGVerticalFrame(p);

	TGHorizontalFrame *SieveCutEnableFrame=new TGHorizontalFrame(SieveCutFrame);
	SieveEnableButt=new TGCheckButton(SieveCutEnableFrame,new TGHotString("Enable"));
	SievePatternAutoRecEnableButt=new TGCheckButton(SieveCutEnableFrame,new TGHotString("Auto Rec Pattern"));
	SieveCutEnableFrame->AddFrame(SieveEnableButt,new TGLayoutHints(kLHintsLeft,5,5,5,5));
	SieveCutEnableFrame->AddFrame(SievePatternAutoRecEnableButt,new TGLayoutHints(kLHintsLeft,5,5,5,5));


	TGHorizontalFrame *SieveCutPlotCommandFrame=new TGHorizontalFrame(SieveCutFrame);
	SieveCutPlotCommandFrame->AddFrame(new TGLabel(SieveCutPlotCommandFrame,new TGHotString("  Plot:   ")),new TGLayoutHints(kLHintsLeft));
	SieveCutplotCommandInput=new TGTextEntry(SieveCutPlotCommandFrame);
	SieveCutplotCommandInput->SetText("L.tr.theta:L.tr.phi");
	SieveCutPlotCommandFrame->AddFrame(SieveCutplotCommandInput,new TGLayoutHints(kLHintsLeft|kLHintsExpandX,3,3,3,3));

	TGHorizontalFrame *SieveCutPlotCutCommandFrame=new TGHorizontalFrame(SieveCutFrame);
	SieveCutPlotCutCommandFrame->AddFrame(new TGLabel(SieveCutPlotCutCommandFrame,new TGHotString("  Cut :  ")),new TGLayoutHints(kLHintsLeft));
	SieveCutplotCutCommandInput=new TGTextEntry(SieveCutPlotCutCommandFrame);
	SieveCutPlotCutCommandFrame->AddFrame(SieveCutplotCutCommandInput,new TGLayoutHints(kLHintsLeft|kLHintsExpandX,3,3,3,3));

//	TGHorizontalFrame *SieveCutAutoRecButtFrame=new TGHorizontalFrame(SieveCutFrame);

	// used for set the cut rang for Row and Col
	TGHorizontalFrame *SieveCutSetRangeFrame=new TGHorizontalFrame(SieveCutFrame);
	SieveCutSetRangeFrame->AddFrame(new TGLabel(SieveCutSetRangeFrame,new TGHotString(" Hole Range  Row:  ")),new TGLayoutHints(kLHintsLeft));
	SieveCutRowRange=new TGTextEntry(SieveCutSetRangeFrame);
	SieveCutRowRange->SetWidth(50);
	SieveCutRowRange->SetText("3-10");
	SieveCutSetRangeFrame->AddFrame(SieveCutRowRange, new TGLayoutHints(kLHintsLeft,2,2,2,2));
	SieveCutSetRangeFrame->AddFrame(new TGLabel(SieveCutSetRangeFrame,new TGHotString("  Col:  ")),new TGLayoutHints(kLHintsLeft));
	SieveCutColRange=new TGTextEntry(SieveCutSetRangeFrame);
	SieveCutColRange->SetWidth(50);
	SieveCutColRange->SetText("0-10");
	SieveCutSetRangeFrame->AddFrame(SieveCutColRange, new TGLayoutHints(kLHintsLeft,2,2,2,2));

	//  the control buttons for the cut
	TGHorizontalFrame *SieveCutButtFrame=new TGHorizontalFrame(SieveCutFrame);
	SieveCutStartCut=new TGTextButton(SieveCutButtFrame,"Start Cut");
	SieveCutSkip=new TGTextButton(SieveCutButtFrame,"Skip");
	SieveCutNext=new TGTextButton(SieveCutButtFrame,"Next");

	SieveCutButtFrame->AddFrame(SieveCutNext,new TGLayoutHints(kLHintsRight,5,5,2,2));
	SieveCutButtFrame->AddFrame(SieveCutSkip,new TGLayoutHints(kLHintsRight,10,10,2,2));
	SieveCutButtFrame->AddFrame(SieveCutStartCut,new TGLayoutHints(kLHintsRight,5,5,2,2));

	SieveCutFrame->AddFrame(SieveCutEnableFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft));
	SieveCutFrame->AddFrame(new TGHorizontal3DLine(SieveCutFrame),new TGLayoutHints(kLHintsLeft|kLHintsExpandX,2,2,2,2));
	SieveCutFrame->AddFrame(SieveCutPlotCommandFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));
	SieveCutFrame->AddFrame(SieveCutPlotCutCommandFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));
	SieveCutFrame->AddFrame(SieveCutSetRangeFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));
	SieveCutFrame->AddFrame(new TGHorizontal3DLine(SieveCutFrame),new TGLayoutHints(kLHintsLeft|kLHintsExpandX,2,2,2,2));
	SieveCutFrame->AddFrame(SieveCutButtFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));

	//SieveCutFrame->AddFrame(new TGHorizontal3DLine(SieveCutFrame),new TGLayoutHints(kLHintsLeft|kLHintsExpandX,2,2,2,2));

	p->AddFrame(SieveCutFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));

}

void UserGUIHRSOpticsWizard::opticsDpCutUnit(TGCompositeFrame *p,TGLayoutHints *l){
//	TGGroupFrame *SieveCutFrame=new TGGroupFrame
	TGVerticalFrame *DpCutFrame=new TGVerticalFrame(p);
	TGHorizontalFrame *DpCutEnableFrame=new TGHorizontalFrame(DpCutFrame);

	TGCheckButton *DpEnableButt=new TGCheckButton(DpCutEnableFrame,new TGHotString("Enable"));

	DpCutEnableFrame->AddFrame(DpEnableButt,new TGLayoutHints(kLHintsLeft,5,5,5,5));
	DpCutFrame->AddFrame(DpCutEnableFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft));
	p->AddFrame(DpCutFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));

}


void UserGUIHRSOpticsWizard::opticsCustmizeCutUnit(TGCompositeFrame *p,TGLayoutHints *l){
	TGVerticalFrame *CustmizeCutFrame=new TGVerticalFrame(p);
	TGHorizontalFrame *CustmizeCutEnableFrame=new TGHorizontalFrame(CustmizeCutFrame);
	TGCheckButton *CustmizeEnableButt=new TGCheckButton(CustmizeCutEnableFrame,new TGHotString("Enable"));

	CustmizeCutEnableFrame->AddFrame(CustmizeEnableButt,new TGLayoutHints(kLHintsLeft,5,5,5,5));
	CustmizeCutFrame->AddFrame(CustmizeCutEnableFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft));
	p->AddFrame(CustmizeCutFrame,new TGLayoutHints(kLHintsTop|kLHintsLeft|kLHintsExpandX));
}


// draw menu on top of the canvas
void UserGUIHRSOpticsWizard::gMenuUnitDraw(TGCompositeFrame *p,TGLayoutHints *l ){
	TGDockableFrame *fMenuDock = new TGDockableFrame(p);
	AddFrame(fMenuDock,l);
	fMenuDock->SetWindowName("UVa GEM Analysis Framework");
	TGLayoutHints *fMenuBarLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX);
	TGLayoutHints *fMenuBarItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0,0);
	TGLayoutHints *fMenuBarHelpLayout = new TGLayoutHints(kLHintsTop | kLHintsRight);
	TGPopupMenu *fMenuFile = new TGPopupMenu(fClient->GetRoot());
	TGPopupMenu *fMenuSet  = new TGPopupMenu(fClient->GetRoot());
	TGPopupMenu *fMenuTool  = new TGPopupMenu(fClient->GetRoot());
	TGPopupMenu *fMenuView  = new TGPopupMenu(fClient->GetRoot());
	TGPopupMenu *fMenuHelp  = new TGPopupMenu(fClient->GetRoot());

	fMenuDock->EnableUndock(kTRUE);
	fMenuDock->EnableHide(kTRUE);
	TGMenuBar *fMenuBar= new TGMenuBar(fMenuDock,1,1,kHorizontalFrame);
	fMenuBar -> AddPopup("&File",fMenuFile,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&Set",fMenuSet,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&Tool",fMenuTool,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&View",fMenuView,fMenuBarItemLayout);
	fMenuBar -> AddPopup("&Help",fMenuHelp,fMenuBarHelpLayout);
	fMenuDock -> AddFrame(fMenuBar,fMenuBarLayout);
	TGHorizontal3DLine *menu_seperator=new TGHorizontal3DLine(this);
	AddFrame(menu_seperator,new TGLayoutHints(kLHintsExpandX));
}

void UserGUIHRSOpticsWizard::opticsTextPrintOutUnit(TGCompositeFrame *p,TGLayoutHints *l){
	TGVerticalFrame *opticsTextPrintoutFrame=new TGVerticalFrame(p);
	Pixel_t backpxl;
	gClient->GetColorByName("#c0c0c0", backpxl);
	opticsTextDisplay = new TGTextView(opticsTextPrintoutFrame,300,94,p->GetUniqueID(),kFixedWidth);
	opticsTextDisplay->SetBackgroundColor(backpxl);
	opticsTextDisplay->SetWidth(300);
	opticsTextDisplay->SetText(new TGText("> Jefferson Lab Hall A Optics Wizard"));
	opticsTextDisplay->AddText(new TGText("> Jefferson Lab Hall A Optics Wizard"));

	opticsTextPrintoutFrame->AddFrame(opticsTextDisplay,new TGLayoutHints(kLHintsLeft|kLHintsExpandX|kLHintsBottom,2,2,2,2));
	p->AddFrame(opticsTextPrintoutFrame,l);
}


void UserGUIHRSOpticsWizard::gStatusUnitDraw(TGCompositeFrame *p,TGLayoutHints *l){
	TGCompositeFrame *fStatusFrame = new TGCompositeFrame(p, 60,20,kHorizontalFrame|kSunkenFrame);

	Pixel_t yellow,green;
	fClient->GetColorByName("yellow",yellow);
	fClient->GetColorByName("green",green);

	TGVertical3DLine *lStatusbarSeparation0=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation0,new TGLayoutHints(kLHintsRight | kLHintsTop |kLHintsExpandY));

	TGLabel *author_display=new TGLabel(fStatusFrame,"UVa GEM Analysis Framework @Siyu");
	fStatusFrame->AddFrame(author_display,new TGLayoutHints(kLHintsRight | kLHintsTop | kLHintsExpandY,5,5,0,0));
	TGVertical3DLine *lStatusbarSeparation1=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation1, new TGLayoutHints(kLHintsRight | kLHintsTop | kLHintsExpandY));

	TGVertical3DLine *lStatusbarSeparation2=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation2, new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY));

	TGLabel *nStatusBarInfor = new TGLabel(fStatusFrame,"Jefferson Lab Hall A HRS Optics");
	fStatusFrame->AddFrame(nStatusBarInfor, new TGLayoutHints(kLHintsRight|kLHintsTop|kLHintsExpandY,15,15,0,0));

	// set the time display
	TGLayoutHints *StatusBarLayout1=new TGLayoutHints(kLHintsLeft|kLHintsTop|kLHintsExpandY,10,10);
	TDatime *lSystemTime = new TDatime();

	TGLabel *nStatusBarTimeLabel = new TGLabel(fStatusFrame,new TGString(Form("%4d : %02d : %02d",lSystemTime->GetYear(),lSystemTime->GetMonth(),lSystemTime->GetDay())));
	nStatusBarTimeLabel->Set3DStyle(0);
	fStatusFrame->AddFrame(nStatusBarTimeLabel,StatusBarLayout1);

	TGVertical3DLine *lStatusbarSeparation3=new TGVertical3DLine(fStatusFrame);
	fStatusFrame->AddFrame(lStatusbarSeparation3, new TGLayoutHints(kLHintsLeft|kLHintsTop|kLHintsExpandY));

	p->AddFrame(fStatusFrame,new TGLayoutHints(kLHintsBottom|kLHintsExpandX,0,0,1,0));
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
