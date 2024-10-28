#include "Image.h"
#include <cstring>
#include <magic_enum.hpp>
#include <utility>

using namespace fragcore;

Image::Image(const unsigned int width, const unsigned int height, const ImageFormat format)
	: Image(width, height, 1, format) {}
Image::Image(const unsigned int width, const unsigned int height, const unsigned int layer, const ImageFormat format)
	: m_width(width), m_height(height), depth(layer), format(format) {
	this->allocateMemory(width, height, depth, format);
}

Image::Image(const Image &other) : Object(other) {
	Object::operator=(other);

	this->allocateMemory(other.width(), other.height(), other.layers(), other.getFormat());
	std::memcpy(this->pixelData, other.pixelData, this->bufferSize);
}
Image::Image(Image &&other) {
	Object::operator=(other);
	this->pixelData = std::exchange(other.pixelData, nullptr);

	this->bufferSize = other.bufferSize;
	this->m_width = other.m_width;
	this->m_height = other.m_height;
	this->depth = other.depth;
	this->format = other.format;
}

Image &Image::operator=(const Image &other) {
	Object::operator=(other);
	this->allocateMemory(other.width(), other.height(), other.layers(), other.getFormat());
	std::memcpy(this->pixelData, other.pixelData, this->bufferSize);
	return *this;
}

Image &Image::operator=(Image &&other) {
	Object::operator=(other);
	this->pixelData = std::exchange(other.pixelData, nullptr);

	this->bufferSize = other.bufferSize;
	this->m_width = other.m_width;
	this->m_height = other.m_height;
	this->depth = other.depth;
	this->format = other.format;
	return *this;
}

Image::~Image() {

	if (this->pixelData != nullptr) {
		free(this->pixelData);
	}
	this->pixelData = nullptr;
	this->bufferSize = 0;
}

Color Image::operator[](unsigned int index) const { return Color::black(); }

Color Image::getColor(unsigned int x_offset, unsigned int y_offset, unsigned int z_offset) const {

	const size_t pixel_index = getPixelMemoryOffset(x_offset, y_offset, z_offset);

	switch (this->getFormat()) {
	case ImageFormat::Alpha8:
	case ImageFormat::R8: {
		const size_t index = pixel_index * (Image::getFormatPixelBitSize(this->getFormat()) / 8);
		uint8_t color = reinterpret_cast<const uint8_t *>(this->pixelData)[index];
		return {static_cast<float>(color / 255.0f), static_cast<float>(color / 255.0f),
				static_cast<float>(color / 255.0f), static_cast<float>(color / 255.0f)};
	}
	case ImageFormat::R16:
	case ImageFormat::R16U:
		return Color::black();
	case ImageFormat::R32:
	case ImageFormat::R32U: {
		const size_t index = pixel_index * (Image::getFormatPixelBitSize(this->getFormat()) / 8);
		uint32_t color = reinterpret_cast<const uint32_t *>(this->pixelData)[index];
		return {static_cast<float>(color), static_cast<float>(color), static_cast<float>(color),
				static_cast<float>(color)};
	}
	case ImageFormat::ARGB4444:
	case ImageFormat::RGBA4444:
		return Color::black();
	case ImageFormat::RGB24:
	case ImageFormat::BGR24: {
		const size_t index = pixel_index * (Image::getFormatPixelBitSize(this->getFormat()) / 8);
		const uint8_t *color = &reinterpret_cast<const uint8_t *>(this->pixelData)[index];
		return {static_cast<float>(color[0] / 255.0f), static_cast<float>(color[1] / 255.0f),
				static_cast<float>(color[2] / 255.0f), 1};
	}
	default:
		return Color::black();
	}
}

