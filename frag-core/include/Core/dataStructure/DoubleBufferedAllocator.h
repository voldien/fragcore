/**
	FragEngine, A Two layer Game Engine.
    Copyright (C) 2014  Valdemar Lindberg

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
#ifndef _FRAG_CORE_DOUBLE_BUFFERED_ALLACATOR_H_
#define _FRAG_CORE_DOUBLE_BUFFERED_ALLACATOR_H_ 1
#include"StackAllactor.h"
#include"../SmartReference.h"

namespace fragcore{
	/**
	 *	DoubleBufferedAllocator is two allocate buffer.
	*/
	class FVDECLSPEC DoubleBufferedAllocator {
	public:
		DoubleBufferedAllocator(void);
		DoubleBufferedAllocator(const DoubleBufferedAllocator &doublebuffer);
		DoubleBufferedAllocator(DoubleBufferedAllocator &&other);
		explicit DoubleBufferedAllocator(unsigned int sizeBytes);
		~DoubleBufferedAllocator(void);

		/**
		 *	Allocate n number of bytes.
		*/
		void alloc(unsigned int sizeBytes);

		/**
		 *	Allocate n number of bytes aligned.
		*
		*	@Return first memory address.
		*/
		void allocateAligned(unsigned int sizeBytes, int alignment);

		/**
		 *	Set marker to zero.
		*/
		void clear(void);

		/**
		 *	Return marker offset.
		*	@Return
		*/
		unsigned int getMarker(void)const;

		/**
		 *	Fetch block of memory.
		*/
		void* fetch(unsigned int sizeBytes);

		/**
		 *	Release everything to marker.
		*/
		void  freeToMarker(unsigned int marker);

		/**
		 *	Swap buffers.
		*/
		void  swap(void);

		/**
		 *	Get current stack buffer.
		*/
		StackAllocator* getCurrentStack(void);

		/**
		 *	Get current by index.
		*/
		StackAllocator* getStack(int index)const;

		/**
		 *	@Return
		*/
		DoubleBufferedAllocator &operator=(const DoubleBufferedAllocator &alloc);

	private:	/*	Attributes.	*/

		/*
		*
		*/
		unsigned int m_curStack;

		/*
		*
		*/
		StackAllocator m_stack[2];
	};

}

#endif
