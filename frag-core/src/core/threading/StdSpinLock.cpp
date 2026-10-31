#include "Threading/StdSpinLock.h"
#include <cassert>
#include <cstdlib>
#ifdef FV_UNIX
#define FRAGCORE_PTHREAD
#include <pthread.h>
#endif

using namespace fragcore;
using schSpinLock = void; /*	Spinlock sync object.   */

StdSpinLock::StdSpinLock() {
#ifdef FRAGCORE_PTHREAD
	int status = pthread_spin_init(reinterpret_cast<pthread_spinlock_t *>(&spinlock), 0);
#endif
}

StdSpinLock::~StdSpinLock() {
#ifdef FRAGCORE_PTHREAD
	int status = pthread_spin_destroy(reinterpret_cast<pthread_spinlock_t *>(&spinlock));
#endif
}

void StdSpinLock::lock() {
#ifdef FRAGCORE_PTHREAD
	int status = pthread_spin_lock(reinterpret_cast<pthread_spinlock_t *>(&spinlock));
#endif
}

bool StdSpinLock::tryLock() noexcept {
#ifdef FRAGCORE_PTHREAD
	int status = pthread_spin_trylock(reinterpret_cast<pthread_spinlock_t *>(&spinlock));
	return status == 0;
#endif
}

void StdSpinLock::unlock() {
#ifdef FRAGCORE_PTHREAD
	int status = pthread_spin_unlock(reinterpret_cast<pthread_spinlock_t *>(&spinlock));
#endif
}
void StdSpinLock::wait(long int nanoTimeout) {
#ifdef FRAGCORE_PTHREAD
#endif
}

intptr_t StdSpinLock::getNativePtr() const { return this->spinlock; }
