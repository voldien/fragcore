#include "Threading/StdSemaphore.h"

using namespace fragcore;

stdSemaphore::stdSemaphore() {
	const int result_code = schCreateSemaphore(&this->semaphore);
	if (result_code != SCH_OK) {
		throw RuntimeException("Failed to create semaphore: {}", schErrorMsg(result_code));
	}
}

stdSemaphore::~stdSemaphore() { schDeleteSemaphore(this->semaphore); }

void stdSemaphore::lock() {
	const int result_code = schSemaphoreWait(this->semaphore);
	if (result_code != SCH_OK) {
		throw RuntimeException("Failed lock semaphore: {}", schErrorMsg(result_code));
	}
}
void stdSemaphore::unlock() {
	const int result_code = schSemaphorePost(this->semaphore);
	if (result_code != SCH_OK) {
		throw RuntimeException("Failed to unlock semaphore: {}", schErrorMsg(result_code));
	}
}
void stdSemaphore::wait(long int nanoTimeout) {
	const int result_code = schSemaphoreWait(this->semaphore);
	if (result_code != SCH_OK) {
		throw RuntimeException("Failed to wait semaphore: {}", schErrorMsg(result_code));
	}
}

intptr_t stdSemaphore::getNativePtr() const { return reinterpret_cast<intptr_t>(this->semaphore); }
