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
#ifndef _FRAG_CORE_SMART_REFERENCE_H_
#define _FRAG_CORE_SMART_REFERENCE_H_ 1
#include"../Def.h"
#include"Object.h"
#include"AtomicRefCount.h"
#include"NoCopyable.h"

namespace fragcore {
	/**
	 *	Responsible for preventing
	 *	data be freed when references
	 *	to the block of data still exists.
	 */
	class FVDECLSPEC SmartReference : public Object, public NoCopyable {
	  public:

		SmartReference(void);
		//SmartReference(SmartReference &&other);

		bool increment(void);

		bool deincreemnt(void);

		int getIndexCount(void) const;

		//TODO determine if shall add a force delete support.

	private:	/*	Attributes.	*/

		AtomicRefCount counter;		/*	Number of references to the pointer.	*/
	};
}

#endif
