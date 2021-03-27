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
#ifndef _FRAG_CORE_UID_OBJECT_H_
#define _FRAG_CORE_UID_OBJECT_H_ 1
#include"UIDGenerator.h"

namespace fragcore {
	/**
	 * Responsible for containing
	 * the UID of object.
	 */
	class FVDECLSPEC UIDObject {
	public:
		UIDObject(void) noexcept;
		UIDObject(UIDGenerator& uidGenerator) noexcept;
		//TODO for selecting what UID pool.

		/**
		 *	Set Unique ID for the object.
		 */
		virtual void setUID(unsigned int uid);

		/**
		 *	Get the unique identifier.
		 *	@Return unique ID.
		 */
		virtual unsigned int getUID(void) const;

		virtual bool operator==(const UIDObject &o1) {
			return uid == o1.uid;
		}

		virtual bool operator!=(const UIDObject &o1) {
			return uid == o1.uid;
		}

		//virtual void operator=(const UIDObject& o);

	private:    /*	Attributes.	*/
		//TODO remove. remove the responsiblity to the generator pool.
		static UIDGenerator generator;
		unsigned int uid;        /*	Unique Identifier.	*/
	};
}

#endif
