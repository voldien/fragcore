#include "Core/SmartReference.h"
using namespace fragcore;

SmartReference::SmartReference() noexcept { this->counter.init(1); }

bool SmartReference::increment() noexcept { return this->counter.ref(); }

bool SmartReference::deincreemnt() noexcept {
	bool status = this->counter.unRef();

	return status;
}

uint32_t SmartReference::getIndexCount() const noexcept { return this->counter.get(); }
