/*
 *	FragCore - Fragment Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FRAGCORE_GL_HELPER_H_
#define _FRAGCORE_GL_HELPER_H_ 1
#include "RenderDesc.h"
#include "Texture.h"
#include <FragCore.h>
#include <GL/glew.h>

/*	Debug verification if any of the OpenGL calls fails.	*/
#if _NDEBUG
#define FVALIDATE_GL_CALL(callX)                                                                                       \
	{                                                                                                                  \
		callX;                                                                                                         \
		GLenum gl_error = glGetError();                                                                                \
		if (gl_error != GL_NO_ERROR) {                                                                                 \
			throw cxxexcept::RuntimeException("{} {} {} - {}", __FILE__, __LINE__, _err, gluErrorString(gl_error));    \
		}                                                                                                              \
	}
#else
#define FVALIDATE_GL_CALL(callX) callX
#endif

namespace fragcore {

	extern FVDECLSPEC void checkError();
	extern FVDECLSPEC void resetErrorFlag();
	extern FVDECLSPEC bool validateExistingProgram();
	extern FVDECLSPEC std::string getProgramValidateString();

	/**
	 *
	 */
	class FVDECLSPEC GLHelper {
	  public:
		/**
		 * Helper functions.
		 */
		static void addMarkerLabel(unsigned int identifier, unsigned int object, const fragcore::MarkerDebug *debug);

		/*	*/
		static unsigned int getWrapMode(const fragcore::TextureWrappingMode mode);

		/*	*/
		static unsigned int getFilterMode(const fragcore::TextureFilterMode mode,
										  const fragcore::TextureFilterMode mips);

		/*	*/
		static unsigned int getCompareMode(const fragcore::TextureCompareFunc mode);

		/*	*/
		static unsigned int getGraphicFormat(const fragcore::GraphicFormat graphicFormat);

		/*	*/
		static unsigned int getTextureFormat(const fragcore::ImageFormat textureFormat, unsigned int *pixelType);



		/*	*/
		static unsigned int getTextureTarget(const fragcore::TextureDesc::TextureTarget target,
											 const int nrSamples = 1);


		/*	*/
		static unsigned int getTextureSwizzle(const fragcore::TextureSwizzle swizzle);

		/*	*/
		static unsigned int getBufferType(const fragcore::BufferDesc::BufferType type);

		/*	*/
		static unsigned int getBufferHint(const fragcore::BufferDesc::BufferHint hint);

		/*	*/
		static unsigned int getPrimitive(const Primitive primitive);

		/*	*/
		static unsigned int getAttributeDataType(const fragcore::GeometryDesc::AttributeType type);

		/*	*/
		// static unsigned int getState(const fragcore::State state);

		/*	*/
		static unsigned int getTextureFilterModeNoMip(fragcore::Texture::FilterMode format);

		/*	*/
		static unsigned int getBlendEqu(fragcore::BlendEqu equ);

		/*	*/
		static unsigned int getBlendFunc(fragcore::BlendFunc func);

		// TODO: remove
		//  Texture.
		static unsigned int getTextureWrapMode(const fragcore::Texture::WrapMode mode);

		/*	*/
		static unsigned int getTextureFilterMode(fragcore::Texture::FilterMode mode);

		/*	*/
		static unsigned int getTextureCompareMode(fragcore::Texture::CompareFunc compareFunc);

		/*	*/
		static unsigned getImageInternalFormat(fragcore::Texture::Format format);

		/*	*/
		static unsigned int getAccess(fragcore::Texture::MapTarget target);

		/*	*/
		static unsigned int getTextureGLFormat(fragcore::Texture::Format format);

		/*	*/
		static fragcore::Texture::Format getTextureFormatUserSpace(unsigned int format);
	};

} // namespace fragcore

#endif
