/*
 * PwmGenerator.cpp
 *
 *  Created on: Jun 20, 2021
 *      Author: nazimyildiz
 */

#include "PwmGenerator.h"
#include <stm32f334x8.h>
#include <math.h>

#define F_HRTIM			128000			// kHz; (HSE / PRE_DIV_HSE) * (PLL_N) * (2) = (8MHz / 2) * (16) * (2)
#define F_HRCK			(F_HRTIM * 32)	// kHZ; DLL value is 32 for HRTIM1
#define MATH_PI 		3.14f			// PI constant
#define DEFAULT_MA		0.77f			// Default amplitude modulation index Ma/Mc

PwmGenerator::PwmGenerator() {

	// TODO Auto-generated constructor stub
	this->sin0.in = 0.0f;
	this->sin0.r_n1 = 1.0f;

	this->sin180.in = 0.0f;
	this->sin180.r_n1 = 1.0f;
}

PwmGenerator::~PwmGenerator() {
	// TODO Auto-generated destructor stub
}

bool PwmGenerator::hardwareDeinit(){

	return true;
}

bool PwmGenerator::hardwareInit(){
	uint16_t i;
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

	for (i = 0; i <= 1; ++i) {
		// TIMA init
		HRTIM1->sTimerxRegs[i].TIMxCR |= (1 << 3); 	// CONT bit set
		HRTIM1->sTimerxRegs[i].TIMxCR |= (1 << 27);	// PREEN is set
		HRTIM1->sTimerxRegs[i].TIMxCR |= (1 << 18);	// TxRSTU is set; Timerx roll-over reset is activated

	//	HRTIM1->sTimerxRegs[i].PERxR = 40959;		// 100kHz
	//	HRTIM1->sTimerxRegs[i].CMP1xR = 10279;		// %25 Duty
		HRTIM1->sTimerxRegs[i].SETx1R |= (1 << 2);	// Set source is PER
		HRTIM1->sTimerxRegs[i].RSTx1R |= (1 << 3);	// Reset source is CMP1
		HRTIM1->sTimerxRegs[i].SETx1R |= (1 << 0);	// Set CHA1 output GPIO for proper startup
		HRTIM1->sTimerxRegs[i].RSTx2R |= (1 << 0);	// Reset CHA2 output GPIO for proper startup
		HRTIM1->sTimerxRegs[i].OUTxR |= (1 << 8);	// Deadtime is actived between CHA1 and CHA2

		HRTIM1->sTimerxRegs[i].DTxR |= (2 << 10);	// DTPRSSC is 2
		HRTIM1->sTimerxRegs[i].DTxR |= 100;			// DTRx is 100; Rising edge deadtime
		HRTIM1->sTimerxRegs[i].DTxR = (100 << 16);	// DTFx is 100; Falling edge deadtime
		HRTIM1->sTimerxRegs[i].DTxR |= (2 << 4);	// CHA1 is inactive when fault occurs
		HRTIM1->sTimerxRegs[i].DTxR |= (2 << 20);	// CHA2 is inactive when fault occurs
	//	HRTIM1->sTimerxRegs[i].DTxR |= (1 << 19);	// CHA2 is in active when in IDLE state
		HRTIM1->sTimerxRegs[i].DTxR |= (1 << 8);	// Deadtime enable

		HRTIM1->sTimerxRegs[i].RSTx1R |= HRTIM_RST1R_SRT;
		HRTIM1->sTimerxRegs[i].SETx2R |= HRTIM_SET2R_SST;
	}


	HRTIM1->sTimerxRegs[0].REPxR = 9;
	HRTIM1->sTimerxRegs[0].TIMxDIER |= (1 << 4);	// Repetation interrupt is enabled.
	NVIC->ISER[3] |= (1 << 4);
	NVIC_EnableIRQ(HRTIM1_TIMA_IRQn);

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
	GPIOA->OSPEEDR &= ~(3 << 16);
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
	// Driver2 enable pin
	GPIOA->MODER &= ~(3 << 10);
	GPIOA->MODER |= (1 << 10);
	GPIOA->OTYPER &= ~(1 << 5);
	GPIOA->OSPEEDR |= (3 << 10);
	GPIOA->BSRR = (1 << 21);
	// Driver2 enable pin
	GPIOA->MODER &= ~(3 << 12);
	GPIOA->MODER |= (1 << 12);
	GPIOA->OTYPER &= ~(1 << 6);
	GPIOA->OSPEEDR |= (3 << 12);
	GPIOA->BSRR = (1 << 22);

	setPWMFreq(100);
	setPWMDeadtime(30);
	setPWMCompare(getPWMPeriod() / 4, getPWMPeriod() / 2);

	calculateSinusParameters(&sin0, 0.0f, 100, 10);
	calculateSinusParameters(&sin180, 180.0f, 100, 10);

	HRTIM1->sCommonRegs.OENR |= 0x000F;			// CHA1, CHA2, CHB1 and CHB2 is enabled
	HRTIM1->sCommonRegs.CR2 |= (1 << 1);		// Software update for TA to transfer preload -> active
	HRTIM1->sMasterRegs.MCR |= (3 << 17);		// TA is enabled

	return true;
}

