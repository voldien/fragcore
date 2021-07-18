#include "GLCommandList.h"
#include "internal_object_type.h"
#include <Exception/InvalidArgumentException.h>
#include <GL/glew.h>
#include <Utils/StringUtil.h>
#include <stdexcept>

using namespace fragcore;

GLCommandList::~GLCommandList(void) {}

void GLCommandList::begin(void) {}
void GLCommandList::end(void) {}

void GLCommandList::copyTexture(const Texture *src, Texture *dst) {}

void GLCommandList::bindPipeline(RenderPipeline *p) {}
void GLCommandList::bindFramebuffer(Ref<FrameBuffer> &framebuffer) {}
void GLCommandList::setviewport(int x, int y, int width, int height) {}

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
void GLCommandList::popDebugGroup(void) {}
void GLCommandList::insertDebugMarker(const char *name) {}