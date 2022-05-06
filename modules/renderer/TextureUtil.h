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
#ifndef _FRAG_CORE_TEXTURE_UTIL_H_
#define _FRAG_CORE_TEXTURE_UTIL_H_ 1
#include "IRenderer.h"
#include "Texture.h"
#include <Core/Ref.h>
#include <ImageFormat.h>
#include <ImageLoader.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC TextureUtil : public ImageLoader {
	  public:
		static void loadTexture(Ref<IO> &io, IRenderer *renderer, Texture **texture);
		static void loadTexture(const std::string &path, IRenderer *renderer, Texture **texture);

		/*	*/
		static void saveTexture(const std::string &filepath, IRenderer *renderer, Texture *texture);
		static void saveTexture(Ref<IO> &io, IRenderer *renderer, Texture *texture);

		/*	*/
		static Texture *createTexture(IRenderer *renderer, unsigned int width, unsigned int height, const Ref<IO> &io,
									  TextureFormat format, GraphicFormat graphicformat);
		static Texture *createTexture(IRenderer *renderer, unsigned int width, unsigned int height, const void *pixels,
									  unsigned int size, TextureFormat format, GraphicFormat graphicformat);

	  private: /*	Prevent one from creating an instance of this class.	*/
		TextureUtil() = default;
		TextureUtil(IFileSystem *fileSystem);
		TextureUtil(const TextureUtil &other);

	  private:
		IFileSystem *fileSystem;
	};
} // namespace fragcore

#endif
