#include "../IRenderer.h"
#include "GLHelper.h"
#include "GraphicFormat.h"
#include "RenderDesc.h"
#include "internal_object_type.h"
#include <GL/glew.h>
#include <fmt/core.h>
#include <magic_enum.hpp>
using namespace fragcore;

/*  Assign marker for debugging.    */
// GL_MAX_LABEL_LENGTH
void GLHelper::addMarkerLabel(unsigned int identifier, unsigned int object, const MarkerDebug *debug) {
	if (identifier != 0) {
		// glObjectPtrLabel
		int maxLabelLength;
		glGetIntegerv(GL_MAX_LABEL_LENGTH, &maxLabelLength);
		checkError();
		if (debug->markerName && glObjectLabel) {
			glObjectLabel(identifier, object, -1, debug->markerName);
			checkError();
		}
	}
}

unsigned int GLHelper::getWrapMode(const TextureWrappingMode mode) {
	switch (mode) {
	case TextureWrappingMode::Repeat:
		return GL_REPEAT;
	case TextureWrappingMode::RepeatMirror:
		return GL_MIRRORED_REPEAT;
	case TextureWrappingMode::Clamp:
		return GL_CLAMP_TO_EDGE;
	case TextureWrappingMode::ClampBorder:
		return GL_CLAMP_TO_BORDER;
	case TextureWrappingMode::NoAddressMode:
	default:
		throw InvalidArgumentException("Invalid address mode - {}", magic_enum::enum_name(mode));
	}
}

unsigned int GLHelper::getFilterMode(const fragcore::TextureFilterMode mode, const fragcore::TextureFilterMode mips) {
	if (mips == TextureFilterMode::NoFilterMode) {
		switch (mode) {
		case TextureFilterMode::Linear:
			return GL_LINEAR;
		case TextureFilterMode::Nearset:
			return GL_NEAREST;
		default:
			break;
		}
	} else {
		switch (mode) {
		case TextureFilterMode::Linear:
			switch (mips) {
			case TextureFilterMode::Linear:
				return GL_LINEAR_MIPMAP_LINEAR;
			case TextureFilterMode::Nearset:
				return GL_LINEAR_MIPMAP_NEAREST;
			case TextureFilterMode::NoFilterMode:
			default:
				break;
			}
			break;
		case TextureFilterMode::Nearset:
			switch (mips) {
			case TextureFilterMode::Linear:
				return GL_NEAREST_MIPMAP_LINEAR;
			case TextureFilterMode::Nearset:
				return GL_NEAREST_MIPMAP_NEAREST;
			case TextureFilterMode::NoFilterMode:
			default:
				break;
			}
			break;
		case TextureFilterMode::NoFilterMode:
		default:
			break;
		}
	}
	throw InvalidArgumentException("Invalid filter mode : {}", magic_enum::enum_name(mode));
}

unsigned int GLHelper::getCompareMode(const TextureCompareFunc mode) {
	switch (mode) {
	case TextureCompareFunc::LessEqual:
		return GL_LEQUAL;
	case TextureCompareFunc::GreaterEqual:
		return GL_GEQUAL;
	case TextureCompareFunc::Less:
		return GL_LESS;
	case TextureCompareFunc::Greater:
		return GL_GREATER;
	case TextureCompareFunc::Equal:
		return GL_EQUAL;
	case TextureCompareFunc::NotEqual:
		return GL_NOTEQUAL;
	case TextureCompareFunc::Always:
		return GL_ALWAYS;
	case TextureCompareFunc::Never:
		return GL_NEVER;
	default:
		throw InvalidArgumentException("Invalid address mode : {}", magic_enum::enum_name(mode));
	}
}

