#include "GLShader.h"
#include "internal_object_type.h"
#include <GL/glew.h>
using namespace fragcore;

void GLShader::bind() { glUseProgram(this->program); }

int GLShader::getLocation(const char *cparamname) { return glGetUniformLocation(this->program, cparamname); }

void GLShader::setInt(int location, int value) { glProgramUniform1i(this->program, location, value); }

void GLShader::setFloat(int location, float value) { glProgramUniform1f(this->program, location, value); }

void GLShader::setFloatv(int location, int n, const float *values) {

	glProgramUniform1fv(this->program, location, n, values);
}

void GLShader::setVec2(int location, const float v1, const float v2) {

	glProgramUniform2f(this->program, location, v1, v2);
}

void GLShader::setVec2v(int location, int n, const float *values) {

	glProgramUniform2fv(this->program, location, n, values);
}

void GLShader::setMatrix4f(int location, const float *pvalue) {

	glProgramUniformMatrix4fv(this->program, location, 1, GL_FALSE, pvalue);
}

void GLShader::setMatrix4fv(int location, int n, const float *pvalue) {

	glProgramUniformMatrix4fv(this->program, location, n, GL_FALSE, pvalue);
}

// TODO determine how to deal with it.
// GL_NUM_PROGRAM_BINARY_FORMATS
// GL_PROGRAM_BINARY_FORMATS

// TODO add support for query.
// glGetProgramResourceiv
/*

void GLShader::bindUniformBuffer(const char* name, BufferObject* buffer){

	GLShaderObject* shadobj = (GLShaderObject*)pobj;
	GLBufferObject* glbuf =  (GLBufferObject*)buffer->getMappedObject();

	GLuint blockindex = glGetUniformBlockIndex(this->program, name);
	glUniformBlockBinding(this->program, blockindex, glbuf->base);

}
*/
void GLShader::getLocalGroupSize(int *sizes) {

	glGetProgramiv(this->program, GL_COMPUTE_WORK_GROUP_SIZE, (GLint *)sizes);
}

// glGetAttachedShaders
// GL_ATTACHED_SHADERS
void *GLShader::getBinary(long int *size, unsigned int *format) {

	GLsizei pBsize;
	GLenum pBformat;

	glGetProgramiv(this->program, GL_PROGRAM_BINARY_LENGTH, &pBsize);
	void *pBinary = malloc(pBsize);
	glGetProgramBinary(this->program, pBsize, nullptr, &pBformat, pBinary);

	if (size)
		*size = pBsize;
	if (format)
		*format = pBformat;
	return pBinary;
}

void *GLShader::getSource(long int *size) { return nullptr; }

void GLShader::setName(const std::string &name) {

	Object::setName(name);
	/*  Update the marker.  */
	MarkerDebug marker = {};
	marker.markerName = name.c_str();
	// addMarkerLabel((const OpenGLCore *) getRenderer()->getData(), GL_SHADER, this->shader, &marker);
}

intptr_t GLShader::getNativePtr() const { return 0; }