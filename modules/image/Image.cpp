#include "Image.h"
#include <magic_enum.hpp>

using namespace fragcore;

Image::Image(const unsigned int width, const unsigned int height, const TextureFormat format) : Image(width, height, 1, format) {}
Image::Image(const unsigned int width, const unsigned int height, const unsigned int layer, const TextureFormat format)
	: w(width), h(height), depth(layer), format(format) {
	this->allocateMemory(width, height, depth, format);
}

Image::~Image() {
	free(this->pixelData);
	this->pixelData = nullptr;
}

void *Image::getPixelData() const noexcept { return this->pixelData; }
void Image::setPixelData(void *srcPixelData, const size_t size) {

	if (size > this->getSize()) {
		throw InvalidArgumentException("Source data size is greater than destination data: {} > {}", size, bufferSize);
	}

	mempcpy(this->pixelData, srcPixelData, size);
}

void Image::allocateMemory(unsigned int width, unsigned int height, unsigned depth, TextureFormat format) {
	this->bufferSize = Image::getTextureSize(width, height, depth, format);
	this->pixelData = malloc(this->bufferSize);
	if (this->pixelData == nullptr) {
		throw RuntimeException("Failed to allocate {}", bufferSize);
	}
}

size_t Image::getFormatPixelSize(TextureFormat format) {

	switch (format) {
	case TextureFormat::Alpha8:
	case TextureFormat::R8:
		return 1l * 8l;
	case TextureFormat::ARGB4444:
	case TextureFormat::RGBA4444:
		return 4l * 4l;
	case TextureFormat::RGB24:
	case TextureFormat::BGR24:
		return 3l * 8l;
	case TextureFormat::RGBA32:
	case TextureFormat::ARGB32:
	case TextureFormat::BGRA32:
		return 4l * 8l;
	case TextureFormat::RGB565:
		return 5l + 6l + 5l;
	case TextureFormat::R16:
		return 1l * 16l;
	case TextureFormat::DXT1:
		break;
	case TextureFormat::DXT5:
		break;
	case TextureFormat::RHalf:
		return 1l * 16l;
	case TextureFormat::RGHalf:
		return 2l * 16;
	case TextureFormat::RGBAHalf:
		return 4l * 16;
	case TextureFormat::RFloat:
		return 1l * 32;
	case TextureFormat::RGFloat:
		return 2l * 32;
	case TextureFormat::RGBFloat:
		return 3l * 32;
	case TextureFormat::RGBAFloat:
		return 4l * 32;
	case TextureFormat::YUY2:
	default:
		break;
	}
	throw NotSupportedException("Invalid texture format: {}", std::string(magic_enum::enum_name(format)));
}

size_t Image::getTextureSize(unsigned int width, unsigned int height, unsigned depth, TextureFormat format) {

	size_t imageRes = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(depth);

	imageRes = (imageRes * Image::getFormatPixelSize(format)) / 8;

	return imageRes;
}