void PwmGenerator::setPWMPeriod(uint32_t period){

	this->period_reg = period;

	HRTIM1->sTimerxRegs[0].PERxR = this->period_reg - 1;
	HRTIM1->sTimerxRegs[1].PERxR = this->period_reg - 1;
}

uint32_t PwmGenerator::getPWMPeriod(){

	return this->period_reg;
}

void PwmGenerator::setPWMFreq(uint32_t freq){

	this->freq = freq;

	// Calculate pwm period register value for stm32f334 hrtim
	setPWMPeriod(F_HRCK / freq);

}

uint32_t PwmGenerator::getPWMFreq(){

	return this->freq;
}

void PwmGenerator::setPWMDeadtime(uint32_t dt_ns){

	uint16_t psc;
	uint16_t power;
	float t_dtg;
	float t_hrtim;
	uint16_t dt_rising;
	uint16_t dt_falling;

	power = (HRTIM1->sTimerxRegs[0].DTxR >> 10) & 0x0007;
	psc = (1 << power);

	t_hrtim = 1.0/(F_HRTIM/1000000.0f);
	t_dtg = (psc * t_hrtim / 8);
	deadtime_reg = dt_ns / t_dtg;

	dt_rising = deadtime_reg;
	dt_falling = deadtime_reg;

	HRTIM1->sTimerxRegs[0].DTxR &= ~(0x01FF01FF);
	HRTIM1->sTimerxRegs[0].DTxR |= (dt_rising);
	HRTIM1->sTimerxRegs[0].DTxR |= (dt_falling << 16);

	HRTIM1->sTimerxRegs[1].DTxR &= ~(0x01FF01FF);
	HRTIM1->sTimerxRegs[1].DTxR |= (dt_rising);
	HRTIM1->sTimerxRegs[1].DTxR |= (dt_falling << 16);
}

uint32_t PwmGenerator::getPWMDeadtime(){

	return this->deadtime_reg;
}

void PwmGenerator::runController(){

	float la, lb;

	// Generate sinus duty values
	la = runSinusGeneratorFunction(&sin0) + 1.0f;
	lb = runSinusGeneratorFunction(&sin180) + 1.0f;

	this->compare1_reg = (la / 2) * getPWMPeriod() * DEFAULT_MA;
	this->compare2_reg = (lb / 2) * getPWMPeriod() * DEFAULT_MA;

	setPWMCompare();
}

void PwmGenerator::runStateMachine(){

	// State machine function for non-time critical tasks...
	// For example, change frequency of referans sinus


	// Request list olacak ve request geldikce, ilgili requestler icin gerekli islemler sirasiyla yapilacak...
}

void PwmGenerator::setPWMCompare(){

	HRTIM1->sTimerxRegs[0].CMP1xR = compare1_reg + 0x0220;
	HRTIM1->sTimerxRegs[1].CMP1xR = compare2_reg + 0x0220;
}

void PwmGenerator::setPWMCompare(uint32_t compare1, uint32_t compare2){

	HRTIM1->sTimerxRegs[0].CMP1xR = compare1;
	HRTIM1->sTimerxRegs[1].CMP2xR = compare2;
}

void PwmGenerator::calculateSinusParameters(SinusZDomain_t *sinObj, float fi_deg, uint32_t freq_khz, uint32_t f_sampling_khz){

	sinObj->ff0 = sin((fi_deg/360.0f)*2*MATH_PI);
	sinObj->ff1 = sin(2*MATH_PI*freq_khz/f_sampling_khz - (fi_deg/360.0)*2*MATH_PI);
	sinObj->fb1 = 2*cos(2*MATH_PI*freq_khz / f_sampling_khz);
	sinObj->fb2 = -1;
}

float PwmGenerator::runSinusGeneratorFunction(SinusZDomain_t *sinObj){

	sinObj->r_n = sinObj->in + sinObj->fb1 * sinObj->r_n1 + sinObj->fb2 * sinObj->r_n2;
	sinObj->out = sinObj->ff0 * sinObj->r_n + sinObj->ff1 * sinObj->r_n1;

	sinObj->r_n2 = sinObj->r_n1;
	sinObj->r_n1 = sinObj->r_n;

	// Add offset voltage for Sinus output
	return sinObj->out;
}

bool PwmGenerator::prerequest(){

	return true;
}

bool PwmGenerator::postOperations(){

	return true;
}


