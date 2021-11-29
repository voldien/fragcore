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
#ifndef _FRAG_CORE_POOL_ALLACTOR_H_
#define _FRAG_CORE_POOL_ALLACTOR_H_ 1
#include "../../Def.h"
#include <cassert>
#include <cstdio>
#include <malloc.h>
#include <memory>
#include <string>

/* add support for C++ allocates in order to remove pure function bug, if it's that it's the culprit*/
// TODO resolve bug.
/*TODO extract the pool information into a subclass such that the memory consumption can be monitored.*/
// TODO add
namespace fragcore {

	/**
	 * @brief Pool allocate
	 *
	 * @tparam T
	 */
	template <class T> class PoolAllocator {
	  public:
		/*	TODO add align and pack .	*/
		typedef struct poolallactoritem {
			T data;
			poolallactoritem *next;
		} PoolAllactorItem;

	  public:
		PoolAllocator() {
			this->item = nullptr;
			this->nrOfElements = 0;
			this->mReserved = 0;
			this->setTypeSize(sizeof(T));
		}

		PoolAllocator(const PoolAllocator &pollallactor) {
			this->item = nullptr;
			this->nrOfElements = 0;
			this->mReserved = 0;
			this->setTypeSize(sizeof(T));
			*this = pollallactor;
		}
		PoolAllocator(PoolAllocator &&other) {}

		PoolAllocator(unsigned int num) : PoolAllocator() { this->resize(num); }

		~PoolAllocator() { delete[] this->item; }
		/**
		 *
		 * @param allocator
		 * @return
		 */
		PoolAllocator &operator=(const PoolAllocator &allocator) {
			this->mReserved = 0;
			this->nrOfElements = 0;
			this->item = nullptr;
			this->setTypeSize(allocator.getTypeSize());

			/*  */
			resize(allocator.reserved());
			this->nrOfElements = allocator.nrOfElements;

			/*  Copy the list.  */
			memcpy(this->item, allocator.item, allocator.getTypeSize() * allocator.reserved());

			/*	Reconstruct next list.	*/
			PoolAllactorItem *item = this->item;
			for (int x = 0; x < this->reserved() - 1; x++) {
				item[x].next = &item[x + 1];
			}
			item[this->reserved() - 1].next = nullptr;

			return *this;
		}

		PoolAllocator &operator=(PoolAllocator &&alloctor) { return *this; }

		/**
		 *	Get a pointer to next object in pool allocator.
		 *
		 *	@return None nullptr pointer if successful.
		 */
		T *obtain() {
			T *alloc;

			/*	if last, resize.    */
			if (this->isFull()) {
				this->resize(nrOfElements + 256);
				return this->obtain();
			}

			/*  Special case.   */
			if (this->nrOfElements == 0)
				alloc = &this->item->data;
			else
				alloc = &this->item->next->data; /*changed.*/

			this->item->next = this->item->next->next; /*	TODO still some bugs in pool allactor.	*/
			this->nrOfElements++;
			return alloc;
		}

		/**
		 *
		 * @param element
		 */
		void Return(T *element) {
			if (!isValidItem(*element))
				throw InvalidArgumentException("invalid pointer returned");

			PoolAllactorItem *alloc = (PoolAllactorItem *)element;
			alloc->next = item->next;
			item->next = alloc;
			this->nrOfElements--;
		}

		/**
		 * Check if the data is in the same address space.
		 * @param data
		 * @return  true if valid item.
		 */
		bool isValidItem(const T &data) const {
			const T *tmp = &data;
			/*  If pointer is less than the base pointer.*/
			if ((void *)tmp < (void *)this->item)
				return false;
			/*  Check if pointer is outside the end point*/
			if (tmp >= (void *)&this->item[this->reserved()])
				return false;
			/*	Check if memory is a multiple of the size from the base pointer.	*/
			return true;
		}

		/**
		 *
		 */
		void clean() {
			free(this->item);
			this->mReserved = 0;
			this->nrOfElements = 0;
		}

		bool isFull() const { return this->nrOfElements >= (this->reserved() - 1); }

		inline int size() const { return this->nrOfElements; }

		inline int reserved() const { return this->mReserved; }

		/**
		 * Get datatype size.
		 * @param size
		 */
		inline void setTypeSize(unsigned int size) { this->typeSize = size; }

		/**
		 *
		 * @return
		 */
		inline unsigned int getTypeSize() const { return this->typeSize; }

		/**
		 *
		 */
		/*	TODO Fix, still some bugs!	*/
		void resize(int size) {
			int i;
			const unsigned int itemSize = getItemSize();

			assert(size >= 0);

			/*  No allocation.  */
			if (this->reserved() == 0) {
				this->item = static_cast<PoolAllactorItem *>(realloc(this->item, itemSize * size));
				void **items = (void **)item;
				for (i = 0; i < size - 1; i++) {
					item[i].next = &item[i + 1];
				}
				item[size - 1].next = nullptr;
			} else {
				/*  */
				if (size < this->reserved())
					throw InvalidArgumentException("Can no be downsized from {} to {}.", this->reserved(), size);

				/*  Reallocate buffer.  */
				i = this->reserved();
				this->item = static_cast<PoolAllactorItem *>(realloc(this->item, itemSize * size));

				PoolAllactorItem *lastItem = &this->item[i];
				PoolAllactorItem *subpool = lastItem;
				for (; i < size - 1; i++) {
					lastItem->next = &item[i + 1];
					lastItem = lastItem->next;
				}
				lastItem->next = nullptr;

				/*  Bind current next to the next subset.  */
				this->item->next = subpool;
			}

			this->mReserved = size;
		}

		/**
		 *
		 * @return
		 */
		inline T *getLast() { return &this->item[this->mReserved - 1].data; }

		/**
		 *
		 *	@return
		 */
		inline T &operator[](int index) { return this->item[index].data; }

		/**
		 *
		 * @param index
		 * @return
		 */
		inline T operator[](int index) const { return item[index].data; }

	  protected:
		/**
		 *
		 * @return
		 */
		unsigned int getItemSize() const { return sizeof(PoolAllactorItem); }

	  private:					/*	attributes.	*/
		PoolAllactorItem *item; /*	Pool data.	*/
		int nrOfElements;		/*	number of elements used.	*/
		int mReserved;			/*	number of allocated elements.	*/
		unsigned int typeSize;	/*	size of the data type.	*/
	};
} // namespace fragcore

#endif