void Image::setColor(unsigned int x_offset, unsigned int y_offset, unsigned int z_offset, const Color &color) {

	const size_t pixel_index = getPixelMemoryOffset(x_offset, y_offset, z_offset);

	switch (this->getFormat()) {
	case ImageFormat::Alpha8:
	case ImageFormat::R8: {
		const size_t index = pixel_index * (Image::getFormatPixelBitSize(this->getFormat()) / 8);
		reinterpret_cast<uint8_t *>(this->pixelData)[index] = (uint8_t)color.r() * 255;
	}
	case ImageFormat::R16:
	case ImageFormat::R16U:
		break;
	case ImageFormat::R32:
	case ImageFormat::R32U: {
		const size_t index = pixel_index * (Image::getFormatPixelBitSize(this->getFormat()) / 8);
		reinterpret_cast<uint32_t *>(this->pixelData)[index] = (uint32_t)color.r();
		break;
	}
	case ImageFormat::ARGB4444:
	case ImageFormat::RGBA4444:
		break;
	case ImageFormat::RGB24: {
		const size_t index = pixel_index * (Image::getFormatPixelBitSize(this->getFormat()) / 8);
		uint8_t *rgb24 = &reinterpret_cast<uint8_t *>(this->pixelData)[index];
		rgb24[0] = (255 * color.r());
		rgb24[1] = (255 * color.g());
		rgb24[2] = (255 * color.b());
	}
	case ImageFormat::BGR24: {
		const size_t index = pixel_index * (Image::getFormatPixelBitSize(this->getFormat()) / 8);
		uint8_t *rgb24 = &reinterpret_cast<uint8_t *>(this->pixelData)[index];
		rgb24[0] = (255 * color.b());
		rgb24[1] = (255 * color.g());
		rgb24[2] = (255 * color.r());
		break;
	}
	default:
		break;
	}
}

void *Image::getPixelData() const noexcept { return this->pixelData; }
void Image::setPixelData(void *srcPixelData, const size_t size) {

	if (size > this->getSize()) {
		throw InvalidArgumentException("Source data size is greater than destination data: {} > {}", size, bufferSize);
	}

	mempcpy(this->pixelData, srcPixelData, size);
}

void Image::allocateMemory(const unsigned int width, const unsigned int height, const unsigned depth,
						   const ImageFormat format) {
	this->bufferSize = Image::getTextureByteSize(width, height, depth, format);
	this->pixelData = malloc(this->bufferSize);
	if (this->pixelData == nullptr) {
		throw RuntimeException("Failed to allocate {}", bufferSize);
	}
}

unsigned int Image::getPixelMemoryOffset(const unsigned int x_offset, const unsigned int y_offset,
										 const unsigned int z_offset) const noexcept {
	const unsigned int pixel_index = this->width() * y_offset + x_offset;
	return pixel_index;
}

unsigned int Image::getFormatPixelBitSize(const ImageFormat format) {

	switch (format) {
	case ImageFormat::Alpha8:
	case ImageFormat::R8:
		return 1l * 8l;
	case ImageFormat::R16:
	case ImageFormat::R16U:
		return 1l * 16;
	case ImageFormat::R32:
	case ImageFormat::R32U:
		return 1l * 32;
	case ImageFormat::ARGB4444:
	case ImageFormat::RGBA4444:
		return 4l * 4l;
	case ImageFormat::RGB24:
	case ImageFormat::BGR24:
		return 3l * 8l;
	case ImageFormat::RGBA32:
	case ImageFormat::ARGB32:
	case ImageFormat::BGRA32:
		return 4l * 8l;
	case ImageFormat::RGB565:
		return 5l + 6l + 5l;
	case ImageFormat::DXT1:
		break;
	case ImageFormat::DXT5:
		break;
	case ImageFormat::RHalf:
		return 1l * 16l;
	case ImageFormat::RGHalf:
		return 2l * 16;
	case ImageFormat::RGBAHalf:
		return 4l * 16;
	case ImageFormat::RFloat:
		return 1l * 32;
	case ImageFormat::RGFloat:
		return 2l * 32;
	case ImageFormat::RGBFloat:
		return 3l * 32;
	case ImageFormat::RGBAFloat:
		return 4l * 32;
	case ImageFormat::YUY2:
	default:
		break;
	}

	throw NotSupportedException("None Supported image format: {}", std::string(magic_enum::enum_name(format)));
}

size_t Image::getTextureByteSize(const unsigned int width, const unsigned int height, const unsigned int depth,
								 const ImageFormat format) {

	size_t imageRes = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(depth);

	imageRes = (imageRes * Image::getFormatPixelBitSize(format)) / 8;

	return imageRes;
}