#include "Core/Threading/StdSpinLock.h"

using namespace fragcore;

StdSpinLock::StdSpinLock() {
	int rc = schCreateSpinLock(&this->spinlock);
	if (rc != SCH_OK)
		throw RuntimeException("Failed to create semaphore: {}", schErrorMsg(rc));
}

StdSpinLock::~StdSpinLock() {
	// int rc = schDeleteSemaphore(this->semaphore);
	// if (rc != SCH_OK)
	// 	throw RuntimeException("Failed to delete semaphore: {}", schErrorMsg(rc));
}

void StdSpinLock::lock() {
	// int rc = schSemaphoreWait(this->semaphore);
	// if (rc != SCH_OK)
	// 	throw RuntimeException("Failed lock semaphore: {}", schErrorMsg(rc));
}
void StdSpinLock::unlock() {
	// int rc = schSemaphorePost(this->semaphore);
	// if (rc != SCH_OK)
	// 	throw RuntimeException("Failed to unlock semaphore: {}", schErrorMsg(rc));
}
void StdSpinLock::wait(long int nanoTimeout) {
	// int rc = schSemaphoreWait(this->semaphore);
	// if (rc != SCH_OK)
	// 	throw RuntimeException("Failed to wait semaphore: {}", schErrorMsg(rc));
}

intptr_t StdSpinLock::getNativePtr() const { return reinterpret_cast<intptr_t>(this->spinlock); }
