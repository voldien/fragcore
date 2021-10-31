#include "Renderer/IRenderer.h"
#include "Renderer/Texture.h"
#include <Renderer/opencl/internal_object_type.h>

using namespace fragcore;

void Texture::bind(unsigned int index) {}

bool Texture::isValid(void) {}

void Texture::setMipLevel(unsigned int level) {}

void Texture::setFilterMode(FilterMode mode) {}

Texture::FilterMode Texture::getFilterMode(void) {}

void Texture::setWrapMode(Texture::WrapMode mode) {}

Texture::WrapMode Texture::getWrapMode(void) {}

void Texture::setAnisotropic(float anisotropic) {}

float Texture::getAnisotropic(void) const {}

Texture::Format Texture::getFormat(void) const { return eR8G8B8; }

unsigned int Texture::width(void) {}

unsigned int Texture::height(void) {}

void *Texture::mapTexture(Format format, unsigned int leve) {
	CLTextureObject *textureObject = (CLTextureObject *)this->pdata;

	// void* pdata = clEnqueueMapImage()
}

void Texture::unMapTexture(void) {}

void Texture::setPixels(Texture::Format format, unsigned int level, const void *pixels, unsigned long size) {}

void *Texture::getPixels(TextureFormat format, unsigned int level, unsigned long *nBytes) {}

intptr_t Texture::getNativePtr(void) const { return 0; }
