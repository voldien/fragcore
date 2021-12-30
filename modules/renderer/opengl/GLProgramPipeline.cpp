#include "../Shader.h"
#include "GLRenderPipeline.h"
#include "internal_object_type.h"
#include <GL/glew.h>
using namespace fragcore;

void GLRenderPipeline::bind() {

	glBindProgramPipeline(this->program);
	// glActiveShaderProgram
}

int GLRenderPipeline::getLocation(const char *name) {

	return glGetUniformLocation(this->program, name);
}

void GLRenderPipeline::setInt(int location, int value) {

	glProgramUniform1i(this->program, location, value);
}

void GLRenderPipeline::setFloat(int location, float value) {

	glProgramUniform1f(this->program, location, value);
}

void GLRenderPipeline::setFloatv(int location, int n, const float *values) {

	glProgramUniform1fv(this->program, location, n, values);
}

void GLRenderPipeline::setVec2(int location, const float v1, const float v2) {

	glProgramUniform2f(this->program, location, v1, v2);
}

void GLRenderPipeline::setVec2v(int location, int n, const float *values) {
	glProgramUniform2fv(this->program, location, n, values);
}

void GLRenderPipeline::setMatrix4f(int location, const float *pvalue) {

	glProgramUniformMatrix4fv(this->program, location, 1, GL_FALSE, pvalue);
}

void GLRenderPipeline::setMatrix4fv(int location, int n, const float *pvalue) {

	glProgramUniformMatrix4fv(this->program, location, n, GL_FALSE, pvalue);
}

static GLenum getShaderFlag(GLRenderPipeline::ShaderType type) {
	GLenum flag = 0;

	if (type & GLRenderPipeline::VERTEX_SHADER)
		flag |= GL_VERTEX_SHADER_BIT;
	if (type & GLRenderPipeline::FRAGMENT_SHADER)
		flag |= GL_FRAGMENT_SHADER_BIT;
	if (type & GLRenderPipeline::GEOMETRY_SHADER)
		flag |= GL_GEOMETRY_SHADER_BIT;
	if (type & GLRenderPipeline::TESSELLATION_CONTROL_SHADER)
		flag |= GL_TESS_CONTROL_SHADER_BIT;
	if (type & GLRenderPipeline::TESSELLATION_EVOLUTION_SHADER)
		flag |= GL_TESS_EVALUATION_SHADER_BIT;
	if (type & GLRenderPipeline::COMPUTE_SHADER)
		flag |= GL_COMPUTE_SHADER_BIT;

	return flag;
}

Shader *GLRenderPipeline::getShader(ShaderType type) {


	// switch (type) {
	// case VERTEX_SHADER:
	// 	return this->v;
	// case FRAGMENT_SHADER:
	// 	return this->f;
	// case GEOMETRY_SHADER:
	// 	return this->g;
	// case TESSELLATION_CONTROL_SHADER:
	// 	return this->tc;
	// case TESSELLATION_EVOLUTION_SHADER:
	// 	return this->te;
	// case COMPUTE_SHADER:
	// 	return this->c;
	// default:
	// 	assert(0);
	// }

	return nullptr;
}

void GLRenderPipeline::setShader(ShaderType type, Shader *shader) {
	// GLProgramPipeline *this = (GLProgramPipeline *)this->pdata;
	// GLShaderObject *glShader = (GLShaderObject *)shader->getObject();

	// /*  Get flag.   */
	// GLenum shaderflag = getShaderFlag(type);

	// /*  Update new shader.  */
	// glUseProgramStages(this->program, shaderflag, glShader->program);

	// /*  Update shader reference. */
	// if (type & GLRenderPipeline::VERTEX_SHADER)
	// 	this->v = shader;
	// if (type & GLRenderPipeline::FRAGMENT_SHADER)
	// 	this->f = shader;
	// if (type & GLRenderPipeline::GEOMETRY_SHADER)
	// 	this->g = shader;
	// if (type & GLRenderPipeline::TESSELLATION_CONTROL_SHADER)
	// 	this->tc = shader;
	// if (type & GLRenderPipeline::TESSELLATION_EVOLUTION_SHADER)
	// 	this->te = shader;
	// if (type & GLRenderPipeline::COMPUTE_SHADER)
	// 	this->c = shader;
}

intptr_t GLRenderPipeline::getNativePtr() const { return this->program; }
