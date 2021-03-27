#include "Renderer/RenderPipeline.h"
#include"Renderer/Shader.h"
#include"Renderer/opengl/internal_object_type.h"
#include<GL/glew.h>
using namespace fragcore;

void RenderPipeline::bind(void) {
	GLProgramPipeline *pipeline = (GLProgramPipeline *) this->pdata;
	glBindProgramPipeline(pipeline->program);
	//glActiveShaderProgram
}

int RenderPipeline::getLocation(const char *name) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	return glGetUniformLocation(shadobj->program, name);
}

void RenderPipeline::setInt(int location, int value) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniform1i(shadobj->program, location, value);
}

void RenderPipeline::setFloat(int location, float value) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniform1f(shadobj->program, location, value);
}

void RenderPipeline::setFloatv(int location, int n, const float *values) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniform1fv(shadobj->program, location, n, values);
}

void RenderPipeline::setVec2(int location, const float v1, const float v2) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniform2f(shadobj->program, location, v1, v2);
}

void RenderPipeline::setVec2v(int location, int n, const float *values) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniform2fv(shadobj->program, location, n, values);
}

void RenderPipeline::setMatrix4f(int location, const float *pvalue) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniformMatrix4fv(shadobj->program, location, 1, GL_FALSE, pvalue);
}

void RenderPipeline::setMatrix4fv(int location, int n, const float *pvalue) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniformMatrix4fv(shadobj->program, location, n, GL_FALSE, pvalue);
}

static GLenum getShaderFlag(RenderPipeline::ShaderType type) {
	GLenum flag = 0;

	if (type & RenderPipeline::VERTEX_SHADER)
		flag |= GL_VERTEX_SHADER_BIT;
	if (type & RenderPipeline::FRAGMENT_SHADER)
		flag |= GL_FRAGMENT_SHADER_BIT;
	if (type & RenderPipeline::GEOMETRY_SHADER)
		flag |= GL_GEOMETRY_SHADER_BIT;
	if (type & RenderPipeline::TESSELLATION_CONTROL_SHADER)
		flag |= GL_TESS_CONTROL_SHADER_BIT;
	if (type & RenderPipeline::TESSELLATION_EVOLUTION_SHADER)
		flag |= GL_TESS_EVALUATION_SHADER_BIT;
	if (type & RenderPipeline::COMPUTE_SHADER)
		flag |= GL_COMPUTE_SHADER_BIT;

	return flag;
}

Shader *RenderPipeline::getShader(ShaderType type) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;

	switch (type) {
		case VERTEX_SHADER:
			return shadobj->v;
		case FRAGMENT_SHADER:
			return shadobj->f;
		case GEOMETRY_SHADER:
			return shadobj->g;
		case TESSELLATION_CONTROL_SHADER:
			return shadobj->tc;
		case TESSELLATION_EVOLUTION_SHADER:
			return shadobj->te;
		case COMPUTE_SHADER:
			return shadobj->c;
		default:
			assert(0);
	}

	return nullptr;
}

void RenderPipeline::setShader(ShaderType type, Shader *shader) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	GLShaderObject *glShader = (GLShaderObject *) shader->getObject();

	/*  Get flag.   */
	GLenum shaderflag = getShaderFlag(type);

	/*  Update new shader.  */
	glUseProgramStages(shadobj->program, shaderflag, glShader->program);

	/*  Update shader reference. */
	if (type & RenderPipeline::VERTEX_SHADER)
		shadobj->v = shader;
	if (type & RenderPipeline::FRAGMENT_SHADER)
		shadobj->f = shader;
	if (type & RenderPipeline::GEOMETRY_SHADER)
		shadobj->g = shader;
	if (type & RenderPipeline::TESSELLATION_CONTROL_SHADER)
		shadobj->tc = shader;
	if (type & RenderPipeline::TESSELLATION_EVOLUTION_SHADER)
		shadobj->te = shader;
	if (type & RenderPipeline::COMPUTE_SHADER)
		shadobj->c = shader;
}

intptr_t RenderPipeline::getNativePtr(void) const {
	return 0;
}