unsigned int GLHelper::getGraphicFormat(const GraphicFormat graphicFormat) {
	switch (graphicFormat) {
	case GraphicFormat::R8_SRGB:
		return GL_SLUMINANCE8;
	case GraphicFormat::R8G8_SRGB:
		return GL_SLUMINANCE8_ALPHA8;
	case GraphicFormat::R8G8B8_SRGB:
		return GL_SRGB8;
	case GraphicFormat::R8G8B8A8_SRGB:
		return GL_SRGB8_ALPHA8;
	case GraphicFormat::R8_UNorm:
		return GL_RED_INTEGER;
	case GraphicFormat::R8G8_UNorm:
		return GL_RG8;
	case GraphicFormat::R8G8B8_UNorm:
		return GL_RGB8;
	case GraphicFormat::R8G8B8A8_UNorm:
		return GL_RGBA8;
	case GraphicFormat::R8_SNorm:
		return GL_R8_SNORM;
	case GraphicFormat::R8G8_SNorm:
		return GL_RG8_SNORM;
	case GraphicFormat::R8G8B8_SNorm:
		return GL_RGB8_SNORM;
	case GraphicFormat::R8G8B8A8_SNorm:
		return GL_RGBA8_SNORM;
	case GraphicFormat::R8_UInt:
		return GL_R8UI;
	case GraphicFormat::R8G8_UInt:
		return GL_RG8UI;
	case GraphicFormat::R8G8B8_UInt:
		return GL_RGB8UI;
	case GraphicFormat::R8G8B8A8_UInt:
		return GL_RGBA8UI;
	case GraphicFormat::R8_SInt:
		return GL_R8I;
	case GraphicFormat::R8G8_SInt:
		return GL_RG8I;
	case GraphicFormat::R8G8B8_SInt:
		return GL_RGB8I;
	case GraphicFormat::R8G8B8A8_SInt:
		return GL_RGB8I;
	case GraphicFormat::R16_UNorm:
		return GL_R16_EXT;
	case GraphicFormat::R16G16_UNorm:
		return GL_RG16_EXT;
	case GraphicFormat::R16G16B16_UNorm:
		return GL_RGB16_EXT;
	case GraphicFormat::R16G16B16A16_UNorm:
		return GL_RGBA16_EXT;
	case GraphicFormat::R16_SNorm:
		return GL_R16_SNORM;
	case GraphicFormat::R16G16_SNorm:
		return GL_RG16_SNORM;
	case GraphicFormat::R16G16B16_SNorm:
		return GL_RGB16_SNORM;
	case GraphicFormat::R16G16B16A16_SNorm:
		return GL_RGBA16_SNORM;
	case GraphicFormat::R16_UInt:
		return GL_R16;
	case GraphicFormat::R16G16_UInt:
		return GL_RG16;
	case GraphicFormat::R16G16B16_UInt:
		return GL_RGB16;
	case GraphicFormat::R16G16B16A16_UInt:
		return GL_RGBA16;
	case GraphicFormat::R16_SInt:

	case GraphicFormat::R16G16_SInt:

	case GraphicFormat::R16G16B16_SInt:

	case GraphicFormat::R16G16B16A16_SInt:

	case GraphicFormat::R32_UInt:
	case GraphicFormat::R32G32_UInt:

	case GraphicFormat::R32G32B32_UInt:

	case GraphicFormat::R32G32B32A32_UInt:

	case GraphicFormat::R32_SInt:
	case GraphicFormat::R32G32_SInt:

	case GraphicFormat::R32G32B32_SInt:

	case GraphicFormat::R32G32B32A32_SInt:

	case GraphicFormat::R16_SFloat:
		return GL_R16F;
	case GraphicFormat::R16G16_SFloat:
		return GL_RG16F;
	case GraphicFormat::R16G16B16_SFloat:
		return GL_RGB16F;
	case GraphicFormat::R16G16B16A16_SFloat:
		return GL_RGBA16F;
	case GraphicFormat::R32_SFloat:
		return GL_R32F;
	case GraphicFormat::R32G32_SFloat:
		return GL_RG32F;
	case GraphicFormat::R32G32B32_SFloat:
		return GL_RGB32F;
	case GraphicFormat::R32G32B32A32_SFloat:
		return GL_RGBA32F;
	case GraphicFormat::B8G8R8_SRGB:
	case GraphicFormat::B8G8R8A8_SRGB:
	case GraphicFormat::B8G8R8_UNorm:

	case GraphicFormat::B8G8R8A8_UNorm:
	case GraphicFormat::B8G8R8_SNorm:

	case GraphicFormat::B8G8R8A8_SNorm:

	case GraphicFormat::B8G8R8_UInt:

	case GraphicFormat::B8G8R8A8_UInt:

	case GraphicFormat::B8G8R8_SInt:

	case GraphicFormat::B8G8R8A8_SInt:

	case GraphicFormat::R4G4B4A4_UNormPack16:

	case GraphicFormat::B4G4R4A4_UNormPack16:

	case GraphicFormat::R5G6B5_UNormPack16:

	case GraphicFormat::B5G6R5_UNormPack16:

	case GraphicFormat::R5G5B5A1_UNormPack16:

	case GraphicFormat::B5G5R5A1_UNormPack16:

	case GraphicFormat::A1R5G5B5_UNormPack16:
		return GL_RGB5_A1;
	case GraphicFormat::E5B9G9R9_UFloatPack32:
		return GL_RGB9_E5;
	case GraphicFormat::B10G11R11_UFloatPack32:
		return GL_R11F_G11F_B10F;
	case GraphicFormat::Depth_32Bit:
		return GL_DEPTH_COMPONENT32;
	case GraphicFormat::Depth_24Bit:
		return GL_DEPTH_COMPONENT24;
	case GraphicFormat::Depth_16Bit:
		return GL_DEPTH_COMPONENT16;
	case GraphicFormat::Depth_24Bit_8Stencil:
		return GL_DEPTH24_STENCIL8;
	default:
		break;
	}
	throw InvalidArgumentException("Invalid texture format : {}", magic_enum::enum_name(graphicFormat));
}

