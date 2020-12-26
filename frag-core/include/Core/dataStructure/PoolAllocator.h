/**
	FragEngine, A Two layer Game Engine.
    Copyright (C) 2018  Valdemar Lindberg

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
#ifndef _FRAG_CORE_ALLACTOR_H_
#define _FRAG_CORE_ALLACTOR_H_ 1
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdexcept>
#include<memory>

/* add support for C++ allocates in order to remove pure function bug, if it's that it's the culprit*/
//TODO resolve bug.
/*TODO extract the pool information into a subclass such that the memory consumption can be monitored.*/
//TODO add
namespace fragcore {
	/**
	 *	Pool allocate
	 */
	template<class T>
	class PoolAllocator {
	public:

		typedef struct poolallactoritem {
			T data;
			poolallactoritem *next;

		} PoolAllactorItem;

	public:
		PoolAllocator(void) {
			this->item = NULL;
			this->numOfElements = 0;
			this->mReserved = 0;
			this->setTypeSize(sizeof(T));
		}

		PoolAllocator(const PoolAllocator &pollallactor) {
			this->item = NULL;
			this->numOfElements = 0;
			this->mReserved = 0;
			this->setTypeSize(sizeof(T));
			*this = pollallactor;
		}

		PoolAllocator(unsigned num) {
			this->item = NULL;
			this->numOfElements = 0;
			this->mReserved = 0;
			this->setTypeSize(sizeof(T));
			this->resize(num);
		}

		~PoolAllocator(void) {
			delete[] this->item;
		}

		/**
		 *	Get a pointer to next object in pool allocator.
		 *
		 *	@Return None NULL pointer if successful.
		 */
		T *obtain(void) {
			T *alloc;

			/*	if last, resize.    */
			if (this->isFull()) {
				this->resize(numOfElements + 256);
				return this->obtain();
			}

			/*  Special case.   */
			if (this->numOfElements == 0)
				alloc = &this->item->data;
			else
				alloc = &this->item->next->data;    /*changed.*/

			this->item->next = this->item->next->next;    /*	TODO still some bugs in pool allactor.	*/
			this->numOfElements++;
			return alloc;
		}

		/**
		 *
		 * @param element
		 */
		void Return(T *element) {
			if (!isValidItem(*element))
				throw std::invalid_argument("invalid pointer returned");

			PoolAllactorItem *alloc = (PoolAllactorItem *) element;
			alloc->next = item->next;
			item->next = alloc;
			this->numOfElements--;
		}

		/**
		 * Check if the data is in the same address space.
		 * @param data
		 * @return  true if valid item.
		 */
		bool isValidItem(const T &data) const {
			const T *tmp = &data;
			/*  If pointer is less than the base pointer.*/
			if ((void *) tmp < (void *) this->item)
				return false;
			/*  Check if pointer is outside the end point*/
			if (tmp >= (void *) &this->item[this->reserved()])
				return false;
			return true;
		}

		/**
		 *
		 */
		void clean(void) {
			free(this->item);
			this->mReserved = 0;
			this->numOfElements = 0;
		}

		bool isFull(void) const {
			return this->numOfElements >= (this->reserved() - 1);
		}

		inline int size(void) const {
			return this->numOfElements;
		}

		inline int reserved(void) const {
			return this->mReserved;
		}

		/**
		 * Get datatype size.
		 * @param size
		 */
		inline void setTypeSize(unsigned int size) {
			this->typeSize = size;
		}

		/**
		 *
		 * @return
		 */
		inline unsigned int getTypeSize(void) const {
			return this->typeSize;
		}

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
				this->item = (PoolAllactorItem *) realloc(this->item, itemSize * size);
				void **items = (void **) item;
				for (i = 0; i < size - 1; i++) {
					item[i].next = &item[i + 1];
				}
				item[size - 1].next = NULL;
			} else {
				/*  */
				if (size < this->reserved())
					throw std::invalid_argument("Can no be downsized.");

				/*  Reallocate buffer.  */
				i = this->reserved();
				this->item = (PoolAllactorItem *) realloc(this->item, itemSize * size);

				PoolAllactorItem *lastItem = &this->item[i];
				PoolAllactorItem *subpool = lastItem;
				for (; i < size - 1; i++) {
					lastItem->next = &item[i + 1];
					lastItem = lastItem->next;
				}
				lastItem->next = NULL;

				/*  Bind current next to the next subset.  */
				this->item->next = subpool;
			}

			this->mReserved = size;
		}

		/**
		 *
		 * @return
		 */
		inline T *getLast(void) {
			return &this->item[this->mReserved - 1].data;
		}

		/**
		 *
		 *	@Return
		 */
		inline T &operator[](int index) {
			return this->item[index].data;
		}

		/**
		 *
		 * @param index
		 * @return
		 */
		inline T operator[](int index) const {
			return item[index].data;
		}


		/**
		 *
		 * @param allocator
		 * @return
		 */
		PoolAllocator &operator=(const PoolAllocator &allocator) {
			this->mReserved = 0;
			this->numOfElements = 0;
			this->item = NULL;
			this->setTypeSize(allocator.getTypeSize());

			/*  */
			resize(allocator.reserved());
			this->numOfElements = allocator.numOfElements;

			/*  Copy the list.  */
			memcpy(this->item, allocator.item, allocator.getTypeSize() * allocator.reserved());

			/*	Reconstruct next list.	*/
			PoolAllactorItem *item = this->item;
			for (int x = 0; x < this->reserved() - 1; x++) {
				item[x].next = &item[x + 1];
			}
			item[this->reserved() - 1].next = NULL;

			return *this;
		}

	protected:

		/**
		 *
		 * @return
		 */
		unsigned int getItemSize(void) const {
			return sizeof(PoolAllactorItem);
		}

	private:    /*	attributes.	*/

		PoolAllactorItem *item;        /*	Pool data.	*/
		int numOfElements;            /*	number of elements used.	*/
		int mReserved;                /*	number of allocated elements.	*/
		unsigned int typeSize;        /*	size of the data type.	*/

	};
}


#endif
