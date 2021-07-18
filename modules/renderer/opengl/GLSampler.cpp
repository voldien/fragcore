#include "Renderer/Sampler.h"
#include "Renderer/Texture.h"
#include "internal_object_type.h"
#include <GL/glew.h>
using namespace fragcore;
Sampler::~Sampler(void) {}

void Sampler::setFilterMode(FilterMode mode) {
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();

	glSamplerParameteri(sampler->sampler, GL_TEXTURE_MAG_FILTER, getTextureFilterMode(mode));
	glSamplerParameteri(sampler->sampler, GL_TEXTURE_MIN_FILTER, getTextureFilterMode(mode));
}

Sampler::FilterMode Sampler::getFilterMode(void) {
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();
	int wrapS;
	int wrapT;

	glGetSamplerParameteriv(sampler->sampler, GL_TEXTURE_MAG_FILTER, &wrapS);
	glGetSamplerParameteriv(sampler->sampler, GL_TEXTURE_MIN_FILTER, &wrapT);
}

void Sampler::setWrapMode(Sampler::WrapMode mode) {
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();
	unsigned int wrapMode = getTextureWrapMode(mode);

	/*  */
	glSamplerParameteri(sampler->sampler, GL_TEXTURE_WRAP_S, wrapMode);
	glSamplerParameteri(sampler->sampler, GL_TEXTURE_WRAP_T, wrapMode);
	glSamplerParameteri(sampler->sampler, GL_TEXTURE_WRAP_R, wrapMode);
}

Sampler::WrapMode Sampler::getWrapMode(void) {
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
}

void Sampler::setAnisotropic(float anisotropy) {
	GLSamplerObject *sample = (GLSamplerObject *)this->getObject();
	glSamplerParameterf(sample->sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
}

float Sampler::getAnisotropic(void) const {
	float ansio;
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();
	glGetSamplerParameterfv(sampler->sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, &ansio);
	return ansio;
}

Sampler::CompareFunc Sampler::getCompare(void) const {
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();
}

void Sampler::setCompareFunc(CompareFunc compareFunc) {
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();
	GLenum glCompareFunc = getTextureCompareMode(compareFunc);
	// TODO resolve.
	// glGetSamplerParameterfv(sampler->sampler, GL_TEXTURE_COMPARE_FUNC, glCompareFunc);
}

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

void Sampler::setName(const std::string &name) {
	GLSamplerObject *sampler = (GLSamplerObject *)this->getObject();
	Object::setName(name);

	/*  Update the marker.  */
	MarkerDebug marker = {};
	marker.markerName = name.c_str();
	// addMarkerLabel((const OpenGLCore *) getRenderer()->getData(), GL_SAMPLER, sampler->sampler, &marker);
}

intptr_t Sampler::getNativePtr(void) const { return 0; }