unsigned int GLHelper::getTextureFormat(const ImageFormat textureFormat, unsigned int *pixelType) {
	switch (textureFormat) {
	case ImageFormat::Alpha8:
		*pixelType = GL_UNSIGNED_BYTE;
		return GL_ALPHA;
	case ImageFormat::ARGB4444:
		*pixelType = GL_UNSIGNED_SHORT_4_4_4_4;
		return GL_RGB;
	case ImageFormat::RGB24:
		*pixelType = GL_UNSIGNED_BYTE;
		return GL_RGBA;
	case ImageFormat::RGBA32:
		*pixelType = GL_UNSIGNED_BYTE;
		return GL_RGBA;
	case ImageFormat::ARGB32:
		*pixelType = GL_UNSIGNED_BYTE;
		return 0;
	case ImageFormat::RGB565:
		*pixelType = GL_UNSIGNED_SHORT_5_6_5;
		return GL_RGB565;
	case ImageFormat::R16:
		break;
	case ImageFormat::R8:
		*pixelType = GL_UNSIGNED_BYTE;
		return GL_RED;
	case ImageFormat::DXT1:
		break;
	case ImageFormat::DXT5:
		break;
	case ImageFormat::RGBA4444:
		*pixelType = GL_UNSIGNED_SHORT_4_4_4_4;
		return GL_RGBA;
	case ImageFormat::BGR24:
		*pixelType = GL_UNSIGNED_BYTE;
		return GL_BGR;
	case ImageFormat::BGRA32:
		*pixelType = GL_UNSIGNED_BYTE;
		return GL_BGRA;
	case ImageFormat::RHalf:
		*pixelType = GL_HALF_FLOAT;
		return GL_RED;
	case ImageFormat::RGHalf:
		*pixelType = GL_HALF_FLOAT;
		return GL_RG;
	case ImageFormat::RGBAHalf:
		*pixelType = GL_HALF_FLOAT;
		return GL_RGBA;
	case ImageFormat::RFloat:
		*pixelType = GL_FLOAT;
		return GL_RED;
	case ImageFormat::RGFloat:
		*pixelType = GL_FLOAT;
		return GL_RG;
	case ImageFormat::RGBAFloat:
		*pixelType = GL_FLOAT;
		return GL_RGBA;
	case ImageFormat::YUY2:
	default:
		break;
	}
	throw InvalidArgumentException("Invalid texture format : {}", magic_enum::enum_name(textureFormat));
}

unsigned int GLHelper::getTextureFormat(const TextureDesc::DataPixelFormat format) {
	switch (format) {
	case TextureDesc::DataPixelFormat::SingleColor:
		return GL_RED;
	case TextureDesc::DataPixelFormat::RGB:
		return GL_RGB;
	case TextureDesc::DataPixelFormat::RGBA:
		return GL_RGBA;
	case TextureDesc::DataPixelFormat::BGR:
		return GL_BGR;
	case TextureDesc::DataPixelFormat::BGRA:
		return GL_BGRA;
	case TextureDesc::DataPixelFormat::RG:
		return GL_RG;
	default:
		throw InvalidArgumentException("Invalid texture format : {}", magic_enum::enum_name(format));
	}
}

