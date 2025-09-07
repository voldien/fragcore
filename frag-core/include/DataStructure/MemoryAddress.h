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
#ifndef _FRAGCORE_MEMORY_ADDRESS_H_
#define _FRAGCORE_MEMORY_ADDRESS_H_ 1
#include "../FragDef.h"
#include "../Math/Math.h"

namespace fragcore {

	/*	*/
	class FVDECLSPEC MemoryAddress {
	  public:
		MemoryAddress() = default;
		MemoryAddress(const size_t size, const size_t offset = 0) {
			this->mSize = size;
			this->baseOffset = offset;
			this->clear();
		}

		size_t alloc(const size_t sizeBytes) {
			size_t current_marker = this->mMarker;
			this->mMarker += sizeBytes;
			return current_marker;
		}

		size_t allocateAligned(const size_t sizeBytes, const unsigned int alignment) {

			const size_t requested_byte_size = fragcore::Math::align<size_t>(sizeBytes, alignment);
			size_t current_marker = this->mMarker;
			this->mMarker += requested_byte_size;

			return current_marker;
		}

		void clear() noexcept { this->mMarker = 0; }
		void freeToMarker(const size_t marker) { this->mMarker = marker; }

	  private:
		size_t mSize = 0;	   /*	*/
		size_t mMarker = 0;	   /*	*/
		size_t baseOffset = 0; /*   */
	};
} // namespace fragcore

#endif
