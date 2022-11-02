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

int GLFrameBuffer::attachmentCount() { return 0; }
std::vector<Texture *> GLFrameBuffer::getColorTargets() { return {}; }

void GLFrameBuffer::bind() { glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer); }

void GLFrameBuffer::unBind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void GLFrameBuffer::write() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->framebuffer); }

void GLFrameBuffer::read() const { glBindFramebuffer(GL_READ_FRAMEBUFFER, this->framebuffer); }

Texture *GLFrameBuffer::getAttachment(unsigned int index) { return this->desc.attach[index]; }

Texture *GLFrameBuffer::getDepthAttachment() { return this->desc.depth; }

Texture *GLFrameBuffer::getStencilAttachment() { return this->desc.stencil; }

int GLFrameBuffer::width() const {

	int width = 0;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(this->framebuffer, GL_FRAMEBUFFER_DEFAULT_WIDTH, &width);
	} else {
		getCurrentFrameBufferRead();
		glGetNamedFramebufferParameteriv(0, GL_FRAMEBUFFER_DEFAULT_WIDTH, &width);
	}

	return width;
}

int GLFrameBuffer::height() const {

	int height = 0;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(this->framebuffer, GL_FRAMEBUFFER_DEFAULT_HEIGHT, &height);
	} else {
		getCurrentFrameBufferRead();
		glGetNamedFramebufferParameteriv(0, GL_FRAMEBUFFER_DEFAULT_HEIGHT, &height);
	}

	return height;
}

int GLFrameBuffer::layers() {

	int layers = 0;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(this->framebuffer, GL_FRAMEBUFFER_DEFAULT_LAYERS, &layers);
	} else {
		getCurrentFrameBufferRead();
		glGetNamedFramebufferParameteriv(0, GL_FRAMEBUFFER_DEFAULT_LAYERS, &layers);
	}

	return layers;
}

int GLFrameBuffer::nrSamples() {

	int nrSamples = 0;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(this->framebuffer, GL_FRAMEBUFFER_DEFAULT_SAMPLES, &nrSamples);
	} else {
	}
	return nrSamples;
}

void GLFrameBuffer::blend(BlendEqu equ, BlendFunc sfactor, BlendFunc dfactor, BufferAttachment bufferAttachment) {
	int index = (int)bufferAttachment - (int)BufferAttachment::Color0;
	GLenum glEqu = GLHelper::getBlendEqu(equ);
	GLenum glsFactor = GLHelper::getBlendFunc(sfactor);
	GLenum gldFactor = GLHelper::getBlendFunc(dfactor);
	if (index < 0)
		throw InvalidArgumentException(::fmt::format("Must be a color attachment, %d.", index));

	/*  */
	glBlendEquationi(index, glEqu);
	glBlendFunci(index, glsFactor, gldFactor);
}

void GLFrameBuffer::blendSeperate(BlendEqu equ, BlendFunc srcRGB, BlendFunc dstRGB, BlendFunc srcAlpha,
								  BlendFunc dstAlpha, BufferAttachment bufferAttachment) {
	int index = (int)bufferAttachment - (int)BufferAttachment::Color0;
	GLenum glEqu = GLHelper::getBlendEqu(equ);
	//	GLenum glsFactor = getBlendFunc(sfactor);
	//	GLenum gldFactor = getBlendFunc(dfactor);
	if (index < 0)
		throw InvalidArgumentException(::fmt::format("Must be a color attachment, %d.", index));

	glBlendEquationi(index, glEqu);
	// glBlendFuncSeparatei
}

void GLFrameBuffer::clear(unsigned int clear) {
	GLbitfield mask = GLHelper::getClearBitMask((CLEARBITMASK)clear);
	glClear(mask);
}

void GLFrameBuffer::clearColor(BufferAttachment colorAttachment, const float *color) {

	int index = (int)colorAttachment - (int)BufferAttachment::Color0;
	if (glClearNamedFramebufferfv) {
		glClearNamedFramebufferfv(this->framebuffer, GL_COLOR, GL_COLOR_ATTACHMENT0 + index, (GLfloat *)color);
	} else {
		this->write();
		glClearBufferfv(GL_COLOR, GL_COLOR_ATTACHMENT0 + index, (const GLfloat *)color);
	}
}

void GLFrameBuffer::clearDepthStencil(float depth, int stencil) {

	if (glClearNamedFramebufferfi) {
		glClearNamedFramebufferfi(this->framebuffer, 0, GL_DEPTH_STENCIL, stencil, depth);
	} else {
		this->write();
		glClearBufferfi(0, GL_DEPTH_STENCIL, stencil, depth);
	}
}

void GLFrameBuffer::setDraws(unsigned int nr, BufferAttachment *attachment) {

	if (glNamedFramebufferDrawBuffers) {
		GLenum buffers[nr];
		for (size_t i = 0; i < nr; i++) {
			buffers[i] = GL_COLOR_ATTACHMENT0 + ((int)attachment[i] - (int)GLFrameBuffer::BufferAttachment::Color0);
		}
		glNamedFramebufferDrawBuffers(this->framebuffer, nr, buffers);
	} else {

		//		glDrawBuffers()
	}
}

void GLFrameBuffer::setDraw(BufferAttachment attachment) {

	if (glNamedFramebufferDrawBuffer) {
		glNamedFramebufferDrawBuffer(
			this->framebuffer, GL_COLOR_ATTACHMENT0 + ((int)attachment - (int)GLFrameBuffer::BufferAttachment::Color0));
	} else {
	}
}

intptr_t GLFrameBuffer::getNativePtr() const { return this->framebuffer; }

void GLFrameBuffer::setName(const std::string &name) {
	Object::setName(name);

	/*  Update the marker.  */
	MarkerDebug marker = {};
	marker.markerName = name.c_str();
	////addMarkerLabel((const OpenGLCore *) getRenderer()->getData(), GL_FRAMEBUFFER, this->framebuffer, &marker);
}

// glClearBuffer

GLFrameBuffer::GLFrameBuffer() {}