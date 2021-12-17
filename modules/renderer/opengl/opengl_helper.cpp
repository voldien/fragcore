#include "../IRenderer.h"
#include "internal_object_type.h"
#include <GL/glew.h>

#include <fmt/core.h>
using namespace fragcore;

/*  Assign marker for debugging.    */
// GL_MAX_LABEL_LENGTH
void addMarkerLabel(const OpenGLCore *glcore, unsigned int identifier, unsigned int object, const MarkerDebug *debug) {
	if (identifier != 0) {
		// glObjectPtrLabel
		int maxLabelLength;
		glGetIntegerv(GL_MAX_LABEL_LENGTH, &maxLabelLength);
		checkError();
		if (glcore->debug && debug->markerName) {
			glObjectLabel(identifier, object, -1, debug->markerName);
			checkError();
		}
	}
}

void checkError() {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		throw RuntimeException(fmt::format("glGetError indicated an error: {}", glewGetErrorString(error)));
	}
}

/*  Helper function for extracting error from OpenGL.   */
void resetErrorFlag() {
	while (glGetError() != GL_NO_ERROR) {
	}
}

unsigned int getWrapMode(SamplerDesc::AddressMode mode) {
	switch (mode) {
	case SamplerDesc::Repeat:
		return GL_REPEAT;
	case SamplerDesc::RepeatMirror:
		return GL_MIRRORED_REPEAT;
	case SamplerDesc::Clamp:
		return GL_CLAMP_TO_EDGE;
	case SamplerDesc::ClampBorder:
		return GL_CLAMP_TO_BORDER;
	case SamplerDesc::NoAddressMode:
	default:
		throw InvalidArgumentException("Invalid address mode.");
	}
}

unsigned int getFilterMode(SamplerDesc::FilterMode mode, SamplerDesc::FilterMode mips) {
	if (mips == SamplerDesc::FilterMode::NoFilterMode) {
		switch (mode) {
		case SamplerDesc::FilterMode::Linear:
			return GL_LINEAR;
		case SamplerDesc::FilterMode::Nearset:
			return GL_NEAREST;
		default:
			break;
		}
	} else {
		switch (mode) {
		case SamplerDesc::FilterMode::Linear:
			switch (mips) {
			case SamplerDesc::FilterMode::Linear:
				return GL_LINEAR_MIPMAP_LINEAR;
			case SamplerDesc::FilterMode::Nearset:
				return GL_LINEAR_MIPMAP_NEAREST;
			case SamplerDesc::FilterMode::NoFilterMode:
			default:
				break;
			}
			break;
		case SamplerDesc::FilterMode::Nearset:
			switch (mips) {
			case SamplerDesc::FilterMode::Linear:
				return GL_NEAREST_MIPMAP_LINEAR;
			case SamplerDesc::FilterMode::Nearset:
				return GL_NEAREST_MIPMAP_NEAREST;
			case SamplerDesc::FilterMode::NoFilterMode:
			default:
				break;
			}
			break;
		}
	}
	throw InvalidArgumentException("Invalid filter mode.");
}

unsigned int getCompareMode(SamplerDesc::CompareFunc mode) {
	switch (mode) {
	case SamplerDesc::lessEqual:
		return GL_LEQUAL;
	case SamplerDesc::greaterEqual:
		return GL_GEQUAL;
	case SamplerDesc::less:
		return GL_LESS;
	case SamplerDesc::greater:
		return GL_GREATER;
	case SamplerDesc::equal:
		return GL_EQUAL;
	case SamplerDesc::notequal:
		return GL_NOTEQUAL;
	case SamplerDesc::always:
		return GL_ALWAYS;
	case SamplerDesc::never:
		return GL_NEVER;
	default:
		throw InvalidArgumentException("Invalid address mode.");
	}
}

