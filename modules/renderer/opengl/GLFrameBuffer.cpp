#include "Renderer/IRenderer.h"
#include "internal_object_type.h"
#include <Exception/InvalidArgumentException.h>
#include <GL/glew.h>

#include <fmt/core.h>
#include <stdexcept>
using namespace fragcore;

static unsigned int getCurrentFrameBufferWrite(void) {
	GLint whichID;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &whichID);
	return whichID;
}

static unsigned int getCurrentFrameBufferRead(void) {
	GLint whichID;
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &whichID);
	return whichID;
}

void FrameBuffer::bind(void) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	glBindFramebuffer(GL_FRAMEBUFFER, fraobj->framebuffer);
}

void FrameBuffer::unBind(void) { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void FrameBuffer::write(void) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fraobj->framebuffer);
}

void FrameBuffer::read(void) const {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fraobj->framebuffer);
}

Texture *FrameBuffer::getAttachment(unsigned int index) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	return fraobj->desc.attach[index];
}

Texture *FrameBuffer::getDepthAttachment(void) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	return fraobj->desc.depth;
}

Texture *FrameBuffer::getStencilAttachment(void) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	return fraobj->desc.stencil;
}

int FrameBuffer::width(void) const {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	int width;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(fraobj->framebuffer, GL_FRAMEBUFFER_DEFAULT_WIDTH, &width);
	} else {
		getCurrentFrameBufferRead();
	}

	return width;
}

int FrameBuffer::height(void) const {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	int height;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(fraobj->framebuffer, GL_FRAMEBUFFER_DEFAULT_HEIGHT, &height);
	} else {
	}

	return height;
}

int FrameBuffer::layers() {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	int layers;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(fraobj->framebuffer, GL_FRAMEBUFFER_DEFAULT_LAYERS, &layers);
	} else {
	}

	return layers;
}

int FrameBuffer::nrSamples(void) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	int nrSamples;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(fraobj->framebuffer, GL_FRAMEBUFFER_DEFAULT_SAMPLES, &nrSamples);
	} else {
	}
	return nrSamples;
}

void FrameBuffer::blend(BlendEqu equ, BlendFunc sfactor, BlendFunc dfactor, BufferAttachment bufferAttachment) {
	int index = (int)bufferAttachment - (int)BufferAttachment::eColor0;
	GLenum glEqu = getBlendEqu(equ);
	GLenum glsFactor = getBlendFunc(sfactor);
	GLenum gldFactor = getBlendFunc(dfactor);
	if (index < 0)
		throw InvalidArgumentException(::fmt::format("Must be a color attachment, %d.", index));

	/*  */
	glBlendEquationi(index, glEqu);
	glBlendFunci(index, glsFactor, gldFactor);
}

void FrameBuffer::blendSeperate(BlendEqu equ, BlendFunc srcRGB, BlendFunc dstRGB, BlendFunc srcAlpha,
								BlendFunc dstAlpha, BufferAttachment bufferAttachment) {
	int index = (int)bufferAttachment - (int)BufferAttachment::eColor0;
	GLenum glEqu = getBlendEqu(equ);
	//	GLenum glsFactor = getBlendFunc(sfactor);
	//	GLenum gldFactor = getBlendFunc(dfactor);
	if (index < 0)
		throw InvalidArgumentException(::fmt::format("Must be a color attachment, %d.", index));

	glBlendEquationi(index, glEqu);
	// glBlendFuncSeparatei
}

void FrameBuffer::clear(unsigned int clear) {
	GLbitfield mask = getClearBitMask((CLEARBITMASK)clear);
	glClear(mask);
}

void FrameBuffer::clearColor(BufferAttachment colorAttachment, const float *color) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	int index = (int)colorAttachment - (int)BufferAttachment::eColor0;
	if (glClearNamedFramebufferfv) {
		glClearNamedFramebufferfv(fraobj->framebuffer, GL_COLOR, GL_COLOR_ATTACHMENT0 + index, (GLfloat *)color);
	} else {
		this->write();
		glClearBufferfv(GL_COLOR, GL_COLOR_ATTACHMENT0 + index, (const GLfloat *)color);
	}
}

void FrameBuffer::clearDepthStencil(float depth, int stencil) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	if (glClearNamedFramebufferfi) {
		glClearNamedFramebufferfi(fraobj->framebuffer, 0, GL_DEPTH_STENCIL, stencil, depth);
	} else {
		this->write();
		glClearBufferfi(0, GL_DEPTH_STENCIL, stencil, depth);
	}
}

void FrameBuffer::setDraws(unsigned int nr, BufferAttachment *attachment) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	if (glNamedFramebufferDrawBuffers) {
		GLenum buffers[nr];
		for (int i = 0; i < nr; i++) {
			buffers[i] = GL_COLOR_ATTACHMENT0 + (attachment[i] - FrameBuffer::eColor0);
		}
		glNamedFramebufferDrawBuffers(fraobj->framebuffer, nr, buffers);
	} else {

		//		glDrawBuffers()
	}
}

void FrameBuffer::setDraw(BufferAttachment attachment) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	if (glNamedFramebufferDrawBuffer) {
		glNamedFramebufferDrawBuffer(fraobj->framebuffer, GL_COLOR_ATTACHMENT0 + (attachment - FrameBuffer::eColor0));
	} else {
	}
}

intptr_t FrameBuffer::getNativePtr(void) const {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	return fraobj->framebuffer;
}

void FrameBuffer::setName(const std::string &name) {
	GLFrameBufferObject *fraobj = (GLFrameBufferObject *)this->getObject();
	Object::setName(name);

	/*  Update the marker.  */
	MarkerDebug marker = {};
	marker.markerName = name.c_str();
	////addMarkerLabel((const OpenGLCore *) getRenderer()->getData(), GL_FRAMEBUFFER, fraobj->framebuffer, &marker);
}

// glClearBuffer