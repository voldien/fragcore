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
#ifndef _FRAGCORE_QUEUE_H_
#define _FRAGCORE_QUEUE_H_ 1
#include "Iterator.h"
#include <memory>
#include <utility>

namespace fragcore {
	/**
	 * @brief
	 *
	 * @tparam T
	 * @tparam U
	 */
	// TODO add atomic support
	template <class T, int U = 0> class Queue : public std::allocator<T> {
	  public:
		static constexpr bool isClass = std::is_class_v<T>;
		using DType = T;
		static constexpr unsigned int DTypeSize = sizeof(DType);

	  public:
		Queue() : Queue(16) {}

		Queue(const size_t nrOfElements) {
			if (nrOfElements <= 0) {
				throw InvalidArgumentException("Must have a non negative number of elements");
			}
			this->mdata = nullptr;
			this->reserved = 0;
			this->nrElements = 0;
			this->clear();
			this->resize(nrOfElements);
		}

		Queue(const Queue &queue) { *this = queue; }

		~Queue() { delete[] this->mdata; }

		T &front() const { return this->getData()[head]; }

		T &back() const { return this->getData()[tail]; }

		T &dequeue() {
			if (this->isEmpty()) {
				throw InvalidArgumentException("");
			}

			/*	*/
			T *obj = &this->getData()[this->head];
			this->head = (this->head + 1) % getReserved();
			this->nrElements--;
			return *obj;
		}

		// TODO resolve circluar queue for enqueue and dequeue.
		T &enqueue(const T &item) {
			/*	*/
			if (this->getSize() >= this->getReserved() - 1) {
				this->resize(this->getReserved() * 2);
			}

			/*	*/
			T *obj = &this->getData()[this->tail];

			/*	*/
			*obj = item;

			/*	*/
			tail = (tail + 1) % this->getReserved();
			this->nrElements++;
			return *obj;
		}

		T &peek(const size_t index) const noexcept {
			const size_t mem_index = (this->head + index) % this->getReserved();
			return this->getData()[mem_index];
		}

		T &operator[](const size_t index) { return this->getData()[index]; }
		const T &operator[](const size_t index) const { return this->getData()[index]; }

		/**
		 *
		 */
		void clear() noexcept {
			this->head = 0;
			this->tail = 0;
			this->nrElements = 0;
		}

		/**
		 *	@return true if queue empty.
		 */
		bool isEmpty() const noexcept { return (this->getSize() <= 0); }

		/**
		 *	@return true if full, false otherwise.
		 */
		bool isFull() const noexcept { return (this->getSize() >= this->getReserved()); }

		/**
		 *	Resize queue.
		 */
		void resize(const size_t newSize) {
			if (newSize <= 0) {
				throw InvalidArgumentException("Must have a non negative number of elements");
			}

			if (this->mdata == nullptr) {
				this->mdata = new T[newSize]();
			} else {

				if constexpr (isClass) {
					T tmp = new T[newSize]();

					/*	Relocate.	*/
					for (size_t index = 0; index < getSize(); index++) {
						this->tmp[index] = peek(index);
					}
					/*	set new pointer.	*/
					delete[] this->mdata;
					this->mdata = tmp;

				} else {
					this->mdata = (T *)std::realloc(this->mdata, newSize * getTypeSize());
				}

				this->head = 0;
				this->tail = this->getSize() - 1;
			}

			ASSERT(this->mdata, "Must be None Null");
			this->reserved = newSize;
		}

		size_t getSize() const noexcept { return this->nrElements; }
		size_t getReserved() const noexcept { return this->reserved; }

		/*  */
		class QueueIterator : public Iterator<T> {
		  public:
			QueueIterator(const Queue<T> *queue, int index) : queue(queue), index(index) {
				this->iterator = &queue->getData()[index];
			}

			Iterator<T> &operator++() override {
				this->index = (this->index + 1) % this->queue->getSize();
				this->iterator = &queue->getData()[index];
				return *this;
			}

			Iterator<T> &operator--() override { return *this; }

			Iterator<T> &operator+=([[maybe_unused]] int n) override { return *this; }

			Iterator<T> &operator-=([[maybe_unused]] int n) override { return *this; }

			Iterator<T> &operator+([[maybe_unused]] int n) override { return *this; }

			Iterator<T> &operator-([[maybe_unused]] int n) override { return *this; }
			Iterator<T> &operator[]([[maybe_unused]] int index) const override { return *this; }

		  private:
			int index;
			const Queue<T> *queue;
		};

		QueueIterator begin() const noexcept { return QueueIterator(this, this->tail); }

		QueueIterator end() const noexcept { return QueueIterator(this, this->head); }

	  private:
		size_t getTypeSize() const noexcept { return DTypeSize; }
		T *getData() const noexcept { return this->mdata; }

	  public: /*  */
		Queue &operator=(const Queue &queue) {

			/*	*/
			this->mdata = static_cast<T *>(malloc(queue.getReserved() * queue.getTypeSize()));
			std::memcpy(mdata, queue.getData(), queue.getReserved());

			/*	*/
			this->head = queue.head;
			this->tail = queue.tail;
			this->reserved = queue.getReserved();
			this->nrElements = queue.getSize();
			return *this;
		}

		Queue &operator=(Queue &&queue) {

			/*	*/
			this->mdata = std::exchange(this->mdata, queue.mdata);

			/*	*/
			this->head = queue.head;
			this->tail = queue.tail;
			this->reserved = queue.getReserved();
			this->nrElements = queue.getSize();
			return *this;
		}

	  private:				 /*  */
		size_t reserved{};	 /*	*/
		size_t nrElements{}; /*	*/
		size_t tail{};		 /*	*/
		size_t head{};		 /*	*/
		T *mdata;			 /*	*/
	};
} // namespace fragcore

#endif
