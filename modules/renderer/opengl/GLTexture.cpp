
#include "Renderer/IRenderer.h"
#include "Renderer/Sampler.h"
#include "Renderer/Texture.h"
#include "internal_object_type.h"
#include <GL/glew.h>

using namespace fragcore;

// TODO add for checking by the target
static int getCurrentTexture(void) {
	GLint whichID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &whichID);

	// TODO add for active texture.
	// glGetIntegerv(GL_ACTIVE_TEXTURE, &whichID);
	return whichID;
}

void Texture::bind(unsigned int index) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;

	/*  */
	if (glBindTextureUnit) {
		glBindTextureUnit(index, texobj->texture);
	} else {
		glActiveTexture(GL_TEXTURE0_ARB + index);
		glBindTexture(texobj->target, texobj->texture);
	}

	/*  Bind sampler if set.    */
	if (texobj->sampler) {
		GLSamplerObject *sample = (GLSamplerObject *)this->getObject();
		glBindSampler(index, sample->sampler);
	}
}

void Texture::bindImage(unsigned int index, int level, MapTarget target, Format format) {
	GLTextureObject *texobj = (GLTextureObject *)this->getObject();
	if (glBindImageTexture) {

		GLenum access = getAccess(target);
		GLenum gformat = getImageInternalFormat(format);

		glBindImageTexture(index, texobj->texture, level, GL_FALSE, 0, access, gformat);
	} else
		throw RuntimeException("glBindImageTexture not supported");
}

bool Texture::isValid(void) {
	GLTextureObject *texobj = (GLTextureObject *)this->getObject();
	return glIsTexture(texobj->texture) == GL_TRUE;
}

void Texture::setSampler(Sampler *sampler) {
	GLTextureObject *texobj = (GLTextureObject *)this->getObject();
	GLSamplerObject *glSamplerObject = (GLSamplerObject *)sampler->getObject();

	/*  Verify if valid sampler object. */
	if (glIsSampler(glSamplerObject->sampler)) {
		texobj->sampler = sampler;
		// Increase the reference count.
		sampler->increment();
	} else
		throw RuntimeException("Invalid sampler object.");
}

void Texture::setMipLevel(unsigned int level) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;
	if (glTextureParameteri) {
		glTextureParameteri(texobj->texture, GL_TEXTURE_LOD_BIAS, level);
	} else {
		unsigned int c = getCurrentTexture();
		this->bind(0);
	}
}

void Texture::setFilterMode(FilterMode mode) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;
	// TODO improve.
	GLuint filter = getTextureFilterMode(mode);
	if (glTextureParameteri) {

		glTextureParameteri(texobj->texture, GL_TEXTURE_MAG_FILTER, filter);
		glTextureParameteri(texobj->texture, GL_TEXTURE_MIN_FILTER, filter);

	} else {
		unsigned int c = getCurrentTexture();
		this->bind(0);

		glTexParameteri(texobj->target, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(texobj->target, GL_TEXTURE_MIN_FILTER, filter);
	}
}

Texture::FilterMode Texture::getFilterMode(void) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;

	// TODO add logic.
	GLint magFilter;
	GLint minFilter;
	if (glGetTextureParameteriv) {

		glGetTextureParameteriv(texobj->texture, GL_TEXTURE_MAG_FILTER, &magFilter);
		glGetTextureParameteriv(texobj->texture, GL_TEXTURE_MIN_FILTER, &minFilter);
	} else {
		unsigned int c = getCurrentTexture();
		this->bind(0);

		glGetTexParameteriv(texobj->target, GL_TEXTURE_MAG_FILTER, &magFilter);
		glGetTexParameteriv(texobj->target, GL_TEXTURE_MIN_FILTER, &minFilter);
	}
}

void Texture::setWrapMode(Texture::WrapMode mode) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;
	unsigned int wrapMode = getTextureWrapMode(mode);

	if (glTextureParameteri) {
		glTextureParameteri(texobj->texture, GL_TEXTURE_WRAP_S, wrapMode);
		glTextureParameteri(texobj->texture, GL_TEXTURE_WRAP_T, wrapMode);
		glTextureParameteri(texobj->texture, GL_TEXTURE_WRAP_R, wrapMode);
	} else {
		unsigned int c = getCurrentTexture();
		this->bind(0);

		glTexParameteri(texobj->target, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(texobj->target, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(texobj->target, GL_TEXTURE_WRAP_R, wrapMode);
		this->bind(c);
	}
}

Texture::WrapMode Texture::getWrapMode(void) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;

	GLint wrapS;
	GLint wrapT;
	GLint wrapR;

	if (glGetTextureParameteriv) {

		glGetTextureParameteriv(texobj->texture, GL_TEXTURE_WRAP_S, &wrapS);
		glGetTextureParameteriv(texobj->texture, GL_TEXTURE_WRAP_T, &wrapT);
		glGetTextureParameteriv(texobj->texture, GL_TEXTURE_WRAP_R, &wrapR);

	} else {
		unsigned int c = getCurrentTexture();
		this->bind(0);

		glGetTexParameteriv(texobj->target, GL_TEXTURE_WRAP_S, &wrapS);
		glGetTexParameteriv(texobj->target, GL_TEXTURE_WRAP_T, &wrapT);
		glGetTexParameteriv(texobj->target, GL_TEXTURE_WRAP_R, &wrapR);
	}

	if (wrapR == wrapS == wrapT == GL_REPEAT)
		return Sampler::WrapMode::eRepeat;
	if (wrapR == wrapS == wrapT == GL_MIRRORED_REPEAT)
		return Sampler::WrapMode::eMirror;
	if (wrapR == wrapS == wrapT == GL_CLAMP_TO_EDGE)
		return Sampler::WrapMode::eClamp;
}

