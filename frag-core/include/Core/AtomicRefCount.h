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
#ifndef _FRAG_CORE_ATOMIC_REF_COUNT_H_
#define _FRAG_CORE_ATOMIC_REF_COUNT_H_ 1
#include"../Def.h"
#include <atomic>

namespace fragcore {
	/**
	 * Atomic counter.
	 */
	class FVDECLSPEC AtomicRefCount {
	public:

		FV_ALWAYS_INLINE bool ref(void) {
			return std::atomic_fetch_add(&count, 1) != 0;
		}

		FV_ALWAYS_INLINE bool unRef(void) {
			return std::atomic_fetch_add(&count, -1) == 0;
		}

		FV_ALWAYS_INLINE uint32_t get(void) const {
			return std::atomic_load(&this->count);
		}

		FV_ALWAYS_INLINE void init(uint32_t value) {
			this->count = value;
		}

	private:    /*  */
		std::atomic_int32_t count;
	};
}
#endif