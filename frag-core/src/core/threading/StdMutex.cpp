#include "Threading/StdMutex.h"
#include <mutex>

using namespace fragcore;

StdMutex::StdMutex() = default;

void StdMutex::lock() { this->m.lock(); }
void StdMutex::unlock() { this->m.unlock(); }
void StdMutex::wait(long int nanoTimeout) {

}