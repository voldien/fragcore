#include "GLFrameBuffer.h"
#include "../IRenderer.h"
#include "internal_object_type.h"
#include <GL/glew.h>

#include <fmt/core.h>
using namespace fragcore;

static unsigned int getCurrentFrameBufferWrite() {
	GLint whichID;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &whichID);
	return whichID;
}

static unsigned int getCurrentFrameBufferRead() {
	GLint whichID;
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &whichID);
	return whichID;
}

int GLFrameBuffer::attachmentCount() {}
std::vector<Texture *> GLFrameBuffer::getColorTargets() {}

void GLFrameBuffer::bind() {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	glBindFramebuffer(GL_FRAMEBUFFER, fraobj->framebuffer);
}

void GLFrameBuffer::unBind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void GLFrameBuffer::write() {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fraobj->framebuffer);
}

void GLFrameBuffer::read() const {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fraobj->framebuffer);
}

Texture *GLFrameBuffer::getAttachment(unsigned int index) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	return fraobj->desc.attach[index];
}

Texture *GLFrameBuffer::getDepthAttachment() {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	return fraobj->desc.depth;
}

Texture *GLFrameBuffer::getStencilAttachment() {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	return fraobj->desc.stencil;
}

int GLFrameBuffer::width() const {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	int width;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(fraobj->framebuffer, GL_FRAMEBUFFER_DEFAULT_WIDTH, &width);
	} else {
		getCurrentFrameBufferRead();
	}

	return width;
}

int GLFrameBuffer::height() const {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	int height;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(fraobj->framebuffer, GL_FRAMEBUFFER_DEFAULT_HEIGHT, &height);
	} else {
	}

	return height;
}

int GLFrameBuffer::layers() {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	int layers;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(fraobj->framebuffer, GL_FRAMEBUFFER_DEFAULT_LAYERS, &layers);
	} else {
	}

	return layers;
}

int GLFrameBuffer::nrSamples() {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	int nrSamples;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(fraobj->framebuffer, GL_FRAMEBUFFER_DEFAULT_SAMPLES, &nrSamples);
	} else {
	}
	return nrSamples;
}

void GLFrameBuffer::blend(BlendEqu equ, BlendFunc sfactor, BlendFunc dfactor, BufferAttachment bufferAttachment) {
	int index = (int)bufferAttachment - (int)BufferAttachment::Color0;
	GLenum glEqu = getBlendEqu(equ);
	GLenum glsFactor = getBlendFunc(sfactor);
	GLenum gldFactor = getBlendFunc(dfactor);
	if (index < 0)
		throw InvalidArgumentException(::fmt::format("Must be a color attachment, %d.", index));

	/*  */
	glBlendEquationi(index, glEqu);
	glBlendFunci(index, glsFactor, gldFactor);
}

void GLFrameBuffer::blendSeperate(BlendEqu equ, BlendFunc srcRGB, BlendFunc dstRGB, BlendFunc srcAlpha,
								  BlendFunc dstAlpha, BufferAttachment bufferAttachment) {
	int index = (int)bufferAttachment - (int)BufferAttachment::Color0;
	GLenum glEqu = getBlendEqu(equ);
	//	GLenum glsFactor = getBlendFunc(sfactor);
	//	GLenum gldFactor = getBlendFunc(dfactor);
	if (index < 0)
		throw InvalidArgumentException(::fmt::format("Must be a color attachment, %d.", index));

	glBlendEquationi(index, glEqu);
	// glBlendFuncSeparatei
}

void GLFrameBuffer::clear(unsigned int clear) {
	GLbitfield mask = getClearBitMask((CLEARBITMASK)clear);
	glClear(mask);
}

void GLFrameBuffer::clearColor(BufferAttachment colorAttachment, const float *color) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	int index = (int)colorAttachment - (int)BufferAttachment::Color0;
	if (glClearNamedFramebufferfv) {
		glClearNamedFramebufferfv(fraobj->framebuffer, GL_COLOR, GL_COLOR_ATTACHMENT0 + index, (GLfloat *)color);
	} else {
		this->write();
		glClearBufferfv(GL_COLOR, GL_COLOR_ATTACHMENT0 + index, (const GLfloat *)color);
	}
}

void GLFrameBuffer::clearDepthStencil(float depth, int stencil) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	if (glClearNamedFramebufferfi) {
		glClearNamedFramebufferfi(fraobj->framebuffer, 0, GL_DEPTH_STENCIL, stencil, depth);
	} else {
		this->write();
		glClearBufferfi(0, GL_DEPTH_STENCIL, stencil, depth);
	}
}

void GLFrameBuffer::setDraws(unsigned int nr, BufferAttachment *attachment) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	if (glNamedFramebufferDrawBuffers) {
		GLenum buffers[nr];
		for (size_t i = 0; i < nr; i++) {
			buffers[i] = GL_COLOR_ATTACHMENT0 + ((int)attachment[i] - (int)GLFrameBuffer::BufferAttachment::Color0);
		}
		glNamedFramebufferDrawBuffers(fraobj->framebuffer, nr, buffers);
	} else {

		//		glDrawBuffers()
	}
}

void GLFrameBuffer::setDraw(BufferAttachment attachment) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	if (glNamedFramebufferDrawBuffer) {
		glNamedFramebufferDrawBuffer(fraobj->framebuffer,
									 GL_COLOR_ATTACHMENT0 +
										 ((int)attachment - (int)GLFrameBuffer::BufferAttachment::Color0));
	} else {
	}
}

intptr_t GLFrameBuffer::getNativePtr() const {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	return fraobj->framebuffer;
}

void GLFrameBuffer::setName(const std::string &name) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	Object::setName(name);

	/*  Update the marker.  */
	MarkerDebug marker = {};
	marker.markerName = name.c_str();
	////addMarkerLabel((const OpenGLCore *) getRenderer()->getData(), GL_FRAMEBUFFER, fraobj->framebuffer, &marker);
}

// glClearBuffer

GLFrameBuffer::GLFrameBuffer() {}