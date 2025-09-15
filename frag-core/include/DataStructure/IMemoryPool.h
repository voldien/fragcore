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
#ifndef _FRAGCORE_IMEMORY_POOL_H_
#define _FRAGCORE_IMEMORY_POOL_H_ 1
#include "FragDef.h"
#include <cstddef>

namespace fragcore {
	/**
	 * @brief
	 */
	class FVDECLSPEC IMemoryPool { //TODO: refractor name
	  public:
		size_t alloc(const size_t sizeBytes) noexcept;
		size_t allocateAligned(const size_t sizeBytes, const unsigned int alignment) noexcept;

		void clear() noexcept;
		void freeToMarker(const size_t marker);
	};
} // namespace fragcore

#endif