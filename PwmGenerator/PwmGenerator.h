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

	typedef struct{
		float ff0;	// Feed-forward coef [n]
		float ff1;	// Feed-forward coef [n-1]

		float fb1;	// Feedback coef [n-1]
		float fb2;	// Feedback coef [n-2]

		float r_n;
		float r_n1;
		float r_n2;

		float in;
		float out;
	}SinusZDomain_t;

public:
	PwmGenerator();
	virtual ~PwmGenerator();

	bool hardwareDeinit();
	bool hardwareInit();

	void setPWMPeriod(uint32_t period);
	uint32_t getPWMPeriod();

	/*
	 * @brief
	 * @param freq Desired frequency for PWM in kHz
	 */
	void setPWMFreq(uint32_t freq);
	uint32_t getPWMFreq();

	void setPWMDeadtime(uint32_t dt_ns);
	uint32_t getPWMDeadtime();

	void runController();	// Fast and high priority task
	void runStateMachine();	// Slow and low priority task
protected:

	bool prerequest();
	bool postOperations();

	// Bu fonksiyonlar interface olmali, farkli farkli donanimlar icin ...
	void setPWMCompare();
	void setPWMCompare(uint32_t compare1, uint32_t compare2);
	uint32_t getPWMCompare1();
	uint32_t getPWMCompare2();

	void calculateSinusParameters(SinusZDomain_t *sinObj, float fi_deg, uint32_t freq_khz, uint32_t f_sampling_khz);
	float runSinusGeneratorFunction(SinusZDomain_t *sinObj);
private:

	uint32_t period_reg;
	uint32_t freq;
	uint32_t deadtime_reg;
	uint32_t compare1_reg;
	uint32_t compare2_reg;

	SinusZDomain_t sin0;
	SinusZDomain_t sin180;

	float debug_o1[200];
	float debug_o2[200];
};

#endif /* PWMGENERATOR_H_ */
