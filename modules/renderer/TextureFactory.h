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
#ifndef _FRAG_CORE_TEXTURE_FACTORY_H_
#define _FRAG_CORE_TEXTURE_FACTORY_H_ 1
#include <FragCore.h>
#include <RenderPrerequisites.h>

namespace fragcore {

	/**
	 *	Texture factory class responsible
	 *	for the creation of predefined
	 *	texture objects.
	 */
	class FVDECLSPEC TextureFactory {
	  public:
	  	/**
	  	 * @brief Create a Checker object
	  	 *
	  	 * @param renderer
	  	 * @param width
	  	 * @param height
	  	 * @return Texture*
	  	 */
		static Texture *createChecker(IRenderer *renderer, int width, int height);
		/**
		 * @brief Create a Perlin Noise object
		 *
		 * @param renderer
		 * @param width
		 * @param height
		 * @return Texture*
		 */
		static Texture *createPerlinNoise(IRenderer *renderer, int width, int height);

		/**
		 * @brief Create a Noise Texture object
		 *
		 * @param renderer
		 * @param width
		 * @param height
		 * @return Texture*
		 */
		static Texture *createNoiseTexture(IRenderer *renderer, int width, int height);

	  public:
		static void createChecker(int width, int height, char **pixels);

	  private: /*	Prevent one from creating an instance of this class.	*/
		TextureFactory();
		TextureFactory(const TextureFactory &other);
		~TextureFactory();
	};
} // namespace fragcore

#endif
