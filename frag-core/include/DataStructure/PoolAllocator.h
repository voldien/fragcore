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
#ifndef _FRAGCORE_POOL_ALLACTOR_H_
#define _FRAGCORE_POOL_ALLACTOR_H_ 1
#include "../FragDef.h"
#include <cassert>
#include <cstdio>
#include <malloc.h>
#include <utility>

namespace fragcore {

	/**
	 * @brief Pool allocator
	 */
	template <typename T> class FVDECLSPEC PoolAllocator {
	  private:
		static constexpr bool isClass = std::is_class_v<T>;

	  public:
		using PoolAllactorItem = struct poolallactoritem {
			T data;
			poolallactoritem *next; // TODO: perhaps index based.
		};

		PoolAllocator() {
			this->item = nullptr;
			this->nrOfElements = 0;
			this->mReserved = 0;
			this->setTypeSize(sizeof(T));
		}

		PoolAllocator(const PoolAllocator &other) {
			this->item = nullptr;
			this->nrOfElements = 0;
			this->mReserved = 0;
			this->setTypeSize(sizeof(T));
			*this = other;
		}

		PoolAllocator(PoolAllocator &&other) {
			this->item = std::exchange(other.item, nullptr);
			this->nrOfElements = other.nrOfElements;
			this->mReserved = other.mReserved;
			this->setTypeSize(sizeof(T));
		}

		PoolAllocator(const size_t num) : PoolAllocator() { this->resize(num); }

		~PoolAllocator() { clean(); }

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
			this->resize(allocator.reserved());
			this->nrOfElements = allocator.nrOfElements;

			/*  Copy the list.  */
			std::memcpy(this->item, allocator.item, allocator.getTypeSize() * allocator.reserved());

			/*	Reconstruct next list.	*/
			PoolAllactorItem *item = this->item;
			for (int index = 0; index < this->reserved() - 1; index++) {
				item[index].next = &item[index + 1];
			}
			item[this->reserved() - 1].next = nullptr;

			return *this;
		}

		PoolAllocator &operator=(PoolAllocator &&other) {
			this->item = std::exchange(other.item, nullptr);
			this->nrOfElements = other.nrOfElements;
			this->mReserved = other.mReserved;
			this->setTypeSize(sizeof(T));
			return *this;
		}

		/**
		 *	Get a pointer to next object in pool allocator.
		 *
		 *	@return None nullptr pointer if successful.
		 */
		T *obtain() {
			T *alloc = nullptr;

			/*	if last, resize.    */ // TODO: one without
			if (this->isFull()) {
				this->resize(nrOfElements + 256);
				return this->obtain();
			}

			/*  Special case.   */
			if (this->nrOfElements == 0) {
				alloc = &this->item->data;
			} else {
				alloc = &this->item->next->data; /*	changed.	*/
			}

			/*	Update entries.	*/
			this->item->next = this->item->next->next;
			this->nrOfElements++;

			return alloc;
		}

		/**
		 *
		 * @param element
		 */
		void Return(T *element) {
			if (!this->isValidItem(*element)) {
				throw InvalidArgumentException("invalid pointer returned");
			}

			/*	*/
			PoolAllactorItem *alloc = static_cast<PoolAllactorItem *>(element);
			alloc->next = item->next;
			item->next = alloc;

			/*	*/
			this->nrOfElements--;
		}

		/**
		 * Check if the data is in the same address space.
		 * @param data
		 * @return  true if valid item.
		 */
		bool isValidItem(const T &data) const noexcept {
			const T *tmp = &data;
			/*  If pointer is less than the base pointer.*/
			if (static_cast<void *>(tmp) < static_cast<void *>(this->item)) {
				return false;
			}
			/*  Check if pointer is outside the end point*/
			if (tmp >= static_cast<void *>(&this->item[this->reserved()])) {
				return false;
			}
			/*	Check if memory is a multiple of the size from the base pointer.	*/
			return true;
		}

		/**
		 *
		 */
		void clean() noexcept {
			if (isClass) {
				delete[] this->item;
			} else {
				free(this->item);
			}

			this->item = nullptr;
			this->mReserved = 0;
			this->nrOfElements = 0;
		}

		bool isFull() const noexcept { return this->nrOfElements >= (size_t)std::max<int>(0, this->reserved() - 1); }

		size_t size() const noexcept { return this->nrOfElements; }

		size_t reserved() const noexcept { return this->mReserved; }

		/**
		 * Get datatype size.
		 */
		void setTypeSize(const size_t size) noexcept { this->typeSize = size; }
		size_t getTypeSize() const noexcept { return this->typeSize; }

		/**
		 *
		 */
		/*	TODO Fix, still some bugs!	*/
		void resize(const size_t nrRequestedElements) {
			size_t index = 0;
			const size_t itemSize = this->getItemSize();

			assert(nrRequestedElements >= 0);

			/*  No allocation.  */
			if (this->reserved() == 0) {

				if constexpr (isClass) {
					this->item = static_cast<PoolAllactorItem *>(new PoolAllactorItem[nrRequestedElements]);
				} else {
					this->item = static_cast<PoolAllactorItem *>(realloc(this->item, itemSize * nrRequestedElements));
				}

				/*	*/
				for (index = 0; index < nrRequestedElements - 1; index++) {
					item[index].next = &item[index + 1];

					if constexpr (isClass) {
						T *ob = new (&item[index].data) T();
						//item[index].data = *ob;
					}
				}

				item[nrRequestedElements - 1].next = nullptr;
			} else {
				/*  */
				if (nrRequestedElements < this->reserved()) {
					throw InvalidArgumentException("Can not be downsized from {} to {}.", this->reserved(),
												   nrRequestedElements);
				}

				/*  Reallocate buffer.  */
				index = this->reserved();
				this->item = static_cast<PoolAllactorItem *>(realloc(this->item, itemSize * nrRequestedElements));

				/*	*/
				PoolAllactorItem *lastItem = &this->item[index];
				PoolAllactorItem *subpool = lastItem;
				for (; index < nrRequestedElements - 1; index++) {
					lastItem->next = &item[index + 1];
					lastItem = lastItem->next;
					if (isClass) {
						// lastItem->data = std::move(T());
					}
				}
				lastItem->next = nullptr;

				/*  Bind current next to the next subset.  */
				this->item->next = subpool;
			}

			this->mReserved = nrRequestedElements;
		}

		T *getLast() noexcept { return &this->item[this->mReserved - 1].data; }

		T &operator[](int index) { return this->item[index].data; }

		T operator[](int index) const { return item[index].data; }

	  protected:
		/**
		 *
		 * @return
		 */
		constexpr size_t getItemSize() const noexcept { return sizeof(PoolAllactorItem); }

	  private:							  /*	attributes.	*/
		PoolAllactorItem *item = nullptr; /*	Pool data.	*/
		size_t nrOfElements = 0;		  /*	number of elements used.	*/
		size_t mReserved = 0;			  /*	number of allocated elements.	*/
		size_t typeSize = 0;			  /*	size of the data type.	*/
	};
} // namespace fragcore

#endif
