#include "GLCommandList.h"
#include "internal_object_type.h"
#include <GL/glew.h>

using namespace fragcore;
GLCommandList::GLCommandList() { this->commands.reserve(1000); }

GLCommandList::~GLCommandList() {}

void GLCommandList::begin() { commands.clear(); }
void GLCommandList::end() {}

void GLCommandList::copyTexture(const Texture *src, Texture *dst) {}

void GLCommandList::bindPipeline(RenderPipeline *p) {}
void GLCommandList::bindFramebuffer(Ref<FrameBuffer> &framebuffer) {}
void GLCommandList::setviewport(int x, int y, int width, int height) {
	GLViewPortCommand command;
	GLViewPortCommand *_command =
		(GLViewPortCommand *)this->stackAlloc.alloc(command.getCommandSize<GLViewPortCommand>());
	glViewport(x, y, width, height);

	/*	*/
	this->commands.push_back(_command);
}

void GLCommandList::clearDepth(float depth) {}
void GLCommandList::clearColorTarget(uint index, const Color &color) {}

void GLCommandList::dispatch(uint groupCountX, uint groupCountY, uint groupCountZ) {
	uint global[3] = {groupCountX, groupCountY, groupCountZ};
	this->compute->dispatchCompute(global, nullptr, 0);
}

void GLCommandList::dispatchIndirect(Buffer *buffer, u_int64_t offset) {

	// uint global[3] = {groupCountX, groupCountY, groupCountZ};
	// this->compute->dispatchCompute(global, nullptr, 0);
}

void GLCommandList::setDepthBounds(float min, float max) {}

void GLCommandList::pushDebugGroup(const char *name) {}
void GLCommandList::popDebugGroup() {}
void GLCommandList::insertDebugMarker(const char *name) {}