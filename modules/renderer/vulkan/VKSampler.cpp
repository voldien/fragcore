#include "internal_object_type.h"
#include <../Sampler.h>
#include <../Texture.h>
using namespace fragcore;

Sampler::~Sampler() {}

void Sampler::setFilterMode(FilterMode mode) {}

Sampler::FilterMode Sampler::getFilterMode() {}

void Sampler::setWrapMode(Sampler::WrapMode mode) {}

Sampler::WrapMode Sampler::getWrapMode() {}

void Sampler::setAnisotropic(float anisotropy) {}

float Sampler::getAnisotropic() const { return 0; }

Sampler::CompareFunc Sampler::getCompare() const {}
void Sampler::setCompareFunc(CompareFunc compareFunc) {}

void Sampler::setMipMapBaseLevel(unsigned int level) {}
unsigned int Sampler::getMipMapBaseLevel() const { return 0; }

void Sampler::setMipMapBias(float bias) {}
float Sampler::getMipMapBias(float bias) const { return 0; }

void Sampler::setBorderColor(float color) {}
float Sampler::getBorderColor() const { return 0; }

unsigned int Sampler::setMaxLod(unsigned int level) { return 0; }
unsigned int Sampler::getMaxLod() const {
	return 0;
}

unsigned int Sampler::setMinLod(unsigned int level) {}
unsigned int Sampler::getMinLod() const { return 0; }

void Sampler::setName(const std::string &name) {}

intptr_t Sampler::getNativePtr() const { return 0; }