void Texture::setAnisotropic(float anisotropic) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;
	if (glTextureParameterf) {
		glTexParameterf(texobj->texture, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic);
	} else {
		GLint prevTex = getCurrentTexture();
		glBindTexture(texobj->target, texobj->texture);
		glTexParameterf(texobj->target, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic);
		glBindTexture(texobj->target, prevTex);
	}
}

float Texture::getAnisotropic(void) const {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;
	float anisotropic;
	if (glGetTextureParameterfv) {
		glGetTextureParameterfv(texobj->texture, GL_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropic);
	} else {
		GLint prevTex = getCurrentTexture();
		glBindTexture(texobj->target, texobj->texture);
		glGetTexParameterfv(texobj->target, GL_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropic);
		glBindTexture(texobj->target, prevTex);
	}
	return anisotropic;
}

Texture::CompareFunc Texture::getCompare(void) const {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;
	if (glGetTextureParameterfv) {

	} else {
	}
}

void Texture::setCompareFunc(CompareFunc compareFunc) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;
	GLenum glCompareFunc = getTextureCompareMode(compareFunc);
	if (glTextureParameterf) {
		glTexParameterf(texobj->texture, GL_TEXTURE_COMPARE_FUNC, glCompareFunc);
	} else {
		GLint prevTex = getCurrentTexture();
		glBindTexture(texobj->target, texobj->texture);
		glTexParameterf(texobj->target, GL_TEXTURE_MAX_ANISOTROPY_EXT, glCompareFunc);
		glBindTexture(texobj->target, prevTex);
	}
}

void Texture::setMipMapBaseLevel(unsigned int level) { GLTextureObject *texobj = (GLTextureObject *)this->pdata; }

unsigned int Texture::getMipMapBaseLevel(void) const { GLTextureObject *texobj = (GLTextureObject *)this->pdata; }

void Texture::setMipMapBias(float bias) { GLTextureObject *texobj = (GLTextureObject *)this->pdata; }

float Texture::getMipMapBias(float bias) const { GLTextureObject *texobj = (GLTextureObject *)this->pdata; }

void Texture::setBorderColor(float color) { GLTextureObject *texobj = (GLTextureObject *)this->pdata; }

float Texture::getBorderColor(void) const { GLTextureObject *texobj = (GLTextureObject *)this->pdata; }

unsigned int Texture::setMaxLod(unsigned int level) { GLTextureObject *texobj = (GLTextureObject *)this->pdata; }

unsigned int Texture::getMaxLod(void) const { GLTextureObject *texobj = (GLTextureObject *)this->pdata; }

unsigned int Texture::setMinLod(unsigned int level) { GLTextureObject *texobj = (GLTextureObject *)this->pdata; }

unsigned int Texture::getMinLod(void) const { GLTextureObject *texobj = (GLTextureObject *)this->pdata; }

Texture::Format Texture::getFormat(void) const {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;

	// glGetTexLevelParameterfv
	// GL_TEXTURE_INTERNAL_FORMAT
	// GL_TEXTURE_BUFFER_SIZE
	// GL_TEXTURE_COMPRESSED_IMAGE_SIZE
	/*  Get the internal fmt::format.    */
	switch (texobj->desc.internalformat) {
	case TextureDesc::eRGB:
		return eR8G8B8;
	case TextureDesc::eRGBA:
		return eR8G8B8A8;
	default:
		throw InvalidArgumentException("");
	}
}

unsigned int Texture::width(void) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;
	return texobj->desc.width;
}

unsigned int Texture::height(void) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;
	return texobj->desc.height;
}

unsigned int Texture::layers(void) const {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;
	return texobj->desc.depth;
}