unsigned int getGraphicFormat(GraphicFormat graphicFormat) {
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
	case R8G8_UNorm:   // A two-component, 16-bit unsigned normalized fmt::format that has an 8-bit R component stored
					   // with sRGB nonlinear encoding in byte 0, and an 8-bit G component stored with sRGB nonlinear
					   // encoding in byte 1.
	case R8G8B8_UNorm: // A three-component, 24-bit unsigned normalized fvformatf that has an 8-bit R component in byte
					   // 0, an 8-bit G component in byte 1, and an 8-bit B component in byte 2.
	case R8G8B8A8_UNorm: //	A four-component, 32-bit unsigned normalized fvformatf that has an 8-bit R component in byte
						 // 0, an 8-bit G component in byte 1, an 8-bit B component in byte 2, and an 8-bit A component
						 // in byte 3.
	case R8_SNorm:		 //	A one-component, 8-bit signed normalized fvformatf that has a single 8-bit R component.
	case R8G8_SNorm: //	A two-component: 16-bit signed normalized fvformatf that has an 8-bit R component stored with
					 // sRGB nonlinear encoding in byte 0: and an 8-bit G component stored with sRGB nonlinear encoding
					 // in byte 1.
	case R8G8B8_SNorm:	 //	A three-component: 24-bit signed normalized fvformatf that has an 8-bit R component in byte
						 // 0: an 8-bit G component in byte 1: and an 8-bit B component in byte 2.
	case R8G8B8A8_SNorm: //	A four-component: 32-bit signed normalized fvformatf that has an 8-bit R component in byte
						 // 0: an 8-bit G component in byte 1: an 8-bit B component in byte 2: and an 8-bit A component
						 // in byte 3.
	case R8_UInt:		 //	A one-component: 8-bit unsigned integer fvformatf that has a single 8-bit R component.
	case R8G8_UInt:	  //	A two-component: 16-bit unsigned integer fvformatf that has an 8-bit R component in byte 0: and
					  // an 8-bit G component in byte 1.
	case R8G8B8_UInt: //	A three-component: 24-bit unsigned integer fvformatf that has an 8-bit R component in byte
					  // 0: an 8-bit G component in byte 1: and an 8-bit B component in byte 2.
	case R8G8B8A8_UInt: //	A four-component: 32-bit unsigned integer fvformatf that has an 8-bit R component in byte 0:
						// an 8-bit G component in byte 1: an 8-bit B component in byte 2: and an 8-bit A component in
						// byte 3.
	case R8_SInt:		//	A one-component: 8-bit signed integer fvformatf that has a single 8-bit R component.
	case R8G8_SInt: //	A two-component: 16-bit signed integer fvformatf that has an 8-bit R component in byte 0: and an
					// 8-bit G component in byte 1.
	case R8G8B8_SInt:	//	A three-component: 24-bit signed integer fvformatf that has an 8-bit R component in byte 0:
						// an 8-bit G component in byte 1: and an 8-bit B component in byte 2.
	case R8G8B8A8_SInt: //	A four-component: 32-bit signed integer fvformatf that has an 8-bit R component in byte 0:
						// an 8-bit G component in byte 1: an 8-bit B component in byte 2: and an 8-bit A component in
						// byte 3.
	case R16_UNorm:		//	A one-component: 16-bit unsigned normalized fvformatf that has a single 16-bit R component.
	case R16G16_UNorm:	//	A two-component: 32-bit unsigned normalized fvformatf that has a 16-bit R component in bytes
						// 0..1: and a 16-bit G component in bytes 2..3.
	case R16G16B16_UNorm:	 //	A three-component: 48-bit unsigned normalized fvformatf that has a 16-bit R component in
							 // bytes 0..1: a 16-bit G component in bytes 2..3: and a 16-bit B component in bytes 4..5.
	case R16G16B16A16_UNorm: //	A four-component: 64-bit unsigned normalized fvformatf that has a 16-bit R component in
							 // bytes 0..1: a 16-bit G component in bytes 2..3: a 16-bit B component in bytes 4..5: and
							 // a 16-bit A component in bytes 6..7.
	case R16_SNorm:		  //	A one-component: 16-bit signed normalized fvformatf that has a single 16-bit R component.
	case R16G16_SNorm:	  //	A two-component: 32-bit signed normalized fvformatf that has a 16-bit R component in bytes
						  // 0..1: and a 16-bit G component in bytes 2..3.
	case R16G16B16_SNorm: //	A three-component: 48-bit signed normalized fvformatf that has a 16-bit R component in
						  // bytes 0..1: a 16-bit G component in bytes 2..3: and a 16-bit B component in bytes 4..5.
	case R16G16B16A16_SNorm: //	A four-component: 64-bit signed normalized fvformatf that has a 16-bit R component in
							 // bytes 0..1: a 16-bit G component in bytes 2..3: a 16-bit B component in bytes 4..5: and
							 // a 16-bit A component in bytes 6..7.
	case R16_UInt:			 //	A one-component: 16-bit unsigned integer fvformatf that has a single 16-bit R component.
	case R16G16_UInt:	 //	A two-component: 32-bit unsigned integer fvformatf that has a 16-bit R component in bytes
						 // 0..1: and a 16-bit G component in bytes 2..3.
	case R16G16B16_UInt: //	A three-component: 48-bit unsigned integer fvformatf that has a 16-bit R component in bytes
						 // 0..1: a 16-bit G component in bytes 2..3: and a 16-bit B component in bytes 4..5.
	case R16G16B16A16_UInt: //	A four-component: 64-bit unsigned integer fvformatf that has a 16-bit R component in
							// bytes 0..1: a 16-bit G component in bytes 2..3: a 16-bit B component in bytes 4..5: and a
							// 16-bit A component in bytes 6..7.
	case R16_SInt:			//	A one-component: 16-bit signed integer fvformatf that has a single 16-bit R component.
	case R16G16_SInt:		//	A two-component: 32-bit signed integer fvformatf that has a 16-bit R component in bytes
							// 0..1: and a 16-bit G component in bytes 2..3.
	case R16G16B16_SInt:	//	A three-component: 48-bit signed integer fvformatf that has a 16-bit R component in bytes
							// 0..1: a 16-bit G component in bytes 2..3: and a 16-bit B component in bytes 4..5.
	case R16G16B16A16_SInt: //	A four-component: 64-bit signed integer fvformatf that has a 16-bit R component in bytes
							// 0..1: a 16-bit G component in bytes 2..3: a 16-bit B component in bytes 4..5: and a
							// 16-bit A component in bytes 6..7.
	case R32_UInt:			//	A one-component: 32-bit unsigned integer fvformatf that has a single 32-bit R component.
	case R32G32_UInt:		//	A two-component: 64-bit unsigned integer fvformatf that has a 32-bit R component in bytes
							// 0..3: and a 32-bit G component in bytes 4..7.
	case R32G32B32_UInt: //	A three-component: 96-bit unsigned integer fvformatf that has a 32-bit R component in bytes
						 // 0..3: a 32-bit G component in bytes 4..7: and a 32-bit B component in bytes 8..11.
	case R32G32B32A32_UInt: //	A four-component: 128-bit unsigned integer fvformatf that has a 32-bit R component in
							// bytes 0..3: a 32-bit G component in bytes 4..7: a 32-bit B component in bytes 8..11: and
							// a 32-bit A component in bytes 12..15.
	case R32_SInt:			//	A one-component: 32-bit signed integer fvformatf that has a single 32-bit R component.
	case R32G32_SInt:		//	A two-component: 64-bit signed integer fvformatf that has a 32-bit R component in bytes
							// 0..3: and a 32-bit G component in bytes 4..7.
	case R32G32B32_SInt:	//	A three-component: 96-bit signed integer fvformatf that has a 32-bit R component in bytes
							// 0..3: a 32-bit G component in bytes 4..7: and a 32-bit B component in bytes 8..11.
	case R32G32B32A32_SInt: //	A four-component: 128-bit signed integer fvformatf that has a 32-bit R component in
							// bytes 0..3: a 32-bit G component in bytes 4..7: a 32-bit B component in bytes 8..11: and
							// a 32-bit A component in bytes 12..15.
	case R16_SFloat:	//	A one-component: 16-bit signed floating-point fvformatf that has a single 16-bit R component.
	case R16G16_SFloat: //	A two-component: 32-bit signed floating-point fvformatf that has a 16-bit R component in
						// bytes 0..1: and a 16-bit G component in bytes 2..3.
	case R16G16B16_SFloat: //	A three-component: 48-bit signed floating-point fvformatf that has a 16-bit R component
						   // in bytes 0..1: a 16-bit G component in bytes 2..3: and a 16-bit B component in bytes 4..5.
	case R16G16B16A16_SFloat: //	A four-component: 64-bit signed floating-point fvformatf that has a 16-bit R
							  // component in bytes 0..1: a 16-bit G component in bytes 2..3: a 16-bit B component in
							  // bytes 4..5: and a 16-bit A component in bytes 6..7.
	case R32_SFloat:	//	A one-component: 32-bit signed floating-point fvformatf that has a single 32-bit R component.
	case R32G32_SFloat: //	A two-component: 64-bit signed floating-point fvformatf that has a 32-bit R component in
						// bytes 0..3: and a 32-bit G component in bytes 4..7.
	case R32G32B32_SFloat:	  //	A three-component: 96-bit signed floating-point fvformatf that has a 32-bit R component
							  // in bytes 0..3: a 32-bit G component in bytes 4..7: and a 32-bit B component in
							  // bytes 8..11.
	case R32G32B32A32_SFloat: //	A four-component: 128-bit signed floating-point fvformatf that has a 32-bit R
							  // component in bytes 0..3: a 32-bit G component in bytes 4..7: a 32-bit B component in
							  // bytes 8..11: and a 32-bit A component in bytes 12..15.
	case B8G8R8_SRGB:	 //	A three-component: 24-bit unsigned normalized fvformatf that has an 8-bit R component stored
						 // with sRGB nonlinear encoding in byte 0: an 8-bit G component stored with sRGB nonlinear
						 // encoding in byte 1: and an 8-bit B component stored with sRGB nonlinear encoding in byte 2.
	case B8G8R8A8_SRGB:	 //	A four-component: 32-bit unsigned normalized fvformatf that has an 8-bit B component stored
						 // with sRGB nonlinear encoding in byte 0: an 8-bit G component stored with sRGB nonlinear
						 // encoding in byte 1: an 8-bit R component stored with sRGB nonlinear encoding in byte 2: and
						 // an 8-bit A component in byte 3.
	case B8G8R8_UNorm:	 //	A three-component: 24-bit unsigned normalized fvformatf that has an 8-bit B component in
						 // byte 0: an 8-bit G component in byte 1: and an 8-bit R component in byte 2.
	case B8G8R8A8_UNorm: //	A four-component: 32-bit unsigned normalized fvformatf that has an 8-bit B component in byte
						 // 0: an 8-bit G component in byte 1: an 8-bit R component in byte 2: and an 8-bit A component
						 // in byte 3.
	case B8G8R8_SNorm:	 //	A three-component: 24-bit signed normalized fvformatf that has an 8-bit B component in byte
						 // 0: an 8-bit G component in byte 1: and an 8-bit R component in byte 2.
	case B8G8R8A8_SNorm: //	A four-component: 32-bit signed normalized fvformatf that has an 8-bit B component in byte
						 // 0: an 8-bit G component in byte 1: an 8-bit R component in byte 2: and an 8-bit A component
						 // in byte 3.
	case B8G8R8_UInt:	 //	A three-component: 24-bit unsigned integer fvformatf that has an 8-bit B component in byte
						 // 0: an 8-bit G component in byte 1: and an 8-bit R component in byte 2
	case B8G8R8A8_UInt:	 //	A four-component: 32-bit unsigned integer fvformatf that has an 8-bit B component in byte 0:
						 // an 8-bit G component in byte 1: an 8-bit R component in byte 2: and an 8-bit A component in
						 // byte 3.
	case B8G8R8_SInt:	 //	A three-component: 24-bit signed integer fvformatf that has an 8-bit B component in byte 0:
						 // an 8-bit G component in byte 1: and an 8-bit R component in byte 2.
	case B8G8R8A8_SInt:	 //	A four-component: 32-bit signed integer fvformatf that has an 8-bit B component in byte 0:
						 // an 8-bit G component in byte 1: an 8-bit R component in byte 2: and an 8-bit A component in
						 // byte 3.
	case R4G4B4A4_UNormPack16: //	A four-component: 16-bit packed unsigned normalized fvformatf that has a 4-bit R
							   // component in bits 12..15: a 4-bit G component in bits 8..11: a 4-bit B component in
							   // bits 4..7: and a 4-bit A component in bits 0..3.
	case B4G4R4A4_UNormPack16: //	A four-component: 16-bit packed unsigned normalized fvformatf that has a 4-bit B
							   // component in bits 12..15: a 4-bit G component in bits 8..11: a 4-bit R component in
							   // bits 4..7: and a 4-bit A component in bits 0..3.
	case R5G6B5_UNormPack16:   //	A three-component: 16-bit packed unsigned normalized fvformatf that has a 5-bit R
							 // component in bits 11..15: a 6-bit G component in bits 5..10: and a 5-bit B component in
							 // bits 0..4.
	case B5G6R5_UNormPack16: //	A three-component: 16-bit packed unsigned normalized fvformatf that has a 5-bit B
							 // component in bits 11..15: a 6-bit G component in bits 5..10: and a 5-bit R component in
							 // bits 0..4.
	case R5G5B5A1_UNormPack16: //	A four-component: 16-bit packed unsigned normalized fvformatf that has a 5-bit R
							   // component in bits 11..15: a 5-bit G component in bits 6..10: a 5-bit B component in
							   // bits 1..5: and a 1-bit A component in bit 0.
	case B5G5R5A1_UNormPack16: //	A four-component: 16-bit packed unsigned normalized fvformatf that has a 5-bit B
							   // component in bits 11..15: a 5-bit G component in bits 6..10: a 5-bit R component in
							   // bits 1..5: and a 1-bit A component in bit 0.
	case A1R5G5B5_UNormPack16: //	A four-component, 16-bit packed unsigned normalized fvformatf that has a 1-bi
		throw InvalidArgumentException("Invalid texture format.");
	}
}

