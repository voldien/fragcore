#include "DefaultFrameBufferTexture.h"
#include "GLRendererInterface.h"
#include "Renderer/IRenderer.h"
#include "Renderer/RenderObject.h"
#include "Renderer/RendererWindow.h"
#include "Renderer/Texture.h"
#include "internal_object_type.h"
#include <Exception/InvalidArgumentException.h>
#include <GL/glew.h>
#include <stdexcept>

using namespace fragcore;

FrameBufferTexture::FrameBufferTexture(void) {
	// this->setName("Default frambuffer back color attachment");
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

	/*  */
	glReadBuffer(GL_BACK_LEFT);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, this->pbo);

	/*  */
	glReadPixels(0, 0, width(), height(), _format, GL_UNSIGNED_BYTE, nullptr);
	return glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY_ARB);
}

void FrameBufferTexture::unMapTexture(void) {
	if (glUnmapNamedBuffer)
		glUnmapNamedBuffer(pbo);
	else
		glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_ARB);
}

Texture::Format FrameBufferTexture::getFormat(void) const { return Texture::eR8G8B8A8; }

void FrameBufferTexture::bind(unsigned int index) {}

bool FrameBufferTexture::isValid(void) { return true; }

void FrameBufferTexture::setMipLevel(unsigned int level) {}

void FrameBufferTexture::setFilterMode(FilterMode mode) {}

Texture::FilterMode FrameBufferTexture::getFilterMode(void) { return Texture::eNearest; }

void FrameBufferTexture::setWrapMode(WrapMode mode) {}

Texture::WrapMode FrameBufferTexture::getWrapMode(void) { return Texture::eClamp; }

void FrameBufferTexture::setAnisotropic(float anisotropic) {}

float FrameBufferTexture::getAnisotropic(void) const { return 0.0f; }

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

unsigned int FrameBufferTexture::width(void) {
	int width;
	int height;
	OpenGLCore *glcore = (OpenGLCore *)this->getRenderer<GLRendererInterface>()->getData();
	fragcore::Window *window = (fragcore::Window *)glcore->drawwindow;
	window->getSize(&width, &height);
	// SDL_GetWindowSize(glcore->drawwindow, &width, &height);
	return width;
}

unsigned int FrameBufferTexture::height(void) {
	int width;
	int height;
	OpenGLCore *glcore = (OpenGLCore *)this->getRenderer()->getData();
	fragcore::Window *window = (fragcore::Window *)glcore->drawwindow;
	window->getSize(&width, &height);
	// SDL_GetWindowSize(glcore->drawwindow, &width, &height);
	return height;
}
