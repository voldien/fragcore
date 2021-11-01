/**
	FragEngine, A Two layer Game Engine.
	Copyright (C) 2018  Valdemar Lindberg

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_RENDER_DESC_H_
#define _FRAG_CORE_RENDER_DESC_H_ 1
#include"Prerequisites.h"

namespace fragcore {
	/**
	 *  Support languages.
	 */
	enum ShaderLanguage {
		GLSL            = 0x1,		/*  OpenGL GLSL.    */
		SPIRV           = 0x2,		/*  SPIRV.  */
		HLSL            = 0x4,      /*  High Level Shading Language.    */
		CLC             = 0x8,      /*  OpenCL C language.  */
		CG              = 0x10,     /*  Legacy. */
		unKnownLanguage = 0x0,		/*	*/
	};

	typedef struct marker_debug_t {
		const char *markerName;
	} MarkerDebug;

	typedef struct memory_info_t {
		long int totalVRam;
		long int currentVRam;
	} MemoryInfo;

	typedef struct renderer_info_t {
		MemoryInfo memoryInfo;
	} RendererInfo;

	typedef struct sampler_desc_t {
		/**
		 *
		 */
		enum FilterMode {
			eNoFilterMode,  /*  */
			eLinear,        /*  */
			eNearset,       /*  */
		};

		/**
		 *
		 */
		enum AddressMode {
			eNoAddressMode,
			eRepeat,        /*  */
			eRepeatMirror,  /*  */
			eClamp,         /*  */
			eClampBorder,   /*  */
		};

		enum CompareFunc {
			eNoCompare,     /*  */
			lessEqual,
			greaterEqual,
			less,
			greater,
			equal,
			notequal,
			always,
			never,
		};

		/*  Sampler.    */
		float anisotropy;           /*  */
		FilterMode minFilter;       /*  */
		FilterMode magFilter;       /*  */
		AddressMode AddressU;       /*  */
		AddressMode AddressV;       /*  */
		AddressMode AddressW;       /*  */
		FilterMode mipmapFilter;   /*  */
		int maxLOD;                 /*  */
		int minLOD;                 /*  */
		int biasLOD;                /*  */
		float borderColor[4];       /*  */
		int compareMode;            /*  */
		CompareFunc compareFunc;    /*  */

		/*  Debug attributes.   */
		MarkerDebug marker; //TODo add support for adding after creating the object.
	} SamplerDesc;

	enum TextureFormat {
		Alpha8,	//Alpha-only texture fvformatf.
		ARGB4444,	//	A 16 bits/pixel texture fvformatf. Texture stores color with an alpha channel.
		RGB24,	//	Color texture fvformatf, 8-bits per channel.
		RGBA32,	//	Color with alpha texture fvformatf, 8-bits per channel.
		ARGB32,	//	Color with alpha texture fvformatf, 8-bits per channel.
		RGB565,	//	A 16 bit color texture fvformatf.
		R16,	//	Single channel (R) texture fvformatf, 16 bit integer.
		DXT1,	//	Compressed color texture fvformatf.
		DXT5,	//	Compressed color with alpha channel texture fvformatf.
		RGBA4444,	//	Color and alpha texture fvformatf, 4 bit per channel.
		BGRA32,	//	Color with alpha texture fvformatf, 8-bits per channel.
		BGR24,
		RHalf,	//	Scalar (R) texture fvformatf, 16 bit floating point.
		RGHalf,	//	Two color (RG) texture fvformatf, 16 bit floating point per channel.
		RGBAHalf,	//	RGB color and alpha texture fvformatf, 16 bit floating point per channel.
		RFloat,	//	Scalar (R) texture fvformatf, 32 bit floating point.
		RGFloat,	//	Two color (RG) texture fvformatf, 32 bit floating point per channel.
		RGBAFloat,	//	RGB color and alpha texture fvformatf, 32-bit floats per channel.
		YUY2,	    //	A format that uses the YUV color space and is often used for video encoding or playback.
		RGB9e5Float,	//	RGB HDR fvformatf, with 9 bit mantissa per channel and a 5 bit shared exponent.
		BC4,	//	Compressed one channel (R) texture fvformatf.
		BC5,	//	Compressed two-channel (RG) texture fvformatf.
		BC6H,	//	HDR compressed color texture fvformatf.
		BC7,	//	High quality compressed color texture fvformatf.
		DXT1Crunched,	//	Compressed color texture fvformatf with Crunch compression for smaller storage sizes.
		DXT5Crunched,	//	Compressed color with alpha channel texture fvformatf with Crunch compression for smaller storage sizes.
		PVRTC_RGB2,	//	PowerVR (iOS) 2 bits/pixel compressed color texture fvformatf.
		PVRTC_RGBA2,	//width	PowerVR (iOS) 2 bits/pixel compressed with alpha channel texture fvformatf.
		PVRTC_RGB4,	//	PowerVR (iOS) 4 bits/pixel compressed color texture fvformatf.
		PVRTC_RGBA4,	//	PowerVR (iOS) 4 bits/pixel compressed with alpha channel texture fvformatf.
		ETC_RGB4,	//	ETC (GLES2.0) 4 bits/pixel compressed RGB texture fvformatf.
		EAC_R,	// / EAC (GL ES 3.0) 4 bits/pixel compressed unsigned single-channel texture fvformatf.
		EAC_R_SIGNED,	//	ETC2 / EAC (GL ES 3.0) 4 bits/pixel compressed signed single-channel texture fvformatf.
		EAC_RG,	//	ETC2 / EAC (GL ES 3.0) 8 bits/pixel compressed unsigned dual-channel (RG) texture fvformatf.
		EAC_RG_SIGNED,	//	ETC2 / EAC (GL ES 3.0) 8 bits/pixel compressed signed dual-channel (RG) texture fvformatf.
		ETC2_RGB,	//	ETC2 (GL ES 3.0) 4 bits/pixel compressed RGB texture fvformatf.
		ETC2_RGBA1,	//	ETC2 (GL ES 3.0) 4 bits/pixel RGB+1-bit alpha texture fvformatf.
		ETC2_RGBA8,	//	ETC2 (GL ES 3.0) 8 bits/pixel compressed RGBA texture fvformatf.
		ASTC_4x4,	//	ASTC (4x4 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		ASTC_5x5,	//	ASTC (5x5 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		ASTC_6x6,	//	ASTC (6x6 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		ASTC_8x8,	//	ASTC (8x8 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		ASTC_10x10,	//	ASTC (10x10 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		ASTC_12x12,	//	ASTC (12x12 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		RG16,	//	Two color (RG) texture fvformatf, 8-bits per channel.
		R8,	//	Single channel (R) texture fvformatf, 8 bit integer.
		ETC_RGB4Crunched,	//	Compressed color texture fvformatf with Crunch compression for smaller storage sizes.
		ETC2_RGBA8Crunched,	//	Compressed color with alpha channel texture fvformatf using Crunch compression for smaller storage sizes.
		ASTC_HDR_4x4,	//	ASTC (4x4 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
		ASTC_HDR_5x5,	//	ASTC (5x5 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
		ASTC_HDR_6x6,	//	ASTC (6x6 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
		ASTC_HDR_8x8,	//	ASTC (8x8 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		ASTC_HDR_10x10,	//	ASTC (10x10 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
		ASTC_HDR_12x12,	//	ASTC (12x12 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
		ASTC_RGB_4x4,	//	ASTC (4x4 pixel block in 128 bits) compressed RGB texture fvformatf.
		ASTC_RGB_5x5,	//	ASTC (5x5 pixel block in 128 bits) compressed RGB texture fvformatf.
		ASTC_RGB_6x6,	//	ASTC (6x6 pixel block in 128 bits) compressed RGB texture fvformatf.
		ASTC_RGB_8x8,	//	ASTC (8x8 pixel block in 128 bits) compressed RGB texture fvformatf.
		ASTC_RGB_10x10,	//	ASTC (10x10 pixel block in 128 bits) compressed RGB texture fvformatf.
		ASTC_RGB_12x12,	//	ASTC (12x12 pixel block in 128 bits) compressed RGB texture fvformatf.
		ASTC_RGBA_4x4,	//	ASTC (4x4 pixel block in 128 bits) compressed RGBA texture fvformatf.
		ASTC_RGBA_5x5,	//	ASTC (5x5 pixel block in 128 bits) compressed RGBA texture fvformatf.
		ASTC_RGBA_6x6,	//	ASTC (6x6 pixel block in 128 bits) compressed RGBA texture fvformatf.
		ASTC_RGBA_8x8,	//	ASTC (8x8 pixel block in 128 bits) compressed RGBA texture fvformatf.
		ASTC_RGBA_10x10,	//	ASTC (10x10 pixel block in 128 bits) compressed RGBA texture fvformatf.
		ASTC_RGBA_12x12,	//	ASTC (12x12 pixel block in 128 bits) compressed RGBA texture fvformatf.
	};

	enum GraphicFormat {
		NoneGraphicFormat,   //The fvformatf is not specified.
		R8_SRGB,   //A one-component, 8-bit unsigned normalized fvformatf that has a single 8-bit R component stored with sRGB nonlinear encoding.
		R8G8_SRGB,   //A two-component, 16-bit unsigned normalized fvformatf that has an 8-bit R component stored with sRGB nonlinear encoding in byte 0, and an 8-bit G component stored with sRGB nonlinear encoding in byte 1.
		R8G8B8_SRGB,   //A three-component, 24-bit unsigned normalized fvformatf that has an 8-bit R component stored with sRGB nonlinear encoding in byte 0, an 8-bit G component stored with sRGB nonlinear encoding in byte 1, and an 8-bit B component stored with sRGB nonlinear encoding in byte 2.
		R8G8B8A8_SRGB,   //A four-component, 32-bit unsigned normalized fvformatf that has an 8-bit R component stored with sRGB nonlinear encoding in byte 0, an 8-bit G component stored with sRGB nonlinear encoding in byte 1, an 8-bit B component stored with sRGB nonlinear encoding in byte 2, and an 8-bit A component in byte 3.
		R8_UNorm,   //A one-component, 8-bit unsigned normalized fvformatf that has a single 8-bit R component.
		R8G8_UNorm,   //A two-component, 16-bit unsigned normalized fvformatf that has an 8-bit R component stored with sRGB nonlinear encoding in byte 0, and an 8-bit G component stored with sRGB nonlinear encoding in byte 1.
		R8G8B8_UNorm,   //A three-component, 24-bit unsigned normalized fvformatf that has an 8-bit R component in byte 0, an 8-bit G component in byte 1, and an 8-bit B component in byte 2.
		R8G8B8A8_UNorm,   //	A four-component, 32-bit unsigned normalized fvformatf that has an 8-bit R component in byte 0, an 8-bit G component in byte 1, an 8-bit B component in byte 2, and an 8-bit A component in byte 3.
		R8_SNorm,   //	A one-component, 8-bit signed normalized fvformatf that has a single 8-bit R component.
		R8G8_SNorm,   //	A two-component, 16-bit signed normalized fvformatf that has an 8-bit R component stored with sRGB nonlinear encoding in byte 0, and an 8-bit G component stored with sRGB nonlinear encoding in byte 1.
		R8G8B8_SNorm,   //	A three-component, 24-bit signed normalized fvformatf that has an 8-bit R component in byte 0, an 8-bit G component in byte 1, and an 8-bit B component in byte 2.
		R8G8B8A8_SNorm,   //	A four-component, 32-bit signed normalized fvformatf that has an 8-bit R component in byte 0, an 8-bit G component in byte 1, an 8-bit B component in byte 2, and an 8-bit A component in byte 3.
		R8_UInt,   //	A one-component, 8-bit unsigned integer fvformatf that has a single 8-bit R component.
		R8G8_UInt,   //	A two-component, 16-bit unsigned integer fvformatf that has an 8-bit R component in byte 0, and an 8-bit G component in byte 1.
		R8G8B8_UInt,   //	A three-component, 24-bit unsigned integer fvformatf that has an 8-bit R component in byte 0, an 8-bit G component in byte 1, and an 8-bit B component in byte 2.
		R8G8B8A8_UInt,   //	A four-component, 32-bit unsigned integer fvformatf that has an 8-bit R component in byte 0, an 8-bit G component in byte 1, an 8-bit B component in byte 2, and an 8-bit A component in byte 3.
		R8_SInt,   //	A one-component, 8-bit signed integer fvformatf that has a single 8-bit R component.
		R8G8_SInt,   //	A two-component, 16-bit signed integer fvformatf that has an 8-bit R component in byte 0, and an 8-bit G component in byte 1.
		R8G8B8_SInt,   //	A three-component, 24-bit signed integer fvformatf that has an 8-bit R component in byte 0, an 8-bit G component in byte 1, and an 8-bit B component in byte 2.
		R8G8B8A8_SInt,   //	A four-component, 32-bit signed integer fvformatf that has an 8-bit R component in byte 0, an 8-bit G component in byte 1, an 8-bit B component in byte 2, and an 8-bit A component in byte 3.
		R16_UNorm,   //	A one-component, 16-bit unsigned normalized fvformatf that has a single 16-bit R component.
		R16G16_UNorm,   //	A two-component, 32-bit unsigned normalized fvformatf that has a 16-bit R component in bytes 0..1, and a 16-bit G component in bytes 2..3.
		R16G16B16_UNorm,   //	A three-component, 48-bit unsigned normalized fvformatf that has a 16-bit R component in bytes 0..1, a 16-bit G component in bytes 2..3, and a 16-bit B component in bytes 4..5.
		R16G16B16A16_UNorm,   //	A four-component, 64-bit unsigned normalized fvformatf that has a 16-bit R component in bytes 0..1, a 16-bit G component in bytes 2..3, a 16-bit B component in bytes 4..5, and a 16-bit A component in bytes 6..7.
		R16_SNorm,   //	A one-component, 16-bit signed normalized fvformatf that has a single 16-bit R component.
		R16G16_SNorm,   //	A two-component, 32-bit signed normalized fvformatf that has a 16-bit R component in bytes 0..1, and a 16-bit G component in bytes 2..3.
		R16G16B16_SNorm,   //	A three-component, 48-bit signed normalized fvformatf that has a 16-bit R component in bytes 0..1, a 16-bit G component in bytes 2..3, and a 16-bit B component in bytes 4..5.
		R16G16B16A16_SNorm,   //	A four-component, 64-bit signed normalized fvformatf that has a 16-bit R component in bytes 0..1, a 16-bit G component in bytes 2..3, a 16-bit B component in bytes 4..5, and a 16-bit A component in bytes 6..7.
		R16_UInt,   //	A one-component, 16-bit unsigned integer fvformatf that has a single 16-bit R component.
		R16G16_UInt,   //	A two-component, 32-bit unsigned integer fvformatf that has a 16-bit R component in bytes 0..1, and a 16-bit G component in bytes 2..3.
		R16G16B16_UInt,   //	A three-component, 48-bit unsigned integer fvformatf that has a 16-bit R component in bytes 0..1, a 16-bit G component in bytes 2..3, and a 16-bit B component in bytes 4..5.
		R16G16B16A16_UInt,   //	A four-component, 64-bit unsigned integer fvformatf that has a 16-bit R component in bytes 0..1, a 16-bit G component in bytes 2..3, a 16-bit B component in bytes 4..5, and a 16-bit A component in bytes 6..7.
		R16_SInt,   //	A one-component, 16-bit signed integer fvformatf that has a single 16-bit R component.
		R16G16_SInt,   //	A two-component, 32-bit signed integer fvformatf that has a 16-bit R component in bytes 0..1, and a 16-bit G component in bytes 2..3.
		R16G16B16_SInt,   //	A three-component, 48-bit signed integer fvformatf that has a 16-bit R component in bytes 0..1, a 16-bit G component in bytes 2..3, and a 16-bit B component in bytes 4..5.
		R16G16B16A16_SInt,   //	A four-component, 64-bit signed integer fvformatf that has a 16-bit R component in bytes 0..1, a 16-bit G component in bytes 2..3, a 16-bit B component in bytes 4..5, and a 16-bit A component in bytes 6..7.
		R32_UInt,   //	A one-component, 32-bit unsigned integer fvformatf that has a single 32-bit R component.
		R32G32_UInt,   //	A two-component, 64-bit unsigned integer fvformatf that has a 32-bit R component in bytes 0..3, and a 32-bit G component in bytes 4..7.
		R32G32B32_UInt,   //	A three-component, 96-bit unsigned integer fvformatf that has a 32-bit R component in bytes 0..3, a 32-bit G component in bytes 4..7, and a 32-bit B component in bytes 8..11.
		R32G32B32A32_UInt,   //	A four-component, 128-bit unsigned integer fvformatf that has a 32-bit R component in bytes 0..3, a 32-bit G component in bytes 4..7, a 32-bit B component in bytes 8..11, and a 32-bit A component in bytes 12..15.
		R32_SInt,   //	A one-component, 32-bit signed integer fvformatf that has a single 32-bit R component.
		R32G32_SInt,   //	A two-component, 64-bit signed integer fvformatf that has a 32-bit R component in bytes 0..3, and a 32-bit G component in bytes 4..7.
		R32G32B32_SInt,   //	A three-component, 96-bit signed integer fvformatf that has a 32-bit R component in bytes 0..3, a 32-bit G component in bytes 4..7, and a 32-bit B component in bytes 8..11.
		R32G32B32A32_SInt,   //	A four-component, 128-bit signed integer fvformatf that has a 32-bit R component in bytes 0..3, a 32-bit G component in bytes 4..7, a 32-bit B component in bytes 8..11, and a 32-bit A component in bytes 12..15.
		R16_SFloat,   //	A one-component, 16-bit signed floating-point fvformatf that has a single 16-bit R component.
		R16G16_SFloat,   //	A two-component, 32-bit signed floating-point fvformatf that has a 16-bit R component in bytes 0..1, and a 16-bit G component in bytes 2..3.
		R16G16B16_SFloat,   //	A three-component, 48-bit signed floating-point fvformatf that has a 16-bit R component in bytes 0..1, a 16-bit G component in bytes 2..3, and a 16-bit B component in bytes 4..5.
		R16G16B16A16_SFloat,   //	A four-component, 64-bit signed floating-point fvformatf that has a 16-bit R component in bytes 0..1, a 16-bit G component in bytes 2..3, a 16-bit B component in bytes 4..5, and a 16-bit A component in bytes 6..7.
		R32_SFloat,   //	A one-component, 32-bit signed floating-point fvformatf that has a single 32-bit R component.
		R32G32_SFloat,   //	A two-component, 64-bit signed floating-point fvformatf that has a 32-bit R component in bytes 0..3, and a 32-bit G component in bytes 4..7.
		R32G32B32_SFloat,   //	A three-component, 96-bit signed floating-point fvformatf that has a 32-bit R component in bytes 0..3, a 32-bit G component in bytes 4..7, and a 32-bit B component in bytes 8..11.
		R32G32B32A32_SFloat,   //	A four-component, 128-bit signed floating-point fvformatf that has a 32-bit R component in bytes 0..3, a 32-bit G component in bytes 4..7, a 32-bit B component in bytes 8..11, and a 32-bit A component in bytes 12..15.
		B8G8R8_SRGB,   //	A three-component, 24-bit unsigned normalized fvformatf that has an 8-bit R component stored with sRGB nonlinear encoding in byte 0, an 8-bit G component stored with sRGB nonlinear encoding in byte 1, and an 8-bit B component stored with sRGB nonlinear encoding in byte 2.
		B8G8R8A8_SRGB,   //	A four-component, 32-bit unsigned normalized fvformatf that has an 8-bit B component stored with sRGB nonlinear encoding in byte 0, an 8-bit G component stored with sRGB nonlinear encoding in byte 1, an 8-bit R component stored with sRGB nonlinear encoding in byte 2, and an 8-bit A component in byte 3.
		B8G8R8_UNorm,   //	A three-component, 24-bit unsigned normalized fvformatf that has an 8-bit B component in byte 0, an 8-bit G component in byte 1, and an 8-bit R component in byte 2.
		B8G8R8A8_UNorm,   //	A four-component, 32-bit unsigned normalized fvformatf that has an 8-bit B component in byte 0, an 8-bit G component in byte 1, an 8-bit R component in byte 2, and an 8-bit A component in byte 3.
		B8G8R8_SNorm,   //	A three-component, 24-bit signed normalized fvformatf that has an 8-bit B component in byte 0, an 8-bit G component in byte 1, and an 8-bit R component in byte 2.
		B8G8R8A8_SNorm,   //	A four-component, 32-bit signed normalized fvformatf that has an 8-bit B component in byte 0, an 8-bit G component in byte 1, an 8-bit R component in byte 2, and an 8-bit A component in byte 3.
		B8G8R8_UInt,   //	A three-component, 24-bit unsigned integer fvformatf that has an 8-bit B component in byte 0, an 8-bit G component in byte 1, and an 8-bit R component in byte 2
		B8G8R8A8_UInt,   //	A four-component, 32-bit unsigned integer fvformatf that has an 8-bit B component in byte 0, an 8-bit G component in byte 1, an 8-bit R component in byte 2, and an 8-bit A component in byte 3.
		B8G8R8_SInt,   //	A three-component, 24-bit signed integer fvformatf that has an 8-bit B component in byte 0, an 8-bit G component in byte 1, and an 8-bit R component in byte 2.
		B8G8R8A8_SInt,   //	A four-component, 32-bit signed integer fvformatf that has an 8-bit B component in byte 0, an 8-bit G component in byte 1, an 8-bit R component in byte 2, and an 8-bit A component in byte 3.
		R4G4B4A4_UNormPack16,   //	A four-component, 16-bit packed unsigned normalized fvformatf that has a 4-bit R component in bits 12..15, a 4-bit G component in bits 8..11, a 4-bit B component in bits 4..7, and a 4-bit A component in bits 0..3.
		B4G4R4A4_UNormPack16,   //	A four-component, 16-bit packed unsigned normalized fvformatf that has a 4-bit B component in bits 12..15, a 4-bit G component in bits 8..11, a 4-bit R component in bits 4..7, and a 4-bit A component in bits 0..3.
		R5G6B5_UNormPack16,   //	A three-component, 16-bit packed unsigned normalized fvformatf that has a 5-bit R component in bits 11..15, a 6-bit G component in bits 5..10, and a 5-bit B component in bits 0..4.
		B5G6R5_UNormPack16, //	A three-component, 16-bit packed unsigned normalized fvformatf that has a 5-bit B component in bits 11..15, a 6-bit G component in bits 5..10, and a 5-bit R component in bits 0..4.
		R5G5B5A1_UNormPack16,   //	A four-component, 16-bit packed unsigned normalized fvformatf that has a 5-bit R component in bits 11..15, a 5-bit G component in bits 6..10, a 5-bit B component in bits 1..5, and a 1-bit A component in bit 0.
		B5G5R5A1_UNormPack16,   //	A four-component, 16-bit packed unsigned normalized fvformatf that has a 5-bit B component in bits 11..15, a 5-bit G component in bits 6..10, a 5-bit R component in bits 1..5, and a 1-bit A component in bit 0.
		A1R5G5B5_UNormPack16,   //	A four-component, 16-bit packed unsigned normalized fvformatf that has a 1-bit A component in bit 15, a 5-bit R component in bits 10..14, a 5-bit G component in bits 5..9, and a 5-bit B component in bits 0..4.
		E5B9G9R9_UFloatPack32,   //	A three-component, 32-bit packed unsigned floating-point fvformatf that has a 5-bit shared exponent in bits 27..31, a 9-bit B component mantissa in bits 18..26, a 9-bit G component mantissa in bits 9..17, and a 9-bit R component mantissa in bits 0..8.
		B10G11R11_UFloatPack32,   //	A three-component, 32-bit packed unsigned floating-point fvformatf that has a 10-bit B component in bits 22..31, an 11-bit G component in bits 11..21, an 11-bit R component in bits 0..10.
		A2B10G10R10_UNormPack32,   //	A four-component, 32-bit packed unsigned normalized fvformatf that has a 2-bit A component in bits 30..31, a 10-bit B component in bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit R component in bits 0..9.
		A2B10G10R10_UIntPack32,   //	A four-component, 32-bit packed unsigned integer fvformatf that has a 2-bit A component in bits 30..31, a 10-bit B component in bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit R component in bits 0..9.
		A2B10G10R10_SIntPack32,   //	A four-component, 32-bit packed signed integer fvformatf that has a 2-bit A component in bits 30..31, a 10-bit B component in bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit R component in bits 0..9.
		A2R10G10B10_UNormPack32,   //	A four-component, 32-bit packed unsigned normalized fvformatf that has a 2-bit A component in bits 30..31, a 10-bit R component in bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit B component in bits 0..9.
		A2R10G10B10_UIntPack32,   //	A four-component, 32-bit packed unsigned integer fvformatf that has a 2-bit A component in bits 30..31, a 10-bit R component in bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit B component in bits 0..9.
		A2R10G10B10_SIntPack32,   //	A four-component, 32-bit packed signed integer fvformatf that has a 2-bit A component in bits 30..31, a 10-bit R component in bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit B component in bits 0..9.
		//	A2R10G10B10_XRUNormPack32	A four-component, 32-bit packed unsigned normalized fvformatf that has a 2-bit A component in bits 30..31, a 10-bit R component in bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit B component in bits 0..9. The components are linearly encoded and their values range from -0.752941 to 1.25098 (pre-expansion). The alpha component is clamped to either 0.0 or 1.0 on sampling, rendering, and writing operations.
//	R10G10B10_XRSRGBPack32	A four-component, 32-bit packed unsigned normalized fvformatf that has a 10-bit R component in bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit B component in bits 0..9. The components are gamma encoded and their values range from -0.5271 to 1.66894. The alpha component is clamped to either 0.0 or 1.0 on sampling, rendering, and writing operations.
//	R10G10B10_XRUNormPack32	A four-component, 32-bit packed unsigned normalized fvformatf that has a 10-bit R component in bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit B component in bits 0..9. The components are linearly encoded and their values range from -0.752941 to 1.25098 (pre-expansion).
//	A10R10G10B10_XRSRGBPack32	A four-component, 64-bit packed unsigned normalized fvformatf that has a 10-bit A component in bits 30..39, a 10-bit R component in bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit B component in bits 0..9. The components are gamma encoded and their values range from -0.5271 to 1.66894. The alpha component is clamped to either 0.0 or 1.0 on sampling, rendering, and writing operations.
//	A10R10G10B10_XRUNormPack32	A four-component, 64-bit packed unsigned normalized fvformatf that has a 10-bit A component in bits 30..39, a 10-bit R component in bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit B component in bits 0..9. The components are linearly encoded and their values range from -0.752941 to 1.25098 (pre-expansion). The alpha component is clamped to either 0.0 or 1.0 on sampling, rendering, and writing operations.
//	RGBA_DXT1_SRGB	A three-component, block-compressed fvformatf. Each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGB texel data with sRGB nonlinear encoding. This format has a 1 bit alpha channel.
//	RGBA_DXT1_UNorm	A three-component, block-compressed fvformatf. Each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGB texel data. This format has a 1 bit alpha channel.
//	RGBA_DXT3_SRGB	A four-component, block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values with sRGB nonlinear encoding.
//	RGBA_DXT3_UNorm	A four-component, block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values.
//	RGBA_DXT5_SRGB	A four-component, block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values with sRGB nonlinear encoding.
//	RGBA_DXT5_UNorm	A four-component, block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values.
//	R_BC4_UNorm	A one-component, block-compressed fvformatf where each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized red texel data.
//	R_BC4_SNorm	A one-component, block-compressed fvformatf where each 64-bit compressed texel block encodes a 4×4 rectangle of signed normalized red texel data.
//	RG_BC5_UNorm	A two-component, block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RG texel data with the first 64 bits encoding red values followed by 64 bits encoding green values.
//	RG_BC5_SNorm	A two-component, block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of signed normalized RG texel data with the first 64 bits encoding red values followed by 64 bits encoding green values.
//	RGB_BC6H_UFloat	A three-component, block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned floating-point RGB texel data.
//	RGB_BC6H_SFloat	A three-component, block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of signed floating-point RGB texel data.
//	RGBA_BC7_SRGB	A four-component, block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data with sRGB nonlinear encoding applied to the RGB components.
//	RGBA_BC7_UNorm	A four-component, block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data.
//	RGB_PVRTC_2Bpp_SRGB	A three-component, PVRTC compressed fvformatf where each 64-bit compressed texel block encodes a 8×4 rectangle of unsigned normalized RGB texel data with sRGB nonlinear encoding. This format has no alpha and is considered opaque.
//	RGB_PVRTC_2Bpp_UNorm	A three-component, PVRTC compressed format where each 64-bit compressed texel block encodes a 8×4 rectangle of unsigned normalized RGB texel data. This fvformatf has no alpha and is considered opaque.
//	RGB_PVRTC_4Bpp_SRGB	A three-component, PVRTC compressed fvformatf where each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGB texel data with sRGB nonlinear encoding. This format has no alpha and is considered opaque.
//	RGB_PVRTC_4Bpp_UNorm	A three-component, PVRTC compressed fvformatf where each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGB texel data. This format has no alpha and is considered opaque.
//	RGBA_PVRTC_2Bpp_SRGB	A four-component, PVRTC compressed fvformatf where each 64-bit compressed texel block encodes a 8×4 rectangle of unsigned normalized RGBA texel data with the first 32 bits encoding alpha values followed by 32 bits encoding RGB values with sRGB nonlinear encoding applied.
//	RGBA_PVRTC_2Bpp_UNorm	A four-component, PVRTC compressed fvformatf where each 64-bit compressed texel block encodes a 8×4 rectangle of unsigned normalized RGBA texel data with the first 32 bits encoding alpha values followed by 32 bits encoding RGB values.
//	RGBA_PVRTC_4Bpp_SRGB	A four-component, PVRTC compressed fvformatf where each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data with the first 32 bits encoding alpha values followed by 32 bits encoding RGB values with sRGB nonlinear encoding applied.
//	RGBA_PVRTC_4Bpp_UNorm	A four-component, PVRTC compressed fvformatf where each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data with the first 32 bits encoding alpha values followed by 32 bits encoding RGB values.
//	RGB_ETC_UNorm	A three-component, ETC compressed format where each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGB texel data. This fvformatf has no alpha and is considered opaque.
//	RGB_ETC2_SRGB	A three-component, ETC2 compressed format where each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGB texel data with sRGB nonlinear encoding. This fvformatf has no alpha and is considered opaque.
//	RGB_ETC2_UNorm	A three-component, ETC2 compressed format where each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGB texel data. This fvformatf has no alpha and is considered opaque.
//	RGB_A1_ETC2_SRGB	A four-component, ETC2 compressed fvformatf where each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGB texel data with sRGB nonlinear encoding, and provides 1 bit of alpha.
//	RGB_A1_ETC2_UNorm	A four-component, ETC2 compressed fvformatf where each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGB texel data, and provides 1 bit of alpha.
//	RGBA_ETC2_SRGB	A four-component, ETC2 compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values with sRGB nonlinear encoding applied.
//	RGBA_ETC2_UNorm	A four-component, ETC2 compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values.
//	R_EAC_UNorm	A one-component, ETC2 compressed fvformatf where each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized red texel data.
//	R_EAC_SNorm	A one-component, ETC2 compressed fvformatf where each 64-bit compressed texel block encodes a 4×4 rectangle of signed normalized red texel data.
//	RG_EAC_UNorm	A two-component, ETC2 compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RG texel data with the first 64 bits encoding red values followed by 64 bits encoding green values.
//	RG_EAC_SNorm	A two-component, ETC2 compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of signed normalized RG texel data with the first 64 bits encoding red values followed by 64 bits encoding green values.
//	RGBA_ASTC4X4_SRGB	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data with sRGB nonlinear encoding applied to the RGB components.
//	RGBA_ASTC4X4_UNorm	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data.
//	RGBA_ASTC5X5_SRGB	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes a 5×5 rectangle of unsigned normalized RGBA texel data with sRGB nonlinear encoding applied to the RGB components.
//	RGBA_ASTC5X5_UNorm	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes a 5×5 rectangle of unsigned normalized RGBA texel data.
//	RGBA_ASTC6X6_SRGB	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes a 6×6 rectangle of unsigned normalized RGBA texel data with sRGB nonlinear encoding applied to the RGB components.
//	RGBA_ASTC6X6_UNorm	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes a 6×6 rectangle of unsigned normalized RGBA texel data.
//	RGBA_ASTC8X8_SRGB	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes an 8×8 rectangle of unsigned normalized RGBA texel data with sRGB nonlinear encoding applied to the RGB components.
//	RGBA_ASTC8X8_UNorm	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes an 8×8 rectangle of unsigned normalized RGBA texel data.
//	RGBA_ASTC10X10_SRGB	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes a 10×10 rectangle of unsigned normalized RGBA texel data with sRGB nonlinear encoding applied to the RGB components.
//	RGBA_ASTC10X10_UNorm	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes a 10×10 rectangle of unsigned normalized RGBA texel data.
//	RGBA_ASTC12X12_SRGB	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes a 12×12 rectangle of unsigned normalized RGBA texel data with sRGB nonlinear encoding applied to the RGB components.
//	RGBA_ASTC12X12_UNorm	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes a 12×12 rectangle of unsigned normalized RGBA texel data.
	};

	/**
	 *	Texture descriptor.
	 */
	typedef struct texture_desc_t {
		enum Target {
			eTexture1D = 0x1,       /*	1D texture.	*/
			eTexture2D = 0x2,       /*	2D texture.	*/
			eTexture3D = 0x4,       /*	3D texture.	*/
			eCubeMap = 0x8,         /*	Cubemap texture.	*/
			eCubeMapArray = 0x10,   /*	Cubemap array texture.	*/
			eTexture2DArray = 0x20, /*  */
		};

		/**
		 *	Texture format.
		 */
		enum Format {   //GraphicsFormat
			eNoFormat,
			eRGB = 0x1,    /*	RGB components.	*/
			eRGBA = 0x2,    /*	RGBA components.	*/
			eBGR = 0x3,    /*	BGR components.	*/
			eBGRA = 0x4,    /*	BGRA components.	*/
			eSRGB = 0x5,    /*	SRGB components.	*/
			eRG = 0x6,
			eA = 0x8,
			eSRGBA = 0x7,    /*	SRGBA components.	*/
			eSingleColor = 0x9,    /*	Single color component.	*/
			eDepth = 0xA,    /*	Depth component.	*/
			eStencil = 0xB,    /*	Stencil component.	*/
			eDepthStencil = 0xC,    /*	Depth and stencil componets.	*/
		};

		/**
		 *	Texture pixel type data.
		 */
		enum Type {
			eNoType,
			eUnsignedByte = 0x1,    /*	Each color component encoded in a single byte.	*/
			eSignedByte = 0x2,    /*	Each color component encoded in a single signed byte.	*/
			eUnsignedShort = 0x3,
			eSignedShort = 0x4,
			eUnsignedInt = 0x5,
			eSignedInt = 0x6,
			eFloat = 0x7,    /*	Each color component encoded in a single float.	*/
			eHalfFloat = 0x8,  /*  */
			eDouble = 0x9,  /*  */
			eUnsigned24_8 = 0xA,    /*	Each color component encoded.	*/
		};

		/**
		 *	Texture compression types.
		 */
		enum Compression {
			eNoCompression = 0x0,    /*	Non-compressed texture.	*/
			eCompression = 0x1,    /*	Compressed texture using default compression.   */
			eRGTC = 0x2,  /*  */
			eDXT1 = 0x4,  /*  */
			eDXT3 = 0x8,  /*  */
			eDXT4 = 0x10, /*  */
			eDXT5 = 0x20, /*  */
			eS3TC = 0x20, /*  */
			eBPTC = 0x40, /*  */
			e3DC = 0x80, /*  */
			eASTC_LDR = 0x100,/*  */
			eETC2 = 0x200,/*  */
		};

		Texture *originalTexture;   /*  Use viewport.   */

		/*  Target. */
		unsigned int target;            /*	Texture target.	*/
		unsigned int width;             /*	Texture width in pixels.	*/
		unsigned int height;            /*	Texture height in pixels.	*/
		unsigned int depth;             /*	Texture depth in pixels.	*/

		/*  */
		// Input format.
		//TODO add support for loading compressed data.
		//TODO add support for texture internal data type.
		unsigned int type;              /*	Texture data type.	*/

		//TODO REMOVE
		unsigned int format;            /*	Texture input data type.	*/

		//TODO Remove
		unsigned int internalformat;    /*	Texture internal data type.	*/
		TextureFormat pixelFormat;      /*  */
		GraphicFormat graphicFormat;    /*  */

		bool immutable;                 /**/

		/*  */
		unsigned int numlevel;          /*	Number of mipmaps.	*/
		unsigned int usemipmaps;        /*	Using mipmaps?	*/
		unsigned int nrSamples;         /*  Enable Multisampling.   */
		//TODO decouple to the graphic fvformatf.
		unsigned int compression;       /*	What Compression to use.	*/
		//TODO decouple to the graphic fvformatf.
		unsigned int srgb;              /*  Enable gamma correction.    */

		enum Swizzle {
			eNoSwizzle = 0x0,   /*  */
			eZero = 0x1,        /*  */
			eOne = 0x2,         /*  */
			eRed = 0x3,         /*  */
			eGreen = 0x4,       /*  */
			eBlue = 0x5,        /*  */
			eAlpha = 0x6,       /*  */
		};

		/*  Set swizzle.    */
		Swizzle Swizzler;       /*  */
		Swizzle Swizzleg;       /*  */
		Swizzle Swizzleb;       /*  */
		Swizzle Swizzlea;       /*  */

		/*	Texture pixel data.	*/
		union {
			const void *pixel;                    /*  Pixel pointer.  */
			struct {
				const void *cubepixel[6];         /*	Cubemap pixel point references. */
			};
			const void *pixelarray; /*  */
		};
		unsigned long pixelSize;        /*  Image size in bytes.    */
		SamplerDesc sampler;            /*  Texture sampling.   */

		/*  Debug attributes.   */
		MarkerDebug marker;
	} TextureDesc;

	/**
	 *
	 */
	typedef struct program_pipeline_desc_t {
		//TODO add binding.
		Shader *v;
		Shader *f;
		Shader *g;
		Shader *tc;
		Shader *te;
		Shader *c;
		/*  Debug attributes.   */
		MarkerDebug marker;
	} ProgramPipelineDesc;

	//TOOD rename all enum without the e infront.
	enum ShaderType {
		eUnknownShader,
		eVertex,        /*  */
		eFrag,          /*  */
		eGeom,          /*  */
		eTesseC,        /*  */
		eTesseE,        /*  */
		eCompute,       /*  */
	};

	enum ShaderCodeType
	{
		eNoShaderType, /*  Undefined.	*/
		eSourceCode,   /*  Source code.    */
		eBinary,	   /*  Binary code.    */
	};

	/**
	 *	Shader descriptor.
	 */
	typedef struct shader_desc_t {


		struct {
			const void *pdata;		/*	*/
			int binarySize;			/*	*/
			unsigned int format;	/*	*/
		} program;

		//TODO add binary format.
		/*  Rendering pipeline shaders. */
		struct {
			union {
				const char **vertexsource;            /*	Vertex shader sources.	*/
				const char *vertexBinary;           /*	Vertex shader sources.	*/
			};
			union {
				unsigned int numvert;                /*	Number of vertex shader string sources.	*/
				unsigned int size;
			};
			unsigned int binaryFormat;
			ShaderCodeType type;
			ShaderLanguage language;
		} vertex;
		struct {
			union {
				const char **fragmentsource;        /*	Fragment shader sources.	*/
				const char *fragmentBinary;         /*	Fragment shader sources.	*/
			};
			union {
				unsigned int numfrag;                /*	Number of vertex shader string sources.	*/
				unsigned int size;
			};
			unsigned int binaryFormat;
			ShaderCodeType type;
			ShaderLanguage language;
		} fragment;
		struct {
			union {
				const char **geometrysource;        /*	Vertex shader sources.	*/
				const char *geometryBinary;         /*	Vertex shader sources.	*/
			};
			union {
				unsigned int numgeo;                /*	Number of vertex shader string sources.	*/
				unsigned int size;
			};
			unsigned int binaryFormat;
			ShaderCodeType type;
			ShaderLanguage language;
		} geometry;
		struct {
			union {
				const char **tessellationco;            /*	Vertex shader sources.	*/
				const char *tessellationcoBinary;       /*	Vertex shader sources.	*/
			};
			union {
				unsigned int numtesco;                /*	Number of vertex shader string sources.	*/
				unsigned int size;
			};
			unsigned int binaryFormat;
			ShaderCodeType type;
			ShaderLanguage language;
		} tessellationControl;

		struct {
			union {
				const char **tessellationev;            /*	Vertex shader sources.	*/
				const char *tessellationeBinary;       /*	Vertex shader sources.	*/
			};
			union {
				unsigned int numtesev;                /*	Number of vertex shader string sources.	*/
				unsigned int size;
			};
			unsigned int binaryFormat;
			ShaderCodeType type;
			ShaderLanguage language;
		} tessellationEvolution;

		struct {
			union {
				const char **computeSource;            /*	Vertex shader sources.	*/
				const char *computeBinary;       /*	Vertex shader sources.	*/
			};
			union {
				unsigned int numcompute;                /*	Number of vertex shader string sources.	*/
				unsigned int size;
			};
			unsigned int binaryFormat;
			ShaderCodeType type;
			ShaderLanguage language;
		} Compute;

		bool separatetable;	/*	TODO evoluate if supported on all the API.	*/

		/*  Debug attributes.   */
		MarkerDebug marker;
	} ShaderDesc;

	/**
	 *	Buffer descriptor.
	 */
	typedef struct buffer_desc_t {
		/**
		 *
		 */
		enum BufferType {
			eArray,             /*	*/
			eElementArray,      /*	*/
			eUniform,           /*	*/
			eTexture,           /*  */
			eShaderStorage,     /*  */
			eTransformFeedback, /*  */
			ePixelUnpack,       /*  */
			ePixelPack,         /*  */
			eIndirectDraw,      /*  */
			eIndirectDispatch   /*  */
		};

		/**
		 *	Hint the usage of the
		 *	buffer object.
		 */
		enum BufferHint {
			eRead = 0x1,        /*  Reading from the buffer.    */
			eWrite = 0x2,       /*  Writing to the buffer.  */
			eStatic = 0x4,      /*  Written once.   */
			eDynamic = 0x8,     /*  Will be updated frequently.    */
			eStream = 0x10,     /*  */
		};

		enum BufferLoc{

		};

		/**
		 *
		 */
		unsigned int type;      /*	Buffer data type.	*/
		unsigned int size;      /*	Buffer size in bytes.	*/
		void *data;             /*	Buffer host data pointer.	*/
		BufferHint hint;        /*	Buffer hint of usages.	*/
		/*  Debug attributes.   */
		MarkerDebug marker;
	} BufferDesc;

