/*
 * IController.cpp
 *
 *  Created on: Jun 19, 2021
 *      Author: nazimyildiz
 */

#include "IController.h"

template <class TypeNumber>
IController<TypeNumber>::IController() {
	// TODO Auto-generated constructor stub

}

template <class TypeNumber>
IController<TypeNumber>::~IController() {
	// TODO Auto-generated destructor stub
}

template <class TypeNumber>
void IController<TypeNumber>::setReference(TypeNumber ref){

	this->reference = ref;
}

template <class TypeNumber>
TypeNumber IController<TypeNumber>::getReference(){

	return this->reference;
}

template <class TypeNumber>
void IController <TypeNumber>::setFeedback(TypeNumber fb){

	this->feedback = fb;
}

template <class TypeNumber>
TypeNumber IController<TypeNumber>::getFeedback(){

	return this->feedback;
}

template <class TypeNumber>
TypeNumber IController<TypeNumber>::getDeviation(){

	return this->deviation;
}

template <class TypeNumber>
TypeNumber IController<TypeNumber>::getOutput(){

	this->output;
}