unsigned int getTextureFormat(TextureFormat textureFormat, unsigned int *pixelType) {
	switch (textureFormat) {
	case TextureFormat::Alpha8:
		*pixelType = GL_UNSIGNED_BYTE;
		return GL_ALPHA;
	case TextureFormat::ARGB4444:
		*pixelType = GL_UNSIGNED_SHORT_4_4_4_4;
		return GL_RGB;
	case TextureFormat::RGB24:
		*pixelType = GL_UNSIGNED_BYTE;
		return GL_RGBA;
	case TextureFormat::RGBA32:
		*pixelType = GL_UNSIGNED_BYTE;
		return GL_RGBA;
	case TextureFormat::ARGB32:
		*pixelType = GL_UNSIGNED_BYTE;
		return 0;
	case TextureFormat::RGB565:
		*pixelType = GL_UNSIGNED_SHORT_5_6_5;
		return GL_RGB565;
	case TextureFormat::R16:
		break;
	case TextureFormat::R8:
		*pixelType = GL_UNSIGNED_BYTE;
		return GL_RED;
	case TextureFormat::DXT1:
		break;
	case TextureFormat::DXT5:
		break;
	case TextureFormat::RGBA4444:
		*pixelType = GL_UNSIGNED_SHORT_4_4_4_4;
		return GL_RGBA;
	case TextureFormat::BGR24:
		*pixelType = GL_UNSIGNED_BYTE;
		return GL_BGR;
	case TextureFormat::BGRA32:
		*pixelType = GL_UNSIGNED_BYTE;
		return GL_BGRA;
	case TextureFormat::RHalf:
		*pixelType = GL_HALF_FLOAT;
		return GL_RED;
	case TextureFormat::RGHalf:
		*pixelType = GL_HALF_FLOAT;
		return GL_RG;
	case TextureFormat::RGBAHalf:
		*pixelType = GL_HALF_FLOAT;
		return GL_RGBA;
	case TextureFormat::RFloat:
		*pixelType = GL_FLOAT;
		return GL_RED;
	case TextureFormat::RGFloat:
		*pixelType = GL_FLOAT;
		return GL_RG;
	case TextureFormat::RGBAFloat:
		*pixelType = GL_FLOAT;
		return GL_RGBA;
	case TextureFormat::YUY2:
	default:
		throw InvalidArgumentException("Invalid texture format.");
	}
}

