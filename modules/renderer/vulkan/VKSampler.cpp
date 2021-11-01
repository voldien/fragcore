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

float Sampler::getAnisotropic() const {}

Sampler::CompareFunc Sampler::getCompare() const {}
void Sampler::setCompareFunc(CompareFunc compareFunc) {}

void Sampler::setMipMapBaseLevel(unsigned int level) {}
unsigned int Sampler::getMipMapBaseLevel() const {}

void Sampler::setMipMapBias(float bias) {}
float Sampler::getMipMapBias(float bias) const {}

void Sampler::setBorderColor(float color) {}
float Sampler::getBorderColor() const {}

unsigned int Sampler::setMaxLod(unsigned int level) {}
unsigned int Sampler::getMaxLod() const {}

unsigned int Sampler::setMinLod(unsigned int level) {}
unsigned int Sampler::getMinLod() const {}

void Sampler::setName(const std::string &name) {}

intptr_t Sampler::getNativePtr() const { return 0; }
