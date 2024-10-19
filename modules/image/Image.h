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
#include <Math3D/Color.h>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Image : public Object {
	  public:
		Image(const unsigned int width, const unsigned int height, const ImageFormat format);
		Image(const unsigned int width, const unsigned int height, const unsigned int layer, const ImageFormat format);

		Image(const Image &other);
		Image(Image &&other);

		Image &operator=(const Image &other);
		Image &operator=(Image &&other);

		~Image() override;

		virtual unsigned int width() const noexcept { return this->m_width; }
		virtual unsigned int height() const noexcept { return this->m_height; }
		virtual unsigned int layers() const noexcept { return this->depth; }
		virtual ImageFormat getFormat() const noexcept { return this->format; }

		virtual Color operator[](unsigned int index) const;

		virtual Color getColor(unsigned int x_offset, unsigned int y_offset, unsigned int z_offset) const;
		virtual void setColor(unsigned int x_offset, unsigned int y_offset, unsigned int z_offset, const Color &color);

		virtual size_t getSize() const noexcept { return this->bufferSize; }

		virtual void *getPixelData() const noexcept;
		virtual void setPixelData(void *srcPixelData, const size_t size); // TODO add offset

	  protected:
		void allocateMemory(unsigned int width, unsigned int height, unsigned depth, ImageFormat format);
		unsigned int getPixelMemoryOffset(const unsigned int x_offset, const unsigned int y_offset,
										  const unsigned int z_offset) const noexcept;

	  public: /*	*/
		static size_t getTextureByteSize(const unsigned int width, const unsigned int height, const unsigned int depth,
										 const ImageFormat format);

		static unsigned int getFormatPixelBitSize(const ImageFormat format);

		static Image &convertImage(Image &image, ImageFormat textureFormat);

	  private:
		unsigned int m_width;
		unsigned int m_height;
		unsigned int depth{1};
		ImageFormat format;

		// TODO encpsulate object.
		void *pixelData = nullptr;
		size_t bufferSize = 0;
	};
} // namespace fragcore

#endif
