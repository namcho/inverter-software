/*
 * leds_init.c
 *
 *  Created on: Jun 19, 2021
 *      Author: nazimyildiz
 */

#include "../HardwareInit/leds_init.h"

#include <stm32f334x8.h>

void ledsInit(){

	// LD3 -> PB6
	RCC->AHBENR |= (1 << 18);		// Clock enable for PortB
	GPIOB->MODER &= ~(3 << 12);		// Reset bit12 and bit13
	GPIOB->MODER |= (1 << 12);		// Set PB.6 as output
	GPIOB->OTYPER &= ~(1 << 6);		// Output type push-pull
	GPIOB->OSPEEDR &= ~(3 << 12);	// Reset bit12 and bit13
	GPIOB->OSPEEDR |= (3 << 12);	// PB.6 in full speed mode
	// LD4 -> PB8

	// LD5 -> PB9

	// LD6 -> PB7


}


