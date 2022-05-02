#include "VKSync.h"
using namespace fragcore;

void VKSync::fence() {}

void VKSync::wait(int timeout) {}

Sync::SyncStatus VKSync::waitClient(int timeout) { return Sync::SyncStatus::Complete; }

intptr_t VKSync::getNativePtr() const { return 0; }