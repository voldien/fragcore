/*
 *	FragCore - Fragment Core - Engine Core
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
#ifndef _FRAGCORE_IMAGE_FORMAT_H_
#define _FRAGCORE_IMAGE_FORMAT_H_ 1

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	enum class ImageFormat : unsigned int {
		Alpha8,				// 	Alpha-only texture format.
		ARGB4444,			//	A 16 bits/pixel texture format. Texture stores color with an alpha channel.
		RGB24,				//	Color texture format, 8-bits per channel.
		RGBA32,				//	Color with alpha texture format, 8-bits per channel.
		ARGB32,				//	Color with alpha texture format, 8-bits per channel.
		RGB565,				//	A 16 bit color texture format.
		RG16,				//	Two color (RG) texture format, 8-bits per channel.
		R8,					//	Single channel (R) texture format, 8 bit integer.
		R16,				//	Single channel (R) texture format, 16 bit integer.
		R16U,				//	Single channel (R) texture format, 16 bit integer.
		R32,				//	Single channel (R) texture format, 32 bit integer.
		R32U,				//	Single channel (R) texture format, 32 bit integer.
		DXT1,				//	Compressed color texture format.
		DXT5,				//	Compressed color with alpha channel texture format.
		RGBA4444,			//	Color and alpha texture format, 4 bit per channel.
		BGRA32,				//	Color with alpha texture format, 8-bits per channel.
		BGR24,				//  Color without alpha texture format, 8-bits per channel.
		RHalf,				//	Scalar (R) texture format, 16 bit floating point.
		RGHalf,				//	Two color (RG) texture format, 16 bit floating point per channel.
		RGBAHalf,			//	RGB color texture format, 16 bit floating point per channel.
		RFloat,				//	Scalar (R) texture format, 32 bit floating point.
		RGFloat,			//	Two color (RG) texture format, 32 bit floating point per channel.
		RGBFloat,			//	RGB color and alpha texture format, 32-bit floats per channel.
		RGBAFloat,			//	RGB color and alpha texture format, 32-bit floats per channel.
		YUY2,				//	A format that uses the YUV color space and is often used for video encoding or playback.
		RGB9e5Float,		//	RGB HDR format, with 9 bit mantissa per channel and a 5 bit shared exponent.
		BC4,				//	Compressed one channel (R) texture format.
		BC5,				//	Compressed two-channel (RG) texture format.
		BC6H,				//	HDR compressed color texture format.
		BC7,				//	High quality compressed color texture format.
		DXT1Crunched,		//	Compressed color texture format with Crunch compression for smaller storage sizes.
		DXT5Crunched,		//	Compressed color with alpha channel texture format with Crunch compression for smaller
							// storage sizes.
		PVRTC_RGB2,			//	PowerVR (iOS) 2 bits/pixel compressed color texture format.
		PVRTC_RGBA2,		//	width	PowerVR (iOS) 2 bits/pixel compressed with alpha channel texture format.
		PVRTC_RGB4,			//	PowerVR (iOS) 4 bits/pixel compressed color texture format.
		PVRTC_RGBA4,		//	PowerVR (iOS) 4 bits/pixel compressed with alpha channel texture format.
		ETC_RGB4,			//	ETC (GLES2.0) 4 bits/pixel compressed RGB texture format.
		EAC_R,				//	EAC (GL ES 3.0) 4 bits/pixel compressed unsigned single-channel texture format.
		EAC_R_SIGNED,		//	ETC2 / EAC (GL ES 3.0) 4 bits/pixel compressed signed single-channel texture format.
		EAC_RG,				//	ETC2 / EAC (GL ES 3.0) 8 bits/pixel compressed unsigned dual-channel (RG) texture format.
		EAC_RG_SIGNED,		//	ETC2 / EAC (GL ES 3.0) 8 bits/pixel compressed signed dual-channel (RG) texture format.
		ETC2_RGB,			//	ETC2 (GL ES 3.0) 4 bits/pixel compressed RGB texture format.
		ETC2_RGBA1,			//	ETC2 (GL ES 3.0) 4 bits/pixel RGB+1-bit alpha texture format.
		ETC2_RGBA8,			//	ETC2 (GL ES 3.0) 8 bits/pixel compressed RGBA texture format.
		ASTC_4x4,			//	ASTC (4x4 pixel block in 128 bits) compressed RGB(A) texture format.
		ASTC_5x5,			//	ASTC (5x5 pixel block in 128 bits) compressed RGB(A) texture format.
		ASTC_6x6,			//	ASTC (6x6 pixel block in 128 bits) compressed RGB(A) texture format.
		ASTC_8x8,			//	ASTC (8x8 pixel block in 128 bits) compressed RGB(A) texture format.
		ASTC_10x10,			//	ASTC (10x10 pixel block in 128 bits) compressed RGB(A) texture format.
		ASTC_12x12,			//	ASTC (12x12 pixel block in 128 bits) compressed RGB(A) texture format.
		ETC_RGB4Crunched,	//	Compressed color texture format with Crunch compression for smaller storage sizes.
		ETC2_RGBA8Crunched, //	Compressed color with alpha channel texture format using Crunch compression for smaller,
							// storage sizes.

		ASTC_HDR_4x4,	 //	ASTC (4x4 pixel block in 128 bits) compressed RGB(A) HDR texture format.
		ASTC_HDR_5x5,	 //	ASTC (5x5 pixel block in 128 bits) compressed RGB(A) HDR texture format.
		ASTC_HDR_6x6,	 //	ASTC (6x6 pixel block in 128 bits) compressed RGB(A) HDR texture format.
		ASTC_HDR_8x8,	 //	ASTC (8x8 pixel block in 128 bits) compressed RGB(A) texture format.
		ASTC_HDR_10x10,	 //	ASTC (10x10 pixel block in 128 bits) compressed RGB(A) HDR texture format.
		ASTC_HDR_12x12,	 //	ASTC (12x12 pixel block in 128 bits) compressed RGB(A) HDR texture format.
		ASTC_RGB_4x4,	 //	ASTC (4x4 pixel block in 128 bits) compressed RGB texture format.
		ASTC_RGB_5x5,	 //	ASTC (5x5 pixel block in 128 bits) compressed RGB texture format.
		ASTC_RGB_6x6,	 //	ASTC (6x6 pixel block in 128 bits) compressed RGB texture format.
		ASTC_RGB_8x8,	 //	ASTC (8x8 pixel block in 128 bits) compressed RGB texture format.
		ASTC_RGB_10x10,	 //	ASTC (10x10 pixel block in 128 bits) compressed RGB texture format.
		ASTC_RGB_12x12,	 //	ASTC (12x12 pixel block in 128 bits) compressed RGB texture format.
		ASTC_RGBA_4x4,	 //	ASTC (4x4 pixel block in 128 bits) compressed RGBA texture format.
		ASTC_RGBA_5x5,	 //	ASTC (5x5 pixel block in 128 bits) compressed RGBA texture format.
		ASTC_RGBA_6x6,	 //	ASTC (6x6 pixel block in 128 bits) compressed RGBA texture format.
		ASTC_RGBA_8x8,	 //	ASTC (8x8 pixel block in 128 bits) compressed RGBA texture format.
		ASTC_RGBA_10x10, //	ASTC (10x10 pixel block in 128 bits) compressed RGBA texture format.
		ASTC_RGBA_12x12, //	ASTC (12x12 pixel block in 128 bits) compressed RGBA texture format.
	};
} // namespace fragcore
#endif