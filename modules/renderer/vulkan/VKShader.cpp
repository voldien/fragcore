#include "VKShader.h"
#include "internal_object_type.h"
using namespace fragcore;

void VKShader::bind() {

	// VulkanCore* vulkanCore = shadobj->vulkanCore;

	//	shadobj->vulkanCore->current_pipeline = shadobj->graphicsPipeline;

	// const uint32_t curFrame = vulkanCore->currentFrame;
	// vkCmdBindPipeline(vulkanCore->swapChain->commandBuffers[curFrame], VK_PIPELINE_BIND_POINT_GRAPHICS,
	// shadobj->graphicsPipeline);
}

int VKShader::getLocation(const char *cparamname) {}

void VKShader::setInt(int location, int value) {}

void VKShader::setFloat(int location, float value) {}

void VKShader::setFloatv(int location, int n, const float *values) {}

void VKShader::setVec2(int location, const float v1, const float v2) {}
void VKShader::setVec2v(int location, int n, const float *values) {}

void VKShader::setMatrix4f(int location, const float *pvalue) {}

void VKShader::setMatrix4fv(int location, int n, const float *pvalue) {}

void VKShader::setName(const std::string &name) { Object::setName(name); }

void VKShader::getLocalGroupSize(int *sizes) {}

// TODO determine if relocate or not!
void *VKShader::getBinary(long int *size, unsigned int *format) { return nullptr; }

// virtual void* getBinaryShader(long int* size, unsigned int* fvformatf, int p);
void *VKShader::getSource(long int *size) { return nullptr; }

intptr_t VKShader::getNativePtr() const { return 0; }
// TODO rename to the shader version instead of program.

/*

void VKShader::bindUniformBuffer(const char* name, BufferObject* buffer){

	GLShaderObject* shadobj = (GLShaderObject*)pobj;
	GLBufferObject* glbuf =  (GLBufferObject*)buffer->getMappedObject();

	GLuint blockindex = glGetUniformBlockIndex(shadobj->program, name);
	glUniformBlockBinding(shadobj->program, blockindex, glbuf->base);

}
*/
