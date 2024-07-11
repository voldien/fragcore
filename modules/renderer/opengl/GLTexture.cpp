
#include "GLTexture.h"
#include "internal_object_type.h"
#include <GL/glew.h>

using namespace fragcore;

// TODO add for checking by the target
static int getCurrentTexture2D() {
	GLint whichID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &whichID);

	// TODO add for active texture.
	// glGetIntegerv(GL_ACTIVE_TEXTURE, &whichID);
	return whichID;
}

GLTexture ::~GLTexture() {}

void GLTexture::bind(unsigned int index) {

	/*  */
	if (glBindTextureUnit) {
		glBindTextureUnit(index, this->texture);
	} else {
		glActiveTexture(GL_TEXTURE0_ARB + index);
		glBindTexture(this->target, this->texture);
	}

	/*  Bind sampler if set.    */
	if (this->sampler) {

		// glBindSampler(index, sample->sampler);
	}
}

void GLTexture::bindImage(unsigned int index, int level, MapTarget target, Format format) {

	if (glBindImageTexture) {

		GLenum access = GLHelper::getAccess(target);
		GLenum gformat = GLHelper::getImageInternalFormat(format);

		glBindImageTexture(index, this->texture, level, GL_FALSE, 0, access, gformat);
	} else
		throw RuntimeException("glBindImageTexture not supported");
}

bool GLTexture::isValid() { return glIsTexture(this->texture) == GL_TRUE; }

void GLTexture::setSampler(Sampler *sampler) {

	/*  Verify if valid sampler object. */
	// if (glIsSampler(glSamplerObject->sampler)) {
	// 	this->sampler = sampler;
	// 	// Increase the reference count.
	// 	sampler->increment();
	// } else
	// 	throw RuntimeException("Invalid sampler object.");
}

void GLTexture::setMipLevel(unsigned int level) {

	if (glTextureParameteri) {
		glTextureParameteri(this->texture, GL_TEXTURE_LOD_BIAS, level);
	} else {
		unsigned int c = getCurrentTexture2D();
		this->bind(0);
	}
}

void GLTexture::setFilterMode(FilterMode mode) {

	// TODO improve.
	GLuint filter = GLHelper::getTextureFilterMode(mode);
	if (glTextureParameteri) {

		glTextureParameteri(this->texture, GL_TEXTURE_MAG_FILTER, filter);
		glTextureParameteri(this->texture, GL_TEXTURE_MIN_FILTER, filter);

	} else {
		unsigned int c = getCurrentTexture2D();
		this->bind(0);

		glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, filter);

		glBindTexture(GL_TEXTURE_2D, c);
	}
}

GLTexture::FilterMode GLTexture::getFilterMode() {

	// TODO add logic.
	GLint magFilter;
	GLint minFilter;
	if (glGetTextureParameteriv) {

		glGetTextureParameteriv(this->texture, GL_TEXTURE_MAG_FILTER, &magFilter);
		glGetTextureParameteriv(this->texture, GL_TEXTURE_MIN_FILTER, &minFilter);
	} else {
		unsigned int c = getCurrentTexture2D();
		this->bind(0);

		glGetTexParameteriv(this->target, GL_TEXTURE_MAG_FILTER, &magFilter);
		glGetTexParameteriv(this->target, GL_TEXTURE_MIN_FILTER, &minFilter);
	}
	return GLTexture::FilterMode::eBilinear;
}

