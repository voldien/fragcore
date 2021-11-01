#include "Renderer/IRenderer.h"
#include "Renderer/Texture.h"
#include <Renderer/opencl/internal_object_type.h>

using namespace fragcore;

void Texture::bind(unsigned int index) {}

bool Texture::isValid() {}

void Texture::setMipLevel(unsigned int level) {}

void Texture::setFilterMode(FilterMode mode) {}

Texture::FilterMode Texture::getFilterMode() {}

void Texture::setWrapMode(Texture::WrapMode mode) {}

Texture::WrapMode Texture::getWrapMode() {}

void Texture::setAnisotropic(float anisotropic) {}

float Texture::getAnisotropic() const {}

Texture::Format Texture::getFormat() const { return eR8G8B8; }

unsigned int Texture::width() {}

unsigned int Texture::height() {}

void *Texture::mapTexture(Format format, unsigned int leve) {
	CLTextureObject *textureObject = (CLTextureObject *)this->pdata;

	// void* pdata = clEnqueueMapImage()
}

void Texture::unMapTexture() {}

void Texture::setPixels(Texture::Format format, unsigned int level, const void *pixels, unsigned long size) {}

void *Texture::getPixels(TextureFormat format, unsigned int level, unsigned long *nBytes) {}

intptr_t Texture::getNativePtr() const { return 0; }
