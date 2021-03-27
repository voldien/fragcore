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
#ifndef _FRAG_CORE_STACK_ALLACTOR_H_
#define _FRAG_CORE_STACK_ALLACTOR_H_ 1
#include "../../Def.h"
#include "../SmartReference.h"

namespace fragcore {
	/**
	 *	Single stack buffer.
	 */
	class FVDECLSPEC StackAllocator {
		public:

		StackAllocator(void);
		StackAllocator(const StackAllocator &stack);
		StackAllocator(StackAllocator &&other);
		explicit StackAllocator(unsigned int stackSizeBytes);
		~StackAllocator(void);

		/**
		 *	Allocate n number of bytes.
			*	@Return first memory address.
			*/
		void* alloc(unsigned int sizeBytes);

		/**
		 *	Get size of the allocated data block
			*	in bytes.
			*/
		unsigned int getSize(void)const;

		/**
		 *	Allocate aligned memory block.
			*	@Return first memory address.
			*/
		void* allocateAligned(unsigned int sizeBytes, int alignment);

		/**
		 *	Set marker to zero.
			*/
		void clear(void);


		/**
		 *	@Return marker offset.
			*/
		unsigned int getMarker(void)const;

		/**
		 *	@Return non null pointer if successfully.
			*/
		void* fetch(unsigned int sizeBytes);

		/**
		 *	Release everything to marker.
			*/
		void  freeToMarker(unsigned int marker);

		/**
		 *	@Return
			*/
		StackAllocator &operator=(const StackAllocator &alloc);

		/**
		 *
		 *	@Return
			*/
		bool operator==(const StackAllocator &alloc);

		/**
		 *
		 *	@Return
			*/
		bool operator!=(const StackAllocator &alloc);

	private: /*	Attributes.	*/

		void* mData;            /*	*/
		unsigned int mSize;        /*	*/
		unsigned int mMarker;    /*	*/
	};
}

#endif
