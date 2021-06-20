/*
 * IHardware.h
 *
 *  Created on: Jun 20, 2021
 *      Author: nazimyildiz
 */

#ifndef IHARDWARE_H_
#define IHARDWARE_H_

#include <string>
#include <stdint.h>

class IHardware {
public:
	IHardware();
	virtual ~IHardware();

	virtual bool hardwareDeinit() const = 0;
	virtual bool hardwareInit() const = 0;

	std::string getHardwareDescriber();
	uint16_t getHardwareId();

protected:

	virtual bool prerequest() const = 0;
	virtual void postOperations() const = 0;

	virtual static void hardwareIdGenerate();

private:

	std::string describer;
	uint16_t id;
};

#endif /* IHARDWARE_H_ */
