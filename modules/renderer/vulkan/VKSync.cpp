#include "VKSync.h"
using namespace fragcore;

void VKSync::fence() {}

void VKSync::wait(int timeout) {}

Sync::SyncStatus VKSync::waitClient(int timeout) {}

intptr_t VKSync::getNativePtr() const {}