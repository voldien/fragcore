#include "Threading/StdSpinLock.h"
#include <cassert>
#include <cstdlib>
#ifdef FV_UNIX
#define FRAGCORE_PTHREAD
#include <pthread.h>
#endif

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

StdSpinLock::~StdSpinLock() {
#ifdef FRAGCORE_PTHREAD
	int status = pthread_spin_destroy(static_cast<pthread_spinlock_t *>(spinlock));
#endif
}

void StdSpinLock::lock() {
#ifdef FRAGCORE_PTHREAD
	int status = pthread_spin_unlock(static_cast<pthread_spinlock_t *>(spinlock));
#endif
}

void StdSpinLock::unlock() {
#ifdef FRAGCORE_PTHREAD
	int status = pthread_spin_unlock(static_cast<pthread_spinlock_t *>(spinlock));
#endif
}
void StdSpinLock::wait(long int nanoTimeout) {
#ifdef FRAGCORE_PTHREAD
#endif
}

intptr_t StdSpinLock::getNativePtr() const { return reinterpret_cast<intptr_t>(this->spinlock); }
