#include "Core/Threading/StdSpinLock.h"

using namespace fragcore;

StdSpinLock::StdSpinLock() {
	int rc = schCreateSpinLock(&this->spinlock);
	if (rc != SCH_OK)
		throw RuntimeException("Failed to create spinlock: {}", schErrorMsg(rc));
}

StdSpinLock::~StdSpinLock() {
	int rc = schDeleteSpinLock(this->spinlock);
	if (rc != SCH_OK) {
		throw RuntimeException("Failed to delete spinlock: {}", schErrorMsg(rc));
	}
}

void StdSpinLock::lock() {

	int rc = schLockSpinLock(this->spinlock);
	if (rc != SCH_OK) {
		throw RuntimeException("Failed lock spinlock: {}", schErrorMsg(rc));
	}
}
void StdSpinLock::unlock() {

	int rc = schUnlockSpinLock(this->spinlock);
	if (rc != SCH_OK) {
		throw RuntimeException("Failed to unlock spinlock: {}", schErrorMsg(rc));
	}
}
void StdSpinLock::wait(long int nanoTimeout) {}

intptr_t StdSpinLock::getNativePtr() const { return reinterpret_cast<intptr_t>(this->spinlock); }
