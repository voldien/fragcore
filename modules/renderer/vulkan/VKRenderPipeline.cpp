#include "VKRenderPipeline.h"
#include "internal_object_type.h"
using namespace fragcore;

VKRenderPipeline ::~VKRenderPipeline() {}

void VKRenderPipeline::bind() {}

int VKRenderPipeline::getLocation(const char *cparamname) { return 0; }

void VKRenderPipeline::setInt(int location, int value) {}

void VKRenderPipeline::setFloat(int location, float value) {}

void VKRenderPipeline::setFloatv(int location, int n, const float *values) {}

void VKRenderPipeline::setVec2(int location, const float v1, const float v2) {}

void VKRenderPipeline::setVec2v(int location, int n, const float *values) {}

void VKRenderPipeline::setMatrix4f(int location, const float *pvalue) {}

void VKRenderPipeline::setMatrix4fv(int location, int n, const float *pvalue) {}

Shader *VKRenderPipeline::getShader(ShaderType type) { return nullptr; }

void VKRenderPipeline::setShader(ShaderType type, Shader *shader) {}

intptr_t VKRenderPipeline::getNativePtr() const { return 0; }
