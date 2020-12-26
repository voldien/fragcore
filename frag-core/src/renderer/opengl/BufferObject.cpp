#include"Renderer/IRenderer.h"
#include"Renderer/Buffer.h"
#include"Renderer/opengl/internal_object_type.h"
#include<GL/glew.h>
using namespace fragcore;

//TODO add for retaining current binded buffer.
Buffer::~Buffer(void) {}

void Buffer::bind() {
	GLBufferObject *bufobj = (GLBufferObject *) this->pdata;
	glBindBufferARB(bufobj->target, bufobj->buffer);
}

void Buffer::bind(unsigned int offset, unsigned int size){
	GLBufferObject *bufobj = (GLBufferObject *) this->pdata;
	glBindBufferRange(bufobj->target, bufobj->base, bufobj->buffer, offset, size);
}

void Buffer::bindBase(unsigned int base) {
	GLBufferObject *bufobj = (GLBufferObject *) this->pdata;

	bufobj->base = base;
	glBindBufferBase(bufobj->target, base, bufobj->buffer);
}

void Buffer::subData(const void *data, unsigned int offset, unsigned int size) {
	GLBufferObject *bufobj = (GLBufferObject *) this->pdata;
	char *pmap;

	this->bind();
	pmap = (char *) this->mapBuffer(Buffer::eWrite);
	if (pmap) {
		memcpy(pmap + offset, data, size);
		this->unMapBuffer();
	} else
		glBufferSubDataARB(bufobj->target, offset, size, data);

}

void *Buffer::getData(unsigned int offset, unsigned int size) {

	void *pbuffer = NULL;
	GLBufferObject *bufobj = (GLBufferObject *) this->pdata;

	/*	*/
	pbuffer = malloc(size);

	if (glGetNamedBufferSubData) {
		glGetNamedBufferSubData(bufobj->buffer, offset, size, pbuffer);
	} else {
		this->bind();
		glGetBufferSubDataARB(bufobj->target, offset, size, pbuffer);
	}

	return pbuffer;
}

bool Buffer::isValid(void) {
	GLBufferObject *bufobj = (GLBufferObject *) this->pdata;
	return glIsBufferARB(bufobj->buffer) == GL_TRUE;
}

long int Buffer::getSize(void){
	GLint size;
	GLBufferObject *bufobj = (GLBufferObject *) this->getObject();
	this->bind();
	glGetBufferParameteriv(bufobj->target, GL_BUFFER_SIZE, &size);
	return size;
}

//TODO relocate to the helper header.
static GLenum getAccessEnum(Buffer::MapTarget target) {
	GLenum access = 0;

	if (target & Buffer::MapTarget::eRead)
		access |= GL_MAP_READ_BIT;
	if (target & Buffer::MapTarget::eWrite)
		access |= GL_MAP_WRITE_BIT;
	if (target & Buffer::MapTarget::eReadWrite)
		access |= GL_MAP_WRITE_BIT | GL_MAP_READ_BIT;
	if (target & Buffer::MapTarget::eNoSync)
		access |= GL_MAP_UNSYNCHRONIZED_BIT;
	if (target & Buffer::MapTarget::ePERSISTENT)
		access |= GL_MAP_PERSISTENT_BIT;
	if(target & Buffer::MapTarget::eFlushExplicit)
		access |= GL_MAP_FLUSH_EXPLICIT_BIT;
	return access;
}

static GLenum getAccessOlder(Buffer::MapTarget target) {
	GLenum access = 0;
	if (target & Buffer::MapTarget::eRead)
		access |= GL_READ_ONLY_ARB;
	if (target & Buffer::MapTarget::eWrite)
		access |= GL_WRITE_ONLY_ARB;
	if (target & Buffer::MapTarget::eReadWrite)
		access |= GL_READ_WRITE_ARB;
	return access;
}

void *Buffer::mapBuffer(Buffer::MapTarget target) {
	GLBufferObject *bufobj = (GLBufferObject *) this->pdata;

	GLenum mode = getAccessOlder(target);

	// GL_MIN_MAP_BUFFER_ALIGNMENT
	if (glMapNamedBuffer)
		return glMapNamedBuffer(bufobj->buffer, mode);
	else {
		this->bind();
		return glMapBufferARB(bufobj->target, mode);
	}
}

void *Buffer::mapBuffer(MapTarget target, unsigned long int offset, unsigned long int length) {
	GLBufferObject *bufobj = (GLBufferObject *) this->pdata;
	GLenum mode = getAccessEnum(target);

	if (glMapNamedBufferRange) {
		return glMapNamedBufferRange(bufobj->buffer, offset, length, mode);
	} else {
		this->bind();
		return glMapBufferRange(bufobj->target, offset, length, mode);
	}
}

void Buffer::flush(unsigned long int offset, unsigned long int length) {
	GLBufferObject *bufobj = (GLBufferObject *) this->pdata;

	if (glFlushMappedNamedBufferRange) {
		glFlushMappedNamedBufferRange(bufobj->buffer, offset, length);
	} else {
		this->bind();
		glFlushMappedBufferRange(bufobj->target, offset, length);
	}
}

void Buffer::unMapBuffer(void) {
	GLBufferObject *bufobj = (GLBufferObject *) this->pdata;
	if (glUnmapNamedBuffer) {
		glUnmapNamedBuffer(bufobj->buffer);
	} else {
		this->bind();
		glUnmapBufferARB(bufobj->target);
	}
}

void Buffer::setName(const std::string &name) {
	GLBufferObject *bufobj = (GLBufferObject *) this->pdata;
	Object::setName(name);

	/*  Update the marker.  */
	MarkerDebug marker = {};
	marker.markerName = name.c_str();
	addMarkerLabel((const OpenGLCore *)getRenderer()->getData(), GL_BUFFER, bufobj->buffer, &marker);
}

intptr_t Buffer::getNativePtr(void) const {
	return 0;
}