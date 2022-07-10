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

void GLCommandList::bindPipeline(RenderPipeline *p) {
	GLGraphicPipelineCommand *_command =
		(GLGraphicPipelineCommand *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLGraphicPipelineCommand>());
	*_command = GLGraphicPipelineCommand(p);
	this->commands.push_back(_command);
}

void GLCommandList::bindComputePipeline(RenderPipeline *pipeline) {
	GLComputePipelineCommand *_command =
		(GLComputePipelineCommand *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLComputePipelineCommand>());
	*_command = GLComputePipelineCommand(pipeline);
	this->commands.push_back(_command);
}

void GLCommandList::bindFramebuffer(Ref<FrameBuffer> &framebuffer) {
	GLBindFrameBufferCommand *_command =
		(GLBindFrameBufferCommand *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLBindFrameBufferCommand>());
	*_command = GLBindFrameBufferCommand(*framebuffer);
	this->commands.push_back(_command);
}

void GLCommandList::setViewport(int x, int y, unsigned int width, unsigned int height) {

	GLViewPortCommand *_command =
		(GLViewPortCommand *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLViewPortCommand>());
	*_command = GLViewPortCommand(0, x, y, width, height);

	/*	*/
	this->commands.push_back(_command);
}
void GLCommandList::setScissor(int x, int y, unsigned int width, unsigned int height) {
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

void GLCommandList::bindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount,
									  const std::vector<Ref<Buffer>> &buffer, const std::vector<size_t> pOffsets) {
	GLBindVertexBuffersCommand *_command = (GLBindVertexBuffersCommand *)this->stackAlloc.alloc(
		GLCommandBase::getCommandSize<GLBindVertexBuffersCommand>());

	this->commands.push_back(_command);
}
void GLCommandList::bindBindIndexBuffers(Ref<Buffer> &buffer, size_t offset, size_t indexType) {
	GLBindIndiceBufferCommand *_command =
		(GLBindIndiceBufferCommand *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLBindIndiceBufferCommand>());

	this->commands.push_back(_command);
}

void GLCommandList::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
	GLDrawCommand *_command = (GLDrawCommand *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLDrawCommand>());
	*_command = GLDrawCommand(vertexCount, instanceCount, firstVertex, firstInstance);
	this->commands.push_back(_command);
}
void GLCommandList::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
								uint32_t firstInstance) {
	GLDrawIndicesCommand *_command =
		(GLDrawIndicesCommand *)this->stackAlloc.alloc(GLCommandBase::getCommandSize<GLDrawIndicesCommand>());
	*_command = GLDrawIndicesCommand(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	this->commands.push_back(_command);
}
void GLCommandList::drawIndirect(Ref<Buffer> &buffer, size_t offset, uint32_t drawCount, uint32_t stride) {}
void GLCommandList::drawIndexedIndirect(Ref<Buffer> &buffer, size_t offset, uint32_t drawCount, uint32_t stride) {
	GLDrawIndexIndirectCommand *_command = (GLDrawIndexIndirectCommand *)this->stackAlloc.alloc(
		GLCommandBase::getCommandSize<GLDrawIndexIndirectCommand>());
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
