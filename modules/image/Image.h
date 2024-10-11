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
#ifndef _FRAG_CORE_IMAGE_H_
#define _FRAG_CORE_IMAGE_H_ 1
#include "ImageFormat.h"
#include <Core/Object.h>
#include <Core/math3D/Color.h>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Image : public Object {
	  public:
		Image(const unsigned int width, const unsigned int height, const TextureFormat format);
		Image(const unsigned int width, const unsigned int height, const unsigned int layer,
			  const TextureFormat format);

		Image(const Image &other) { Object::operator=(other); }
		Image(Image &&other) { Object::operator=(other); }

		~Image() override;

		virtual unsigned int width() const noexcept { return this->w; }

		virtual unsigned int height() const noexcept { return this->h; }

		virtual unsigned int layers() const noexcept { return this->depth; }

		virtual TextureFormat getFormat() const noexcept { return this->format; }

		virtual Color operator[](unsigned int index) const { return Color::black(); }

		virtual Color getColor(unsigned int x, unsigned int y, unsigned int z) const { return Color::black(); }

		virtual size_t getSize() const noexcept { return this->bufferSize; }

		virtual void *getPixelData() const noexcept;
		virtual void setPixelData(void *srcPixelData, const size_t size); // TODO add offset

	  protected:
		void allocateMemory(unsigned int width, unsigned int height, unsigned depth, TextureFormat format);

	  public:
		static size_t getTextureSize(unsigned int width, unsigned int height, unsigned depth, TextureFormat format);
		/**
		 * @brief Get the Format Pixel Size in bits
		 *
		 * @param format
		 * @return size_t
		 */
		static size_t getFormatPixelSize(TextureFormat format);

		Image &convertImage(Image &image, TextureFormat textureFormat);

	  private:
		unsigned int w;
		unsigned int h;
		unsigned int depth{1};
		TextureFormat format;

		// TODO encpsulate object.
		void *pixelData = nullptr;
		size_t bufferSize;
	};
} // namespace fragcore

#endif
