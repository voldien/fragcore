#include "Core/Threading/StdMutex.h"
#include <mutex>
#include <thread>

using namespace fragcore;

StdMutex::StdMutex() {}

void StdMutex::lock() { this->m.lock(); }
void StdMutex::unlock() { this->m.unlock(); }
void StdMutex::wait(long int nanoTimeout) {

}