unsigned int GLHelper::getInternalTextureFormat(TextureDesc::DataPixelFormat format, bool sRGB,
												TextureDesc::Compression compression, TextureDesc::PixelDataType type) {

	// No
	if (!sRGB && compression == TextureDesc::Compression::NoCompression) {
		switch (format) {
		case TextureDesc::DataPixelFormat::RGB:
			if (type == TextureDesc::PixelDataType::UnsignedByte) {
				return GL_RGB8;
			}
			if (type == TextureDesc::PixelDataType::Float) {
				return GL_RGB32F;
			}
			break;
		case TextureDesc::DataPixelFormat::RGBA:
			if (type == TextureDesc::PixelDataType::UnsignedByte) {
				return GL_RGBA8;
			}
			if (type == TextureDesc::PixelDataType::Float) {
				return GL_RGBA32F;
			}
			break;
		case TextureDesc::DataPixelFormat::BGR:
			return GL_BGR;
		case TextureDesc::DataPixelFormat::BGRA:
			return GL_BGRA;
		case TextureDesc::DataPixelFormat::SRGB:
			return GL_SRGB;
		case TextureDesc::DataPixelFormat::SRGBA:
			return GL_SRGB_ALPHA;
		case TextureDesc::DataPixelFormat::SingleColor:
			return GL_R8;
		case TextureDesc::DataPixelFormat::Depth:
			return GL_DEPTH;
		case TextureDesc::DataPixelFormat::Stencil:
			return GL_STENCIL;
		case TextureDesc::DataPixelFormat::DepthStencil:
			return GL_DEPTH_STENCIL;
		default:
			break;
		}
	} else {
		/*  Gamma correction and */
		if (sRGB && compression != TextureDesc::Compression::NoCompression) {
			switch (format) {
			case TextureDesc::DataPixelFormat::RGB:
				switch (compression) {
				case TextureDesc::Compression::Compression:
					return GL_COMPRESSED_SRGB;
				case TextureDesc::Compression::ETC2:
					return GL_COMPRESSED_SRGB8_ETC2;
				default:
					return 0;
				}
			case TextureDesc::DataPixelFormat::RGBA:
				switch (compression) {
				case TextureDesc::Compression::Compression:
					return GL_COMPRESSED_SRGB_ALPHA;
				case TextureDesc::Compression::ETC2:
					return GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC;
				default:
					return 0;
				}
			default:
				break;
			}
		}

		/*  Only compression.   */
		if ((unsigned int)compression) {
			switch (format) {
			case TextureDesc::DataPixelFormat::RGB:
				switch (compression) {
				case TextureDesc::Compression::Compression:
					return GL_COMPRESSED_RGB;
				case TextureDesc::Compression::RGTC:
					return GL_COMPRESSED_RED_RGTC1;
				default:
					return 0;
				}
			default:
				break;
			}
		}

		/*  Only gamma correction.  */
		if (sRGB) {
			switch (format) {
			case TextureDesc::DataPixelFormat::RGB:
				return GL_SRGB8;
			case TextureDesc::DataPixelFormat::RGBA:
				return GL_SRGB8_ALPHA8;
			case TextureDesc::DataPixelFormat::SingleColor:
				return GL_SLUMINANCE8;
			default:
				break;
			}
		}
	}
	throw InvalidArgumentException("Invalid texture format {}.", magic_enum::enum_name(format));
}

unsigned int GLHelper::getTextureTarget(const TextureDesc::TextureTarget target, const int nrSamples) {
	switch (target) {
	case TextureDesc::TextureTarget::Texture1D:
		return GL_TEXTURE_1D;
	case TextureDesc::TextureTarget::Texture2D:
		if (nrSamples > 1) {
			return GL_TEXTURE_2D_MULTISAMPLE;
		} else {
			return GL_TEXTURE_2D;
		}
	case TextureDesc::TextureTarget::Texture3D:
		return GL_TEXTURE_3D;
	case TextureDesc::TextureTarget::CubeMap:
		return GL_TEXTURE_CUBE_MAP;
	case TextureDesc::TextureTarget::Texture2DArray:
		if (nrSamples > 1) {
			return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
		} else {
			return GL_TEXTURE_2D_ARRAY;
		}
	case TextureDesc::TextureTarget::CubeMapArray:
		return GL_TEXTURE_CUBE_MAP_ARRAY;
	default:
		throw InvalidArgumentException("Invalid texture target {}.", magic_enum::enum_name(target));
	}
}

