#include "Image.h"
#include <magic_enum.hpp>

using namespace fragcore;

Image::Image(unsigned int width, unsigned int height, TextureFormat format) : Image(width, height, 1, format) {}
Image::Image(unsigned int width, unsigned int height, unsigned int layer, TextureFormat format)
	: w(width), h(height), depth(layer), format(format) {
	this->allocateMemory(width, height, depth, format);
}

Image::~Image() {
	free(this->pixelData);
	this->pixelData = nullptr;
}

void *Image::getPixelData() const noexcept { return this->pixelData; }
void Image::setPixelData(void *srcPixelData, size_t size) {

	if (size > this->getSize()) {
		throw InvalidArgumentException("Source data size is greater than destination data: {} > {}", size, bufferSize);
	}
	mempcpy(this->pixelData, srcPixelData, size);
}

void Image::allocateMemory(unsigned int width, unsigned int height, unsigned depth, TextureFormat format) {
	this->bufferSize = this->getTextureSize(width, height, depth, format);
	this->pixelData = malloc(this->bufferSize);
	if (this->pixelData == nullptr) {
		throw RuntimeException("Failed to allocate {}", bufferSize);
	}
}

size_t Image::getFormatPixelSize(TextureFormat format) {

	switch (format) {
	case TextureFormat::Alpha8:
	case TextureFormat::R8:
		return 1 * 8;
	case TextureFormat::ARGB4444:
	case TextureFormat::RGBA4444:
		return 4 * 4;
	case TextureFormat::RGB24:
	case TextureFormat::BGR24:
		return 3 * 8;
	case TextureFormat::RGBA32:
	case TextureFormat::ARGB32:
	case TextureFormat::BGRA32:
		return 4 * 8;
	case TextureFormat::RGB565:
		return 5 + 6 + 5;
	case TextureFormat::R16:
		return 1 * 16;
	case TextureFormat::DXT1:
		break;
	case TextureFormat::DXT5:
		break;
	case TextureFormat::RHalf:
		return 1 * 16;
	case TextureFormat::RGHalf:
		return 2 * 16;
	case TextureFormat::RGBAHalf:
		return 4 * 16;
	case TextureFormat::RFloat:
		return 1 * 32;
	case TextureFormat::RGFloat:
		return 2 * 32;
	case TextureFormat::RGBFloat:
		return 3 * 32;
	case TextureFormat::RGBAFloat:
		return 4 * 32;
	case TextureFormat::YUY2:
	default:
		break;
	}
	throw NotSupportedException("Invalid texture format: {}", std::string(magic_enum::enum_name(format)));
}

size_t Image::getTextureSize(unsigned int width, unsigned int height, unsigned depth, TextureFormat format) {

	size_t imageRes = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(depth);

	imageRes = (imageRes * getFormatPixelSize(format)) / 8;

	return imageRes;
}