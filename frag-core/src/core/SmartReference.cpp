#include"Core/SmartReference.h"
using namespace fragcore;

SmartReference::SmartReference(void) noexcept { this->counter.init(1); }

// SmartReference::SmartReference(SmartReference &&other) {

// }

bool SmartReference::increment(void) noexcept { return this->counter.ref(); }

bool SmartReference::deincreemnt(void) noexcept {
	bool status = this->counter.unRef();

	return status;
}

int SmartReference::getIndexCount(void) const noexcept { return this->counter.get(); }
