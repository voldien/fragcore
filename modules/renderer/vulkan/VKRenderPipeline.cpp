#include "internal_object_type.h"
#include <../RenderPipeline.h>
#include <../Shader.h>
using namespace fragcore;

void RenderPipeline::bind() {}

int RenderPipeline::getLocation(const char *cparamname) {}

void RenderPipeline::setInt(int location, int value) {}

void RenderPipeline::setFloat(int location, float value) {}

void RenderPipeline::setFloatv(int location, int n, const float *values) {}

void RenderPipeline::setVec2(int location, const float v1, const float v2) {}

void RenderPipeline::setVec2v(int location, int n, const float *values) {}

void RenderPipeline::setMatrix4f(int location, const float *pvalue) {}

void RenderPipeline::setMatrix4fv(int location, int n, const float *pvalue) {}

Shader *RenderPipeline::getShader(ShaderType type) {}

void RenderPipeline::setShader(ShaderType type, Shader *shader) {}

intptr_t RenderPipeline::getNativePtr() const { return 0; }
