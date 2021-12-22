#include "GLCommandList.h"
#include "internal_object_type.h"
#include <GL/glew.h>

using namespace fragcore;
GLCommandList::GLCommandList() {
	this->commands.reserve(1024);
	stackAlloc = StackAllocator(1024);
}

GLCommandList::~GLCommandList() {}

void GLCommandList::begin() { commands.clear(); }
void GLCommandList::end() {}

void GLCommandList::copyTexture(const Texture *src, Texture *dst) {}

void GLCommandList::bindPipeline(RenderPipeline *p) {}
void GLCommandList::bindFramebuffer(Ref<FrameBuffer> &framebuffer) {
	GLBindFrameBufferCommand *_command =
		(GLBindFrameBufferCommand *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLBindFrameBufferCommand>());
	*_command = GLBindFrameBufferCommand(*framebuffer);
	this->commands.push_back(_command);
}
void GLCommandList::setViewport(int x, int y, int width, int height) {

	GLViewPortCommand *_command =
		(GLViewPortCommand *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLViewPortCommand>());
	*_command = GLViewPortCommand(0, x, y, width, height);

	/*	*/
	this->commands.push_back(_command);
}
void GLCommandList::setScissor(int x, int y, int width, int height) {
	GLScissorPortCommand *_command =
		(GLScissorPortCommand *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLViewPortCommand>());
	*_command = GLScissorPortCommand(0, x, y, width, height);
	this->commands.push_back(_command);
}

void GLCommandList::clearDepth(float depth) {}

void GLCommandList::clearColorTarget(uint index, const Color &color) {

	GLCommandClearColor *_command =
		(GLCommandClearColor *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLCommandClearColor>());
	*_command = GLCommandClearColor(index, color);

	/*	*/
	this->commands.push_back(_command);
}

void GLCommandList::dispatch(uint groupCountX, uint groupCountY, uint groupCountZ) {
	GLCommandDispatch *_command =
		(GLCommandDispatch *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLCommandDispatch>());
	*_command = GLCommandDispatch(groupCountX, groupCountY, groupCountZ);
	this->commands.push_back(_command);
}

void GLCommandList::dispatchIndirect(Buffer *buffer, u_int64_t offset) {
	GLCommandDispatchIndirect *_command =
		(GLCommandDispatchIndirect *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLCommandDispatchIndirect>());
	*_command = GLCommandDispatchIndirect(buffer, offset);
	this->commands.push_back(_command);
}

void GLCommandList::setDepthBounds(float min, float max) {}

void GLCommandList::pushDebugGroup(const char *name) {
	GLPushGroupMarkerCommand *_command =
		(GLPushGroupMarkerCommand *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLPushGroupMarkerCommand>());
	this->commands.push_back(_command);
}
void GLCommandList::popDebugGroup() {
	GLPopGroupMarkerCommand *_command =
		(GLPopGroupMarkerCommand *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLPopGroupMarkerCommand>());
		
	this->commands.push_back(_command);
}
void GLCommandList::insertDebugMarker(const char *name) {
	GLInsertGroupMarkerCommand *_command =
		(GLInsertGroupMarkerCommand *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLPopGroupMarkerCommand>());


	this->commands.push_back(_command);
}

void GLCommandList::draw(Ref<Buffer> &buffer, uint32_t vertexCount, uint32_t instanceCount) {}
