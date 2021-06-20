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

	virtual bool hardwareDeinit() override;
	virtual bool hardwareInit() override;

	void indicationStateMachine();
	void setLedBlinkPeriod(uint32_t ms);
protected:

	typedef enum{
		LED_STATE_OFF = 0,
		LED_STATE_WAIT,
		LED_STATE_ON
	}ledState_e;

	bool prerequest() override;
	bool postOperations() override ;
	void setLedState(ledState_e LED_STATE_x);
private:

	ledState_e state_e, statePrev_e;
	uint32_t ticker_end;
	uint32_t period;
};

#endif /* LEDINDICATOR_H_ */
