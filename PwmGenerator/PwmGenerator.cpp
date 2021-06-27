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


	HRTIM1->sCommonRegs.OENR |= 0x000F;		// CHA1, CHA2, CHB1 and CHB2 is enabled

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

	return true;
}

bool PwmGenerator::prerequest(){

	return true;
}

bool PwmGenerator::postOperations(){

	return true;
}


