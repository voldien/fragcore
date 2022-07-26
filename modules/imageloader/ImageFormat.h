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
#ifndef _FRAG_CORE_IMAGE_FORMAT_H_
#define _FRAG_CORE_IMAGE_FORMAT_H_ 1
#include <FragCore.h>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	enum class TextureFormat {
		Alpha8,	  // 	Alpha-only texture fvformatf.
		ARGB4444, //	A 16 bits/pixel texture fvformatf. Texture stores color with an alpha channel.
		RGB24,	  //	Color texture fvformatf, 8-bits per channel.
		RGBA32,	  //	Color with alpha texture fvformatf, 8-bits per channel.
		ARGB32,	  //	Color with alpha texture fvformatf, 8-bits per channel.
		RGB565,	  //	A 16 bit color texture fvformatf.
		R16,	  //	Single channel (R) texture fvformatf, 16 bit integer.
		DXT1,	  //	Compressed color texture fvformatf.
		DXT5,	  //	Compressed color with alpha channel texture fvformatf.
		RGBA4444, //	Color and alpha texture fvformatf, 4 bit per channel.
		BGRA32,	  //	Color with alpha texture fvformatf, 8-bits per channel.
		BGR24,
		RHalf,		   //	Scalar (R) texture fvformatf, 16 bit floating point.
		RGHalf,		   //	Two color (RG) texture fvformatf, 16 bit floating point per channel.
		RGBAHalf,	   //	RGB color and alpha texture fvformatf, 16 bit floating point per channel.
		RFloat,		   //	Scalar (R) texture fvformatf, 32 bit floating point.
		RGFloat,	   //	Two color (RG) texture fvformatf, 32 bit floating point per channel.
		RGBAFloat,	   //	RGB color and alpha texture fvformatf, 32-bit floats per channel.
		YUY2,		   //	A format that uses the YUV color space and is often used for video encoding or playback.
		RGB9e5Float,   //	RGB HDR fvformatf, with 9 bit mantissa per channel and a 5 bit shared exponent.
		BC4,		   //	Compressed one channel (R) texture fvformatf.
		BC5,		   //	Compressed two-channel (RG) texture fvformatf.
		BC6H,		   //	HDR compressed color texture fvformatf.
		BC7,		   //	High quality compressed color texture fvformatf.
		DXT1Crunched,  //	Compressed color texture fvformatf with Crunch compression for smaller storage sizes.
		DXT5Crunched,  //	Compressed color with alpha channel texture fvformatf with Crunch compression for smaller
					   // storage sizes.
		PVRTC_RGB2,	   //	PowerVR (iOS) 2 bits/pixel compressed color texture fvformatf.
		PVRTC_RGBA2,   // width	PowerVR (iOS) 2 bits/pixel compressed with alpha channel texture fvformatf.
		PVRTC_RGB4,	   //	PowerVR (iOS) 4 bits/pixel compressed color texture fvformatf.
		PVRTC_RGBA4,   //	PowerVR (iOS) 4 bits/pixel compressed with alpha channel texture fvformatf.
		ETC_RGB4,	   //	ETC (GLES2.0) 4 bits/pixel compressed RGB texture fvformatf.
		EAC_R,		   // / EAC (GL ES 3.0) 4 bits/pixel compressed unsigned single-channel texture fvformatf.
		EAC_R_SIGNED,  //	ETC2 / EAC (GL ES 3.0) 4 bits/pixel compressed signed single-channel texture fvformatf.
		EAC_RG,		   //	ETC2 / EAC (GL ES 3.0) 8 bits/pixel compressed unsigned dual-channel (RG) texture fvformatf.
		EAC_RG_SIGNED, //	ETC2 / EAC (GL ES 3.0) 8 bits/pixel compressed signed dual-channel (RG) texture fvformatf.
		ETC2_RGB,	   //	ETC2 (GL ES 3.0) 4 bits/pixel compressed RGB texture fvformatf.
		ETC2_RGBA1,	   //	ETC2 (GL ES 3.0) 4 bits/pixel RGB+1-bit alpha texture fvformatf.
		ETC2_RGBA8,	   //	ETC2 (GL ES 3.0) 8 bits/pixel compressed RGBA texture fvformatf.
		ASTC_4x4,	   //	ASTC (4x4 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		ASTC_5x5,	   //	ASTC (5x5 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		ASTC_6x6,	   //	ASTC (6x6 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		ASTC_8x8,	   //	ASTC (8x8 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		ASTC_10x10,	   //	ASTC (10x10 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		ASTC_12x12,	   //	ASTC (12x12 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		RG16,		   //	Two color (RG) texture fvformatf, 8-bits per channel.
		R8,			   //	Single channel (R) texture fvformatf, 8 bit integer.
		ETC_RGB4Crunched,	//	Compressed color texture fvformatf with Crunch compression for smaller storage sizes.
		ETC2_RGBA8Crunched, //	Compressed color with alpha channel texture fvformatf using Crunch compression for
							// smaller storage sizes.
		ASTC_HDR_4x4,		//	ASTC (4x4 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
		ASTC_HDR_5x5,		//	ASTC (5x5 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
		ASTC_HDR_6x6,		//	ASTC (6x6 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
		ASTC_HDR_8x8,		//	ASTC (8x8 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
		ASTC_HDR_10x10,		//	ASTC (10x10 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
		ASTC_HDR_12x12,		//	ASTC (12x12 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
		ASTC_RGB_4x4,		//	ASTC (4x4 pixel block in 128 bits) compressed RGB texture fvformatf.
		ASTC_RGB_5x5,		//	ASTC (5x5 pixel block in 128 bits) compressed RGB texture fvformatf.
		ASTC_RGB_6x6,		//	ASTC (6x6 pixel block in 128 bits) compressed RGB texture fvformatf.
		ASTC_RGB_8x8,		//	ASTC (8x8 pixel block in 128 bits) compressed RGB texture fvformatf.
		ASTC_RGB_10x10,		//	ASTC (10x10 pixel block in 128 bits) compressed RGB texture fvformatf.
		ASTC_RGB_12x12,		//	ASTC (12x12 pixel block in 128 bits) compressed RGB texture fvformatf.
		ASTC_RGBA_4x4,		//	ASTC (4x4 pixel block in 128 bits) compressed RGBA texture fvformatf.
		ASTC_RGBA_5x5,		//	ASTC (5x5 pixel block in 128 bits) compressed RGBA texture fvformatf.
		ASTC_RGBA_6x6,		//	ASTC (6x6 pixel block in 128 bits) compressed RGBA texture fvformatf.
		ASTC_RGBA_8x8,		//	ASTC (8x8 pixel block in 128 bits) compressed RGBA texture fvformatf.
		ASTC_RGBA_10x10,	//	ASTC (10x10 pixel block in 128 bits) compressed RGBA texture fvformatf.
		ASTC_RGBA_12x12,	//	ASTC (12x12 pixel block in 128 bits) compressed RGBA texture fvformatf.
	};
} // namespace fragcore
#endif