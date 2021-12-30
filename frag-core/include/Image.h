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
#include "Core/Color.h"
#include "Renderer/Texture.h"

namespace fragcore {

	// System memory bound texture data.
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Image {
	  public:
		// TODO determine of Image or surface or both classes will be used.
		Image(int width, int height, TextureFormat format);

		// TODO set const for the width and height
		virtual unsigned int width() noexcept  { return this->w; }

		virtual unsigned int height() noexcept  { return this->h; }

		virtual unsigned int layers() noexcept  { return this->depth; }

		// virtual unsigned int layers() const;
		Color operator[](unsigned int index) { return Color::black(); }
		void *getPixels() const noexcept;

	  private:
		unsigned int w;
		unsigned int h;
		unsigned int depth{1};
		TextureFormat format;
		// Color color;
	};
} // namespace fragcore

#endif
