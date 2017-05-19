/*
 * UserGUIMapWizard.h
 *
 *  Created on: May 16, 2017
 *      Author: newdriver
 */

#ifndef USERGUIMAPWIZARD_H_
#define USERGUIMAPWIZARD_H_

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
// detector mapping structure

struct Mapping_structure{
	int MPDID;
	int GEMID;
	int Xis; // x:0 y:1
	int ADCid;
	int I2C;
	int Pos;
	int Invert; // true 1, false:0

};

enum USERGUICommandIdentifiers{
	// top level button identifiers
	GUI_MAPPING_WIZARD_K_MAIN_FRAME_CANCEL,
	GUI_MAPPING_WIZARD_K_MAIN_FRAME_SAVE,
	GUI_MAPPING_WIZARD_K_MAIN_FRAME_SAVE_EXIT,

	// control level identifiers

	//-----------------------------
	GUI_MAPPING_WIZARD_K_ADDAPV,
	GUI_MAPPING_WIZARD_K_DELETEAPV,
	GUI_MAPPING_WIZARD_K_DELETEMPD,
	GUI_MAPPING_WIZARD_K_DELETEDETECTOR
};

class UserGUIMapWizard : public TGTransientFrame
{
public:
	UserGUIMapWizard(const TGWindow *p, const TGWindow *main, UInt_t w, UInt_t h,
            UInt_t options = kVerticalFrame);

	virtual void CloseWindow();
	virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
	virtual ~UserGUIMapWizard();

private:

	Bool_t MappingGenerate();   // mapping generating interface

private:
	// used to save the final mappings
	std::map<int,std::vector<Mapping_structure>> Detector_mapping_buffer;

private:
	// gui elements
	TGCompositeFrame *mvMainFrame;

	TGHorizontalFrame *mvMainFrameButton;
	TGHorizontalFrame *mvMainFrameDisplay;

	// lever three
	TGVerticalFrame *mvMapping_controlframe;
	TGCompositeFrame   *mvMappingmv_Canvasframe;

	// level four, the control button and list box ragion
	TGHorizontalFrame *mvDisCtrl_ListBoxFrame;
	TGHorizontalFrame *mvDisCtrl_Input_buttonFrame;

	TGButtonGroup *mvButtonGroupDetectorID,*mvButtonGroupMPDID,*mvButtonGroupAPVID;
	TGListBox	  *mvListDetectorID, *mvListMPDID, *mvListAPVID;

	//TGHorizontalFrame *mvControlConfigureFrame;    //
	TGButtonGroup *mvConfigureInputGroup, *mvConfigureButtonGroup;
	TGNumberEntry *mvDetectorID,*mvMPDID,*mvADCID,*mvI2C,*mvXis,*mvPos,*Inverse;
	TGButton *mvConfigureAddAPVButton,*mvConfigureDeleteAPVButton,*mvConfigureDeleteMPDButton,*mvConfigureDeleteDetectorButton;
	TRootEmbeddedCanvas *APVLayoutDisplayCanvas;

	TGButton *mvMainFrameCancle, *mvMainFrameSave,*mvMainFrameSave_Exit;
	//TGVButtonGroup *mvControlButton;

};

#endif /* USERGUIMAPWIZARD_H_ */
