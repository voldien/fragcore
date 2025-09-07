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
#ifndef _FRAGCORE_TEXTURE_H_
#define _FRAGCORE_TEXTURE_H_ 1
#include "ImageFormat.h"
#include "Sampler.h"
#include <Core/Object.h>

namespace fragcore {

	/**
	 * @brief Texture object.
	 *
	 */
	class FVDECLSPEC Texture : public Sampler {
		friend class IRenderer;

	  public:
		/**
		 *
		 */
		// TODO remove and replace with the main TextureFormat enum.
		enum Format {
			Alpha8 = 0,		// Alpha-only texture fvformatf.
			ARGB4444 = 1,	// A 16 bits/pixel texture fvformatf. Texture stores color with an alpha channel.
			RGB24 = 2,		// Color texture fvformatf, 8-bits per channel.
			RGBA32 = 3,		// Color with alpha texture fvformatf, 8-bits per channel.
			ARGB32 = 4,		// Color with alpha texture fvformatf, 8-bits per channel.
			RGB565 = 5,		// A 16 bit color texture fvformatf.
			R16 = 6,		// Single channel (R) texture fvformatf, 16 bit integer.
			DXT1 = 7,		//	Compressed color texture fvformatf.
			DXT5 = 8,		//	Compressed color with alpha channel texture fvformatf.
			RGBA4444 = 9,	//	Color and alpha texture fvformatf, 4 bit per channel.
			BGRA32 = 10,	//	Color with alpha texture fvformatf, 8-bits per channel.
			RHalf = 11,		//	Scalar (R) texture fvformatf, 16 bit floating point.
			RGHalf = 12,	//	Two color (RG) texture fvformatf, 16 bit floating point per channel.
			RGBAHalf = 13,	//	RGB color and alpha texture fvformatf, 16 bit floating point per channel.
			RFloat = 14,	//	Scalar (R) texture fvformatf, 32 bit floating point.
			RGFloat = 15,	//	Two color (RG) texture fvformatf, 32 bit floating point per channel.
			RGBAFloat = 16, //	RGB color and alpha texture fvformatf, 32-bit floats per channel.
			YUY2 = 17, //	A fvformatf that uses the YUV color space and is often used for video encoding or playback.
			RGB9e5Float = 18, //	RGB HDR fvformatf, with 9 bit mantissa per channel and a 5 bit shared exponent.
			BC4 = 19,		  //	Compressed one channel (R) texture fvformatf.
			BC5 = 20,		  //	Compressed two-channel (RG) texture fvformatf.
			BC6H = 21,		  //	HDR compressed color texture fvformatf.
			BC7 = 22,		  //	High quality compressed color texture fvformatf.
			DXT1Crunched =
				23,			   //	Compressed color texture fvformatf with Crunch compression for smaller storage sizes.
			DXT5Crunched = 24, //	Compressed color with alpha channel texture fvformatf with Crunch compression for
							   // smaller storage sizes.
			PVRTC_RGB2 = 25,   //	PowerVR (iOS) 2 bits/pixel compressed color texture fvformatf.
			PVRTC_RGBA2 = 26,  //	PowerVR (iOS) 2 bits/pixel compressed with alpha channel texture fvformatf.
			PVRTC_RGB4 = 27,   //	PowerVR (iOS) 4 bits/pixel compressed color texture fvformatf.
			PVRTC_RGBA4 = 28,  //	PowerVR (iOS) 4 bits/pixel compressed with alpha channel texture fvformatf.
			ETC_RGB4 = 29,	   //	ETC (GLES2.0) 4 bits/pixel compressed RGB texture fvformatf.
			EAC_R = 30, //	ETC2 / EAC (GL ES 3.0) 4 bits/pixel compressed unsigned single-channel texture fvformatf.
			EAC_R_SIGNED =
				31,		 //	ETC2 / EAC (GL ES 3.0) 4 bits/pixel compressed signed single-channel texture fvformatf.
			EAC_RG = 32, //	ETC2 / EAC (GL ES 3.0) 8 bits/pixel compressed unsigned dual-channel (RG) texture fvformatf.
			EAC_RG_SIGNED = 33,	   //	ETC2 / EAC (GL ES 3.0) 8 bits/pixel compressed signed dual-channel (RG) texture
								   // fvformatf.
			ETC2_RGB = 34,		   //	ETC2 (GL ES 3.0) 4 bits/pixel compressed RGB texture fvformatf.
			ETC2_RGBA1 = 35,	   //	ETC2 (GL ES 3.0) 4 bits/pixel RGB+1-bit alpha texture fvformatf.
			ETC2_RGBA8 = 36,	   //	ETC2 (GL ES 3.0) 8 bits/pixel compressed RGBA texture fvformatf.
			ASTC_4x4 = 37,		   //	ASTC (4x4 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			ASTC_5x5 = 38,		   //	ASTC (5x5 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			ASTC_6x6 = 39,		   //	ASTC (6x6 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			ASTC_8x8 = 40,		   //	ASTC (8x8 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			ASTC_10x10 = 41,	   //	ASTC (10x10 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			ASTC_12x12 = 42,	   //	ASTC (12x12 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			RG16 = 43,			   //	Two color (RG) texture fvformatf, 8-bits per channel.
			R8 = 44,			   //	Single channel (R) texture fvformatf, 8 bit integer.
			ETC_RGB4Crunched = 45, //	Compressed color texture fvformatf with Crunch compression for smaller storage
								   // sizes.
			ETC2_RGBA8Crunched =
				46,				  //	Compressed color with alpha channel texture fvformatf using Crunch compression for
								  // smaller storage sizes.
			ASTC_HDR_4x4 = 47,	  //	ASTC (4x4 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
			ASTC_HDR_5x5 = 48,	  //	ASTC (5x5 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
			ASTC_HDR_6x6 = 49,	  //	ASTC (6x6 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
			ASTC_HDR_8x8 = 50,	  //	ASTC (8x8 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			ASTC_HDR_10x10 = 51,  //	ASTC (10x10 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
			ASTC_HDR_12x12 = 52,  //	ASTC (12x12 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
			ASTC_RGB_4x4 = 53,	  //	ASTC (4x4 pixel block in 128 bits) compressed RGB texture fvformatf.
			ASTC_RGB_5x5 = 54,	  //	ASTC (5x5 pixel block in 128 bits) compressed RGB texture fvformatf.
			ASTC_RGB_6x6 = 55,	  //	ASTC (6x6 pixel block in 128 bits) compressed RGB texture fvformatf.
			ASTC_RGB_8x8 = 56,	  //	ASTC (8x8 pixel block in 128 bits) compressed RGB texture fvformatf.
			ASTC_RGB_10x10 = 57,  //	ASTC (10x10 pixel block in 128 bits) compressed RGB texture fvformatf.
			ASTC_RGB_12x12 = 58,  //	ASTC (12x12 pixel block in 128 bits) compressed RGB texture fvformatf.
			ASTC_RGBA_4x4 = 59,	  //	ASTC (4x4 pixel block in 128 bits) compressed RGBA texture fvformatf.
			ASTC_RGBA_5x5 = 60,	  //	ASTC (5x5 pixel block in 128 bits) compressed RGBA texture fvformatf.
			ASTC_RGBA_6x6 = 61,	  //	ASTC (6x6 pixel block in 128 bits) compressed RGBA texture fvformatf.
			ASTC_RGBA_8x8 = 62,	  //	ASTC (8x8 pixel block in 128 bits) compressed RGBA texture fvformatf.
			ASTC_RGBA_10x10 = 63, //	ASTC (10x10 pixel block in 128 bits) compressed RGBA texture fvformatf.
			ASTC_RGBA_12x12 = 64, //	ASTC (12x12 pixel block in 128 bits) compressed RGBA texture fvformatf.

