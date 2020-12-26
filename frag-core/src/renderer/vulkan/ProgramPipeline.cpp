#include"Renderer/ProgramPipeline.h"
#include"Renderer/Shader.h"
#include"Renderer/vulkan/internal_object_type.h"
using namespace fragcore;

void ProgramPipeline::bind(void) {

}

int ProgramPipeline::getLocation(const char *cparamname) {

}

void ProgramPipeline::setInt(int location, int value) {

}

void ProgramPipeline::setFloat(int location, float value) {

}

void ProgramPipeline::setFloatv(int location, int n, const float *values) {

}

void ProgramPipeline::setVec2(int location, const float v1, const float v2) {

}

void ProgramPipeline::setVec2v(int location, int n, const float *values) {

}

void ProgramPipeline::setMatrix4f(int location, const float *pvalue) {

}

void ProgramPipeline::setMatrix4fv(int location, int n, const float *pvalue) {

}

Shader *ProgramPipeline::getShader(ShaderType type) {
}

void ProgramPipeline::setShader(ShaderType type, Shader *shader) {

}

intptr_t ProgramPipeline::getNativePtr(void) const {
	return 0;
}
