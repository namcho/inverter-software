/*
 * LedIndicator.h
 *
 *  Created on: Jun 20, 2021
 *      Author: nazimyildiz
 */

#ifndef LEDINDICATOR_H_
#define LEDINDICATOR_H_

#include "../HardwareInit/IHardware.h"

class LedIndicator : public IHardware {
public:
	LedIndicator();
	virtual ~LedIndicator();

	bool hardwareDeinit();
	bool hardwareInit();

protected:
	bool prerequest();
	bool postOperations();

};

#endif /* LEDINDICATOR_H_ */
