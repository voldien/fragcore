#include"Core/SmartReference.h"
using namespace fragcore;

SmartReference::SmartReference(void) {
	this->counter.init(1);
}

bool SmartReference::increment(void) {
	return this->counter.ref();
}

bool SmartReference::deincreemnt(void) {
	bool status = this->counter.unRef();

	return status;
}

int SmartReference::getIndexCount(void) const {
	return this->counter.get();
}

