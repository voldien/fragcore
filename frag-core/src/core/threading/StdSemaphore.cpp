#include "Threading/StdSemaphore.h"

using namespace fragcore;

// typedef void schSemaphore;	 /*	Semaphore sync object.  */

// static int schCreateSemaphore(schSemaphore **pSemaphore) {

// 	sem_t *sem = (sem_t *)malloc(sizeof(sem_t));
// 	*pSemaphore = (schSemaphore *)sem;
// 	assert(sem);

// 	if (sem_init(sem, 0, 0) == 0) {
// 		return SCH_OK;
// 	} else {
// 		return pthread_error_code2sch_error_code(errno);
// 	}
// }

// static int schDeleteSemaphore(schSemaphore *pSemaphore) {
// 	sem_t *sem = pSemaphore;
// 	int status = pthread_error_code2sch_error_code(sem_destroy(sem));
// 	free(pSemaphore);
// 	return status;
// }

stdSemaphore::stdSemaphore() {}

stdSemaphore::~stdSemaphore() {}

void stdSemaphore::lock() {}
void stdSemaphore::unlock() {}
void stdSemaphore::wait(long int nanoTimeout) {}

intptr_t stdSemaphore::getNativePtr() const { return reinterpret_cast<intptr_t>(this->semaphore); }