			eR8G8B8 = 0x1,	 /*  */
			eR8G8B8A8 = 0x2, /*  */
			eRGB32F = 0x3,	 /*  */
			eRGBA32F = 0x4,	 /*  */
			eR8_SRGB = 0x5,
			eR8G8_SRGB = 6,
			eR8G8B8_SRGB = 7,
			R8G8B8A8_SRGB = 8,
		};

		enum MapTarget {
			eRead = 1,		/*  */
			eWrite = 2,		/*  */
			eReadWrite = 3, /*  */
		};

		~Texture() override = default;

		/**
		 *
		 * @param index
		 */
		virtual void bind(unsigned int index) = 0;

		/**
		 *
		 * @param index
		 * @param level
		 * @param format
		 */
		virtual void bindImage(unsigned int index, int level, MapTarget target, Format format) = 0;

		/**
		 *
		 * @return
		 */
		virtual bool isValid() = 0;

		/**
		 *
		 * @param sampler
		 */
		virtual void setSampler(Sampler *sampler) = 0;

		/**
		 *
		 * @param level
		 */
		virtual void setMipLevel(unsigned int level) = 0;

		/**
		 *
		 * @param mode
		 */
		void setFilterMode(FilterMode mode) override = 0;

		/**
		 *
		 * @return
		 */
		FilterMode getFilterMode() override = 0;

		/**
		 *
		 * @param mode
		 */
		void setWrapMode(WrapMode mode) override = 0;

		/**
		 *
		 * @return
		 */
		WrapMode getWrapMode() override = 0;

		/**
		 *
		 * @param anisotropic
		 */
		void setAnisotropic(float anisotropic) override = 0;

		/**
		 *
		 * @return
		 */
		float getAnisotropic() const override = 0;

		CompareFunc getCompare() const override = 0;

		void setCompareFunc(CompareFunc compareFunc) override = 0;

		void setMipMapBaseLevel(unsigned int level) override = 0;

		unsigned int getMipMapBaseLevel() const override = 0;

		void setMipMapBias(float bias) override = 0;

		float getMipMapBias(float bias) const override = 0;

		void setBorderColor(float color) override = 0;

		float getBorderColor() const override = 0;

		unsigned int setMaxLod(unsigned int level) override = 0;

		unsigned int getMaxLod() const override = 0;

		unsigned int setMinLod(unsigned int level) override = 0;

		unsigned int getMinLod() const override = 0;

		virtual Format getFormat() const = 0;

		virtual unsigned int width() = 0;

		virtual unsigned int height() = 0;

		virtual unsigned int layers() const = 0;

		// TODO add mip map streaming.

		virtual void resize(int width, int height, Texture::Format format, bool hasMipMap) = 0;

		/**
		 *
		 * @param size
		 * @return
		 */
		virtual void *mapTexture(Format format, unsigned int level) = 0;

		/**
		 *
		 */
		virtual void unMapTexture() = 0;

		/**
		 *
		 * @param pixels
		 * @param size
		 */
		virtual void setPixels(Format format, unsigned int level, const void *pixels, unsigned long size) = 0;

		/**
		 *
		 * @param mipmap
		 * @return
		 */
		virtual void *getPixels(ImageFormat format, unsigned int level, unsigned long *nBytes) = 0;

		virtual void clear() = 0;
	};
} // namespace fragcore

#endif
