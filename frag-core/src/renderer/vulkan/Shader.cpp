#include"Renderer/Shader.h"
#include"Renderer/vulkan/internal_object_type.h"
using namespace fragcore;

void Shader::bind(void) {
	VKShaderObject  *shadobj = (VKShaderObject *) this->pdata;
	VulkanCore* vulkanCore = shadobj->vulkanCore;


	shadobj->vulkanCore->current_pipeline = shadobj->graphicsPipeline;

	const uint32_t curFrame = vulkanCore->currentFrame;
	vkCmdBindPipeline(vulkanCore->swapChain->commandBuffers[curFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, shadobj->graphicsPipeline);
}

int Shader::getLocation(const char *cparamname) {
	VKShaderObject  *shadobj = (VKShaderObject *) this->pdata;

}


void Shader::setInt(int location, int value) {
	VKShaderObject *shadobj = (VKShaderObject *) this->pdata;

}

void Shader::setFloat(int location, float value) {
	VKShaderObject *shadobj = (VKShaderObject *) this->pdata;

}

void Shader::setFloatv(int location, int n, const float* values){
	VKShaderObject *shadobj = (VKShaderObject *) this->pdata;

}

void Shader::setVec2(int location, const float v1, const float v2){
	VKShaderObject *shadobj = (VKShaderObject *) this->pdata;

}
void Shader::setVec2v(int location, int n, const float* values){
	VKShaderObject *shadobj = (VKShaderObject *) this->pdata;

}

void Shader::setMatrix4f(int location, const float *pvalue) {
	VKShaderObject *shadobj = (VKShaderObject *) this->pdata;

}

void Shader::setMatrix4fv(int location, int n, const float *pvalue) {
	VKShaderObject *shadobj = (VKShaderObject *) this->pdata;

}

void Shader::setName(const std::string &name) {
	VKShaderObject *shadobj = (VKShaderObject *) this->pdata;
	Object::setName(name);
}

void Shader::getLocalGroupSize(int *sizes){}

//TODO determine if relocate or not!
void *Shader::getBinary(long int *size, unsigned int *format){}

//virtual void* getBinaryShader(long int* size, unsigned int* fvformatf, int p);
void *Shader::getSource(long int *size){}

intptr_t Shader::getNativePtr(void) const {
	return 0;
}
//TODO rename to the shader version instead of program.

/*

void Shader::bindUniformBuffer(const char* name, BufferObject* buffer){

	GLShaderObject* shadobj = (GLShaderObject*)pobj;
	GLBufferObject* glbuf =  (GLBufferObject*)buffer->getMappedObject();

	GLuint blockindex = glGetUniformBlockIndex(shadobj->program, name);
	glUniformBlockBinding(shadobj->program, blockindex, glbuf->base);

}
*/

