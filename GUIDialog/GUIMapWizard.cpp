/*
 * UserGUIMapWizard.cpp
 *
 *  Created on: May 16, 2017
 *      Author: newdriver
 */

#include "GUIMapWizard.h"

UserGUIMapWizard::UserGUIMapWizard(const TGWindow *p, const TGWindow *main, UInt_t w,
        UInt_t h, UInt_t options) : TGTransientFrame(p, main, w, h, options)
{
	SetCleanup(kDeepCleanup);
	mvMainFrame = new TGVerticalFrame(this,60,20,kFixedWidth);

	mvMainFrameButton = new TGHorizontalFrame(mvMainFrame);
	mvMainFrameDisplay = new TGHorizontalFrame(mvMainFrame);

	// add the top level control buttons
	mvMainFrameCancle = new TGTextButton(mvMainFrameButton,"C&ancel",GUI_MAPPING_WIZARD_K_MAIN_FRAME_CANCEL);
	mvMainFrameCancle ->Associate(this);
	mvMainFrameSave   = new TGTextButton(mvMainFrameButton,"S&ave",GUI_MAPPING_WIZARD_K_MAIN_FRAME_SAVE);
	mvMainFrameSave   ->Associate(this);
	mvMainFrameSave_Exit   = new TGTextButton(mvMainFrameButton,"Save &Exit",GUI_MAPPING_WIZARD_K_MAIN_FRAME_SAVE_EXIT);
	mvMainFrameSave_Exit   ->Associate(this);
	//mvMainFrameCancle = new TGTextButton();
	mvMainFrameButton->AddFrame(mvMainFrameSave_Exit,new TGLayoutHints(kLHintsBottom|kLHintsRight|kLHintsTop,2,2,2,2));
	mvMainFrameButton->AddFrame(mvMainFrameSave,new TGLayoutHints(kLHintsBottom|kLHintsRight|kLHintsTop,2,2,2,2));
	mvMainFrameButton->AddFrame(mvMainFrameCancle,new TGLayoutHints(kLHintsBottom|kLHintsRight|kLHintsTop,2,2,2,2));
	//mvMainFrameButton->Resize(150,mvMainFrameSave_Exit->GetHeight());

	// the main mapping frame display control panels
	mvMapping_controlframe = new TGVerticalFrame(mvMainFrameDisplay);

	mvDisCtrl_ListBoxFrame = new TGHorizontalFrame(mvMapping_controlframe);

	// add the list box frame
	mvButtonGroupDetectorID =new TGButtonGroup(mvDisCtrl_ListBoxFrame,"DetectorID");
	mvButtonGroupMPDID      =new TGButtonGroup(mvDisCtrl_ListBoxFrame,"MPDID");
	mvButtonGroupAPVID		= new TGButtonGroup(mvDisCtrl_ListBoxFrame,"APVID");
	// add the listbox  into the list box

	mvListDetectorID = new TGListBox(mvButtonGroupDetectorID);
	mvListMPDID      = new TGListBox(mvButtonGroupMPDID);
	mvListAPVID		 = new TGListBox(mvButtonGroupAPVID);
	mvListDetectorID->Resize(80,100);
	mvListMPDID		->Resize(80,100);
	mvListAPVID		->Resize(80,100);
	// add the list box to the frame handler
	mvButtonGroupDetectorID ->AddFrame( mvListDetectorID);
	mvButtonGroupMPDID		->AddFrame(mvListMPDID);
	mvButtonGroupAPVID		->AddFrame(mvListAPVID);

	mvDisCtrl_ListBoxFrame->AddFrame(mvButtonGroupDetectorID,new TGLayoutHints(kLHintsLeft|kLHintsTop));
	mvDisCtrl_ListBoxFrame->AddFrame(mvButtonGroupMPDID,new TGLayoutHints(kLHintsLeft|kLHintsTop));
	mvDisCtrl_ListBoxFrame->AddFrame(mvButtonGroupAPVID,new TGLayoutHints(kLHintsLeft|kLHintsBottom));

	//configuration input frame
	mvDisCtrl_Input_buttonFrame = new TGHorizontalFrame(mvMapping_controlframe);

	mvConfigureButtonGroup= new TGButtonGroup(mvDisCtrl_Input_buttonFrame,"Input Panel");
	mvConfigureInputGroup = new TGButtonGroup(mvDisCtrl_Input_buttonFrame,"Control Panel");


	//-----------------------------------------------------------------------------------------------------------
	// input number input
	TGHorizontalFrame *mvDetectorIDFrame=new TGHorizontalFrame(mvConfigureInputGroup);
	mvDetectorID = new TGNumberEntry(mvDetectorIDFrame);
	TGLabel *mvDetectorIDLabel = new TGLabel(mvDetectorIDFrame,"DetectorID");
	mvDetectorIDFrame->AddFrame(mvDetectorID,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2) );
	mvDetectorIDFrame->AddFrame(mvDetectorIDLabel,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2));
	mvConfigureInputGroup->AddFrame(mvDetectorIDFrame,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2));

	TGHorizontalFrame *mvMPDIDFrame=new TGHorizontalFrame(mvConfigureInputGroup);
	mvMPDID		 = new TGNumberEntry(mvMPDIDFrame);
	TGLabel *mvMPDIDLabel = new TGLabel(mvMPDIDFrame,"MPD ID");
	mvMPDIDFrame->AddFrame(mvMPDID,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2) );
	mvMPDIDFrame->AddFrame(mvMPDIDLabel,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2));
	mvConfigureInputGroup->AddFrame(mvMPDIDFrame,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2));

	TGHorizontalFrame *mvADCIDFrame=new TGHorizontalFrame(mvConfigureInputGroup);
	mvADCID      = new TGNumberEntry(mvADCIDFrame);
	TGLabel *mvADCIDLabel = new TGLabel(mvADCIDFrame,"ADC ID");
	mvADCIDFrame ->AddFrame(mvADCID,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2) );
	mvADCIDFrame ->AddFrame(mvADCIDLabel,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2));
	mvConfigureInputGroup->AddFrame(mvADCIDFrame,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2));

	TGHorizontalFrame *mvI2CIDFrame=new TGHorizontalFrame(mvConfigureInputGroup);
	mvI2C		 = new TGNumberEntry(mvI2CIDFrame);
	TGLabel *mvI2CIDLabel = new TGLabel(mvI2CIDFrame,"I2C");
	mvI2CIDFrame->AddFrame(mvI2C,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2) );
	mvI2CIDFrame->AddFrame(mvI2CIDLabel,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2) );
	mvConfigureInputGroup->AddFrame(mvI2CIDFrame,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2));

	TGHorizontalFrame *mvPosFrame=new TGHorizontalFrame(mvConfigureInputGroup);
	mvPos        = new TGNumberEntry(mvPosFrame);
	TGLabel *mvPosLabel = new TGLabel(mvPosFrame,"Position");
	mvPosFrame->AddFrame(mvPos,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2) );
	mvPosFrame->AddFrame(mvPosLabel,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2) );
	mvConfigureInputGroup->AddFrame(mvPosFrame,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2));
	//---------------------------------------------------------------------------------------------------------

	//++++++++++++++++++ control buttton++++++++++++++++++++++++++++++++++++++
	//--------------------------------------------------------------------------------------------------------
	TGVerticalFrame *lControlButtonFrame = new TGVerticalFrame(mvConfigureButtonGroup);

	mvConfigureAddAPVButton         = new TGTextButton(lControlButtonFrame,"   A&dd APV      ",GUI_MAPPING_WIZARD_K_ADDAPV);
	mvConfigureDeleteAPVButton      = new TGTextButton(lControlButtonFrame,"  Delete APV   ",GUI_MAPPING_WIZARD_K_DELETEAPV);
	mvConfigureDeleteMPDButton      = new TGTextButton(lControlButtonFrame,"  Delete MPD   ",GUI_MAPPING_WIZARD_K_DELETEMPD);
	mvConfigureDeleteDetectorButton = new TGTextButton(lControlButtonFrame,"Delete Detector",GUI_MAPPING_WIZARD_K_DELETEDETECTOR);

	mvConfigureAddAPVButton->Associate(this);
	mvConfigureDeleteAPVButton->Associate(this);
	mvConfigureDeleteMPDButton->Associate(this);
	mvConfigureDeleteDetectorButton->Associate(this);

	lControlButtonFrame->AddFrame(mvConfigureAddAPVButton,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2));
	lControlButtonFrame->AddFrame(mvConfigureDeleteAPVButton,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2));
	lControlButtonFrame->AddFrame(mvConfigureDeleteMPDButton,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2));
	lControlButtonFrame->AddFrame(mvConfigureDeleteDetectorButton,new TGLayoutHints(kLHintsCenterY | kLHintsRight, 2, 2, 2, 2));
	mvConfigureButtonGroup->AddFrame(lControlButtonFrame, new TGLayoutHints(kLHintsRight|kLHintsExpandY,2,2,2,2));

	//--------------------------------------------------------------------------------------------------------

	mvDisCtrl_Input_buttonFrame->AddFrame(mvConfigureInputGroup, new TGLayoutHints(kLHintsTop|kLHintsBottom|kLHintsLeft|kLHintsCenterX,2,2,2,2));
	mvDisCtrl_Input_buttonFrame->AddFrame(mvConfigureButtonGroup,new TGLayoutHints(kLHintsTop|kLHintsBottom|kLHintsCenterX|kLHintsRight,2,2,2,2));
	mvMapping_controlframe->AddFrame(mvDisCtrl_ListBoxFrame, new TGLayoutHints(kLHintsTop|kLHintsExpandX));
	TGHorizontal3DLine *mvDisCtrl_Seperation=new TGHorizontal3DLine(mvMapping_controlframe);
	mvMapping_controlframe->AddFrame(mvDisCtrl_Seperation,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
	mvMapping_controlframe->AddFrame(mvDisCtrl_Input_buttonFrame, new TGLayoutHints(kLHintsTop|kLHintsExpandX));
	mvMainFrameDisplay->AddFrame(mvMapping_controlframe, new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2));
	TGVertical3DLine *mapping_seperations= new TGVertical3DLine(mvMainFrameDisplay);
	mvMainFrameDisplay->AddFrame(mapping_seperations,new TGLayoutHints(kLHintsExpandY));

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	mvMappingmv_Canvasframe = new TGCompositeFrame(mvMainFrameDisplay);

	APVLayoutDisplayCanvas = new TRootEmbeddedCanvas("Electronics Layout",mvMappingmv_Canvasframe);
	mvMappingmv_Canvasframe->AddFrame(APVLayoutDisplayCanvas,new TGLayoutHints(kLHintsExpandX|kLHintsExpandY));

	mvMainFrameDisplay->AddFrame(mvMappingmv_Canvasframe, new TGLayoutHints(kLHintsLeft|kLHintsExpandY|kLHintsExpandX,2,2,2,2));
	//---------------------------------------------------------------------------------------------------

	mvMainFrame->AddFrame(mvMainFrameDisplay,new TGLayoutHints(kLHintsTop|kLHintsExpandX|kLHintsExpandY,2,2,2,2));
	TGHorizontal3DLine *mainlevelseperation = new TGHorizontal3DLine(mvMainFrame);
	mvMainFrame ->AddFrame(mainlevelseperation,new TGLayoutHints(kLHintsTop|kLHintsRight|kLHintsLeft|kLHintsExpandX,2,2));
	mvMainFrame ->AddFrame(mvMainFrameButton,new TGLayoutHints(kLHintsBottom|kLHintsExpandX|kLHintsTop,2,2,2,2));
	mvMainFrame->Resize(800,300);

	//redraw the main windows
	AddFrame(mvMainFrame,new TGLayoutHints(kLHintsBottom | kLHintsRight|kLHintsExpandX|kLHintsExpandY, 2, 2, 5, 1));
	SetWindowName("Detector Mapping Wizard");
	MapSubwindows();
	Resize();   // resize to default size
	MapWindow();
	// position relative to the parent's window
	CenterOnParent();
	Print();
}


// signal process unit
Bool_t UserGUIMapWizard::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2) {

}

// close  the windows
void UserGUIMapWizard::CloseWindow() {

	 DeleteWindow();
}

UserGUIMapWizard::~UserGUIMapWizard() {
	// TODO Auto-generated destructor stub
}

