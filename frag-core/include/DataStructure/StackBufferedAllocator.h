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
#ifndef _FRAGCORE_DOUBLE_BUFFERED_ALLOCATOR_H_
#define _FRAGCORE_DOUBLE_BUFFERED_ALLOCATOR_H_ 1
#include "Core/UIDObject.h"
#include "StackAllactor.h"

namespace fragcore {

	/**
	 * @brief Stack of buffer allocator.
	 *
	 */
	class FVDECLSPEC StackBufferedAllocator : public UIDObject, public IMemoryPool {
	  public:
		// StackBufferedAllocator(size_t nrStack = 0);
		StackBufferedAllocator(const StackBufferedAllocator &doublebuffer);
		StackBufferedAllocator(StackBufferedAllocator &&other);
		explicit StackBufferedAllocator(size_t sizeBytes);
		virtual ~StackBufferedAllocator();

		/**
		 *	Allocate n number of bytes.
		 */
		void alloc(size_t sizeBytes);

		/**
		 *	Allocate n number of bytes aligned.
		 *
		 *	@return first memory address.
		 */
		void allocateAligned(size_t sizeBytes, int alignment);

		/**
		 *	Set marker to zero.
		 */
		void clear() noexcept;

		/**
		 *	Return marker offset.
		 *	@return
		 */
		size_t getMarker() const noexcept;

		/**
		 *	Fetch block of memory.
		 */
		void *fetch(size_t sizeBytes);

		/**
		 *	Release everything to marker.
		 */
		void freeToMarker(unsigned int marker);

		/**
		 *	Swap buffers.
		 */
		void next();

		/**
		 *	Get current stack buffer.
		 */
		StackAllocator *getCurrentStack();

		/**
		 *	Get current by index.
		 */
		const StackAllocator *getStack(int index) const;

		/**
		 * @brief Get the Stack object
		 *
		 * @param index
		 * @return StackAllocator*
		 */
		StackAllocator *getStack(int index);

		StackBufferedAllocator &operator=(const StackBufferedAllocator &alloc);
		StackBufferedAllocator &operator=(StackBufferedAllocator &&alloc);

	  private: /*	Attributes.	*/
		unsigned int m_curStack;
		StackAllocator m_stack[2];
	};

} // namespace fragcore

#endif
