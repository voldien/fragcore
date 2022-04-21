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
	typedef struct opengl_shader_object_t {
		unsigned int program;
		unsigned int shader;
	} GLShaderObject;

	/**
	 *
	 */
	typedef struct opengl_geometry_object_t {
		GeometryDesc desc;
		unsigned int mode;
		unsigned int vao;
		unsigned int indicesType;
		Buffer *indicesbuffer;
		Buffer *vertexbuffer;
	} GLGeometryObject;

} // namespace fragcore

// TODO change name to resolve potential symbol name conflict.
/**
 * Helper functions.
 */
extern FVDECLSPEC void addMarkerLabel(unsigned int identifier, unsigned int object, const fragcore::MarkerDebug *debug);

extern FVDECLSPEC unsigned int getWrapMode(fragcore::SamplerDesc::AddressMode mode);

extern FVDECLSPEC unsigned int getFilterMode(fragcore::SamplerDesc::FilterMode mode,
											 fragcore::SamplerDesc::FilterMode mips);

extern FVDECLSPEC unsigned int getCompareMode(fragcore::SamplerDesc::CompareFunc mode);

extern FVDECLSPEC unsigned int getGraphicFormat(fragcore::GraphicFormat graphicFormat);
extern FVDECLSPEC unsigned int getTextureFormat(fragcore::TextureFormat textureFormat, unsigned int *pixelType);

extern FVDECLSPEC unsigned int getTextureFormat(fragcore::TextureDesc::Format format);
extern FVDECLSPEC unsigned int getInternalTextureFormat(fragcore::TextureDesc::Format format, bool sRGB,
														fragcore::TextureDesc::Compression compression,
														fragcore::TextureDesc::Type type);

extern FVDECLSPEC unsigned int getTextureTarget(fragcore::TextureDesc::Target target, int nrSamples);

extern FVDECLSPEC unsigned int getTextureType(fragcore::TextureDesc::Type type);

extern FVDECLSPEC unsigned int getTextureSwizzle(fragcore::TextureDesc::Swizzle swizzle);

extern FVDECLSPEC unsigned int getBufferType(fragcore::BufferDesc::BufferType type);

extern FVDECLSPEC unsigned int getBufferHint(fragcore::BufferDesc::BufferHint hint);

extern FVDECLSPEC unsigned int getPrimitive(fragcore::GeometryDesc::Primitive primitive);

extern FVDECLSPEC unsigned int getAttributeDataType(fragcore::GeometryDesc::AttributeType type);

extern FVDECLSPEC unsigned int getState(fragcore::IRenderer::State state);

extern FVDECLSPEC unsigned int getTextureFilterModeNoMip(fragcore::Texture::FilterMode format);

// FrameBuffer
extern FVDECLSPEC unsigned int getBlendEqu(fragcore::FrameBuffer::BlendEqu equ);

extern FVDECLSPEC unsigned int getBlendFunc(fragcore::FrameBuffer::BlendFunc func);

extern FVDECLSPEC unsigned int getClearBitMask(fragcore::CLEARBITMASK clearbitmask);

// Texture.
extern FVDECLSPEC unsigned int getTextureWrapMode(fragcore::Texture::WrapMode mode);

extern FVDECLSPEC unsigned int getTextureFilterMode(fragcore::Texture::FilterMode mode);

extern FVDECLSPEC unsigned int getTextureCompareMode(fragcore::Texture::CompareFunc compareFunc);

extern FVDECLSPEC unsigned getImageInternalFormat(fragcore::Texture::Format format);

extern FVDECLSPEC unsigned int getAccess(fragcore::Texture::MapTarget target);

extern FVDECLSPEC unsigned int getTextureGLFormat(fragcore::Texture::Format format);

extern FVDECLSPEC fragcore::Texture::Format getTextureFormatUserSpace(unsigned int format);

#endif
