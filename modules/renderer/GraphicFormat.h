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
#ifndef _FRAG_CORE_GRAPHIC_FORMAT_H_
#define _FRAG_CORE_GRAPHIC_FORMAT_H_ 1

namespace fragcore {
	enum class GraphicFormat {
		NoneGraphicFormat, // The fvformatf is not specified.
		R8_SRGB, // A one-component, 8-bit unsigned normalized fvformatf that has a single 8-bit R component stored with
				 // sRGB nonlinear encoding.
		R8G8_SRGB, // A two-component, 16-bit unsigned normalized fvformatf that has an 8-bit R component stored with
				   // sRGB nonlinear encoding in byte 0, and an 8-bit G component stored with sRGB nonlinear encoding in
				   // byte 1.
		R8G8B8_SRGB, // A three-component, 24-bit unsigned normalized fvformatf that has an 8-bit R component stored
					 // with sRGB nonlinear encoding in byte 0, an 8-bit G component stored with sRGB nonlinear encoding
					 // in byte 1, and an 8-bit B component stored with sRGB nonlinear encoding in byte 2.
		R8G8B8A8_SRGB, // A four-component, 32-bit unsigned normalized fvformatf that has an 8-bit R component stored
					   // with sRGB nonlinear encoding in byte 0, an 8-bit G component stored with sRGB nonlinear
					   // encoding in byte 1, an 8-bit B component stored with sRGB nonlinear encoding in byte 2, and an
					   // 8-bit A component in byte 3.
		R8_UNorm,	   // A one-component, 8-bit unsigned normalized fvformatf that has a single 8-bit R component.
		R8G8_UNorm,	  // A two-component, 16-bit unsigned normalized fvformatf that has an 8-bit R component stored with
					  // sRGB nonlinear encoding in byte 0, and an 8-bit G component stored with sRGB nonlinear encoding
					  // in byte 1.
		R8G8B8_UNorm, // A three-component, 24-bit unsigned normalized fvformatf that has an 8-bit R component in byte
					  // 0, an 8-bit G component in byte 1, and an 8-bit B component in byte 2.
		R8G8B8A8_UNorm, //	A four-component, 32-bit unsigned normalized fvformatf that has an 8-bit R component in byte
						// 0, an 8-bit G component in byte 1, an 8-bit B component in byte 2, and an 8-bit A component
						// in byte 3.
		R8_SNorm,	  //	A one-component, 8-bit signed normalized fvformatf that has a single 8-bit R component.
		R8G8_SNorm,	  //	A two-component, 16-bit signed normalized fvformatf that has an 8-bit R component stored with
					  // sRGB nonlinear encoding in byte 0, and an 8-bit G component stored with sRGB nonlinear encoding
					  // in byte 1.
		R8G8B8_SNorm, //	A three-component, 24-bit signed normalized fvformatf that has an 8-bit R component in byte
					  // 0, an 8-bit G component in byte 1, and an 8-bit B component in byte 2.
		R8G8B8A8_SNorm, //	A four-component, 32-bit signed normalized fvformatf that has an 8-bit R component in byte
						// 0, an 8-bit G component in byte 1, an 8-bit B component in byte 2, and an 8-bit A component
						// in byte 3.
		R8_UInt,	 //	A one-component, 8-bit unsigned integer fvformatf that has a single 8-bit R component.
		R8G8_UInt,	 //	A two-component, 16-bit unsigned integer fvformatf that has an 8-bit R component in byte 0, and
					 // an 8-bit G component in byte 1.
		R8G8B8_UInt, //	A three-component, 24-bit unsigned integer fvformatf that has an 8-bit R component in byte 0, an
					 // 8-bit G component in byte 1, and an 8-bit B component in byte 2.
		R8G8B8A8_UInt, //	A four-component, 32-bit unsigned integer fvformatf that has an 8-bit R component in byte 0,
					   // an 8-bit G component in byte 1, an 8-bit B component in byte 2, and an 8-bit A component in
					   // byte 3.
		R8_SInt,	   //	A one-component, 8-bit signed integer fvformatf that has a single 8-bit R component.
		R8G8_SInt,	 //	A two-component, 16-bit signed integer fvformatf that has an 8-bit R component in byte 0, and an
					 // 8-bit G component in byte 1.
		R8G8B8_SInt, //	A three-component, 24-bit signed integer fvformatf that has an 8-bit R component in byte 0, an
					 // 8-bit G component in byte 1, and an 8-bit B component in byte 2.
		R8G8B8A8_SInt,	 //	A four-component, 32-bit signed integer fvformatf that has an 8-bit R component in byte 0,
						 // an 8-bit G component in byte 1, an 8-bit B component in byte 2, and an 8-bit A component in
						 // byte 3.
		R16_UNorm,		 //	A one-component, 16-bit unsigned normalized fvformatf that has a single 16-bit R component.
		R16G16_UNorm,	 //	A two-component, 32-bit unsigned normalized fvformatf that has a 16-bit R component in bytes
						 // 0..1, and a 16-bit G component in bytes 2..3.
		R16G16B16_UNorm, //	A three-component, 48-bit unsigned normalized fvformatf that has a 16-bit R component in
						 // bytes 0..1, a 16-bit G component in bytes 2..3, and a 16-bit B component in bytes 4..5.
		R16G16B16A16_UNorm, //	A four-component, 64-bit unsigned normalized fvformatf that has a 16-bit R component in
							// bytes 0..1, a 16-bit G component in bytes 2..3, a 16-bit B component in bytes 4..5, and a
							// 16-bit A component in bytes 6..7.
		R16_SNorm,			//	A one-component, 16-bit signed normalized fvformatf that has a single 16-bit R component.
		R16G16_SNorm,	 //	A two-component, 32-bit signed normalized fvformatf that has a 16-bit R component in bytes
						 // 0..1, and a 16-bit G component in bytes 2..3.
		R16G16B16_SNorm, //	A three-component, 48-bit signed normalized fvformatf that has a 16-bit R component in bytes
						 // 0..1, a 16-bit G component in bytes 2..3, and a 16-bit B component in bytes 4..5.
		R16G16B16A16_SNorm, //	A four-component, 64-bit signed normalized fvformatf that has a 16-bit R component in
							// bytes 0..1, a 16-bit G component in bytes 2..3, a 16-bit B component in bytes 4..5, and a
							// 16-bit A component in bytes 6..7.
		R16_UInt,			//	A one-component, 16-bit unsigned integer fvformatf that has a single 16-bit R component.
		R16G16_UInt, //	A two-component, 32-bit unsigned integer fvformatf that has a 16-bit R component in bytes 0..1,
					 // and a 16-bit G component in bytes 2..3.
		R16G16B16_UInt, //	A three-component, 48-bit unsigned integer fvformatf that has a 16-bit R component in bytes
						// 0..1, a 16-bit G component in bytes 2..3, and a 16-bit B component in bytes 4..5.
		R16G16B16A16_UInt, //	A four-component, 64-bit unsigned integer fvformatf that has a 16-bit R component in
						   // bytes 0..1, a 16-bit G component in bytes 2..3, a 16-bit B component in bytes 4..5, and a
						   // 16-bit A component in bytes 6..7.
		R16_SInt,		   //	A one-component, 16-bit signed integer fvformatf that has a single 16-bit R component.
		R16G16_SInt, //	A two-component, 32-bit signed integer fvformatf that has a 16-bit R component in bytes 0..1,
					 // and a 16-bit G component in bytes 2..3.
		R16G16B16_SInt, //	A three-component, 48-bit signed integer fvformatf that has a 16-bit R component in bytes
						// 0..1, a 16-bit G component in bytes 2..3, and a 16-bit B component in bytes 4..5.
		R16G16B16A16_SInt, //	A four-component, 64-bit signed integer fvformatf that has a 16-bit R component in bytes
						   // 0..1, a 16-bit G component in bytes 2..3, a 16-bit B component in bytes 4..5, and a 16-bit
						   // A component in bytes 6..7.
		R32_UInt,		   //	A one-component, 32-bit unsigned integer fvformatf that has a single 32-bit R component.
		R32G32_UInt, //	A two-component, 64-bit unsigned integer fvformatf that has a 32-bit R component in bytes 0..3,
					 // and a 32-bit G component in bytes 4..7.
		R32G32B32_UInt, //	A three-component, 96-bit unsigned integer fvformatf that has a 32-bit R component in bytes
						// 0..3, a 32-bit G component in bytes 4..7, and a 32-bit B component in bytes 8..11.
		R32G32B32A32_UInt, //	A four-component, 128-bit unsigned integer fvformatf that has a 32-bit R component in
						   // bytes 0..3, a 32-bit G component in bytes 4..7, a 32-bit B component in bytes 8..11, and a
						   // 32-bit A component in bytes 12..15.
		R32_SInt,		   //	A one-component, 32-bit signed integer fvformatf that has a single 32-bit R component.
		R32G32_SInt, //	A two-component, 64-bit signed integer fvformatf that has a 32-bit R component in bytes 0..3,
					 // and a 32-bit G component in bytes 4..7.
		R32G32B32_SInt, //	A three-component, 96-bit signed integer fvformatf that has a 32-bit R component in bytes
						// 0..3, a 32-bit G component in bytes 4..7, and a 32-bit B component in bytes 8..11.
		R32G32B32A32_SInt, //	A four-component, 128-bit signed integer fvformatf that has a 32-bit R component in
						   // bytes 0..3, a 32-bit G component in bytes 4..7, a 32-bit B component in bytes 8..11, and a
						   // 32-bit A component in bytes 12..15.
		R16_SFloat,	   //	A one-component, 16-bit signed floating-point fvformatf that has a single 16-bit R component.
		R16G16_SFloat, //	A two-component, 32-bit signed floating-point fvformatf that has a 16-bit R component in
					   // bytes 0..1, and a 16-bit G component in bytes 2..3.
		R16G16B16_SFloat, //	A three-component, 48-bit signed floating-point fvformatf that has a 16-bit R component
						  // in bytes 0..1, a 16-bit G component in bytes 2..3, and a 16-bit B component in bytes 4..5.
		R16G16B16A16_SFloat, //	A four-component, 64-bit signed floating-point fvformatf that has a 16-bit R component
							 // in bytes 0..1, a 16-bit G component in bytes 2..3, a 16-bit B component in bytes 4..5,
							 // and a 16-bit A component in bytes 6..7.
		R32_SFloat,	   //	A one-component, 32-bit signed floating-point fvformatf that has a single 32-bit R component.
		R32G32_SFloat, //	A two-component, 64-bit signed floating-point fvformatf that has a 32-bit R component in
					   // bytes 0..3, and a 32-bit G component in bytes 4..7.
		R32G32B32_SFloat, //	A three-component, 96-bit signed floating-point fvformatf that has a 32-bit R component
						  // in bytes 0..3, a 32-bit G component in bytes 4..7, and a 32-bit B component in bytes 8..11.
		R32G32B32A32_SFloat, //	A four-component, 128-bit signed floating-point fvformatf that has a 32-bit R component
							 // in bytes 0..3, a 32-bit G component in bytes 4..7, a 32-bit B component in bytes 8..11,
							 // and a 32-bit A component in bytes 12..15.
		B8G8R8_SRGB, //	A three-component, 24-bit unsigned normalized fvformatf that has an 8-bit R component stored
					 // with sRGB nonlinear encoding in byte 0, an 8-bit G component stored with sRGB nonlinear encoding
					 // in byte 1, and an 8-bit B component stored with sRGB nonlinear encoding in byte 2.
		B8G8R8A8_SRGB, //	A four-component, 32-bit unsigned normalized fvformatf that has an 8-bit B component stored
					   // with sRGB nonlinear encoding in byte 0, an 8-bit G component stored with sRGB nonlinear
					   // encoding in byte 1, an 8-bit R component stored with sRGB nonlinear encoding in byte 2, and an
					   // 8-bit A component in byte 3.
		B8G8R8_UNorm,  //	A three-component, 24-bit unsigned normalized fvformatf that has an 8-bit B component in
					   // byte 0, an 8-bit G component in byte 1, and an 8-bit R component in byte 2.
		B8G8R8A8_UNorm, //	A four-component, 32-bit unsigned normalized fvformatf that has an 8-bit B component in byte
						// 0, an 8-bit G component in byte 1, an 8-bit R component in byte 2, and an 8-bit A component
						// in byte 3.
		B8G8R8_SNorm,	//	A three-component, 24-bit signed normalized fvformatf that has an 8-bit B component in byte
						// 0, an 8-bit G component in byte 1, and an 8-bit R component in byte 2.
		B8G8R8A8_SNorm, //	A four-component, 32-bit signed normalized fvformatf that has an 8-bit B component in byte
						// 0, an 8-bit G component in byte 1, an 8-bit R component in byte 2, and an 8-bit A component
						// in byte 3.
		B8G8R8_UInt, //	A three-component, 24-bit unsigned integer fvformatf that has an 8-bit B component in byte 0, an
					 // 8-bit G component in byte 1, and an 8-bit R component in byte 2
		B8G8R8A8_UInt, //	A four-component, 32-bit unsigned integer fvformatf that has an 8-bit B component in byte 0,
					   // an 8-bit G component in byte 1, an 8-bit R component in byte 2, and an 8-bit A component in
					   // byte 3.
		B8G8R8_SInt,   //	A three-component, 24-bit signed integer fvformatf that has an 8-bit B component in byte 0, an
					   // 8-bit G component in byte 1, and an 8-bit R component in byte 2.
		B8G8R8A8_SInt, //	A four-component, 32-bit signed integer fvformatf that has an 8-bit B component in byte 0,
					   // an 8-bit G component in byte 1, an 8-bit R component in byte 2, and an 8-bit A component in
					   // byte 3.
		R4G4B4A4_UNormPack16, //	A four-component, 16-bit packed unsigned normalized fvformatf that has a 4-bit R
							  // component in bits 12..15, a 4-bit G component in bits 8..11, a 4-bit B component in
							  // bits 4..7, and a 4-bit A component in bits 0..3.
		B4G4R4A4_UNormPack16, //	A four-component, 16-bit packed unsigned normalized fvformatf that has a 4-bit B
							  // component in bits 12..15, a 4-bit G component in bits 8..11, a 4-bit R component in
							  // bits 4..7, and a 4-bit A component in bits 0..3.
		R5G6B5_UNormPack16,	  //	A three-component, 16-bit packed unsigned normalized fvformatf that has a 5-bit R
							  // component in bits 11..15, a 6-bit G component in bits 5..10, and a 5-bit B component in
							  // bits 0..4.
		B5G6R5_UNormPack16,	  //	A three-component, 16-bit packed unsigned normalized fvformatf that has a 5-bit B
							  // component in bits 11..15, a 6-bit G component in bits 5..10, and a 5-bit R component in
							  // bits 0..4.
		R5G5B5A1_UNormPack16, //	A four-component, 16-bit packed unsigned normalized fvformatf that has a 5-bit R
							  // component in bits 11..15, a 5-bit G component in bits 6..10, a 5-bit B component in
							  // bits 1..5, and a 1-bit A component in bit 0.
		B5G5R5A1_UNormPack16, //	A four-component, 16-bit packed unsigned normalized fvformatf that has a 5-bit B
							  // component in bits 11..15, a 5-bit G component in bits 6..10, a 5-bit R component in
							  // bits 1..5, and a 1-bit A component in bit 0.
		A1R5G5B5_UNormPack16, //	A four-component, 16-bit packed unsigned normalized fvformatf that has a 1-bit A
							  // component in bit 15, a 5-bit R component in bits 10..14, a 5-bit G component in
							  // bits 5..9, and a 5-bit B component in bits 0..4.
		E5B9G9R9_UFloatPack32, //	A three-component, 32-bit packed unsigned floating-point fvformatf that has a 5-bit
							   // shared exponent in bits 27..31, a 9-bit B component mantissa in bits 18..26, a 9-bit G
							   // component mantissa in bits 9..17, and a 9-bit R component mantissa in bits 0..8.
		B10G11R11_UFloatPack32,	 //	A three-component, 32-bit packed unsigned floating-point fvformatf that has a 10-bit
								 // B component in bits 22..31, an 11-bit G component in bits 11..21, an 11-bit R
								 // component in bits 0..10.
		A2B10G10R10_UNormPack32, //	A four-component, 32-bit packed unsigned normalized fvformatf that has a 2-bit A
								 // component in bits 30..31, a 10-bit B component in bits 20..29, a 10-bit G component
								 // in bits 10..19, and a 10-bit R component in bits 0..9.
		A2B10G10R10_UIntPack32,	 //	A four-component, 32-bit packed unsigned integer fvformatf that has a 2-bit A
								 // component in bits 30..31, a 10-bit B component in bits 20..29, a 10-bit G component
								 // in bits 10..19, and a 10-bit R component in bits 0..9.
		A2B10G10R10_SIntPack32,	 //	A four-component, 32-bit packed signed integer fvformatf that has a 2-bit A
								 // component in bits 30..31, a 10-bit B component in bits 20..29, a 10-bit G component
								 // in bits 10..19, and a 10-bit R component in bits 0..9.
		A2R10G10B10_UNormPack32, //	A four-component, 32-bit packed unsigned normalized fvformatf that has a 2-bit A
								 // component in bits 30..31, a 10-bit R component in bits 20..29, a 10-bit G component
								 // in bits 10..19, and a 10-bit B component in bits 0..9.
		A2R10G10B10_UIntPack32,	 //	A four-component, 32-bit packed unsigned integer fvformatf that has a 2-bit A
								 // component in bits 30..31, a 10-bit R component in bits 20..29, a 10-bit G component
								 // in bits 10..19, and a 10-bit B component in bits 0..9.
		A2R10G10B10_SIntPack32,	 //	A four-component, 32-bit packed signed integer fvformatf that has a 2-bit A
								 // component in bits 30..31, a 10-bit R component in bits 20..29, a 10-bit G component
								 // in bits 10..19, and a 10-bit B component in bits 0..9.
		//	A2R10G10B10_XRUNormPack32	A four-component, 32-bit packed unsigned normalized fvformatf that has a 2-bit A
		// component in bits 30..31, a 10-bit R component in bits 20..29, a 10-bit G component in bits 10..19, and a
		// 10-bit B component in bits 0..9. The components are linearly encoded and their values range from -0.752941
		// to 1.25098 (pre-expansion). The alpha component is clamped to either 0.0 or 1.0 on sampling, rendering, and
		// writing operations.
		//	R10G10B10_XRSRGBPack32	A four-component, 32-bit packed unsigned normalized fvformatf that has a 10-bit R
		// component in bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit B component in bits 0..9. The
		// components are gamma encoded and their values range from -0.5271 to 1.66894. The alpha component is clamped
		// to either 0.0 or 1.0 on sampling, rendering, and writing operations. 	R10G10B10_XRUNormPack32	A
		// four-component, 32-bit packed unsigned normalized fvformatf that has a 10-bit R component in bits 20..29, a
		// 10-bit G component in bits 10..19, and a 10-bit B component in bits 0..9. The components are linearly encoded
		// and their values
		// range from -0.752941 to 1.25098 (pre-expansion). 	A10R10G10B10_XRSRGBPack32	A four-component, 64-bit
		// packed unsigned normalized fvformatf that has a 10-bit A component in bits 30..39, a 10-bit R component in
		// bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit B component in bits 0..9. The components are
		// gamma encoded and their values range from -0.5271 to 1.66894. The alpha component is clamped to either 0.0
		// or 1.0 on sampling, rendering, and writing operations. 	A10R10G10B10_XRUNormPack32	A four-component, 64-bit
		// packed unsigned normalized fvformatf that has a 10-bit A component in bits 30..39, a 10-bit R component in
		// bits 20..29, a 10-bit G component in bits 10..19, and a 10-bit B component in bits 0..9. The components are
		// linearly encoded and their values range from -0.752941 to 1.25098 (pre-expansion). The alpha component is
		// clamped to either 0.0 or 1.0 on sampling, rendering, and writing operations.
		//	RGBA_DXT1_SRGB	A three-component, block-compressed fvformatf. Each 64-bit compressed texel block encodes a
		// 4×4 rectangle of unsigned normalized RGB texel data with sRGB nonlinear encoding. This format has a 1 bit
		// alpha channel. 	RGBA_DXT1_UNorm	A three-component, block-compressed fvformatf. Each 64-bit compressed texel
		// block encodes a 4×4 rectangle of unsigned normalized RGB texel data. This format has a 1 bit alpha channel.
		//	RGBA_DXT3_SRGB	A four-component, block-compressed fvformatf where each 128-bit compressed texel block
		// encodes a 4×4 rectangle of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values
		// followed by 64 bits encoding RGB values with sRGB nonlinear encoding. 	RGBA_DXT3_UNorm	A four-component,
		// block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned
		// normalized RGBA texel data with the first 64 bits encoding alpha values followed by 64 bits encoding RGB
		// values.
		//	RGBA_DXT5_SRGB	A four-component, block-compressed fvformatf where each 128-bit compressed texel block
		// encodes a 4×4 rectangle of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values
		// followed by 64 bits encoding RGB values with sRGB nonlinear encoding. 	RGBA_DXT5_UNorm	A four-component,
		// block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned
		// normalized RGBA texel data with the first 64 bits encoding alpha values followed by 64 bits encoding RGB
		// values. 	R_BC4_UNorm	A one-component, block-compressed fvformatf where each 64-bit compressed texel block
		// encodes a 4×4 rectangle of unsigned normalized red texel data.
		//	R_BC4_SNorm	A one-component, block-compressed fvformatf where each 64-bit compressed texel block encodes a
		// 4×4 rectangle of signed normalized red texel data. 	RG_BC5_UNorm	A two-component, block-compressed
		// fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RG texel
		// data with the first 64 bits encoding red values followed by 64 bits encoding green values. 	RG_BC5_SNorm
		// A two-component, block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle
		// of signed normalized RG texel data with the first 64 bits encoding red values followed by 64 bits encoding
		// green values. 	RGB_BC6H_UFloat	A three-component, block-compressed fvformatf where each 128-bit compressed
		// texel block encodes a 4×4 rectangle of unsigned floating-point RGB texel data. 	RGB_BC6H_SFloat	A
		// three-component, block-compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle
		// of signed floating-point RGB texel data. 	RGBA_BC7_SRGB	A four-component, block-compressed fvformatf
		// where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data with
		// sRGB nonlinear encoding applied to the RGB components. 	RGBA_BC7_UNorm	A four-component, block-compressed
		// fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel
		// data. 	RGB_PVRTC_2Bpp_SRGB	A three-component, PVRTC compressed fvformatf where each 64-bit compressed texel
		// block encodes a 8×4 rectangle of unsigned normalized RGB texel data with sRGB nonlinear encoding. This format
		// has no alpha and is considered opaque. 	RGB_PVRTC_2Bpp_UNorm	A three-component, PVRTC compressed format
		// where each 64-bit compressed texel block encodes a 8×4 rectangle of unsigned normalized RGB texel data. This
		// fvformatf has no alpha and is considered opaque. 	RGB_PVRTC_4Bpp_SRGB	A three-component, PVRTC compressed
		// fvformatf where each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGB texel
		// data with sRGB nonlinear encoding. This format has no alpha and is considered opaque. 	RGB_PVRTC_4Bpp_UNorm
		// A three-component, PVRTC compressed fvformatf where each 64-bit compressed texel block encodes a 4×4
		// rectangle of unsigned normalized RGB texel data. This format has no alpha and is considered opaque.
		// RGBA_PVRTC_2Bpp_SRGB A four-component, PVRTC compressed fvformatf where each 64-bit compressed texel block
		// encodes a 8×4 rectangle of unsigned normalized RGBA texel data with the first 32 bits encoding alpha values
		// followed by 32 bits encoding RGB values with sRGB nonlinear encoding applied. 	RGBA_PVRTC_2Bpp_UNorm	A
		// four-component, PVRTC compressed fvformatf where each 64-bit compressed texel block encodes a 8×4 rectangle
		// of unsigned normalized RGBA texel data with the first 32 bits encoding alpha values followed by 32 bits
		// encoding RGB values. 	RGBA_PVRTC_4Bpp_SRGB	A four-component, PVRTC compressed fvformatf where each
		// 64-bit compressed
		// texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data with the first 32 bits encoding
		// alpha values followed by 32 bits encoding RGB values with sRGB nonlinear encoding applied.
		// RGBA_PVRTC_4Bpp_UNorm	A four-component, PVRTC compressed fvformatf where each 64-bit compressed texel
		// block encodes a 4×4 rectangle of unsigned normalized RGBA texel data with the first 32 bits encoding alpha
		// values followed by 32 bits encoding RGB values. 	RGB_ETC_UNorm	A three-component, ETC compressed format
		// where each 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGB texel data. This
		// fvformatf
		// has no alpha and is considered opaque. 	RGB_ETC2_SRGB	A three-component, ETC2 compressed format where each
		// 64-bit compressed texel block encodes a 4×4 rectangle of unsigned normalized RGB texel data with sRGB
		// nonlinear encoding. This fvformatf has no alpha and is considered opaque. 	RGB_ETC2_UNorm	A
		// three-component, ETC2 compressed format where each 64-bit compressed texel block encodes a 4×4 rectangle of
		// unsigned normalized RGB texel data. This fvformatf has no alpha and is considered opaque. 	RGB_A1_ETC2_SRGB
		// A four-component, ETC2 compressed fvformatf where each 64-bit compressed texel block encodes a 4×4 rectangle
		// of unsigned normalized RGB texel data with sRGB nonlinear encoding, and provides 1 bit of alpha.
		//	RGB_A1_ETC2_UNorm	A four-component, ETC2 compressed fvformatf where each 64-bit compressed texel block
		// encodes a 4×4 rectangle of unsigned normalized RGB texel data, and provides 1 bit of alpha.
		//	RGBA_ETC2_SRGB	A four-component, ETC2 compressed fvformatf where each 128-bit compressed texel block
		// encodes a 4×4 rectangle of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values
		// followed by 64 bits encoding RGB values with sRGB nonlinear encoding applied. 	RGBA_ETC2_UNorm	A
		// four-component, ETC2 compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle
		// of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values followed by 64 bits
		// encoding RGB values. 	R_EAC_UNorm	A one-component, ETC2 compressed fvformatf where each 64-bit compressed
		// texel block encodes a 4×4 rectangle of unsigned normalized red texel data. 	R_EAC_SNorm	A one-component,
		// ETC2 compressed fvformatf where each 64-bit compressed texel block encodes a 4×4 rectangle of signed
		// normalized red texel data. 	RG_EAC_UNorm	A two-component, ETC2 compressed fvformatf where each 128-bit
		// compressed texel block encodes a 4×4 rectangle of unsigned normalized RG texel data with the first 64 bits
		// encoding red values followed by 64 bits encoding green values. 	RG_EAC_SNorm	A two-component, ETC2
		// compressed fvformatf where each 128-bit compressed texel block encodes a 4×4 rectangle of signed normalized
		// RG texel data with the first 64 bits encoding red values followed by 64 bits encoding green values.
		// RGBA_ASTC4X4_SRGB	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block
		// encodes a 4×4 rectangle of unsigned normalized RGBA texel data with sRGB nonlinear encoding applied to the
		// RGB components. 	RGBA_ASTC4X4_UNorm	A four-component, ASTC compressed fvformatf where each 128-bit
		// compressed texel block encodes a 4×4 rectangle of unsigned normalized RGBA texel data. 	RGBA_ASTC5X5_SRGB
		// A
		// four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes a 5×5 rectangle
		// of
		// unsigned normalized RGBA texel data with sRGB nonlinear encoding applied to the RGB components.
		// RGBA_ASTC5X5_UNorm	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block
		// encodes a 5×5 rectangle of unsigned normalized RGBA texel data. 	RGBA_ASTC6X6_SRGB	A four-component, ASTC
		// compressed fvformatf where each 128-bit compressed texel block encodes a 6×6 rectangle of unsigned normalized
		// RGBA texel data with sRGB nonlinear encoding applied to the RGB components. 	RGBA_ASTC6X6_UNorm	A
		// four-component, ASTC compressed fvformatf where each 128-bit compressed texel block encodes a 6×6 rectangle
		// of
		// unsigned normalized RGBA texel data. 	RGBA_ASTC8X8_SRGB	A four-component, ASTC compressed fvformatf
		// where each 128-bit compressed texel block encodes an 8×8 rectangle of unsigned normalized RGBA texel data
		// with sRGB nonlinear encoding applied to the RGB components. 	RGBA_ASTC8X8_UNorm	A four-component, ASTC
		// compressed fvformatf where each 128-bit compressed texel block encodes an 8×8 rectangle of unsigned
		// normalized RGBA texel data. 	RGBA_ASTC10X10_SRGB	A four-component, ASTC compressed fvformatf where each
		// 128-bit compressed texel
		// block encodes a 10×10 rectangle of unsigned normalized RGBA texel data with sRGB nonlinear encoding applied
		// to
		// the RGB components. 	RGBA_ASTC10X10_UNorm	A four-component, ASTC compressed fvformatf where each 128-bit
		// compressed texel block encodes a 10×10 rectangle of unsigned normalized RGBA texel data.
		//	RGBA_ASTC12X12_SRGB	A four-component, ASTC compressed fvformatf where each 128-bit compressed texel block
		// encodes a 12×12 rectangle of unsigned normalized RGBA texel data with sRGB nonlinear encoding applied to the
		// RGB components. 	RGBA_ASTC12X12_UNorm	A four-component, ASTC compressed fvformatf where each 128-bit
		// compressed texel block encodes a 12×12 rectangle of unsigned normalized RGBA texel data.
	};
}

#endif