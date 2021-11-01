#include "../Sync.h"
using namespace fragcore;

void Sync::fence() {}

void Sync::wait(int timeout) {}

Sync::SyncStatus Sync::waitClient(int timeout) {}

intptr_t Sync::getNativePtr() const {}