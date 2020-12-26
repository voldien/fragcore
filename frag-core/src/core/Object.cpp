#include"Core/Object.h"
using namespace fragcore;

UIDGenerator UIDObject::generator = UIDGenerator();

UIDObject::UIDObject(void) {
	this->setUID(generator.getNextUID());
}


void UIDObject::setUID(unsigned int uid) {
	this->uid = uid;
}

unsigned int UIDObject::getUID(void) const {
	return this->uid;
}