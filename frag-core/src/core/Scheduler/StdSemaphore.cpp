#include "Core/Threading/StdSemaphore.h"

using namespace fragcore;

stdSemaphore::stdSemaphore() {
	int rc = schCreateSemaphore(&this->semaphore);
	if (rc != SCH_OK)
		throw RuntimeException("Failed to create semaphore: {}", schErrorMsg(rc));
}

void stdSemaphore::lock() { int rc = schSemaphoreWait(this->semaphore);
	if (rc != SCH_OK)
		throw RuntimeException("Failed lock semaphore: {}", schErrorMsg(rc));
 }
void stdSemaphore::unlock() { int rc = schSemaphorePost(this->semaphore);
	if (rc != SCH_OK)
		throw RuntimeException("Failed to unlock semaphore: {}", schErrorMsg(rc));
 }
void stdSemaphore::wait(long int nanoTimeout) { int rc = schSemaphoreWait(this->semaphore);
	if (rc != SCH_OK)
		throw RuntimeException("Failed to wait semaphore: {}", schErrorMsg(rc));
 }