unsigned int getTextureFormat(TextureDesc::Format format) {
	switch (format) {
	case TextureDesc::Format::eSingleColor:
		return GL_RED;
	case TextureDesc::Format::eRGB:
		return GL_RGB;
	case TextureDesc::Format::eRGBA:
		return GL_RGBA;
	case TextureDesc::Format::eBGR:
		return GL_BGR;
	case TextureDesc::Format::eBGRA:
		return GL_BGRA;
	case TextureDesc::Format::eRG:
		return GL_RG;
	default:
		throw InvalidArgumentException("Invalid texture format.");
	}
}

unsigned int getInternalTextureFormat(TextureDesc::Format format, bool sRGB, TextureDesc::Compression compression,
									  TextureDesc::Type type) {

	// No
	if (!sRGB && compression == TextureDesc::Compression::eNoCompression) {
		switch (format) {
		case TextureDesc::eRGB:
			if (type == TextureDesc::eUnsignedByte) {
				return GL_RGB8;
			}
			if (type == TextureDesc::eFloat) {
				return GL_RGB32F;
			}
		case TextureDesc::eRGBA:
			if (type == TextureDesc::eUnsignedByte) {
				return GL_RGBA8;
			}
			if (type == TextureDesc::eFloat) {
				return GL_RGBA32F;
			}
			break;
		case TextureDesc::eBGR:
			return GL_BGR;
		case TextureDesc::eBGRA:
			return GL_BGRA;
		case TextureDesc::eSRGB:
			return GL_SRGB;
		case TextureDesc::eSRGBA:
			return GL_SRGB_ALPHA;
		case TextureDesc::eSingleColor:
			return GL_R8;
		case TextureDesc::eDepth:
			return GL_DEPTH;
		case TextureDesc::eStencil:
			return GL_STENCIL;
		case TextureDesc::eDepthStencil:
			return GL_DEPTH_STENCIL;
		default:
			break;
		}
	} else {
		/*  Gamma correction and */
		if (sRGB && compression != TextureDesc::Compression::eNoCompression) {
			switch (format) {
			case TextureDesc::eRGB:
				switch (compression) {
				case TextureDesc::Compression::eCompression:
					return GL_COMPRESSED_SRGB;
				case TextureDesc::Compression::eETC2:
					return GL_COMPRESSED_SRGB8_ETC2;
				}
			case TextureDesc::eRGBA:
				switch (compression) {
				case TextureDesc::Compression::eCompression:
					return GL_COMPRESSED_SRGB_ALPHA;
				case TextureDesc::eETC2:
					return GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC;
				}
			default:
				break;
			}
		}

		/*  Only compression.   */
		if (compression) {
			switch (format) {
			case TextureDesc::eRGB:
				switch (compression) {
				case TextureDesc::Compression::eCompression:
					return GL_COMPRESSED_RGB;
				case TextureDesc::Compression::eRGTC:
					return GL_COMPRESSED_RED_RGTC1;
				}
			default:
				break;
			}
		}

		/*  Only gamma correction.  */
		if (sRGB) {
			switch (format) {
			case TextureDesc::eRGB:
				return GL_SRGB8;
			case TextureDesc::eRGBA:
				return GL_SRGB8_ALPHA8;
			case TextureDesc::eSingleColor:
				GL_SLUMINANCE8;
			default:
				break;
			}
		}
	}
	throw InvalidArgumentException(::fmt::format("Invalid texture fvformatf %d.", format));
}

