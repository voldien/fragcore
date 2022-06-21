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
#ifndef _FRAG_CORE_DOUBLE_BUFFERED_ALLACATOR_H_
#define _FRAG_CORE_DOUBLE_BUFFERED_ALLACATOR_H_ 1
#include "../SmartReference.h"
#include "StackAllactor.h"

namespace fragcore {

	/**
	 *	DoubleBufferedAllocator is two allocate buffer.
	 *	//TODO rename to stack buffered, to allow select number of stack.
	 */
	class FVDECLSPEC StackBufferedAllocator {
	  public:
		//StackBufferedAllocator(size_t nrStack = 0);
		StackBufferedAllocator(const StackBufferedAllocator &doublebuffer);
		StackBufferedAllocator(StackBufferedAllocator &&other);
		explicit StackBufferedAllocator(unsigned int sizeBytes);
		~StackBufferedAllocator();

		/**
		 *	Allocate n number of bytes.
		 */
		void alloc(unsigned int sizeBytes);

		/**
		 *	Allocate n number of bytes aligned.
		 *
		 *	@return first memory address.
		 */
		void allocateAligned(unsigned int sizeBytes, int alignment);

		/**
		 *	Set marker to zero.
		 */
		void clear();

		/**
		 *	Return marker offset.
		 *	@return
		 */
		unsigned int getMarker() const;

		/**
		 *	Fetch block of memory.
		 */
		void *fetch(unsigned int sizeBytes);

		/**
		 *	Release everything to marker.
		 */
		void freeToMarker(unsigned int marker);

		/**
		 *	Swap buffers.
		 */
		void swap();

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

		/**
		 *	@return
		 */
		StackBufferedAllocator &operator=(const StackBufferedAllocator &alloc);
		StackBufferedAllocator &operator=(StackBufferedAllocator &&alloc);

	  private: /*	Attributes.	*/
		unsigned int m_curStack;
		StackAllocator m_stack[2];
	};

} // namespace fragcore

#endif
