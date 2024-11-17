#include "GLBuffer.h"
#include "GLHelper.h"
#include "internal_object_type.h"

using namespace fragcore;

// TODO add for retaining current binded buffer.

void GLBuffer::bind() {

	glBindBufferARB(this->target, this->buffer);
	checkError();
}

void GLBuffer::bind(unsigned int offset, unsigned int size) {

	glBindBufferRange(this->target, this->base, this->buffer, offset, size);
	checkError();
}

void GLBuffer::bindBase(unsigned int base) {

	this->base = base;
	glBindBufferBase(this->target, base, this->buffer);
	checkError();
}

void GLBuffer::subData(const void *data, unsigned int offset, unsigned int size) {
	char *pmap;

	this->bind();
	pmap = (char *)this->mapBuffer(GLBuffer::Write);
	checkError();
	if (pmap) {
		memcpy(pmap + offset, data, size);
		this->unMapBuffer();
	} else {
		glBufferSubDataARB(this->target, offset, size, data);
	}
}

void *GLBuffer::getData(unsigned int offset, unsigned int size) {

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

bool GLBuffer::isValid() { return glIsBufferARB(this->buffer) == GL_TRUE; }

long int GLBuffer::getSize() {
	GLint size;
	this->bind();
	glGetBufferParameteriv(this->target, GL_BUFFER_SIZE, &size);
	checkError();
	return size;
}

// TODO relocate to the helper header.
static GLenum getAccessEnum(GLBuffer::MapTarget target) {
	GLenum access = 0;

	if (target & GLBuffer::MapTarget::Read) {
		access |= GL_MAP_READ_BIT;
	}
	if (target & GLBuffer::MapTarget::Write) {
		access |= GL_MAP_WRITE_BIT;
	}
	if (target & GLBuffer::MapTarget::ReadWrite) {
		access |= GL_MAP_WRITE_BIT | GL_MAP_READ_BIT;
	}
	if (target & GLBuffer::MapTarget::NoSync) {
		access |= GL_MAP_UNSYNCHRONIZED_BIT;
	}
	if (target & GLBuffer::MapTarget::PERSISTENT) {
		access |= GL_MAP_PERSISTENT_BIT;
	}
	if (target & GLBuffer::MapTarget::FlushExplicit) {
		access |= GL_MAP_FLUSH_EXPLICIT_BIT;
	}
	return access;
}

static GLenum getAccessOlder(GLBuffer::MapTarget target) {
	GLenum access = 0;
	if (target & GLBuffer::MapTarget::Read) {
		access |= GL_READ_ONLY_ARB;
	}
	if (target & GLBuffer::MapTarget::Write) {
		access |= GL_WRITE_ONLY_ARB;
	}
	if (target & GLBuffer::MapTarget::ReadWrite) {
		access |= GL_READ_WRITE_ARB;
	}
	return access;
}

void *GLBuffer::mapBuffer(GLBuffer::MapTarget target) {

	GLenum mode = getAccessOlder(target);

	// GL_MIN_MAP_BUFFER_ALIGNMENT
	if (glMapNamedBuffer) {
		return glMapNamedBuffer(this->buffer, mode);
}
	 		this->bind();
		return glMapBufferARB(this->target, mode);

}

void *GLBuffer::mapBuffer(MapTarget target, unsigned long int offset, unsigned long int length) {

	GLenum mode = getAccessEnum(target);

	if (glMapNamedBufferRange) {
		return glMapNamedBufferRange(this->buffer, offset, length, mode);
	}
	this->bind();
	return glMapBufferRange(this->target, offset, length, mode);
}

void GLBuffer::flush(unsigned long int offset, unsigned long int length) {

	if (glFlushMappedNamedBufferRange) {
		glFlushMappedNamedBufferRange(this->buffer, offset, length);
		checkError();
	} else {
		this->bind();
		glFlushMappedBufferRange(this->target, offset, length);
		checkError();
	}
}

void GLBuffer::unMapBuffer() {

	if (glUnmapNamedBuffer) {
		glUnmapNamedBuffer(this->buffer);
	} else {
		this->bind();
		glUnmapBufferARB(this->target);
	}
}

void GLBuffer::setName(const std::string &name) {
	Object::setName(name);

	/*  Update the marker.  */
	MarkerDebug marker = {};
	marker.markerName = name.c_str();
	GLHelper::addMarkerLabel(GL_BUFFER, this->buffer, &marker);
}

intptr_t GLBuffer::getNativePtr() const { return this->buffer; }