unsigned int getTextureTarget(TextureDesc::Target target, int nrSamples) {
	switch (target) {
	case TextureDesc::eTexture1D:
		return GL_TEXTURE_1D;
	case TextureDesc::eTexture2D:
		if (nrSamples > 0) {
			return GL_TEXTURE_2D_MULTISAMPLE;
		} else {
			return GL_TEXTURE_2D;
		}
	case TextureDesc::eTexture3D:
		return GL_TEXTURE_3D;
	case TextureDesc::eCubeMap:
		return GL_TEXTURE_CUBE_MAP;
	case TextureDesc::eTexture2DArray:
		if (nrSamples > 0) {
			return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
		} else {
			return GL_TEXTURE_2D_ARRAY;
		}
	case TextureDesc::eCubeMapArray:
		return GL_TEXTURE_CUBE_MAP_ARRAY;
	default:
		throw InvalidArgumentException(fmt::format("Invalid texture target %d.", target));
	}
}

unsigned int getTextureType(TextureDesc::Type type) {
	switch (type) {
	case TextureDesc::eUnsignedByte:
		return GL_UNSIGNED_BYTE;
	case TextureDesc::eFloat:
		return GL_FLOAT;
	case TextureDesc::eHalfFloat:
		return GL_HALF_FLOAT_ARB;
	case TextureDesc::eSignedByte:
		return GL_BYTE;
	case TextureDesc::eUnsigned24_8:
		return GL_UNSIGNED_INT_24_8;
	default:
		throw InvalidArgumentException(fmt::format("Invalid texture type %d.", type));
	}
}

