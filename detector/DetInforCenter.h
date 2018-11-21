/*
 * DetInforCenter.h
 *
 *  Created on: Nov 21, 2018
 *      Author: newdriver
 */

#ifndef DETINFORCENTER_H_
#define DETINFORCENTER_H_

class DetInforCenter {
public:
	DetInforCenter();
	virtual ~DetInforCenter();
	void LoadDetMapping();
	void LoadDetCaliration();
};

#endif /* DETINFORCENTER_H_ */
