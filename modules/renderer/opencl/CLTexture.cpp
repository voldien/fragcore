#include "CLTexture.h"

using namespace fragcore;

void CLTexture::bind(unsigned int index) {}

bool CLTexture::isValid() {}

void CLTexture::setMipLevel(unsigned int level) {}

void CLTexture::setFilterMode(FilterMode mode) {}

CLTexture::FilterMode CLTexture::getFilterMode() {}

void CLTexture::setWrapMode(CLTexture::WrapMode mode) {}

CLTexture::WrapMode CLTexture::getWrapMode() {}

void CLTexture::setAnisotropic(float anisotropic) {}

float CLTexture::getAnisotropic() const {}

CLTexture::Format CLTexture::getFormat() const { return eR8G8B8; }

unsigned int CLTexture::width() { return 0; }

unsigned int CLTexture::height() { return 0; }

void *CLTexture::mapCLTexture(Format format, unsigned int leve) {

	// void* pdata = clEnqueueMapImage()
}

void CLTexture::unMapCLTexture() {}

void CLTexture::setPixels(CLTexture::Format format, unsigned int level, const void *pixels, unsigned long size) {}

void *CLTexture::getPixels(CLTextureFormat format, unsigned int level, unsigned long *nBytes) {}

intptr_t CLTexture::getNativePtr() const { return 0; }
