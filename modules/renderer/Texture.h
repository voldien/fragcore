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
#ifndef _FRAG_CORE_TEXTURE_H_
#define _FRAG_CORE_TEXTURE_H_ 1
#include <Core/Object.h>
#include "RenderObject.h"
#include "Sampler.h"
#include "RenderDesc.h"

namespace fragcore {
	/**
	 *	Texture object.
	 */
	class FVDECLSPEC Texture : public Sampler {
		friend class IRenderer;

	public:

		/**
		 *
		 */
		 //TODO remove and replace with the main TextureFormat enum.
		enum Format {
			Alpha8,    //Alpha-only texture fvformatf.
			ARGB4444,    //A 16 bits/pixel texture fvformatf. Texture stores color with an alpha channel.
			RGB24,    //Color texture fvformatf, 8-bits per channel.
			RGBA32,    //Color with alpha texture fvformatf, 8-bits per channel.
			ARGB32,    //Color with alpha texture fvformatf, 8-bits per channel.
			RGB565,    //A 16 bit color texture fvformatf.
			R16,    //Single channel (R) texture fvformatf, 16 bit integer.
			DXT1,    //	Compressed color texture fvformatf.
			DXT5,    //	Compressed color with alpha channel texture fvformatf.
			RGBA4444,    //	Color and alpha texture fvformatf, 4 bit per channel.
			BGRA32,    //	Color with alpha texture fvformatf, 8-bits per channel.
			RHalf,    //	Scalar (R) texture fvformatf, 16 bit floating point.
			RGHalf,    //	Two color (RG) texture fvformatf, 16 bit floating point per channel.
			RGBAHalf,    //	RGB color and alpha texture fvformatf, 16 bit floating point per channel.
			RFloat,    //	Scalar (R) texture fvformatf, 32 bit floating point.
			RGFloat,    //	Two color (RG) texture fvformatf, 32 bit floating point per channel.
			RGBAFloat,    //	RGB color and alpha texture fvformatf, 32-bit floats per channel.
			YUY2,    //	A fvformatf that uses the YUV color space and is often used for video encoding or playback.
			RGB9e5Float,    //	RGB HDR fvformatf, with 9 bit mantissa per channel and a 5 bit shared exponent.
			BC4,    //	Compressed one channel (R) texture fvformatf.
			BC5,    //	Compressed two-channel (RG) texture fvformatf.
			BC6H,    //	HDR compressed color texture fvformatf.
			BC7,    //	High quality compressed color texture fvformatf.
			DXT1Crunched,    //	Compressed color texture fvformatf with Crunch compression for smaller storage sizes.
			DXT5Crunched,    //	Compressed color with alpha channel texture fvformatf with Crunch compression for smaller storage sizes.
			PVRTC_RGB2,    //	PowerVR (iOS) 2 bits/pixel compressed color texture fvformatf.
			PVRTC_RGBA2,    //	PowerVR (iOS) 2 bits/pixel compressed with alpha channel texture fvformatf.
			PVRTC_RGB4,    //	PowerVR (iOS) 4 bits/pixel compressed color texture fvformatf.
			PVRTC_RGBA4,    //	PowerVR (iOS) 4 bits/pixel compressed with alpha channel texture fvformatf.
			ETC_RGB4,    //	ETC (GLES2.0) 4 bits/pixel compressed RGB texture fvformatf.
			EAC_R,    //	ETC2 / EAC (GL ES 3.0) 4 bits/pixel compressed unsigned single-channel texture fvformatf.
			EAC_R_SIGNED,    //	ETC2 / EAC (GL ES 3.0) 4 bits/pixel compressed signed single-channel texture fvformatf.
			EAC_RG,    //	ETC2 / EAC (GL ES 3.0) 8 bits/pixel compressed unsigned dual-channel (RG) texture fvformatf.
			EAC_RG_SIGNED,    //	ETC2 / EAC (GL ES 3.0) 8 bits/pixel compressed signed dual-channel (RG) texture fvformatf.
			ETC2_RGB,    //	ETC2 (GL ES 3.0) 4 bits/pixel compressed RGB texture fvformatf.
			ETC2_RGBA1,    //	ETC2 (GL ES 3.0) 4 bits/pixel RGB+1-bit alpha texture fvformatf.
			ETC2_RGBA8,    //	ETC2 (GL ES 3.0) 8 bits/pixel compressed RGBA texture fvformatf.
			ASTC_4x4,    //	ASTC (4x4 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			ASTC_5x5,    //	ASTC (5x5 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			ASTC_6x6,    //	ASTC (6x6 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			ASTC_8x8,    //	ASTC (8x8 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			ASTC_10x10,    //	ASTC (10x10 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			ASTC_12x12,    //	ASTC (12x12 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			RG16,    //	Two color (RG) texture fvformatf, 8-bits per channel.
			R8,    //	Single channel (R) texture fvformatf, 8 bit integer.
			ETC_RGB4Crunched,    //	Compressed color texture fvformatf with Crunch compression for smaller storage sizes.
			ETC2_RGBA8Crunched,    //	Compressed color with alpha channel texture fvformatf using Crunch compression for smaller storage sizes.
			ASTC_HDR_4x4,    //	ASTC (4x4 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
			ASTC_HDR_5x5,    //	ASTC (5x5 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
			ASTC_HDR_6x6,    //	ASTC (6x6 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
			ASTC_HDR_8x8,    //	ASTC (8x8 pixel block in 128 bits) compressed RGB(A) texture fvformatf.
			ASTC_HDR_10x10,    //	ASTC (10x10 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
			ASTC_HDR_12x12,    //	ASTC (12x12 pixel block in 128 bits) compressed RGB(A) HDR texture fvformatf.
			ASTC_RGB_4x4,    //	ASTC (4x4 pixel block in 128 bits) compressed RGB texture fvformatf.
			ASTC_RGB_5x5,    //	ASTC (5x5 pixel block in 128 bits) compressed RGB texture fvformatf.
			ASTC_RGB_6x6,    //	ASTC (6x6 pixel block in 128 bits) compressed RGB texture fvformatf.
			ASTC_RGB_8x8,    //	ASTC (8x8 pixel block in 128 bits) compressed RGB texture fvformatf.
			ASTC_RGB_10x10,    //	ASTC (10x10 pixel block in 128 bits) compressed RGB texture fvformatf.
			ASTC_RGB_12x12,    //	ASTC (12x12 pixel block in 128 bits) compressed RGB texture fvformatf.
			ASTC_RGBA_4x4,    //	ASTC (4x4 pixel block in 128 bits) compressed RGBA texture fvformatf.
			ASTC_RGBA_5x5,    //	ASTC (5x5 pixel block in 128 bits) compressed RGBA texture fvformatf.
			ASTC_RGBA_6x6,    //	ASTC (6x6 pixel block in 128 bits) compressed RGBA texture fvformatf.
			ASTC_RGBA_8x8,    //	ASTC (8x8 pixel block in 128 bits) compressed RGBA texture fvformatf.
			ASTC_RGBA_10x10,    //	ASTC (10x10 pixel block in 128 bits) compressed RGBA texture fvformatf.
			ASTC_RGBA_12x12,    //	ASTC (12x12 pixel block in 128 bits) compressed RGBA texture fvformatf.