unsigned int GLHelper::getTextureType(const TextureDesc::PixelDataType type) {
	switch (type) {
	case TextureDesc::PixelDataType::UnsignedByte:
		return GL_UNSIGNED_BYTE;
	case TextureDesc::PixelDataType::Float:
		return GL_FLOAT;
	case TextureDesc::PixelDataType::HalfFloat:
		return GL_HALF_FLOAT_ARB;
	case TextureDesc::PixelDataType::SignedByte:
		return GL_BYTE;
	case TextureDesc::PixelDataType::Unsigned24_8:
		return GL_UNSIGNED_INT_24_8;
	default:
		throw InvalidArgumentException("Invalid texture type {}.", magic_enum::enum_name(type));
	}
}

unsigned int GLHelper::getTextureSwizzle(const fragcore::TextureSwizzle swizzle) {
	switch (swizzle) {
	case TextureSwizzle::Zero:
		return GL_ZERO;
	case TextureSwizzle::One:
		return GL_ONE;
	case TextureSwizzle::Red:
		return GL_RED;
	case TextureSwizzle::Green:
		return GL_GREEN;
	case TextureSwizzle::Blue:
		return GL_BLUE;
	case TextureSwizzle::eAlpha:
		return GL_ALPHA;
	default:
		throw InvalidArgumentException("None Supported Swizzled {}", magic_enum::enum_name(swizzle));
	}
}

unsigned int GLHelper::getBufferType(const BufferDesc::BufferType type) {
	switch (type) {
	case BufferDesc::BufferType::eArray:
		return GL_ARRAY_BUFFER_ARB;
	case BufferDesc::BufferType::eElementArray:
		return GL_ELEMENT_ARRAY_BUFFER_ARB;
	case BufferDesc::BufferType::eUniform:
		return GL_UNIFORM_BUFFER;
	case BufferDesc::BufferType::eTexture:
		return GL_TEXTURE_BUFFER;
	case BufferDesc::BufferType::eShaderStorage:
		return GL_SHADER_STORAGE_BUFFER;
	case BufferDesc::BufferType::eTransformFeedback:
		return GL_TRANSFORM_FEEDBACK_BUFFER;
	case BufferDesc::BufferType::ePixelPack:
		return GL_PIXEL_PACK_BUFFER;
	case BufferDesc::BufferType::ePixelUnpack:
		return GL_PIXEL_UNPACK_BUFFER;
	case BufferDesc::BufferType::eIndirectDraw:
		return GL_DRAW_INDIRECT_BUFFER;
	case BufferDesc::BufferType::eIndirectDispatch:
		return GL_DISPATCH_INDIRECT_BUFFER;
	default:
		throw InvalidArgumentException("Invalid buffer type {}.", magic_enum::enum_name(type));
	}
}

unsigned int GLHelper::getBufferHint(const BufferDesc::BufferHint hint) {

	const unsigned int subhint = (hint & ~(BufferDesc::eWrite | BufferDesc::eRead));

	if (hint & BufferDesc::eWrite) {
		switch (subhint) {
		case BufferDesc::eStatic:
			return GL_STATIC_DRAW_ARB;
		case BufferDesc::eDynamic:
			return GL_DYNAMIC_DRAW_ARB;
		case BufferDesc::eStream:
			return GL_STREAM_DRAW_ARB;
		default:
			assert(0);
			throw InvalidArgumentException("None matching write buffer hint");
		}
	} else if (hint & BufferDesc::eRead) {
		switch (subhint) {
		case BufferDesc::eStatic:
			return GL_STATIC_READ_ARB;
		case BufferDesc::eDynamic:
			return GL_DYNAMIC_READ_ARB;
		case BufferDesc::eStream:
			return GL_STREAM_READ_ARB;
		default:
			assert(0);
			throw InvalidArgumentException("None matching read buffer hint");
		}
	}

	assert(0);
	throw InvalidArgumentException("None matching buffer hint : {}", magic_enum::enum_name(hint));
}