void Texture::resize(int width, int height, Texture::Format format, bool hasMipMap) {
	GLTextureObject *texobj = (GLTextureObject *)this->getObject();

	// this->bind(0);

	// GL_TEXTURE_IMMUTABLE_FORMAT'
	glTexImage2D(texobj->target, 0, getImageInternalFormat(format), width, height, 0, GL_RED, GL_UNSIGNED_BYTE,
				 nullptr);
	// glTextureStorage2D(texobj->texture, 1, getImageInternalFormat(Texture::Format::eR8G8B8A8), width,
	//               height); // TODO add only if mipmaps is used with nullptr pixel object.

	texobj->desc.width = width;
	texobj->desc.height = height;
}

void *Texture::mapTexture(Format format, unsigned int level) {
	GLTextureObject *texobj = (GLTextureObject *)this->getObject();
	unsigned int width = this->width();
	unsigned int height = this->height();
	unsigned int depth = 1;
	const unsigned int pixelSize = 4;

	// TODO add compute size of the buffer.
	const unsigned int bufferSize = (width * height * depth * pixelSize) / (level + 1);
	// GL_TEXTURE_COMPRESSED_IMAGE_SIZE
	GLenum _format = getTextureGLFormat(format);
	GLenum _type = GL_UNSIGNED_BYTE;

	glGenBuffersARB(1, &texobj->pbo);
	glBindBufferARB(GL_PIXEL_PACK_BUFFER, texobj->pbo);
	glBufferDataARB(GL_PIXEL_PACK_BUFFER, bufferSize, nullptr, GL_DYNAMIC_COPY);

	if (glGetTextureImage) {
		glGetTextureImage(texobj->texture, level, _format, _type, bufferSize, nullptr);
	} else {
		/* TODO resolve texture type.   */
		glBindTexture(texobj->target, texobj->texture);
		glGetTexImage(texobj->target, level, _format, _type, nullptr);
	}

	// TODO add PBO buffer array in the OpenGL object.

	// TODo add range version for perhaps better.
	void *map = glMapBufferARB(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);

	return map;
}

// glTextureSubImage2D
// glTexSubImage2D
void Texture::unMapTexture(void) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;

	glUnmapBufferARB(GL_PIXEL_PACK_BUFFER);
	if (glInvalidateBufferData)
		glInvalidateBufferData(texobj->pbo);
	glDeleteBuffers(1, &texobj->pbo);
	texobj->pbo = 0;
}

void Texture::setPixels(Format format, unsigned int level, const void *pixels, unsigned long size) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;

	// TODO improve.
	GLenum gformat = getTextureGLFormat(format);
	GLenum type = GL_UNSIGNED_BYTE;

	// GL_PIXEL_UNPACK_BUFFER

	if (glTextureSubImage2D) {
		glTextureSubImage2D(texobj->texture, level, 0, 0, texobj->desc.width, texobj->desc.height, gformat,
							GL_UNSIGNED_BYTE, pixels);
		// Update mipmaps.
		glGenerateTextureMipmap(texobj->texture);

	} else {
		glBindTexture(texobj->target, texobj->texture);

		glTexSubImage2D(texobj->target, level, 0, 0, texobj->desc.width, texobj->desc.height, gformat, GL_UNSIGNED_BYTE,
						pixels);

		// Update mipmaps.
		glGenerateMipmap(texobj->target);
		glBindTexture(texobj->target, 0);
	}
}

void *Texture::getPixels(TextureFormat format, unsigned int level, unsigned long *nBytes) {
	GLTextureObject *texobj = (GLTextureObject *)this->getObject();
	unsigned int width = this->width();
	unsigned int height = this->height();
	unsigned int depth = 1;
	const unsigned int pixelSize = 4;

	// TODO add compute size of the buffer.

	// GL_TEXTURE_COMPRESSED_IMAGE_SIZE
	GLenum _type;
	GLenum _format = getTextureFormat(format, &type);

	const unsigned int bufferSize = (width * height * depth * pixelSize) / (level + 1);
	void *pbuffer = malloc(bufferSize);

	if (glGetTextureImage) {
		glGetTextureImage(texobj->texture, level, _format, _type, bufferSize, pbuffer);
	} else {
		/* TODO resolve texture type.   */
		GLint prevTex = getCurrentTexture();
		glBindTexture(texobj->target, texobj->texture);
		glGetTexImage(texobj->target, level, _format, _type, pbuffer);
		glBindTexture(texobj->target, prevTex);
	}

	return pbuffer;
}

void Texture::clear(void) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;

	if (glClearTexImage)
		glClearTexImage(texobj->texture, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	else {
	}
}

intptr_t Texture::getNativePtr(void) const {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;
	return texobj->texture;
}

void Texture::setName(const std::string &name) {
	GLTextureObject *texobj = (GLTextureObject *)this->pdata;
	Object::setName(name);

	/*  Update the marker.  */
	MarkerDebug marker = {};
	marker.markerName = name.c_str();
	// addMarkerLabel((const OpenGLCore *) getRenderer()->getData(), GL_TEXTURE, texobj->texture, &marker);
}
