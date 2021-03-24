/**
	FragEngine, A Two layer Game Engine.
    Copyright (C) 2016  Valdemar Lindberg

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
#ifndef _FRAG_CORE_AUDIOOBJECT_H_
#define _FRAG_CORE_AUDIOOBJECT_H_ 1
#include"Prerequisites.h"
#include"../Core/SmartReference.h"
#include"../Core/Ref.h"

namespace fragcore {
	class AudioInterface; //TODO relocate.
	//TODO add based on a dynamic interface object class. Similar to the rendering and possibliy physic.
	/**
	 *
	 */
	class AudioObject : public SmartReference {
	public:
		FV_ALWAYS_INLINE AudioInterface *getAudio(void) const {
			return this->audioInterface;
		}

		FV_ALWAYS_INLINE void *getObject(void) {
			return this->pdata;
		}

		FV_ALWAYS_INLINE void *getObject(void) const {
			return this->pdata;
		}

		virtual intptr_t getNativePtr(void) const = 0;

	protected:  /*  */
		void *pdata;
		AudioInterface *audioInterface;   //TOOD change to ref.
	};
}


#endif