unsigned int GLHelper::getPrimitive(const Primitive primitive) {

	switch (primitive) {
	case Primitive::Point:
		return GL_POINTS;
	case Primitive::Lines:
		return GL_LINES;
	case Primitive::LineLoop:
		return GL_LINE_LOOP;
	case Primitive::LineStrip:
		return GL_LINE_STRIP;
	case Primitive::Triangles:
		return GL_TRIANGLES;
	case Primitive::TriangleStrip:
		return GL_TRIANGLE_STRIP;
	case Primitive::TriangleAdjacant:
		return GL_TRIANGLES_ADJACENCY;
	case fragcore::Primitive::Patchs:
		return GL_PATCHES;
	default:
		throw InvalidArgumentException("None matching primitive type : {}.", magic_enum::enum_name(primitive));
	}
}

unsigned int GLHelper::getAttributeDataType(const GeometryDesc::AttributeType type) {
	switch (type) {
	case GeometryDesc::AttributeType::eInt:
		return GL_INT;
	case GeometryDesc::AttributeType::eFloat:
		return GL_FLOAT;
	case GeometryDesc::AttributeType::eDouble:
		return GL_DOUBLE;
	case GeometryDesc::AttributeType::eHalf:
		return GL_HALF_FLOAT;
	default:
		throw InvalidArgumentException("Invalid attribute type- {}.", magic_enum::enum_name(type));
	}
}

unsigned int GLHelper::getState(const IRenderer::State state) {
	switch (state) {
	case IRenderer::State::DepthTest:
		return GL_DEPTH_TEST;
	case IRenderer::State::StencilTest:
		return GL_STENCIL_TEST;
	case IRenderer::State::ScissorTest:
		return GL_SCISSOR_TEST;
	case IRenderer::State::AlphaTest:
		return GL_ALPHA_TEST;
	case IRenderer::State::Blend:
		return GL_BLEND;
	case IRenderer::State::Cullface:
		return GL_CULL_FACE;
	case IRenderer::State::Dither:
		return GL_DITHER;
	case IRenderer::State::SRGB:
		return GL_FRAMEBUFFER_SRGB;
	case IRenderer::State::MultiSampling:
		return GL_MULTISAMPLE;
	case IRenderer::State::DiscardRasterization:
		return GL_RASTERIZER_DISCARD;
	case IRenderer::State::SampleShading:
		return GL_SAMPLE_SHADING;
	case IRenderer::State::SampleAlphaCoverage:
		return GL_SAMPLE_ALPHA_TO_COVERAGE;
	default:
		throw InvalidArgumentException("Invalid state - {}.", magic_enum::enum_name(state));
	}
}
unsigned int GLHelper::getTextureFilterModeNoMip(Texture::FilterMode format) {
	switch (format) {
	case Texture::FilterMode::eNearest:
		return GL_NEAREST;
	case Texture::FilterMode::eBilinear:
		return GL_LINEAR;
	default:
		throw InvalidArgumentException("");
	}
}

unsigned int GLHelper::getBlendEqu(BlendEqu equ) {
	switch (equ) {
	case BlendEqu::Addition:
		return GL_FUNC_ADD;
	case BlendEqu ::Subtract:
		return GL_FUNC_SUBTRACT;
	default:
		throw InvalidArgumentException("");
	}
}
unsigned int GLHelper::getBlendFunc(BlendFunc func) {
	switch (func) {
	case BlendFunc::Zero:
		return GL_ZERO;
	case BlendFunc ::eOne:
		return GL_ONE;
	case BlendFunc::eSrcColor:
		return GL_SRC_COLOR;
	case BlendFunc::eOneMinusSrcColor:
		return GL_ONE_MINUS_SRC_COLOR;
	case BlendFunc::eSrcAlpha:
		return GL_SRC_ALPHA;
	case BlendFunc::eOneMinusSrcAlpha:
		return GL_ONE_MINUS_SRC_ALPHA;
	default:
		throw InvalidArgumentException("");
	}
}

