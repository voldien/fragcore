
#include "VKTexture.h"
#include "VKRenderInterface.h"
#include "internal_object_type.h"
#include <../IRenderer.h>
using namespace fragcore;

VKTexture ::~VKTexture() {}

void VKTexture::bind(unsigned int index) {}

void VKTexture::bindImage(unsigned int index, int level, MapTarget target, Format format) {}

bool VKTexture::isValid() { return true; }

void VKTexture::setSampler(Sampler *sampler) {}

void VKTexture::setMipLevel(unsigned int level) {}

void VKTexture::setFilterMode(FilterMode mode) {}

VKTexture::FilterMode VKTexture::getFilterMode() {}

void VKTexture::setWrapMode(VKTexture::WrapMode mode) {}

VKTexture::WrapMode VKTexture::getWrapMode() {}

void VKTexture::setAnisotropic(float anisotropic) {}

float VKTexture::getAnisotropic() const {}

VKTexture::CompareFunc VKTexture::getCompare() const {}

void VKTexture::setCompareFunc(CompareFunc compareFunc) {}

void VKTexture::setMipMapBaseLevel(unsigned int level) {}

unsigned int VKTexture::getMipMapBaseLevel() const {}

void VKTexture::setMipMapBias(float bias) {}

float VKTexture::getMipMapBias(float bias) const {}

void VKTexture::setBorderColor(float color) {}

float VKTexture::getBorderColor() const {}

unsigned int VKTexture::setMaxLod(unsigned int level) {}

unsigned int VKTexture::getMaxLod() const {}

unsigned int VKTexture::setMinLod(unsigned int level) {}

unsigned int VKTexture::getMinLod() const { ; }

VKTexture::Format VKTexture::getFormat() const { return eR8G8B8; }

unsigned int VKTexture::width() { return this->desc.width; }

unsigned int VKTexture::height() { return this->desc.height; }
unsigned int VKTexture::layers() const { return this->desc.depth; }

void VKTexture::resize(int width, int height, VKTexture::Format format, bool hasMipMap) {}

void *VKTexture::mapTexture(Format format, unsigned int level) {
	VkResult result;

	VkDeviceSize mapSize = VK_WHOLE_SIZE;
	// if(size > 0)
	//	mapSize = (VkDeviceSize)size;

	void *data;
	VKRenderInterface *renderer = static_cast<VKRenderInterface *>(this->getRenderer());
	result = vkMapMemory(renderer->getDevice()->getHandle(), this->imageMemory, 0, mapSize, 0, &data);
	VKS_VALIDATE(result);

	return data;
}

void VKTexture::unMapTexture() {

	VKRenderInterface *renderer = static_cast<VKRenderInterface *>(this->getRenderer());

	vkUnmapMemory(renderer->getDevice()->getHandle(), this->imageMemory);
}

void VKTexture::setPixels(Texture::Format format, unsigned int level, const void *pixels, unsigned long size) {}

void *VKTexture::getPixels(TextureFormat format, unsigned int level, unsigned long *nBytes) {}

void VKTexture::clear() {}

void VKTexture::setName(const std::string &name) { Object::setName(name); }

intptr_t VKTexture::getNativePtr() const { return reinterpret_cast<intptr_t>(this->texture); }
