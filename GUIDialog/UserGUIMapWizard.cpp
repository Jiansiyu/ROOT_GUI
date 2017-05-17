/*
 * UserGUIMapWizard.cpp
 *
 *  Created on: May 16, 2017
 *      Author: newdriver
 */

#include "UserGUIMapWizard.h"

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
	mvButtonGroupMPDID      =new TGButtonGroup(mvDisCtrl_ListBoxFrame,"MPDid");
	mvButtonGroupAPVID		= new TGButtonGroup(mvDisCtrl_ListBoxFrame,"APVidD");
	// add the listbox  into the list box




	mvDisCtrl_ListBoxFrame->AddFrame(mvButtonGroupMPDID,new TGLayoutHints(kLHintsLeft|kLHintsTop));
	mvDisCtrl_ListBoxFrame->AddFrame(mvButtonGroupDetectorID,new TGLayoutHints(kLHintsLeft|kLHintsTop));
	mvDisCtrl_ListBoxFrame->AddFrame(mvButtonGroupAPVID,new TGLayoutHints(kLHintsLeft|kLHintsBottom));



	mvMapping_controlframe->AddFrame(mvDisCtrl_ListBoxFrame, new TGLayoutHints(kLHintsTop|kLHintsExpandX));
	TGHorizontal3DLine *mvDisCtrl_Seperation=new TGHorizontal3DLine(mvMapping_controlframe);
	mvMapping_controlframe->AddFrame(mvDisCtrl_Seperation,new TGLayoutHints(kLHintsExpandX|kLHintsTop));
	mvDisCtrl_Input_buttonFrame = new TGHorizontalFrame(mvMapping_controlframe);
	mvMapping_controlframe->AddFrame(mvDisCtrl_Input_buttonFrame, new TGLayoutHints(kLHintsTop|kLHintsExpandX));

	mvMainFrameDisplay->AddFrame(mvMapping_controlframe, new TGLayoutHints(kLHintsTop|kLHintsLeft,2,2,2,2));
	TGVertical3DLine *mapping_seperations= new TGVertical3DLine(mvMainFrameDisplay);
	mvMainFrameDisplay->AddFrame(mapping_seperations,new TGLayoutHints(kLHintsExpandY));

	mvMappingmv_Canvasframe = new TGCompositeFrame(mvMainFrameDisplay);
	mvMainFrameDisplay->AddFrame(mvMappingmv_Canvasframe, new TGLayoutHints(kLHintsLeft|kLHintsExpandY,2,2,2,2));



	mvMainFrame->AddFrame(mvMainFrameDisplay,new TGLayoutHints(kLHintsTop|kLHintsExpandX|kLHintsExpandY,2,2,2,2));
	TGHorizontal3DLine *mainlevelseperation = new TGHorizontal3DLine(mvMainFrame);
	mvMainFrame ->AddFrame(mainlevelseperation,new TGLayoutHints(kLHintsTop|kLHintsRight|kLHintsLeft|kLHintsExpandX,2,2));
	mvMainFrame ->AddFrame(mvMainFrameButton,new TGLayoutHints(kLHintsBottom|kLHintsExpandX|kLHintsTop,2,2,2,2));
	mvMainFrame->Resize(500,500);
	AddFrame(mvMainFrame,new TGLayoutHints(kLHintsBottom | kLHintsRight|kLHintsExpandX|kLHintsExpandY, 2, 2, 5, 1));
	// visualize the windows
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

