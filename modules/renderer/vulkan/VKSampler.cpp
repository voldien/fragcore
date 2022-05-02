#include "VKSampler.h"
#include "VKRenderInterface.h"
#include "internal_object_type.h"
using namespace fragcore;

VKSampler::~VKSampler() {
	VKRenderInterface *renderInterface = getRenderer<VKRenderInterface>();
	vkDestroySampler(renderInterface->getDevice()->getHandle(), sampler, nullptr);
}

void VKSampler::setFilterMode(FilterMode mode) {}

VKSampler::FilterMode VKSampler::getFilterMode() {
	int wrapS;
	int wrapT;

	return VKSampler::FilterMode::eBilinear;
}

void VKSampler::setWrapMode(VKSampler::WrapMode mode) {}

VKSampler::WrapMode VKSampler::getWrapMode() {
	int wrapS;
	int wrapT;
	int wrapR;
}

void VKSampler::setAnisotropic(float anisotropy) {}

float VKSampler::getAnisotropic() const {
	float ansio;

	return ansio;
}

VKSampler::CompareFunc VKSampler::getCompare() const { return VKSampler::CompareFunc::NoCompare; }

void VKSampler::setCompareFunc(CompareFunc compareFunc) {

	// TODO resolve.
	// glGetSamplerParameterfv(this->sampler, GL_TEXTURE_COMPARE_FUNC, glCompareFunc);
}

void VKSampler::setMipMapBaseLevel(unsigned int level) {}

unsigned int VKSampler::getMipMapBaseLevel() const { return 0; }

void VKSampler::setMipMapBias(float bias) {}

float VKSampler::getMipMapBias(float bias) const { return 0; }

void VKSampler::setBorderColor(float color) {}

float VKSampler::getBorderColor() const { return 0; }

unsigned int VKSampler::setMaxLod(unsigned int level) { return 0; }

unsigned int VKSampler::getMaxLod() const { return 0; }

unsigned int VKSampler::setMinLod(unsigned int level) { return 0; }

unsigned int VKSampler::getMinLod() const { return 0; }

void VKSampler::setName(const std::string &name) {}

intptr_t VKSampler::getNativePtr() const { return 0; }