unsigned int getTextureSwizzle(TextureDesc::Swizzle swizzle) {
	switch (swizzle) {
	case TextureDesc::eZero:
		return GL_ZERO;
	case TextureDesc::eOne:
		return GL_ONE;
	case TextureDesc::eRed:
		return GL_RED;
	case TextureDesc::eGreen:
		return GL_GREEN;
	case TextureDesc::eBlue:
		return GL_BLUE;
	case TextureDesc::eAlpha:
		return GL_ALPHA;
	default:
		throw InvalidArgumentException("");
	}
}

unsigned int getBufferType(BufferDesc::BufferType type) {
	switch (type) {
	case BufferDesc::eArray:
		return GL_ARRAY_BUFFER_ARB;
	case BufferDesc::eElementArray:
		return GL_ELEMENT_ARRAY_BUFFER_ARB;
	case BufferDesc::eUniform:
		return GL_UNIFORM_BUFFER;
	case BufferDesc::eTexture:
		return GL_TEXTURE_BUFFER;
	case BufferDesc::eShaderStorage:
		return GL_SHADER_STORAGE_BUFFER;
	case BufferDesc::eTransformFeedback:
		return GL_TRANSFORM_FEEDBACK_BUFFER;
	case BufferDesc::ePixelPack:
		return GL_PIXEL_PACK_BUFFER;
	case BufferDesc::ePixelUnpack:
		return GL_PIXEL_UNPACK_BUFFER;
	case BufferDesc::eIndirectDraw:
		return GL_DRAW_INDIRECT_BUFFER;
	case BufferDesc::eIndirectDispatch:
		return GL_DISPATCH_INDIRECT_BUFFER;
	default:
		throw InvalidArgumentException(fmt::format("Invalid buffer type %d.", type));
	}
}

