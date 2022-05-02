#include "GLSampler.h"
#include "../Texture.h"
#include "internal_object_type.h"
#include <GL/glew.h>
using namespace fragcore;
GLSampler::~GLSampler() {}

void GLSampler::setFilterMode(FilterMode mode) {

	glSamplerParameteri(this->sampler, GL_TEXTURE_MAG_FILTER, getTextureFilterMode(mode));
	glSamplerParameteri(this->sampler, GL_TEXTURE_MIN_FILTER, getTextureFilterMode(mode));
}

GLSampler::FilterMode GLSampler::getFilterMode() {
	int wrapS;
	int wrapT;

	glGetSamplerParameteriv(this->sampler, GL_TEXTURE_MAG_FILTER, &wrapS);
	glGetSamplerParameteriv(this->sampler, GL_TEXTURE_MIN_FILTER, &wrapT);

	return GLSampler::FilterMode::eBilinear;
}

void GLSampler::setWrapMode(GLSampler::WrapMode mode) {
	unsigned int wrapMode = getTextureWrapMode(mode);

	/*  */
	glSamplerParameteri(this->sampler, GL_TEXTURE_WRAP_S, wrapMode);
	glSamplerParameteri(this->sampler, GL_TEXTURE_WRAP_T, wrapMode);
	glSamplerParameteri(this->sampler, GL_TEXTURE_WRAP_R, wrapMode);
}

GLSampler::WrapMode GLSampler::getWrapMode() {
	int wrapS;
	int wrapT;
	int wrapR;

	glGetSamplerParameteriv(this->sampler, GL_TEXTURE_WRAP_S, &wrapS);
	glGetSamplerParameteriv(this->sampler, GL_TEXTURE_WRAP_T, &wrapT);
	glGetSamplerParameteriv(this->sampler, GL_TEXTURE_WRAP_R, &wrapR);
	if (wrapR == wrapS == wrapT == GL_REPEAT)
		return GLSampler::WrapMode::eRepeat;
	if (wrapR == wrapS == wrapT == GL_MIRRORED_REPEAT)
		return GLSampler::WrapMode::eMirror;
	if (wrapR == wrapS == wrapT == GL_CLAMP_TO_EDGE)
		return GLSampler::WrapMode::eClamp;

	return GLSampler::WrapMode::eClamp;
}

void GLSampler::setAnisotropic(float anisotropy) {

	glSamplerParameterf(this->sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
}

float GLSampler::getAnisotropic() const {
	float ansio;

	glGetSamplerParameterfv(this->sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, &ansio);
	return ansio;
}

GLSampler::CompareFunc GLSampler::getCompare() const { return GLSampler::CompareFunc::NoCompare; }

void GLSampler::setCompareFunc(CompareFunc compareFunc) {

	GLenum glCompareFunc = getTextureCompareMode(compareFunc);
	// TODO resolve.
	// glGetSamplerParameterfv(this->sampler, GL_TEXTURE_COMPARE_FUNC, glCompareFunc);
}

void GLSampler::setMipMapBaseLevel(unsigned int level) {}

unsigned int GLSampler::getMipMapBaseLevel() const { return 0; }

void GLSampler::setMipMapBias(float bias) {}

float GLSampler::getMipMapBias(float bias) const { return 0; }

void GLSampler::setBorderColor(float color) {}

float GLSampler::getBorderColor() const { return 0; }

unsigned int GLSampler::setMaxLod(unsigned int level) { return 0; }

unsigned int GLSampler::getMaxLod() const { return 0; }

unsigned int GLSampler::setMinLod(unsigned int level) { return 0; }

unsigned int GLSampler::getMinLod() const { return 0; }

void GLSampler::setName(const std::string &name) {
	Object::setName(name);

	/*  Update the marker.  */
	MarkerDebug marker = {};
	marker.markerName = name.c_str();
	// addMarkerLabel((const OpenGLCore *) getRenderer()->getData(), GL_SAMPLER, this->sampler, &marker);
}

intptr_t GLSampler::getNativePtr() const { return 0; }
