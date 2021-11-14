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
		explicit StackAllocator(size_t stackSizeBytes);
		~StackAllocator();

		/**
		 *	Allocate n number of bytes.
		 *	@Return first memory address.
		 */
		void *alloc(size_t sizeBytes);

		/**
		 *	Get size of the allocated data block
		 *	in bytes.
		 */
		size_t getSize() const;

		/**
		 *	Allocate aligned memory block.
		 *	@Return first memory address.
		 */
		void *allocateAligned(size_t sizeBytes, int alignment);

		/**
		 *	Set marker to zero.
		 */
		void clear();

		/**
		 *	@Return marker offset.
		 */
		size_t getMarker() const;

		/**
		 *	@Return non null pointer if successfully.
		 */
		void *fetch(size_t sizeBytes);

		/**
		 *	Release everything to marker.
		 */
		void freeToMarker(size_t marker);

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

	  private:			/*	Attributes.	*/
		void *mData;	/*	*/
		size_t mSize;	/*	*/
		size_t mMarker; /*	*/
	};
} // namespace fragcore

#endif
