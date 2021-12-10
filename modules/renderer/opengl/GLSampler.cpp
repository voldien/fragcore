#include "../Sampler.h"
#include "../Texture.h"
#include "internal_object_type.h"
#include <GL/glew.h>
using namespace fragcore;
Sampler::~Sampler() {}

void Sampler::setFilterMode(FilterMode mode) {
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();

	glSamplerParameteri(sampler->sampler, GL_TEXTURE_MAG_FILTER, getTextureFilterMode(mode));
	glSamplerParameteri(sampler->sampler, GL_TEXTURE_MIN_FILTER, getTextureFilterMode(mode));
}

Sampler::FilterMode Sampler::getFilterMode() {
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();
	int wrapS;
	int wrapT;

	glGetSamplerParameteriv(sampler->sampler, GL_TEXTURE_MAG_FILTER, &wrapS);
	glGetSamplerParameteriv(sampler->sampler, GL_TEXTURE_MIN_FILTER, &wrapT);

	return Sampler::FilterMode::eBilinear;
}

void Sampler::setWrapMode(Sampler::WrapMode mode) {
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();
	unsigned int wrapMode = getTextureWrapMode(mode);

	/*  */
	glSamplerParameteri(sampler->sampler, GL_TEXTURE_WRAP_S, wrapMode);
	glSamplerParameteri(sampler->sampler, GL_TEXTURE_WRAP_T, wrapMode);
	glSamplerParameteri(sampler->sampler, GL_TEXTURE_WRAP_R, wrapMode);
}

Sampler::WrapMode Sampler::getWrapMode() {
	int wrapS;
	int wrapT;
	int wrapR;
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();
	glGetSamplerParameteriv(sampler->sampler, GL_TEXTURE_WRAP_S, &wrapS);
	glGetSamplerParameteriv(sampler->sampler, GL_TEXTURE_WRAP_T, &wrapT);
	glGetSamplerParameteriv(sampler->sampler, GL_TEXTURE_WRAP_R, &wrapR);
	if (wrapR == wrapS == wrapT == GL_REPEAT)
		return Sampler::WrapMode::eRepeat;
	if (wrapR == wrapS == wrapT == GL_MIRRORED_REPEAT)
		return Sampler::WrapMode::eMirror;
	if (wrapR == wrapS == wrapT == GL_CLAMP_TO_EDGE)
		return Sampler::WrapMode::eClamp;

	return Sampler::WrapMode::eClamp;
}

void Sampler::setAnisotropic(float anisotropy) {
	GLSamplerObject *sample = (GLSamplerObject *)this->getObject();
	glSamplerParameterf(sample->sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
}

float Sampler::getAnisotropic() const {
	float ansio;
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();
	glGetSamplerParameterfv(sampler->sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, &ansio);
	return ansio;
}

Sampler::CompareFunc Sampler::getCompare() const { GLSamplerObject *sampler = (GLSamplerObject *)this->getObject(); }

void Sampler::setCompareFunc(CompareFunc compareFunc) {
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();
	GLenum glCompareFunc = getTextureCompareMode(compareFunc);
	// TODO resolve.
	// glGetSamplerParameterfv(sampler->sampler, GL_TEXTURE_COMPARE_FUNC, glCompareFunc);
}

void Sampler::setMipMapBaseLevel(unsigned int level) {}

unsigned int Sampler::getMipMapBaseLevel() const { return 0; }

void Sampler::setMipMapBias(float bias) {}

float Sampler::getMipMapBias(float bias) const { return 0; }

void Sampler::setBorderColor(float color) {}

float Sampler::getBorderColor() const { return 0; }

unsigned int Sampler::setMaxLod(unsigned int level) { return 0; }

unsigned int Sampler::getMaxLod() const { return 0; }

unsigned int Sampler::setMinLod(unsigned int level) { return 0; }

unsigned int Sampler::getMinLod() const { return 0; }

void Sampler::setName(const std::string &name) {
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();
	Object::setName(name);

	/*  Update the marker.  */
	MarkerDebug marker = {};
	marker.markerName = name.c_str();
	// addMarkerLabel((const OpenGLCore *) getRenderer()->getData(), GL_SAMPLER, sampler->sampler, &marker);
}

intptr_t Sampler::getNativePtr() const { return 0; }