/**
 *	Framebuffer description.
 */
	typedef struct framebuffer_desc_t {
		Texture *attach[16];    /*	Texture attachment.	*/
		Texture *depth;            /*	Depth attachment.	*/
		Texture *stencil;       /*	Stencil attachment.	*/
		Texture *depthstencil;  /*	depth stencil attachment.	*/
		/*  Debug attributes.   */
		MarkerDebug marker;
	} FrameBufferDesc;

	//TODO determine if packing compiler qualifier needed.
	/**
	 *
	 */
	typedef struct indirect_draw_array_t {
		unsigned int count;         /*  */
		unsigned int instanceCount; /*  */
		unsigned int first;         /*  */
		unsigned int baseInstance;  /*  */
	} IndirectDrawArray;

	/**
	 *
	 */
	typedef struct indirect_draw_element_t {
		unsigned int count;             /*  */
		unsigned int instanceCount;     /*  */
		unsigned int firstIndex;        /*  */
		unsigned int baseVertex;        /*  */
		unsigned int baseInstance;      /*  */
	} IndirectDrawElement;

	typedef struct indirect_dispatch_t {
		unsigned int num_groups_x;
		unsigned int num_groups_y;
		unsigned int num_groups_z;
	} IndirectDispatch;

	/**
	 *	Geometry description.
	 */
	typedef struct geometry_desc_t {
		enum Primitive {
			ePoint = 0x1,    /**/
			eLines = 0x2,    /**/
			eTriangles = 0x3,    /**/
			eTriangleStrips = 0x4,    /**/
			eTriangleAdjacant = 0x5,  /**/
		};

		enum AttributeType {
			eFloat = 0x1,    /*	*/
			eInt = 0x2,    /*	*/
			eDouble = 0x3,  /*  */
			eHalf = 0x4,  /*  */
		};

		/*	*/
		unsigned int primitive;             /*	Primitive type.	*/
		unsigned int numVerticecs;          /*	Number of vertices.	*/
		unsigned int numIndices;            /*	Number of indices.	*/
		unsigned int indicesStride;         /*	Size per indices in bytes.	*/
		unsigned int vertexStride;          /*	Size per vertex in bytes.	*/

		/*  */
		void *indices;                      /*	Indices host pointer data.	*/
		void *buffer;                       /*	Vertex buffer.	*/

		/*	*/
		unsigned int numVertexAttributes;    /*	Number of vertex attributes */
		struct {
			unsigned int index;                /*	Index of vertex attributes.	*/
			unsigned int size;                /*	Size in numbers of components (1 - 4).	*/
			unsigned int datatype;            /*	Data type ( AttributeType ) */
			unsigned int offset;            /*	Offset in bytes.	*/
		} vertexattribute[16];

		/*	bound.	*/
		bool autoBound;

		/*  Debug attributes.   */
		MarkerDebug marker;
	} GeometryDesc;


	/**
	 *	Query description.
	 */
	typedef struct query_desc_t {
		/*  Debug attributes.   */
		MarkerDebug marker;
	} QueryDesc;

	typedef struct sync_desc_t {
		/*  Debug attributes.   */
		MarkerDebug marker;
	} SyncDesc;

	/**
	 *
	 */
	typedef struct features_t{
		bool computeShaders;
		bool geomtryShader;
		bool tesselationShader;
		bool raytracing;
		bool multiviewports;
		bool samplerLodBias;
		bool drawBaseVertex;
		bool drawBaseInstance;
		bool drawIndirect;
		bool drawIndirectBaseInstance;
		bool independentBlend;
		bool mapBuffer;
		bool commandListDebugMarkers;
		bool variableRateShading;
		bool shaderFloat64;
		bool ansi;
	} Features;

	/**
	 *
	 */
	typedef struct compatibility_t {
		bool windowRenderer;
		bool sDepthStencil; /*  */
		bool sFramebuffer;
		bool sFrameBufferMSAA; /*  */
		bool sShaderPrecision;
		bool sNonAttachableFrameBuffer;
		bool sMSAA; /*  */
		bool sCSAA;
		bool sCubeMap;
		bool sCubeMapArray;   /*  */
		bool sTextureArray;   /*  */
		bool sMultiBind;      /*	Support multi bind texture.	*/
		bool sQueryOcclusion; /*  */
		bool sQueryTime;
		bool sRenderTarget;           /*  */
		bool sShaderImageLoadStorage; /*  */
		bool sShaderStorageBuffer;    /*  */
		bool sUniformBuffer;
		bool sMapBuffer;
		bool sInstancing;   /*  Instance rendering supported.   */
		bool sIndirectDraw; /*  */
		bool sIndirectMultiDraw;
		bool sSpritePoint;         /*  Point sprite supported. */
		bool sRenderTargetCubeMap; /*  */
		bool sShadow;              /*  */
		bool sVertexShader;
		bool sFragmentShader;     /*  */
		bool sGeometryShader;     /*  */
		bool sTessellation;       /*  */
		bool sComputeShader;      /*  */
		bool sTransformFeedback;  /*  */
		bool sTextureCompression; /*  */
		bool sSRGB;
		bool rayTracing;
		bool sVirtualViewPort;
		bool sConditionalRendering;
		bool sShaderBinary;
		bool sVirtualTexture; /*  */
		bool variableRateShading;

		/*	*/
		long int sMaxElementVertices;
		long int sMaxElementIndices;
		int sMaxVertexAttributes;

		/*	*/

		int sMaxTextureUnitActive; /**/
		int numMaxSubRoutines;     /**/

		/*	*/
		int sMaxUniformBufferBinding; /*		GL_MAX_UNIFORM_BUFFER_BINDINGS
										*/
		int sMaxUniformBlockSize;     /*		GL_MAX_UNIFORM_BLOCK_SIZE
										*/
		int sMaxUniformLocations;     /*		GL_MAX_UNIFORM_LOCATIONS
										*/

		/*  Textures.   */
		int sNumCompressedTexture;
		int sMaxTextureBufferSize;
		int sMaxTextureLODBias;
		int sMaxTextureSamples;
		int sMaxTextureSize;
		int sMaxTexture3DSize;
		int sMaxTextureArrayLayers;
		int sMaxCubeTextureSize;
		int sMaxAnisotropy;

		/*  Viewports.  */
		int sMaxViewPortDims;
		int sMaxViewPorts;
		int sViewPortBoundRange[2];

		/*  Render target.  */
		int sMaxFrameBufferWidth;
		int sMaxFrameBufferHeight;
		int sMaxFrameBufferLayers;
		int sMaxFrameBufferSamples;
		int sMaxDrawBuffers;           /*	GL_MAX_DRAW_BUFFERS	*/
		int sMaxColorAttachments;      /*	GL_MAX_COLOR_ATTACHMENTS
										*/
		int sMaxFrameBufferAttachment; /**/

		/*  Compute shaders. */
		int sMaxComputeTextureImageUnits;
		int sMaxComputeSharedMemory;
		int sMaxLocalWorkGroupSize[3];
		int sMaxWorKGroupSize[3];
		int sWorkGroupDimensions;

		/*  Line.   */
		float lineWidthGranularity;
		float lineWidthRange[2];

		int sMinMapBufferAlignment;
	} Capability;

	/**
	 *
	 */
//TODO improve.
	typedef struct device_info_t
	{
		const char *name;
		const int deviceType;
		const char *vendor;
		Capability capability;
	} DeviceInfo;
}

#endif