unsigned int getBufferHint(BufferDesc::BufferHint hint) {

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
	throw InvalidArgumentException(fmt::format("None matching buffer hint - %d", hint));
}

unsigned int getPrimitive(GeometryDesc::Primitive primitive) {

	switch (primitive) {
	case GeometryDesc::ePoint:
		return GL_POINTS;
	case GeometryDesc::eLines:
		return GL_LINES;
	case GeometryDesc::eTriangles:
		return GL_TRIANGLES;
	case GeometryDesc::eTriangleStrips:
		return GL_TRIANGLE_STRIP;
	default:
		throw InvalidArgumentException(fmt::format("None matching primitive type - %d.", primitive));
	}
}

unsigned int getAttributeDataType(GeometryDesc::AttributeType type) {
	switch (type) {
	case GeometryDesc::eInt:
		return GL_INT;
	case GeometryDesc::eFloat:
		return GL_FLOAT;
	case GeometryDesc::eDouble:
		return GL_DOUBLE;
	case GeometryDesc::eHalf:
		return GL_HALF_FLOAT;
	default:
		throw InvalidArgumentException(fmt::format("Invalid attribute type- %d.", type));
	}
}

unsigned int getState(IRenderer::State state) {
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
		throw InvalidArgumentException(fmt::format("Invalid state - %d.", (int)state));
	}
}
unsigned int getTextureFilterModeNoMip(Texture::FilterMode format) {
	switch (format) {
	case Texture::FilterMode::eNearest:
		return GL_NEAREST;
	case Texture::FilterMode::eBilinear:
		return GL_LINEAR;
	default:
		throw InvalidArgumentException("");
	}
}

