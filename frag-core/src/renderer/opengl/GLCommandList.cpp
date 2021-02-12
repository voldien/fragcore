#include "Renderer/opengl/GLCommandList.h"
#include "Renderer/opengl/internal_object_type.h"
#include <Exception/InvalidArgumentException.h>
#include <GL/glew.h>
#include <Renderer/opengl/internal_object_type.h>
#include <Utils/StringUtil.h>
#include <stdexcept>

using namespace fragcore;

GLCommandList::~GLCommandList(void){

}

void GLCommandList::begin(void){
	
}
void GLCommandList::end(void){

}

void GLCommandList::bindPipeline(RenderPipeline *p)
{
}
void GLCommandList::bindFramebuffer(Ref<FrameBuffer> &framebuffer)
{
}

void GLCommandList::dispatch(uint groupCountX, uint groupCountY, uint groupCountZ) {
	uint global[3] = {groupCountX, groupCountY, groupCountZ};
	this->compute->dispatchCompute(global, NULL, 0);
}

void GLCommandList::dispatchIndirect(Buffer* buffer, u_int64_t offset)
{

	//uint global[3] = {groupCountX, groupCountY, groupCountZ};
	//this->compute->dispatchCompute(global, NULL, 0);
}

void GLCommandListpushDebugGroup(const char *name)
{
	
}
void GLCommandListpopDebugGroup(void)
{

}
void GLCommandListinsertDebugMarker(const char *name)
{

}