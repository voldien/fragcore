
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
#ifndef _FRAG_CORE_FONTFACTORY_H_
#define _FRAG_CORE_FONTFACTORY_H_ 1
#include "Core/IO/IO.h"
#include "Core/Ref.h"
#include "Def.h"
#include "Font.h"
#include "Prerequisites.h"

namespace fragcore {
	/**
	 *
	 */
	// TODO relocate to the util directory
	class FVDECLSPEC FontFactory {
	  public:
		enum Encoding {
			ASCII, /*	*/
			UTF8,  /*	*/
			UTF16, /*	*/
		};

		// TODO add support for generating the font and its texture without the dependent of a rendering API.
		// static Font *createFont(Ref<IRenderer> &renderer, Ref<IO> &io, float size, Encoding encoding = ASCII);
		// static Font *createSDFFont(Ref<IRenderer> &renderer, Ref<IO> &io, float size, Encoding encoding = ASCII);
		// static Font *createFont(Image *image, Ref<IO> &io, float size, Encoding encoding = UTF8);
		// static void CreateSDF();

		// TODO add a method for saving the font for allowing it to be a tool for generating on the build rather than in
		// executable time.
		// static void saveFont(Ref<IO> &io, Ref<Font> &font);

	  private: /*	Prevent one from creating an instance of this class.	*/
		FontFactory() = default;
		FontFactory(const FontFactory &other) = default;
	};
} // namespace fragcore

#endif
