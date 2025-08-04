/*
 *	FragCore - Fragment Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FRAGCORE_REFPTR_H_
#define _FRAGCORE_REFPTR_H_ 1
#include "../FragDef.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 * @tparam T
	 */
	template <class T> class FVDECLSPEC RefPtr {
	  public:
		static_assert(std::is_object_v<T>, "Must be a class object.");

		FV_ALWAYS_INLINE T *operator->() noexcept { return ref; }

		FV_ALWAYS_INLINE T *operator*() noexcept { return ref; }

		FV_ALWAYS_INLINE const T *operator->() const noexcept { return ref; }

		FV_ALWAYS_INLINE const T *ptr() const noexcept { return ref; }

		FV_ALWAYS_INLINE T *ptr() noexcept { return ref; }

		FV_ALWAYS_INLINE const T *operator*() const noexcept { return ref; }

		FV_ALWAYS_INLINE RefPtr() { this->ref = nullptr; }

		FV_ALWAYS_INLINE RefPtr(T *ref) { this->ref = ref; }

		FV_ALWAYS_INLINE ~RefPtr() = default;

	  private:
		T *ref;
		// AtomicRefCount count;
	};
} // namespace fragcore

#endif
