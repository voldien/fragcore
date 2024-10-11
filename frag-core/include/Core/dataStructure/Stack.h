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
#ifndef _FRAG_CORE_STACK_H_
#define _FRAG_CORE_STACK_H_ 1
#include "../../FragDef.h"
#include "Iterator.h"
#include <utility>

namespace fragcore {

	/**
	 * @brief
	 *
	 * @tparam T
	 */
	// TODO add atomic support
	template <class T> class Stack : public std::allocator<T> {
	  public:
		Stack() {
			this->data = nullptr;
			this->nrElements = 0;
			this->mreserved = 0;
		}
		Stack(size_t size) : Stack() { this->reserve(size); }

		Stack(const Stack &stack) : Stack() {
			reserve(stack.getReserved());
			memcpy(this->data, stack.data, stack.getSize() * sizeof(T));
		}

		Stack(Stack &&other) {
			this->data = std::exchange(other.data, nullptr);
			this->nrElements = std::exchange(other.nrElements, 0);
			this->mreserved = std::exchange(other.mreserved, 0);
		}

		~Stack() { delete data; }

		/**
		 *
		 * @param allocator
		 * @return
		 */
		Stack &operator=(const Stack &allocator) {
			reserve(allocator.getReserved());
			mempcpy(this->data, allocator.data, sizeof(T) * allocator.getReserved());
			this->nrElements = allocator.nrElements;
			this->mreserved = allocator.mreserved;
			return *this;
		}

		Stack &operator=(Stack &&alloctor) {
			this->data = std::exchange(alloctor.data, nullptr);
			this->nrElements = std::exchange(alloctor.nrElements, 0);
			this->mreserved = std::exchange(alloctor.mreserved, 0);
			return *this;
		}

		T &push(const T &p) {
			this->data[this->nrElements] = p;
			this->nrElements++;
			return this->data[this->nrElements - 1];
		}

		T &peek() const { return data[this->nrElements - 1]; }

		T &pop() {
			// TODO add validate
			if (this->nrElements > 0) {
				this->nrElements--;
				return this->data[this->nrElements + 1];
			}
			throw RuntimeException();
		}

		T &operator[](int index) { return this->data[index]; }

		const T &operator[](int index) const { return this->data[index]; }

		void reserve(int nrOfElement) {
			this->data = static_cast<T *>(realloc(this->data, nrOfElement * sizeof(T)));
			if (this->data == nullptr) {
				throw RuntimeException("Out of memory"); // TODO replace with the system exception.
			}
			this->mreserved = nrOfElement;
		}

		inline void clear() noexcept { this->nrElements = 0; }

		inline bool isEmpty() const noexcept { return this->nrElements != 0; }

		inline size_t getSize() const noexcept { return this->nrElements; }

		inline size_t getReserved() const noexcept { return this->mreserved; }

		class StackIterator : public Iterator<T> {};

		StackIterator begin() {}
		StackIterator end() {}

	  private:			   /*	Attributes.	*/
		size_t nrElements; /*	Number of elements in the stack.	*/
		size_t mreserved;  /*	Number of reserved elements in the stack.	*/
		T *data;		   /*	*/
	};

} // namespace fragcore

#endif
