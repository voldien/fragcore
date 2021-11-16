#include "Core/Threading/StdSemaphore.h"

using namespace fragcore;

stdSemaphore::stdSemaphore() {
	int error = schCreateSemaphore(&this->semaphore);
	if (error != SCH_OK)
		throw RuntimeException(fmt::format("Failed to create semaphore {}", schErrorMsg(error)));
}

void stdSemaphore::lock() { int rc = schSemaphoreWait(this->semaphore); }
void stdSemaphore::unlock() { int rc = schSemaphorePost(this->semaphore); }
void stdSemaphore::wait(long int nanoTimeout) { int rc = schSemaphoreWait(this->semaphore); }