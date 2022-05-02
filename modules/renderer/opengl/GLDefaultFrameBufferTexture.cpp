#include "../IRenderer.h"
#include "../RenderObject.h"
#include "../RendererWindow.h"
#include "../Texture.h"
#include "DefaultFrameBufferTexture.h"
#include "GLRendererInterface.h"
#include "internal_object_type.h"
#include <GL/glew.h>

using namespace fragcore;

FrameBufferTexture::FrameBufferTexture() {
	this->setName("Default frambuffer back color attachment");

	glGenBuffersARB(1, &this->pbo);
	glBindBufferARB(GL_PIXEL_PACK_BUFFER, pbo);
	// TODO improve
	glBufferDataARB(GL_PIXEL_PACK_BUFFER, 4147200 * 4, nullptr,
					GL_DYNAMIC_COPY); // TODO deal with the allocation and etc.
}

void *FrameBufferTexture::mapTexture(Format format, unsigned int level) {
	GLenum _format = getTextureGLFormat(format);
	GLenum _type = GL_UNSIGNED_BYTE; // TODO determine the type based on the fmt::format.

	if (level != 0)
		throw InvalidArgumentException("Level 0 is only available.");

	/*  Bind pixel pack to create memory mapping from GPU to CPU memory.	*/
	glReadBuffer(GL_BACK_LEFT);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, this->pbo);

	/*  */
	glReadPixels(0, 0, width(), height(), _format, GL_UNSIGNED_BYTE, nullptr);
	return glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY_ARB);
}

void FrameBufferTexture::unMapTexture() {
	if (glUnmapNamedBuffer) {
		glUnmapNamedBuffer(pbo);
	} else {
		glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_ARB);
	}
}

Texture::Format FrameBufferTexture::getFormat() const { return Texture::eR8G8B8A8; }

void FrameBufferTexture::bind(unsigned int index) {}

bool FrameBufferTexture::isValid() { return true; }

void FrameBufferTexture::setMipLevel(unsigned int level) {}

void FrameBufferTexture::setFilterMode(FilterMode mode) {}

Texture::FilterMode FrameBufferTexture::getFilterMode() { return Texture::FilterMode::eNearest; }

void FrameBufferTexture::setWrapMode(WrapMode mode) {}

Texture::WrapMode FrameBufferTexture::getWrapMode() { return Texture::eClamp; }

void FrameBufferTexture::setAnisotropic(float anisotropic) {}

float FrameBufferTexture::getAnisotropic() const { return 0.0f; }

void FrameBufferTexture::setPixels(Format format, unsigned int level, const void *pixels, unsigned long size) {
	// TODO
	// glBlitNamedFramebuffer
	// Texture::setPixels(fmt::format, level, pixels, size);
}

void *FrameBufferTexture::getPixels(Format format, unsigned int level, unsigned long *nBytes) {
	// TODO add support.
	// if PBO supported, otherwise, the slow versino.
	GLenum glformat = getTextureGLFormat(format);
	glReadPixels(0, 0, this->width(), this->height(), glformat, GL_UNSIGNED_BYTE, nullptr);
	return nullptr; // Texture::getPixels(mipmap, nBytes);
}

unsigned int FrameBufferTexture::width() {
	int width = 0;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(0, GL_FRAMEBUFFER_DEFAULT_WIDTH, &width);
	} else {
		// getCurrentFrameBufferRead();
	}

	return width;
}

unsigned int FrameBufferTexture::height() {
	int height = 0;
	if (glGetNamedFramebufferParameteriv) {
		glGetNamedFramebufferParameteriv(0, GL_FRAMEBUFFER_DEFAULT_HEIGHT, &height);
	} else {
	}

	return height;
}
