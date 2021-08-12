#include "Core/Object.h"
using namespace fragcore;

UIDGenerator UIDObject::generator = UIDGenerator();

void UIDObject::setUID(unsigned int uid) { this->uid = uid; }

unsigned int UIDObject::getUID(void) const { return this->uid; }