			eR8G8B8 = 0x1,  /*  */
			eR8G8B8A8 = 0x2,  /*  */
			eRGB32F = 0x3,  /*  */
			eRGBA32F = 0x4,  /*  */
			eR8_SRGB = 0x5,
			eR8G8_SRGB,
			eR8G8B8_SRGB,
			R8G8B8A8_SRGB,
		};

		enum MapTarget {
			eRead = 1,          /*  */
			eWrite = 2,         /*  */
			eReadWrite = 3,     /*  */
		};

		Texture() = default;

		/**
		 *
		 * @param index
		 */
		virtual void bind(unsigned int index);

		/**
		 *
		 * @param index
		 * @param level
		 * @param format
		 */
		virtual void bindImage(unsigned int index, int level, MapTarget target, Format format);

		/**
		 *
		 * @return
		 */
		virtual bool isValid();

		/**
		 *
		 * @param sampler
		 */
		virtual void setSampler(Sampler *sampler);

		/**
		 *
		 * @param level
		 */
		virtual void setMipLevel(unsigned int level);

		/**
		 *
		 * @param mode
		 */
		virtual void setFilterMode(FilterMode mode) override;

		/**
		 *
		 * @return
		 */
		virtual FilterMode getFilterMode() override;

		/**
		 *
		 * @param mode
		 */
		virtual void setWrapMode(WrapMode mode) override;

		/**
		 *
		 * @return
		 */
		virtual WrapMode getWrapMode() override;

		/**
		 *
		 * @param anisotropic
		 */
		virtual void setAnisotropic(float anisotropic) override;

		/**
		 *
		 * @return
		 */
		virtual float getAnisotropic() const override;

		virtual CompareFunc getCompare() const override;

		virtual void setCompareFunc(CompareFunc compareFunc) override;

		void setMipMapBaseLevel(unsigned int level) override;

		unsigned int getMipMapBaseLevel() const override;

		void setMipMapBias(float bias) override;

		float getMipMapBias(float bias) const override;

		void setBorderColor(float color) override;

		float getBorderColor() const override;

		unsigned int setMaxLod(unsigned int level) override;

		unsigned int getMaxLod() const override;

		unsigned int setMinLod(unsigned int level) override;

		unsigned int getMinLod() const override;

		virtual Format getFormat() const;

		//TODO set const for the width and height
		virtual unsigned int width();

		virtual unsigned int height();

		virtual unsigned int layers() const;

		//TODO add mip map streaming.

		virtual void resize(int width, int height, Texture::Format format, bool hasMipMap);

		/**
		 *
		 * @param size
		 * @return
		 */
		virtual void *mapTexture(Format format, unsigned int level);    //TODO add map target.

		/**
		 *
		 */
		virtual void unMapTexture();

		//TOOD determine of range mapping is possible with flushing and etc.

		/**
		 *
		 * @param pixels
		 * @param size
		 */
		virtual void
		setPixels(Format format, unsigned int level, const void *pixels, unsigned long size); //TODO add fvformatf

		/**
		 *
		 * @param mipmap
		 * @return
		 */
		virtual void *getPixels(TextureFormat format, unsigned int level, unsigned long *nBytes); // TOOD add fvformatf.

		//virtual bool UseSRGB();
		virtual void clear();

		//TODO add get native ptr object.
		virtual intptr_t getNativePtr() const;

		virtual void setName(const std::string &name) override;
	};
}

#endif
