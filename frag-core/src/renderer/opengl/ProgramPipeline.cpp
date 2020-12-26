#include"Renderer/ProgramPipeline.h"
#include"Renderer/Shader.h"
#include"Renderer/opengl/internal_object_type.h"
#include<GL/glew.h>
using namespace fragcore;

void ProgramPipeline::bind(void) {
	GLProgramPipeline *pipeline = (GLProgramPipeline *) this->pdata;
	glBindProgramPipeline(pipeline->program);
	//glActiveShaderProgram
}

int ProgramPipeline::getLocation(const char *name) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	return glGetUniformLocation(shadobj->program, name);
}

void ProgramPipeline::setInt(int location, int value) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniform1i(shadobj->program, location, value);
}

void ProgramPipeline::setFloat(int location, float value) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniform1f(shadobj->program, location, value);
}

void ProgramPipeline::setFloatv(int location, int n, const float *values) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniform1fv(shadobj->program, location, n, values);
}

void ProgramPipeline::setVec2(int location, const float v1, const float v2) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniform2f(shadobj->program, location, v1, v2);
}

void ProgramPipeline::setVec2v(int location, int n, const float *values) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniform2fv(shadobj->program, location, n, values);
}

void ProgramPipeline::setMatrix4f(int location, const float *pvalue) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniformMatrix4fv(shadobj->program, location, 1, GL_FALSE, pvalue);
}

void ProgramPipeline::setMatrix4fv(int location, int n, const float *pvalue) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	glProgramUniformMatrix4fv(shadobj->program, location, n, GL_FALSE, pvalue);
}

static GLenum getShaderFlag(ProgramPipeline::ShaderType type) {
	GLenum flag = 0;

	if (type & ProgramPipeline::VERTEX_SHADER)
		flag |= GL_VERTEX_SHADER_BIT;
	if (type & ProgramPipeline::FRAGMENT_SHADER)
		flag |= GL_FRAGMENT_SHADER_BIT;
	if (type & ProgramPipeline::GEOMETRY_SHADER)
		flag |= GL_GEOMETRY_SHADER_BIT;
	if (type & ProgramPipeline::TESSELLATION_CONTROL_SHADER)
		flag |= GL_TESS_CONTROL_SHADER_BIT;
	if (type & ProgramPipeline::TESSELLATION_EVOLUTION_SHADER)
		flag |= GL_TESS_EVALUATION_SHADER_BIT;
	if (type & ProgramPipeline::COMPUTE_SHADER)
		flag |= GL_COMPUTE_SHADER_BIT;

	return flag;
}

Shader *ProgramPipeline::getShader(ShaderType type) {
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

	return NULL;
}

void ProgramPipeline::setShader(ShaderType type, Shader *shader) {
	GLProgramPipeline *shadobj = (GLProgramPipeline *) this->pdata;
	GLShaderObject *glShader = (GLShaderObject *) shader->getObject();

	/*  Get flag.   */
	GLenum shaderflag = getShaderFlag(type);

	/*  Update new shader.  */
	glUseProgramStages(shadobj->program, shaderflag, glShader->program);

	/*  Update shader reference. */
	if (type & ProgramPipeline::VERTEX_SHADER)
		shadobj->v = shader;
	if (type & ProgramPipeline::FRAGMENT_SHADER)
		shadobj->f = shader;
	if (type & ProgramPipeline::GEOMETRY_SHADER)
		shadobj->g = shader;
	if (type & ProgramPipeline::TESSELLATION_CONTROL_SHADER)
		shadobj->tc = shader;
	if (type & ProgramPipeline::TESSELLATION_EVOLUTION_SHADER)
		shadobj->te = shader;
	if (type & ProgramPipeline::COMPUTE_SHADER)
		shadobj->c = shader;
}

intptr_t ProgramPipeline::getNativePtr(void) const {
	return 0;
}

