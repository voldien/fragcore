/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAG_CORE_INTERNAL_OBJECT_TYPE_H_
#define _FRAG_CORE_INTERNAL_OBJECT_TYPE_H_ 1
#include "../Buffer.h"
#include "../IRenderer.h"
#include "../RenderDesc.h"
#include <../RendererWindow.h>
#include <GL/glew.h>

namespace fragcore {

	/**
	 *
	 */
	using GLShaderObject = struct opengl_shader_object_t {
		unsigned int program;
		unsigned int shader;
	};

	/**
	 *
	 */
	using GLGeometryObject = struct opengl_geometry_object_t {
		GeometryDesc desc;
		unsigned int mode;
		unsigned int vao;
		unsigned int indicesType;
		Buffer *indicesbuffer;
		Buffer *vertexbuffer;
	};

	class FVDECLSPEC GLHelper {
	  public:
		/**
		 * Helper functions.
		 */
		static void addMarkerLabel(unsigned int identifier, unsigned int object, const fragcore::MarkerDebug *debug);

		static unsigned int getWrapMode(fragcore::SamplerDesc::AddressMode mode);

		static unsigned int getFilterMode(fragcore::SamplerDesc::FilterMode mode,
										  fragcore::SamplerDesc::FilterMode mips);

		static unsigned int getCompareMode(fragcore::SamplerDesc::CompareFunc mode);

		static unsigned int getGraphicFormat(fragcore::GraphicFormat graphicFormat);
		static unsigned int getTextureFormat(fragcore::TextureFormat textureFormat, unsigned int *pixelType);

		static unsigned int getTextureFormat(fragcore::TextureDesc::Format format);
		static unsigned int getInternalTextureFormat(fragcore::TextureDesc::Format format, bool sRGB,
													 fragcore::TextureDesc::Compression compression,
													 fragcore::TextureDesc::Type type);

		static unsigned int getTextureTarget(fragcore::TextureDesc::Target target, int nrSamples);

		static unsigned int getTextureType(fragcore::TextureDesc::Type type);

		static unsigned int getTextureSwizzle(fragcore::TextureDesc::Swizzle swizzle);

		static unsigned int getBufferType(fragcore::BufferDesc::BufferType type);

		static unsigned int getBufferHint(fragcore::BufferDesc::BufferHint hint);

		static unsigned int getPrimitive(fragcore::GeometryDesc::Primitive primitive);

		static unsigned int getAttributeDataType(fragcore::GeometryDesc::AttributeType type);

		static unsigned int getState(fragcore::IRenderer::State state);

		static unsigned int getTextureFilterModeNoMip(fragcore::Texture::FilterMode format);

		// FrameBuffer
		static unsigned int getBlendEqu(fragcore::FrameBuffer::BlendEqu equ);

		static unsigned int getBlendFunc(fragcore::FrameBuffer::BlendFunc func);

		static unsigned int getClearBitMask(fragcore::CLEARBITMASK clearbitmask);

		// Texture.
		static unsigned int getTextureWrapMode(fragcore::Texture::WrapMode mode);

		static unsigned int getTextureFilterMode(fragcore::Texture::FilterMode mode);

		static unsigned int getTextureCompareMode(fragcore::Texture::CompareFunc compareFunc);

		static unsigned getImageInternalFormat(fragcore::Texture::Format format);

		static unsigned int getAccess(fragcore::Texture::MapTarget target);

		static unsigned int getTextureGLFormat(fragcore::Texture::Format format);

		static fragcore::Texture::Format getTextureFormatUserSpace(unsigned int format);
	};
} // namespace fragcore

#endif
