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
#ifndef _FRAG_CORE_REFPTR_H_
#define _FRAG_CORE_REFPTR_H_ 1

#include "../FragDef.h"
#include "SmartReference.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 * @tparam T
	 */
	template <class T> class RefPtr {
	  public:
		static_assert(std::is_object<T>::value, "Must be a class object.");

		FV_ALWAYS_INLINE T *operator->() noexcept { return ref; }

		FV_ALWAYS_INLINE T *operator*() noexcept { return ref; }

		FV_ALWAYS_INLINE const T *operator->() const noexcept { return ref; }

		FV_ALWAYS_INLINE const T *ptr() const noexcept { return ref; }

		FV_ALWAYS_INLINE T *ptr() noexcept { return ref; }

		FV_ALWAYS_INLINE const T *operator*() const noexcept { return ref; }

	  public:
		FV_ALWAYS_INLINE RefPtr() { this->ref = nullptr; }

		FV_ALWAYS_INLINE RefPtr(T *ref) { this->ref = ref; }

		FV_ALWAYS_INLINE ~RefPtr() {}

	  private:
		T *ref;
		// AtomicRefCount count;
	};
} // namespace fragcore

#endif
