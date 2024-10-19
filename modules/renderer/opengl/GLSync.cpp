#include "GLSync.h"
#include <GL/glew.h>

using namespace fragcore;

void GLSync::fence() { this->sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0); }

void GLSync::wait(int timeout) { glWaitSync((GLsync)this->sync, GL_SYNC_FLUSH_COMMANDS_BIT, timeout); }

Sync::SyncStatus GLSync::waitClient(int timeout) {
	GLenum status =
		glClientWaitSync((GLsync)this->sync, GL_SYNC_FLUSH_COMMANDS_BIT, static_cast<GLuint64>(timeout * 1000000));

	switch (status) {
	case GL_CONDITION_SATISFIED:
	case GL_ALREADY_SIGNALED:
		return Sync::SyncStatus::Complete;
	case GL_TIMEOUT_EXPIRED:
		return Sync::SyncStatus::TimeOutExpired;
	case GL_WAIT_FAILED:
		return Sync::SyncStatus::Error;
	default:
		return Sync::SyncStatus::Error;
	}
}

intptr_t GLSync::getNativePtr() const { return (intptr_t)this->sync; }