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
#ifndef _FRAGCORE_STACK_ALLOCATOR_H_
#define _FRAGCORE_STACK_ALLOCATOR_H_ 1
#include "../FragDef.h"
#include "Core/UIDObject.h"

namespace fragcore {

	/**
	 * @brief Single stack buffer.
	 *
	 */
	class FVDECLSPEC StackAllocator : public UIDObject {
	  public:
		StackAllocator();
		StackAllocator(const StackAllocator &stack);
		StackAllocator(StackAllocator &&other);
		explicit StackAllocator(const size_t stackSizeBytes);
		StackAllocator(void *buffer, size_t sizeBytes);
		virtual ~StackAllocator();

		/**
		 *	Allocate n number of bytes.
		 *	@return first memory address.
		 */
		void *alloc(const size_t sizeBytes);

		/**
		 *	Get size of the allocated data block
		 *	in bytes.
		 */
		size_t getSize() const noexcept;

		/**
		 *	Allocate aligned memory block.
		 *	@return first memory address.
		 */
		void *allocateAligned(const size_t sizeBytes, const unsigned int alignment);

		/**
		 *	Set marker to zero.
		 */
		void clear() noexcept;

		/**
		 *	@return marker offset.
		 */
		size_t getMarker() const noexcept;

		/**
		 *	@return non null pointer if successfully.
		 */
		void *fetch(const size_t sizeBytes);

		/**
		 *	Release everything to marker.
		 */
		void freeToMarker(const size_t marker) noexcept;

		StackAllocator &operator=(const StackAllocator &other);
		StackAllocator &operator=(StackAllocator &&other);

		bool operator==(const StackAllocator &other);
		bool operator!=(const StackAllocator &other);

	  private:			/*	Attributes.	*/
		void *mData;	/*	*/
		size_t mSize;	/*	*/
		size_t mMarker; /*	*/
		bool ownData = true;
	};
} // namespace fragcore

#endif
