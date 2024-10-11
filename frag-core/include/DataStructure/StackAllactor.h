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
#ifndef _FRAG_CORE_STACK_ALLACTOR_H_
#define _FRAG_CORE_STACK_ALLACTOR_H_ 1
#include "../FragDef.h"
#include <cstdint>

namespace fragcore {

	/**
	 * @brief Single stack buffer.
	 *
	 */
	class FVDECLSPEC StackAllocator {
	  public:
		StackAllocator();
		StackAllocator(const StackAllocator &stack);
		StackAllocator(StackAllocator &&other);
		explicit StackAllocator(const size_t stackSizeBytes);
		~StackAllocator();

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
		void *allocateAligned(size_t sizeBytes, unsigned int alignment);

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
		void freeToMarker(const size_t marker);

		/**
		 *	@return
		 */
		StackAllocator &operator=(const StackAllocator &alloc);

		/**
		 *
		 *	@return
		 */
		bool operator==(const StackAllocator &alloc);

		/**
		 *
		 *	@return
		 */
		bool operator!=(const StackAllocator &alloc);

	  private:			/*	Attributes.	*/
		void *mData;	/*	*/
		size_t mSize;	/*	*/
		size_t mMarker; /*	*/
	};
} // namespace fragcore

#endif
