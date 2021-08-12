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
#ifndef _FRAG_CORE_UID_GENERATOR_H_
#define _FRAG_CORE_UID_GENERATOR_H_ 1
#include "../Def.h"

namespace fragcore {
	/**
	 *	Unique Identifier generator.
	 *	This is a simple UID generator.
	 *	Each next will be an increment of previosuly.
	 */
	class FVDECLSPEC UIDGenerator /*: public UIDObject*/ {
	  public:
		UIDGenerator(void) noexcept {
			this->nextUID = 0;
			this->nextLUID = 0;
		}

		UIDGenerator(const UIDGenerator &other) = default;

		/**
		 *	@Return next unique id.
		 */
		unsigned int getNextUID(void) noexcept { return this->nextUID++; }

		/**
		 *	@Return next long uniqie id.
		 */
		unsigned long int getNextLUID(void) noexcept { return this->nextLUID++; }

		virtual bool operator==(const UIDGenerator &o1) noexcept { return uid == o1.uid; }
		virtual bool operator!=(const UIDGenerator &o1) noexcept { return uid != o1.uid; }

	  private:						/*	Attributes.	*/
		unsigned int nextUID;		/*	*/
		unsigned long int nextLUID; /*	*/
		unsigned int uid;			// TODO resolve.
	};

} // namespace fragcore

#endif
