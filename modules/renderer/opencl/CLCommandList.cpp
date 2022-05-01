#include "CLCommandList.h"
#include "internal_object_type.h"

using namespace fragcore;

cl_command_queue createCommandQueue(cl_context context, cl_device_id device) {

	cl_int ciErrNum;
	cl_command_queue queue;
	cl_command_queue_properties pro = 0;

	/*  Create command.	*/
	queue = clCreateCommandQueue(context, device, pro, &ciErrNum);
	/*  Check error.    */
	if (ciErrNum != CL_SUCCESS) {
		throw RuntimeException("failed to create command queue - {}", getCLStringError(ciErrNum));
	}

	return queue;
}

CLCommandList::GLCommCLCommandListandList() {}

CLCommandList::~CLCommandList() {}

void CLCommandList::begin() { commands.clear(); }
void CLCommandList::end() {}

void CLCommandList::copyTexture(const Texture *src, Texture *dst) {}

void CLCommandList::bindPipeline(RenderPipeline *p) {}
void CLCommandList::bindComputePipeline(RenderPipeline *pipeline) {}

void CLCommandList::bindFramebuffer(Ref<FrameBuffer> &framebuffer) {}
void CLCommandList::setViewport(int x, int y, unsigned int width, unsigned int height) {}
void CLCommandList::setScissor(int x, int y, unsigned int width, unsigned int height) {}

void CLCommandList::clearDepth(float depth) {}

void CLCommandList::clearColorTarget(uint index, const Color &color) {}

void CLCommandList::dispatch(uint groupCountX, uint groupCountY, uint groupCountZ) {}

void CLCommandList::dispatchIndirect(Buffer *buffer, u_int64_t offset) {}

void CLCommandList::setDepthBounds(float min, float max) {}

void CLCommandList::pushDebugGroup(const char *name) {}
void CLCommandList::popDebugGroup() {}
void CLCommandList::insertDebugMarker(const char *name) {}

void CLCommandList::draw(Ref<Buffer> &buffer, uint32_t vertexCount, uint32_t instanceCount) {}