unsigned int GLHelper::getClearBitMask(CLEARBITMASK clearbitmask) {
	GLbitfield mask = 0;
	mask |= ((unsigned int)clearbitmask & (unsigned int)CLEARBITMASK::Color) != 0 ? GL_COLOR_BUFFER_BIT : 0;
	mask |= ((unsigned int)clearbitmask & (unsigned int)CLEARBITMASK::Depth) != 0 ? GL_DEPTH_BUFFER_BIT : 0;
	mask |= ((unsigned int)clearbitmask & (unsigned int)CLEARBITMASK::Stencil) != 0 ? GL_STENCIL_BUFFER_BIT : 0;
	return mask;
}

/*  Texture.    */
unsigned int GLHelper::getTextureWrapMode(const Texture::WrapMode mode) {
	switch (mode) {
	case Texture::WrapMode::eClamp:
		return GL_CLAMP_TO_EDGE;
	case Texture::WrapMode::eRepeat:
		return GL_REPEAT;
	case Texture::WrapMode::eMirror:
		return GL_MIRRORED_REPEAT;
	default:
		throw InvalidArgumentException("Invalid Wrap mode.");
	}
}

unsigned int GLHelper::getTextureFilterMode(Texture::FilterMode mode) {
	switch (mode) {
	case Texture::FilterMode::eNearest:

		return GL_NEAREST_MIPMAP_NEAREST;
	case Texture::FilterMode::eBilinear:
		return GL_LINEAR_MIPMAP_NEAREST;
	case Texture::FilterMode::eTrilinear:
		return GL_LINEAR_MIPMAP_LINEAR;
	default:
		throw InvalidArgumentException("Invalid Filter Mode.");
	}
}

unsigned int GLHelper::getTextureCompareMode(Texture::CompareFunc compareFunc) {
	switch (compareFunc) {
	case Texture::CompareFunc::lessEqual:
		return GL_LEQUAL;
	case Texture::CompareFunc::greaterEqual:
		return GL_GEQUAL;
	case Texture::CompareFunc::less:
		return GL_LESS;
	case Texture::CompareFunc::greater:
		return GL_GREATER;
	case Texture::CompareFunc::equal:
		return GL_EQUAL;
	case Texture::CompareFunc::notequal:
		return GL_NOTEQUAL;
	case Texture::CompareFunc::always:
		return GL_ALWAYS;
	case Texture::CompareFunc::never:
		return GL_NEVER;
	default:
		throw InvalidArgumentException("Invalid address mode.");
	}
}
unsigned int GLHelper::getImageInternalFormat(Texture::Format format) {
	GLenum gformat;
	switch (format) {
	case Texture::Format::eR8G8B8:
		gformat = GL_RGB8;
		break;
	case Texture::Format::eR8G8B8A8:
		gformat = GL_RGBA8;
		break;
	case Texture::Format::eRGB32F:
		gformat = GL_RGB32F;
		break;
	case Texture::Format::eRGBA32F:
		gformat = GL_RGBA32F;
		break;
	default:
		throw InvalidArgumentException("Invalid fvformatf requested.");
		// GL_RGBA16_SNORM
	}

	return gformat;
}

unsigned int GLHelper::getAccess(Texture::MapTarget target) {
	GLenum access;
	switch (target) {
	case Texture::MapTarget::eWrite:
		access = GL_WRITE_ONLY;
		break;
	case Texture::MapTarget::eRead:
		access = GL_READ_ONLY;
		break;
	case Texture::MapTarget::eReadWrite:
		access = GL_READ_WRITE;
		break;
	default:
		throw InvalidArgumentException("Invalid access requested.");
	}
	return access;
}

unsigned int GLHelper::getTextureGLFormat(Texture::Format format) {
	switch (format) {
	case Texture::Format::eR8G8B8A8:
		return GL_RGBA;
	case Texture::Format::eR8G8B8:
		return GL_RGB;
	case Texture::Format::eRGB32F:
		return GL_RGB;
	case Texture::Format::eRGBA32F:
		return GL_RGBA;
	default:
		return 0;
	}
}

Texture::Format GLHelper::getTextureFormatUserSpace(unsigned int format) {
	switch (format) {
	case GL_RGBA8:
		return Texture::eR8G8B8;
	default:
		return Texture::Format::R8;
	}
}