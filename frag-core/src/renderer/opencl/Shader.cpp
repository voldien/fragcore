#include"Renderer/Shader.h"
using namespace fragcore;

void Shader::bind(void){
	
}

int Shader::getLocation(const char *cparamname){}

void Shader::setInt(int location, int value){}

void Shader::setFloat(int location, float value){}

void Shader::setFloatv(int location, int n, const float *values){}

void Shader::setVec2(int location, const float v1, const float v2){}

void Shader::setVec2v(int location, int n, const float *values){}

void Shader::setMatrix4f(int location, const float *pvalue){}

void Shader::setMatrix4fv(int location, int n, const float *pvalue){}

intptr_t Shader::getNativePtr(void) const {
	return 0;
}