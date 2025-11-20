#include "Threading/StdSpinLock.h"
#include <cassert>
#include <cstdlib>

using namespace fragcore;
using schSpinLock = void; /*	Spinlock sync object.   */

// int schDeleteSpinLock(schSpinLock *spinlock) {
// 	int status = pthread_spin_destroy(spinlock);
// 	free(spinlock);
// 	return status;
// }

// int schLockSpinLock(schSpinLock *spinlock) { return pthread_spin_lock(spinlock); }

// int schTryLockSpinLock(schSpinLock *spinLock) { return pthread_spin_trylock(spinLock); }

// int schUnlockSpinLock(schSpinLock *spinlock) {
// 	return pthread_spin_unlock(spinlock); //== 0 ? SCH_OK : SCH_ERROR_UNKNOWN;
// }

StdSpinLock::StdSpinLock() {}

StdSpinLock::~StdSpinLock() {}

void StdSpinLock::lock() {}

void StdSpinLock::unlock() {}
void StdSpinLock::wait(long int nanoTimeout) {}

intptr_t StdSpinLock::getNativePtr() const { return reinterpret_cast<intptr_t>(this->spinlock); }
