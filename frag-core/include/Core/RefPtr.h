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
#ifndef _FRAG_CORE_REFPTR_H_
#define _FRAG_CORE_REFPTR_H_ 1

#include"../Def.h"
#include "SmartReference.h"

namespace fragcore {

	/**
	 * 
	 */
	template<class T>
	class RefPtr {
	public:

		FV_ALWAYS_INLINE T *operator->() {

			return ref;
		}

		FV_ALWAYS_INLINE T *operator*() {

			return ref;
		}

		FV_ALWAYS_INLINE const T *operator->() const {

			return ref;
		}

		FV_ALWAYS_INLINE const T *ptr() const {

			return ref;
		}

		FV_ALWAYS_INLINE T *ptr() {

			return ref;
		}

		FV_ALWAYS_INLINE const T *operator*() const {

			return ref;
		}
	public:

		FV_ALWAYS_INLINE RefPtr(void) {
			this->ref = NULL;
		}

		FV_ALWAYS_INLINE RefPtr(T *ref) {
			this->ref = ref;
		}

		FV_ALWAYS_INLINE ~RefPtr(void) {

		}

	private:
		T *ref;
		//AtomicRefCount count;
	};
}

#endif