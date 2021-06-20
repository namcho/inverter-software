/*
 * IHardware.cpp
 *
 *  Created on: Jun 20, 2021
 *      Author: nazimyildiz
 */

#include "../HardwareInit/IHardware.h"

IHardware::IHardware() {
	// TODO Auto-generated constructor stub

}

IHardware::~IHardware() {
	// TODO Auto-generated destructor stub
}

std::string IHardware::getHardwareDescriber(){

	return this->describer;
}

uint16_t IHardware::getHardwareId(){

	return objectCount;
}

bool IHardware::prerequest(){

	return true;
}

bool IHardware::postOperations(){

	return true;
}

void IHardware::hardwareIdGenerate(){

}
