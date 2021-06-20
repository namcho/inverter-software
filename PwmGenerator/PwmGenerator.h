/*
 * PwmGenerator.h
 *
 *  Created on: Jun 20, 2021
 *      Author: nazimyildiz
 */

#ifndef PWMGENERATOR_H_
#define PWMGENERATOR_H_

#include "../HardwareInit/IHardware.h"

class PwmGenerator : public IHardware{
public:
	PwmGenerator();
	virtual ~PwmGenerator();

	bool IHardware::hardwareDeinit();
	bool IHardware::hardwareInit();

protected:

	bool IHardware::prerequest();
	bool IHardware::postOperations();
private:

};

#endif /* PWMGENERATOR_H_ */
