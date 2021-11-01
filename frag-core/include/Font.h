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
#ifndef _FRAG_CORE_FONT_H_
#define _FRAG_CORE_FONT_H_ 1
#include "Core/SmartReference.h"
#include <map>

namespace fragcore {
	/**
	 *
	 */
	// TODO check if some polymorpihsm can be used.
	class FVDECLSPEC Font : public SmartReference {
		friend class FontFactory;

		struct Character {
			int width;
			int height;
			int Bearing[2]; // Offset from baseline to left/top of glyph
			float Advance;	// Offset to advance to next glyph
						   //		float* advance;             /*  */
			/*  UV rectangle.   */
			float tex_x1; /*  */
			float tex_x2; /*  */
			float tex_y1; /*  */
			float tex_y2; /*  */
			/*  UV offset.  */
			float offset_x; /*  */
			float offset_y; /*  */
			/*  Size of the font.   */
			float size; /*  */
		};

	  public:
		Font();

		~Font();

		unsigned int getNumberCharacter() const;

		// TODO rename index to unicode or something.
		Character getCharacter(unsigned int index);

		const Character &getCharacter(unsigned int index) const;

		unsigned int getEncoding() const;

		//Texture *getTexture() const;

		// TODO add support for config file for lookup.
	  private:
		//Texture *texture;
		std::map<unsigned int, Character> characters;
	};
} // namespace fragcore

#endif
