#include "Core/Object.h"
using namespace fragcore;

UIDGenerator<unsigned int> UIDObject::generator = UIDGenerator<unsigned int>();

void UIDObject::setUID(unsigned int uid) { this->uid = uid; }

unsigned int UIDObject::getUID() const { return this->uid; }