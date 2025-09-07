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
#ifndef _FRAGCORE_INTERNAL_OBJECT_TYPE_H_
#define _FRAGCORE_INTERNAL_OBJECT_TYPE_H_ 1
#include "../Buffer.h"
#include "../IRenderer.h"
#include "../RenderDesc.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include <../RendererWindow.h>
#include <GL/glew.h>

namespace fragcore {

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
		static unsigned int getTextureFormat(const fragcore::TextureDesc::DataPixelFormat format);
		static unsigned int getInternalTextureFormat(fragcore::TextureDesc::DataPixelFormat format, bool sRGB,
													 fragcore::TextureDesc::Compression compression,
													 fragcore::TextureDesc::PixelDataType type);

		/*	*/
		static unsigned int getTextureTarget(const fragcore::TextureDesc::TextureTarget target,
											 const int nrSamples = 1);

		/*	*/
		static unsigned int getTextureType(const fragcore::TextureDesc::PixelDataType type);

		/*	*/
		static unsigned int getTextureSwizzle(const fragcore::TextureSwizzle swizzle);

		/*	*/
		static unsigned int getBufferType(const fragcore::BufferDesc::BufferType type);

		/*	*/
		static unsigned int getBufferHint(const fragcore::BufferDesc::BufferHint hint);

		/*	*/
		static unsigned int getPrimitive(const fragcore::GeometryDesc::Primitive primitive);

		/*	*/
		static unsigned int getAttributeDataType(const fragcore::GeometryDesc::AttributeType type);

		/*	*/
		static unsigned int getState(const fragcore::IRenderer::State state);

		/*	*/
		static unsigned int getTextureFilterModeNoMip(fragcore::Texture::FilterMode format);

		/*	*/
		static unsigned int getBlendEqu(fragcore::BlendEqu equ);

		/*	*/
		static unsigned int getBlendFunc(fragcore::BlendFunc func);

		/*	*/
		static unsigned int getClearBitMask(fragcore::CLEARBITMASK clearbitmask);

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
