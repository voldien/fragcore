#include "Renderer/Sync.h"
#include "internal_object_type.h"
#include <GL/glew.h>
using namespace fragcore;

void Sync::fence(void) {
	GLSync *glSync = (GLSync *)this->getObject();
	glSync->sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void Sync::wait(int timeout) {
	GLSync *glSync = (GLSync *)this->getObject();
	glWaitSync((GLsync)glSync->sync, GL_SYNC_FLUSH_COMMANDS_BIT, timeout);
}

Sync::SyncStatus Sync::waitClient(int timeout) {
	GLSync *glSync = (GLSync *)this->getObject();
	GLenum status = glClientWaitSync((GLsync)glSync->sync, GL_SYNC_FLUSH_COMMANDS_BIT, timeout * 1000000);

	switch (status) {
	case GL_CONDITION_SATISFIED:
	case GL_ALREADY_SIGNALED:
		return Complete;
	case GL_TIMEOUT_EXPIRED:
		return TimeOutExpired;
	case GL_WAIT_FAILED:
		return Error;
	}
}

intptr_t Sync::getNativePtr(void) const {
	GLSync *glSync = (GLSync *)this->getObject();
	return (intptr_t)glSync->sync;
}