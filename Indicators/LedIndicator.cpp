/*
 * LedIndicator.cpp
 *
 *  Created on: Jun 20, 2021
 *      Author: nazimyildiz
 */

#include "LedIndicator.h"
#include <stm32f334x8.h>

LedIndicator::LedIndicator() {
	// TODO Auto-generated constructor stub

	this->hardwareIdGenerate();

}

LedIndicator::~LedIndicator() {
	// TODO Auto-generated destructor stub
}

bool LedIndicator::hardwareDeinit(){

	return true;
}

bool LedIndicator::hardwareInit(){

	// LD3 -> PB6
	RCC->AHBENR |= (1 << 18);		// Clock enable for PortB
	GPIOB->MODER &= ~(3 << 12);		// Reset bit12 and bit13
	GPIOB->MODER |= (1 << 12);		// Set PB.6 as output
	GPIOB->OTYPER &= ~(1 << 6);		// Output type push-pull
	GPIOB->OSPEEDR &= ~(3 << 12);	// Reset bit12 and bit13
	GPIOB->OSPEEDR |= (3 << 12);	// PB.6 in full speed mode

	// LD4 -> PB8
	GPIOB->MODER &= (3 << 16);		// Reset bit16 and bit17
	GPIOB->MODER |= (1 << 16);		// PB.8 output
	GPIOB->OTYPER &= ~(1 << 8);		// Push-pull
	GPIOB->OSPEEDR &= (3 << 16);
	GPIOB->OSPEEDR |= (3 << 16);	// PB.8 in full speed mode

	// LD5 -> PB9
	GPIOB->MODER &= (3 << 18);
	GPIOB->MODER |= (1 << 18);		// PB.9 output
	GPIOB->OTYPER &= ~(1 << 9);		// Push-pull
	GPIOB->OSPEEDR &= ~(3 << 18);
	GPIOB->OSPEEDR |= (3 << 18);	// Full speed

	// LD6 -> PB7
	GPIOB->MODER &= (3 << 14);
	GPIOB->MODER |= (1 << 14);
	GPIOB->OTYPER &= ~(1 << 7);
	GPIOB->OSPEEDR &= ~(3 << 14);
	GPIOB->OSPEEDR |= (3 << 14);


	return true;
}
