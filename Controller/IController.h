/*
 * IController.h
 *
 *  Created on: Jun 19, 2021
 *      Author: nazimyildiz
 */

#ifndef ICONTROLLER_H_
#define ICONTROLLER_H_

#include <stdint.h>

template <class TypeNumber>
class IController {
public:
	IController(uint16_t len_in, uint16_t len_out);
	virtual ~IController();

	virtual void controlAlgorithm() const = 0;

	virtual void setReference(TypeNumber ref);
	virtual TypeNumber getReference();

	virtual void setFeedback(TypeNumber fb);
	virtual TypeNumber getFeedback();

	virtual TypeNumber getDeviation();
	virtual TypeNumber getOutput();

protected:

	TypeNumber reference;
	TypeNumber feedback;
	TypeNumber deviation;
	TypeNumber output;

private:

};

#endif /* ICONTROLLER_H_ */
