/*
 * PwmGenerator.cpp
 *
 *  Created on: Jun 20, 2021
 *      Author: nazimyildiz
 */

#include "PwmGenerator.h"
#include <stm32f334x8.h>

PwmGenerator::PwmGenerator() {
	// TODO Auto-generated constructor stub

}

PwmGenerator::~PwmGenerator() {
	// TODO Auto-generated destructor stub
}

bool PwmGenerator::hardwareDeinit(){

	return true;
}

bool PwmGenerator::hardwareInit(){

	/// 1) IO configuration must be done first prior to HRTIM registers(Control, Period, interrept enbles, ... kind of things)
	/// 2) HRTIM register configuration have to be done as second
	/// 3) HRTIM output pins which is going to use for electrical PWM signals

	/// HRTIM FAULT1 setting

	/// HRTIM clock enable and calibration
	RCC->APB2ENR |= (1 << 29);	// HRTIM clock is enabled
	RCC->CFGR3 |= (1 << 12);	// HRTIM1SW bit is set, so HRTIM will have 144MHz

	HRTIM1->sCommonRegs.DLLCR |= 0x01;	// Start calibration
	while(!((HRTIM1->sCommonRegs.ISR >> 16) && 0x01));	// Wait for calibration
	HRTIM1->sCommonRegs.DLLCR |= (1 << 2);	// Periodic calibration is enabled


	// TIMA init
	HRTIM1->sTimerxRegs[0].TIMxCR |= (1 << 3); 	// CONT bit set
	HRTIM1->sTimerxRegs[0].TIMxCR |= (1 << 27);	// PREEN is set
	HRTIM1->sTimerxRegs[0].TIMxCR |= (1 << 18);	// TxRSTU is set; Timerx roll-over reset is activated
	HRTIM1->sTimerxRegs[0].PERxR = 40959;		// 100kHz
	HRTIM1->sTimerxRegs[0].CMP1xR = 10279;		// %25 Duty
	HRTIM1->sTimerxRegs[0].SETx1R |= (1 << 2);	// Set source is PER
	HRTIM1->sTimerxRegs[0].RSTx1R |= (1 << 3);	// Reset source is CMP1
	HRTIM1->sTimerxRegs[0].SETx1R |= (1 << 0);	// Set CHA1 output GPIO for proper startup
	HRTIM1->sTimerxRegs[0].RSTx2R |= (1 << 0);	// Reset CHA2 output GPIO for proper startup
	HRTIM1->sTimerxRegs[0].OUTxR |= (1 << 8);	// Deadtime is actived between CHA1 and CHA2

	HRTIM1->sTimerxRegs[0].DTxR |= (2 << 10);	// DTPRSSC is 2
	HRTIM1->sTimerxRegs[0].DTxR |= 100;			// DTRx is 100; Rising edge deadtime
	HRTIM1->sTimerxRegs[0].DTxR = (100 << 16);	// DTFx is 100; Falling edge deadtime
	HRTIM1->sTimerxRegs[0].DTxR |= (2 << 4);	// CHA1 is inactive when fault occurs
	HRTIM1->sTimerxRegs[0].DTxR |= (2 << 20);	// CHA2 is inactive when fault occurs
//	HRTIM1->sTimerxRegs[0].DTxR |= (1 << 19);	// CHA2 is in active when in IDLE state
	HRTIM1->sTimerxRegs[0].DTxR |= (1 << 8);	// Deadtime enable

	HRTIM1->sTimerxRegs[0].RSTx1R |= HRTIM_RST1R_SRT;
	HRTIM1->sTimerxRegs[0].SETx2R |= HRTIM_SET2R_SST;


	// If wanna start HRTIM with DMA support, we need to set specific EN bit in CR register...

	// Built-in comperators must be initialized(if need to use) before output routed to HRTIM


	/// GPIO pin AF(13) config - PA8 -> CHA1, PA9 -> CHA2, PA10 -> CHB1, PA11 -> CHB2
	// HRTIM I/Os must be initliazed in two phases, First HRTIM inputs must be init
	// The HRTIM outputs must be initialized after the HRTIM control registers programming
	RCC->AHBENR |= (1 << 17);

	// GPIOA8 set as HRTIM1-CHA1 output
	GPIOA->MODER &= ~(3 << 16);
	GPIOA->MODER |= (2 << 16);
	GPIOA->OTYPER &= ~(1 << 8);		// Push pull
	GPIOA->OSPEEDR |= (3 << 16);	// High speed
	GPIOA->AFR[1] &= ~(15 << 0);
	GPIOA->AFR[1] |= (13 << 0);		// AF13

	// GPIOA9 set as HRTIM1-CHA2 output
	GPIOA->MODER &= ~(3 << 18);
	GPIOA->MODER |= (2 << 18);
	GPIOA->OTYPER &= ~(1 << 9);		// Push pull
	GPIOA->OSPEEDR |= (3 << 18);
	GPIOA->AFR[1] &= ~(15 << 4);
	GPIOA->AFR[1] |= (13 << 4);		// AF13

	// GPIOA10 set as HRTIM1-CHB1 output
	GPIOA->MODER &= ~(3 << 20);
	GPIOA->MODER |= (2 << 20);
	GPIOA->OTYPER &= ~(1 << 10);	// Push pull
	GPIOA->OSPEEDR |= (3  << 20);
	GPIOA->AFR[1] &= ~(15 << 8);
	GPIOA->AFR[1] |= (13 << 8);

	// GPIOA11 set as HRTIM1-CHB2 output
	GPIOA->MODER &= ~(3 << 22);
	GPIOA->MODER |= (2 << 22);
	GPIOA->OTYPER &= ~(1 << 11);
	GPIOA->OSPEEDR |= (3 << 22);
	GPIOA->AFR[1] &= ~(15 << 12);
	GPIOA->AFR[1] |= (13 << 12);

	/// Other perhipherals can be initialized
	// NVIC, DMA, Comperator vs...

	HRTIM1->sCommonRegs.OENR |= 0x000F;			// CHA1, CHA2, CHB1 and CHB2 is enabled
	HRTIM1->sCommonRegs.CR2 |= (1 << 1);		// Software update for TA to transfer preload -> active
	HRTIM1->sMasterRegs.MCR |= (1 << 17);		// TA is enabled

	return true;
}

bool PwmGenerator::prerequest(){

	return true;
}

bool PwmGenerator::postOperations(){

	return true;
}


