/*
 * HRSOpticsCut.h
 *
 *  Created on: Oct 13, 2019
 *      Author: newdriver
 */

#ifndef HRSOPTICSCUT_H_
#define HRSOPTICSCUT_H_

#include <string.h>
#include <TCut.h>
#include <TCutG.h>
#include <random>

class HRSOpticsCut {
public:
	HRSOpticsCut();
	virtual ~HRSOpticsCut();

public:
	virtual void EmbededCanvasConfig();
	virtual void PlotToCanvas();
	virtual void ReadCanvasConfig();
	virtual Bool_t AddCustomizedCut(std::string plotItems,TCut *cut);
	virtual Bool_t EnableVzCut();  // used for select which of the target will be used in the optimization
	virtual Bool_t EnableDpCut();  // used for select Dp
	virtual Bool_t EnableSieveCut();
	virtual Bool_t SaveCut();   // save the cut in harddrive
private:
	void AddCut();


};

#endif /* HRSOPTICSCUT_H_ */
