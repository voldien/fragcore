#include "Threading/StdSpinLock.h"

using namespace fragcore;

StdSpinLock::StdSpinLock() {
	int rcode = schCreateSpinLock(&this->spinlock);
	if (rcode != SCH_OK) {
		throw RuntimeException("Failed to create spinlock: {}", schErrorMsg(rcode));
	}
}

StdSpinLock::~StdSpinLock() { schDeleteSpinLock(this->spinlock); }

void StdSpinLock::lock() {

	int rcode = schLockSpinLock(this->spinlock);
	if (rcode != SCH_OK) {
		throw RuntimeException("Failed lock spinlock: {}", schErrorMsg(rcode));
	}
}
void StdSpinLock::unlock() {

	int rcode = schUnlockSpinLock(this->spinlock);
	if (rcode != SCH_OK) {
		throw RuntimeException("Failed to unlock spinlock: {}", schErrorMsg(rcode));
	}
}
void StdSpinLock::wait(long int nanoTimeout) {}

intptr_t StdSpinLock::getNativePtr() const { return reinterpret_cast<intptr_t>(this->spinlock); }
