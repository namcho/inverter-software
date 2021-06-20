/*
 * PwmGenerator.h
 *
 *  Created on: Jun 20, 2021
 *      Author: nazimyildiz
 */

#ifndef PWMGENERATOR_H_
#define PWMGENERATOR_H_

#include <stdint.h>
#include "../HardwareInit/IHardware.h"

class PwmGenerator : public IHardware{
public:
	PwmGenerator();
	virtual ~PwmGenerator();

	bool hardwareDeinit();
	bool hardwareInit();

	void setPwmPeriod(uint32_t period);
	uint32_t getPwmPeriod();

	void setPWMFreq(uint32_t freq);
	uint32_t getPwmFreq();

	void setPwmDeadtime(uint32_t dt);
	uint32_t getPwmDeadtime();

	void setPwmCompare(uint32_t compare);
	uint32_t getPwmCompare();


protected:

	bool prerequest();
	bool postOperations();
private:

	uint32_t period_reg;
	uint32_t freq;
	uint32_t deadtime_reg;
	uint32_t compare_reg;
};

#endif /* PWMGENERATOR_H_ */
