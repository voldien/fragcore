#include "internal_object_type.h"
#include <Renderer/Sampler.h>
#include <Renderer/Texture.h>
using namespace fragcore;

Sampler::~Sampler(void) {}

void Sampler::setFilterMode(FilterMode mode) {}

Sampler::FilterMode Sampler::getFilterMode(void) {}

void Sampler::setWrapMode(Sampler::WrapMode mode) {}

Sampler::WrapMode Sampler::getWrapMode(void) {}

void Sampler::setAnisotropic(float anisotropy) {}

float Sampler::getAnisotropic(void) const {}

Sampler::CompareFunc Sampler::getCompare(void) const {}
void Sampler::setCompareFunc(CompareFunc compareFunc) {}

void Sampler::setMipMapBaseLevel(unsigned int level) {}
unsigned int Sampler::getMipMapBaseLevel(void) const {}

void Sampler::setMipMapBias(float bias) {}
float Sampler::getMipMapBias(float bias) const {}

void Sampler::setBorderColor(float color) {}
float Sampler::getBorderColor(void) const {}

unsigned int Sampler::setMaxLod(unsigned int level) {}
unsigned int Sampler::getMaxLod(void) const {}

unsigned int Sampler::setMinLod(unsigned int level) {}
unsigned int Sampler::getMinLod(void) const {}

void Sampler::setName(const std::string &name) {}

intptr_t Sampler::getNativePtr(void) const { return 0; }
