#include "Exception/RuntimeException.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Texture.h"
#include "Renderer/vulkan/internal_object_type.h"
#include "Utils/StringUtil.h"
#include<fmt/core.h>
using namespace fragcore;

void Texture::bind(unsigned int index) {}

void Texture::bindImage(unsigned int index, int level, MapTarget target, Format format) {}

bool Texture::isValid(void) {}

void Texture::setSampler(Sampler *sampler) {}

void Texture::setMipLevel(unsigned int level) {}

void Texture::setFilterMode(FilterMode mode) {}

Texture::FilterMode Texture::getFilterMode(void) {}

void Texture::setWrapMode(Texture::WrapMode mode) {}

Texture::WrapMode Texture::getWrapMode(void) {}

void Texture::setAnisotropic(float anisotropic) {}

float Texture::getAnisotropic(void) const {}

Texture::CompareFunc Texture::getCompare(void) const {}

void Texture::setCompareFunc(CompareFunc compareFunc) {}

void Texture::setMipMapBaseLevel(unsigned int level) { Sampler::setMipMapBaseLevel(level); }

unsigned int Texture::getMipMapBaseLevel(void) const { return Sampler::getMipMapBaseLevel(); }

void Texture::setMipMapBias(float bias) { Sampler::setMipMapBias(bias); }

float Texture::getMipMapBias(float bias) const { return Sampler::getMipMapBias(bias); }

void Texture::setBorderColor(float color) { Sampler::setBorderColor(color); }

float Texture::getBorderColor(void) const { return Sampler::getBorderColor(); }

unsigned int Texture::setMaxLod(unsigned int level) { return Sampler::setMaxLod(level); }

unsigned int Texture::getMaxLod(void) const { return Sampler::getMaxLod(); }

unsigned int Texture::setMinLod(unsigned int level) { return Sampler::setMinLod(level); }

unsigned int Texture::getMinLod(void) const { return Sampler::getMinLod(); }

Texture::Format Texture::getFormat(void) const { return eR8G8B8; }

unsigned int Texture::width(void) {
	VKTextureObject *texture = (VKTextureObject *)this->pdata;
	return texture->desc.width;
}

unsigned int Texture::height(void) {
	VKTextureObject *texture = (VKTextureObject *)this->pdata;
	return texture->desc.height;
}
unsigned int Texture::layers(void) const {
	VKTextureObject *texobj = (VKTextureObject *)this->pdata;
	return texobj->desc.depth;
}

void Texture::resize(int width, int height, Texture::Format format, bool hasMipMap) {}

void *Texture::mapTexture(Format format, unsigned int level) {
	VkResult result;
	VKTextureObject *texture = (VKTextureObject *)this->pdata;
	VulkanCore *vulkanCore = texture->vulkanCore;

	VkDeviceSize mapSize = VK_WHOLE_SIZE;
	// if(size > 0)
	//	mapSize = (VkDeviceSize)size;

	void *data;
	result = vkMapMemory(vulkanCore->device, texture->imageMemory, 0, mapSize, 0, &data);
	if (result != VK_SUCCESS)
		throw RuntimeException(::fmt::format("Failed to map texture memory - %d", result));

	return data;
}

void Texture::unMapTexture(void) {

	VKTextureObject *texture = (VKTextureObject *)this->pdata;
	VulkanCore *vulkanCore = texture->vulkanCore;

	vkUnmapMemory(vulkanCore->device, texture->imageMemory);
}

void Texture::setPixels(Texture::Format format, unsigned int level, const void *pixels, unsigned long size) {}

void *Texture::getPixels(TextureFormat format, unsigned int level, unsigned long *nBytes) {}

void Texture::clear(void) {}

void Texture::setName(const std::string &name) { Object::setName(name); }

intptr_t Texture::getNativePtr(void) const {
	VKTextureObject *texture = (VKTextureObject *)this->pdata;
	return (intptr_t)texture->texture;
}
