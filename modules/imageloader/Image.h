/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
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
		Image(unsigned int width, unsigned int height, TextureFormat format);
		Image(unsigned int width, unsigned int height, unsigned int layer, TextureFormat format);

		Image(const Image &other) {}
		Image(Image &&other) {}

		virtual ~Image();

		// TODO set const for the width and height
		virtual unsigned int width() const noexcept { return this->w; }

		virtual unsigned int height() const noexcept { return this->h; }

		virtual unsigned int layers() const noexcept { return this->depth; }

		virtual TextureFormat getFormat() const noexcept { return this->format; }

		virtual Color operator[](unsigned int index) { return Color::black(); }

		virtual Color getColor(unsigned int x, unsigned int y, unsigned int z) { return Color::black(); }

		virtual size_t getSize() const noexcept { return this->bufferSize; }

		virtual void *getPixelData() const noexcept;
		virtual void setPixelData(void *srcPixelData, size_t size);

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

	  private:
		unsigned int w;
		unsigned int h;
		unsigned int depth{1};
		TextureFormat format;
		// TODO encpsulate object.
		void *pixelData = nullptr;
		size_t bufferSize;
		// Color color;
	};
} // namespace fragcore

#endif
