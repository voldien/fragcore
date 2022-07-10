#include "CLBuffer.h"
#include "internal_object_type.h"
#include <opencl-c.h>
#include <opencl.h>

using namespace fragcore;

// TODO add for retaining current binded buffer.

void CLBuffer::bind() {

	glBindBufferARB(this->target, this->buffer);
	checkError();
}

void CLBuffer::bind(unsigned int offset, unsigned int size) {

	glBindBufferRange(this->target, this->base, this->buffer, offset, size);
	checkError();
}

void CLBuffer::bindBase(unsigned int base) {

	this->base = base;
	glBindBufferBase(this->target, base, this->buffer);
	checkError();
}

void CLBuffer::subData(const void *data, unsigned int offset, unsigned int size) {
	char *pmap;

	this->bind();
	pmap = (char *)this->mapBuffer(CLBuffer::Write);
	checkError();
	if (pmap) {
		memcpy(pmap + offset, data, size);
		this->unMapBuffer();
	} else
		CLBufferSubDataARB(this->target, offset, size, data);
}

void *CLBuffer::getData(unsigned int offset, unsigned int size) {

	void *pbuffer = nullptr;

	/*	*/
	pbuffer = malloc(size);

	if (glGetNamedBufferSubData) {
		glGetNamedBufferSubData(this->buffer, offset, size, pbuffer);
		checkError();
	} else {
		this->bind();
		glGetBufferSubDataARB(this->target, offset, size, pbuffer);
		checkError();
	}

	return pbuffer;
}

bool CLBuffer::isValid() { return glIsBufferARB(this->buffer) == GL_TRUE; }

long int CLBuffer::getSize() {
	GLint size;
	this->bind();
	glGetBufferParameteriv(this->target, GL_BUFFER_SIZE, &size);
	checkError();
	return size;
}

// TODO relocate to the helper header.
static GLenum getAccessEnum(CLBuffer::MapTarget target) {
	GLenum access = 0;

	if (target & CLBuffer::MapTarget::Read)
		access |= GL_MAP_READ_BIT;
	if (target & CLBuffer::MapTarget::Write)
		access |= GL_MAP_WRITE_BIT;
	if (target & CLBuffer::MapTarget::ReadWrite)
		access |= GL_MAP_WRITE_BIT | GL_MAP_READ_BIT;
	if (target & CLBuffer::MapTarget::NoSync)
		access |= GL_MAP_UNSYNCHRONIZED_BIT;
	if (target & CLBuffer::MapTarget::PERSISTENT)
		access |= GL_MAP_PERSISTENT_BIT;
	if (target & CLBuffer::MapTarget::FlushExplicit)
		access |= GL_MAP_FLUSH_EXPLICIT_BIT;
	return access;
}

static GLenum getAccessOlder(CLBuffer::MapTarget target) {
	GLenum access = 0;
	if (target & CLBuffer::MapTarget::Read)
		access |= GL_READ_ONLY_ARB;
	if (target & CLBuffer::MapTarget::Write)
		access |= GL_WRITE_ONLY_ARB;
	if (target & CLBuffer::MapTarget::ReadWrite)
		access |= GL_READ_WRITE_ARB;
	return access;
}

void *CLBuffer::mapBuffer(CLBuffer::MapTarget target) {

	GLenum mode = getAccessOlder(target);

	// GL_MIN_MAP_BUFFER_ALIGNMENT
	if (glMapNamedBuffer)
		return glMapNamedBuffer(this->buffer, mode);
	else {
		this->bind();
		return glMapBufferARB(this->target, mode);
	}
}

void *CLBuffer::mapBuffer(MapTarget target, unsigned long int offset, unsigned long int length) {

	GLenum mode = getAccessEnum(target);

	if (glMapNamedBufferRange) {
		return glMapNamedBufferRange(this->buffer, offset, length, mode);
	} else {
		this->bind();
		return glMapBufferRange(this->target, offset, length, mode);
	}
}

void CLBuffer::flush(unsigned long int offset, unsigned long int length) {

	if (glFlushMappedNamedBufferRange) {
		glFlushMappedNamedBufferRange(this->buffer, offset, length);
		checkError();
	} else {
		this->bind();
		glFlushMappedBufferRange(this->target, offset, length);
		checkError();
	}
}

void CLBuffer::unMapBuffer() {

	if (glUnmapNamedBuffer) {
		glUnmapNamedBuffer(this->buffer);
	} else {
		this->bind();
		glUnmapBufferARB(this->target);
	}
}

void CLBuffer::setName(const std::string &name) {
	Object::setName(name);

	/*  Update the marker.  */
	MarkerDebug marker = {};
	marker.markerName = name.c_str();
	addMarkerLabel(GL_BUFFER, this->buffer, &marker);
}

intptr_t CLBuffer::getNativePtr() const { return this->buffer; }