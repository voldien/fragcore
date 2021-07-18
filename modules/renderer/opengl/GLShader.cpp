#include "Renderer/Shader.h"
#include "internal_object_type.h"
#include <GL/glew.h>
using namespace fragcore;

void Shader::bind(void) {
	GLShaderObject *shadobj = (GLShaderObject *)this->pdata;
	glUseProgram(shadobj->program);
}

int Shader::getLocation(const char *cparamname) {
	GLShaderObject *shadobj = (GLShaderObject *)this->pdata;
	return glGetUniformLocation(shadobj->program, cparamname);
}

void Shader::setInt(int location, int value) {
	GLShaderObject *shadobj = (GLShaderObject *)this->pdata;
	glProgramUniform1i(shadobj->program, location, value);
}

void Shader::setFloat(int location, float value) {
	GLShaderObject *shadobj = (GLShaderObject *)this->pdata;
	glProgramUniform1f(shadobj->program, location, value);
}

void Shader::setFloatv(int location, int n, const float *values) {
	GLShaderObject *shadobj = (GLShaderObject *)this->pdata;
	glProgramUniform1fv(shadobj->program, location, n, values);
}

void Shader::setVec2(int location, const float v1, const float v2) {
	GLShaderObject *shadobj = (GLShaderObject *)this->pdata;
	glProgramUniform2f(shadobj->program, location, v1, v2);
}

void Shader::setVec2v(int location, int n, const float *values) {
	GLShaderObject *shadobj = (GLShaderObject *)this->pdata;
	glProgramUniform2fv(shadobj->program, location, n, values);
}

void Shader::setMatrix4f(int location, const float *pvalue) {
	GLShaderObject *shadobj = (GLShaderObject *)this->pdata;
	glProgramUniformMatrix4fv(shadobj->program, location, 1, GL_FALSE, pvalue);
}

void Shader::setMatrix4fv(int location, int n, const float *pvalue) {
	GLShaderObject *shadobj = (GLShaderObject *)this->pdata;
	glProgramUniformMatrix4fv(shadobj->program, location, n, GL_FALSE, pvalue);
}

// TODO determine how to deal with it.
// GL_NUM_PROGRAM_BINARY_FORMATS
// GL_PROGRAM_BINARY_FORMATS

// TODO add support for query.
// glGetProgramResourceiv
/*

void Shader::bindUniformBuffer(const char* name, BufferObject* buffer){

	GLShaderObject* shadobj = (GLShaderObject*)pobj;
	GLBufferObject* glbuf =  (GLBufferObject*)buffer->getMappedObject();

	GLuint blockindex = glGetUniformBlockIndex(shadobj->program, name);
	glUniformBlockBinding(shadobj->program, blockindex, glbuf->base);

}
*/
void Shader::getLocalGroupSize(int *sizes) {
	GLShaderObject *shadobj = (GLShaderObject *)this->pdata;
	glGetProgramiv(shadobj->program, GL_COMPUTE_WORK_GROUP_SIZE, (GLint *)sizes);
}

// glGetAttachedShaders
// GL_ATTACHED_SHADERS
void *Shader::getBinary(long int *size, unsigned int *format) {
	GLShaderObject *shadobj = (GLShaderObject *)this->pdata;
	GLsizei pBsize;
	GLenum pBformat;

	glGetProgramiv(shadobj->program, GL_PROGRAM_BINARY_LENGTH, &pBsize);
	void *pBinary = malloc(pBsize);
	glGetProgramBinary(shadobj->program, pBsize, nullptr, &pBformat, pBinary);

	if (size)
		*size = pBsize;
	if (format)
		*format = pBformat;
	return pBinary;
}

void *Shader::getSource(long int *size) {}

void Shader::setName(const std::string &name) {
	GLShaderObject *shadobj = (GLShaderObject *)this->pdata;
	Object::setName(name);
	/*  Update the marker.  */
	MarkerDebug marker = {};
	marker.markerName = name.c_str();
	// addMarkerLabel((const OpenGLCore *) getRenderer()->getData(), GL_SHADER, shadobj->shader, &marker);
}

intptr_t Shader::getNativePtr(void) const { return 0; }