void GLTexture::setWrapMode(GLTexture::WrapMode mode) {

	unsigned int wrapMode = GLHelper::getTextureWrapMode(mode);

	if (glTextureParameteri) {
		glTextureParameteri(this->texture, GL_TEXTURE_WRAP_S, wrapMode);
		glTextureParameteri(this->texture, GL_TEXTURE_WRAP_T, wrapMode);
		glTextureParameteri(this->texture, GL_TEXTURE_WRAP_R, wrapMode);
	} else {
		unsigned int c = getCurrentTexture2D();
		this->bind(0);

		glTexParameteri(this->target, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(this->target, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(this->target, GL_TEXTURE_WRAP_R, wrapMode);
		this->bind(c);
	}
}

GLTexture::WrapMode GLTexture::getWrapMode() {

	GLint wrapS;
	GLint wrapT;
	GLint wrapR;

	if (glGetTextureParameteriv) {

		glGetTextureParameteriv(this->texture, GL_TEXTURE_WRAP_S, &wrapS);
		glGetTextureParameteriv(this->texture, GL_TEXTURE_WRAP_T, &wrapT);
		glGetTextureParameteriv(this->texture, GL_TEXTURE_WRAP_R, &wrapR);

	} else {
		unsigned int c = getCurrentTexture2D();
		this->bind(0);

		glGetTexParameteriv(this->target, GL_TEXTURE_WRAP_S, &wrapS);
		glGetTexParameteriv(this->target, GL_TEXTURE_WRAP_T, &wrapT);
		glGetTexParameteriv(this->target, GL_TEXTURE_WRAP_R, &wrapR);
	}

	if (wrapR == GL_REPEAT && wrapS == GL_REPEAT && wrapT == GL_REPEAT) {
		return Sampler::WrapMode::eRepeat;
	}
	if (wrapR == GL_MIRRORED_REPEAT && wrapS == GL_MIRRORED_REPEAT && wrapT == GL_MIRRORED_REPEAT) {
		return Sampler::WrapMode::eMirror;
	}
	if (wrapR == GL_CLAMP_TO_EDGE && wrapS == GL_CLAMP_TO_EDGE && wrapT == GL_CLAMP_TO_EDGE) {
		return Sampler::WrapMode::eClamp;
	}
	return Sampler::WrapMode::eClamp;
}

void GLTexture::setAnisotropic(float anisotropic) {

	if (glTextureParameterf) {
		glTexParameterf(this->texture, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic);
	} else {
		GLint prevTex = getCurrentTexture2D();
		glBindTexture(this->target, this->texture);
		glTexParameterf(this->target, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic);
		glBindTexture(this->target, prevTex);
	}
}

float GLTexture::getAnisotropic() const {

	float anisotropic;
	if (glGetTextureParameterfv) {
		glGetTextureParameterfv(this->texture, GL_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropic);
	} else {
		GLint prevTex = getCurrentTexture2D();
		glBindTexture(this->target, this->texture);
		glGetTexParameterfv(this->target, GL_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropic);
		glBindTexture(this->target, prevTex);
	}
	return anisotropic;
}

GLTexture::CompareFunc GLTexture::getCompare() const {

	if (glGetTextureParameterfv) {
		return GLTexture::CompareFunc::NoCompare;
	} else {
		return GLTexture::CompareFunc::NoCompare;
	}
}

void GLTexture::setCompareFunc(CompareFunc compareFunc) {

	GLenum glCompareFunc = GLHelper::getTextureCompareMode(compareFunc);
	if (glTextureParameterf) {
		glTexParameterf(this->texture, GL_TEXTURE_COMPARE_FUNC, glCompareFunc);
	} else {
		GLint prevTex = getCurrentTexture2D();
		glBindTexture(this->target, this->texture);
		glTexParameterf(this->target, GL_TEXTURE_MAX_ANISOTROPY_EXT, glCompareFunc);
		glBindTexture(this->target, prevTex);
	}
}

void GLTexture::setMipMapBaseLevel(unsigned int level) {}

unsigned int GLTexture::getMipMapBaseLevel() const { return 0; }

void GLTexture::setMipMapBias(float bias) {}

float GLTexture::getMipMapBias(float bias) const { return 0; }

void GLTexture::setBorderColor(float color) {}

float GLTexture::getBorderColor() const { return 0; }

unsigned int GLTexture::setMaxLod(unsigned int level) { return 0; }

unsigned int GLTexture::getMaxLod() const { return 0; }

unsigned int GLTexture::setMinLod(unsigned int level) { return 0; }

unsigned int GLTexture::getMinLod() const { return 0; }

GLTexture::Format GLTexture::getFormat() const {

	// glGetTexLevelParameterfv
	// GL_TEXTURE_INTERNAL_FORMAT
	// GL_TEXTURE_BUFFER_SIZE
	// GL_TEXTURE_COMPRESSED_IMAGE_SIZE
	/*  Get the internal fmt::format.    */
	switch (this->desc.internalformat) {
	case TextureDesc::Format::RGB:
		return eR8G8B8;
	case TextureDesc::Format::RGBA:
		return eR8G8B8A8;
	default:
		throw InvalidArgumentException("");
	}
}

unsigned int GLTexture::width() { return this->desc.width; }

unsigned int GLTexture::height() { return this->desc.height; }

unsigned int GLTexture::layers() const { return this->desc.depth; }

void GLTexture::resize(int width, int height, GLTexture::Format format, bool hasMipMap) {

	// this->bind(0);

	// GL_TEXTURE_IMMUTABLE_FORMAT'
	glTexImage2D(this->target, 0, GLHelper::getImageInternalFormat(format), width, height, 0, GL_RED, GL_UNSIGNED_BYTE,
				 nullptr);
	// glTextureStorage2D(this->texture, 1, getImageInternalFormat(GLTexture::Format::eR8G8B8A8), width,
	//               height); // TODO add only if mipmaps is used with nullptr pixel object.

	this->desc.width = width;
	this->desc.height = height;
}

void *GLTexture::mapTexture(Format format, unsigned int level) {

	unsigned int width = this->width();
	unsigned int height = this->height();
	unsigned int depth = 1;
	const unsigned int pixelSize = 4;

	// TODO add compute size of the buffer.
	const unsigned int bufferSize = (width * height * depth * pixelSize) / (level + 1);
	// GL_TEXTURE_COMPRESSED_IMAGE_SIZE
	GLenum _format = GLHelper::getTextureGLFormat(format);
	GLenum _type = GL_UNSIGNED_BYTE;

	glGenBuffersARB(1, &this->pbo);
	glBindBufferARB(GL_PIXEL_PACK_BUFFER, this->pbo);
	glBufferDataARB(GL_PIXEL_PACK_BUFFER, bufferSize, nullptr, GL_DYNAMIC_COPY);

	if (glGetTextureImage) {
		glGetTextureImage(this->texture, level, _format, _type, bufferSize, nullptr);
	} else {
		/* TODO resolve texture type.   */
		glBindTexture(this->target, this->texture);
		glGetTexImage(this->target, level, _format, _type, nullptr);
	}

	// TODO add PBO buffer array in the OpenGL object.

	// TODo add range version for perhaps better.
	void *map = glMapBufferARB(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);

	return map;
}

// glTextureSubImage2D
// glTexSubImage2D
void GLTexture::unMapTexture() {

	glUnmapBufferARB(GL_PIXEL_PACK_BUFFER);
	if (glInvalidateBufferData)
		glInvalidateBufferData(this->pbo);
	glDeleteBuffers(1, &this->pbo);
	this->pbo = 0;
}

void GLTexture::setPixels(Format format, unsigned int level, const void *pixels, unsigned long size) {

	// TODO improve.
	GLenum gformat = GLHelper::getTextureGLFormat(format);
	GLenum type = GL_UNSIGNED_BYTE;

	// GL_PIXEL_UNPACK_BUFFER

	if (glTextureSubImage2D) {
		glTextureSubImage2D(this->texture, level, 0, 0, this->desc.width, this->desc.height, gformat, GL_UNSIGNED_BYTE,
							pixels);
		// Update mipmaps.
		glGenerateTextureMipmap(this->texture);

	} else {
		glBindTexture(this->target, this->texture);

		glTexSubImage2D(this->target, level, 0, 0, this->desc.width, this->desc.height, gformat, GL_UNSIGNED_BYTE,
						pixels);

		// Update mipmaps.
		glGenerateMipmap(this->target);
		glBindTexture(this->target, 0);
	}
}

void *GLTexture::getPixels(TextureFormat format, unsigned int level, unsigned long *nBytes) {

	unsigned int width = this->width();
	unsigned int height = this->height();
	unsigned int depth = 1;
	const unsigned int pixelSize = 4;

	// TODO add compute size of the buffer.

	// GL_TEXTURE_COMPRESSED_IMAGE_SIZE
	GLenum _type = 0;
	GLenum _format = GLHelper::getTextureFormat(format, &type);

	const unsigned int bufferSize = (width * height * depth * pixelSize) / (level + 1);
	void *pbuffer = malloc(bufferSize);

	if (glGetTextureImage) {
		glGetTextureImage(this->texture, level, _format, _type, bufferSize, pbuffer);
	} else {
		/* TODO resolve texture type.   */
		GLint prevTex = getCurrentTexture2D();
		glBindTexture(this->target, this->texture);
		glGetTexImage(this->target, level, _format, _type, pbuffer);
		glBindTexture(this->target, prevTex);
	}

	return pbuffer;
}

void GLTexture::clear() {

	if (glClearTexImage) {
		glClearTexImage(this->texture, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	} else {
	}
}

intptr_t GLTexture::getNativePtr() const { return this->texture; }

void GLTexture::setName(const std::string &name) {
	Object::setName(name);

	/*  Update the marker.  */
	MarkerDebug marker = {};
	marker.markerName = name.c_str();
	// addMarkerLabel((const OpenGLCore *) getRenderer()->getData(), GL_TEXTURE, this->texture, &marker);
}
