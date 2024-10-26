/*
 *	FragCore - Fragment Core - Engine Core
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
#ifndef _FRAG_CORE_ATOMIC_REF_COUNT_H_
#define _FRAG_CORE_ATOMIC_REF_COUNT_H_ 1
#include "../FragDef.h"
#include <atomic>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC AtomicRefCount {
	  public:
		FV_ALWAYS_INLINE bool ref() noexcept { return std::atomic_fetch_add(&count, 1) != 0; }

		FV_ALWAYS_INLINE bool unRef() noexcept { return std::atomic_fetch_add(&count, -1) == 0; }

		FV_ALWAYS_INLINE uint32_t get() const noexcept { return std::atomic_load(&this->count); }

		FV_ALWAYS_INLINE void init(int32_t value) noexcept { this->count = value; }

	  private: /*  */
		std::atomic_int32_t count;
	};
} // namespace fragcore
#endif