unsigned int getBlendEqu(FrameBuffer::BlendEqu equ) {
	switch (equ) {
	case FrameBuffer::BlendEqu::Addition:
		return GL_FUNC_ADD;
	case FrameBuffer::BlendEqu ::Subtract:
		return GL_FUNC_SUBTRACT;
	default:
		throw InvalidArgumentException("");
	}
}
unsigned int getBlendFunc(FrameBuffer::BlendFunc func) {
	switch (func) {
	case FrameBuffer::BlendFunc::Zero:
		return GL_ZERO;
	case FrameBuffer::BlendFunc ::eOne:
		return GL_ONE;
	case FrameBuffer::BlendFunc::eSrcColor:
		return GL_SRC_COLOR;
	case FrameBuffer::BlendFunc::eOneMinusSrcColor:
		return GL_ONE_MINUS_SRC_COLOR;
	case FrameBuffer::BlendFunc::eSrcAlpha:
		return GL_SRC_ALPHA;
	case FrameBuffer::BlendFunc::eOneMinusSrcAlpha:
		return GL_ONE_MINUS_SRC_ALPHA;
	default:
		throw InvalidArgumentException("");
	}
}

unsigned int getClearBitMask(CLEARBITMASK clearbitmask) {
	GLbitfield mask = 0;
	mask |= ((unsigned int)clearbitmask & (unsigned int)CLEARBITMASK::Color) != 0 ? GL_COLOR_BUFFER_BIT : 0;
	mask |= ((unsigned int)clearbitmask & (unsigned int)CLEARBITMASK::Depth) != 0 ? GL_DEPTH_BUFFER_BIT : 0;
	mask |= ((unsigned int)clearbitmask & (unsigned int)CLEARBITMASK::eStencil) != 0 ? GL_STENCIL_BUFFER_BIT : 0;
	return mask;
}

/*  Texture.    */
unsigned int getTextureWrapMode(Texture::WrapMode mode) {
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

unsigned int getTextureFilterMode(Texture::FilterMode mode) {
	switch (mode) {
	case Texture::FilterMode::eNearest:

		return GL_NEAREST_MIPMAP_NEAREST;
	case Texture::FilterMode::eBilinear:
		return GL_LINEAR_MIPMAP_NEAREST;
	case Texture::FilterMode::eTrilinear:
		return GL_LINEAR_MIPMAP_LINEAR;
	default:
		throw InvalidArgumentException("");
	}
}

unsigned int getTextureCompareMode(Texture::CompareFunc compareFunc) {
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
unsigned int getImageInternalFormat(Texture::Format format) {
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

unsigned int getAccess(Texture::MapTarget target) {
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

unsigned int getTextureGLFormat(Texture::Format format) {
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

Texture::Format getTextureFormatUserSpace(unsigned int format) {
	switch (format) {
	case GL_RGBA8:
		return Texture::eR8G8B8;
	default:
		return Texture::Format::R8;
	}
}