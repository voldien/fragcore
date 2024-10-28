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
#ifndef _FRAGCORE_QUEUE_H_
#define _FRAGCORE_QUEUE_H_ 1
#include "Iterator.h"
#include <memory>

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
		Queue() : Queue(16) {}

		Queue(const size_t nrOfElements) {
			if (nrOfElements <= 0) {
				throw InvalidArgumentException("");
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
			T *obj = &this->getData()[head];
			head = (head + 1) % getReserved();
			this->nrElements--;
			return *obj;
		}

		// TODO resolve circluar queue for enqueue and dequeue.
		T &enqueue(const T &item) {
			/*	*/
			if (this->getSize() >= this->getReserved() - 1) {
				this->resize(this->getReserved() * 2);
			}

			T *obj = &this->getData()[tail];

			*obj = item;
			// memcpy(obj, &item, getTypeSize());
			tail = (tail + 1) % getReserved();
			this->nrElements++;
			return *obj;
		}

		/**
		 *
		 */
		inline void clear() noexcept {
			this->head = 0;
			this->tail = 0;
		}

		/**
		 *	@return true if queue empty.
		 */
		inline bool isEmpty() const noexcept { return (this->getSize() == 0); }

		/**
		 *	@return true if full, false otherwise.
		 */
		inline bool isFull() const noexcept { return (this->getSize() == this->getReserved()); }

		/**
		 *	Resize queue.
		 */
		void resize(const size_t size) {
			if (this->mdata == nullptr) {
				this->mdata = new T[size]();
			} else {
				this->mdata = new T[size]();

				/*	Relocate.	*/
				for (size_t i = 0; i < getSize(); i++) {
					this->mdata[i] = this->mdata[(this->head + i) % getReserved()];
				}
				this->head = 0;
				this->tail = this->getSize() - 1;
			}
			this->reserved = size;
		}

		size_t getSize() const noexcept { return nrElements; }

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

			Iterator<T> &operator+=(int n) override { return *this; }

			Iterator<T> &operator-=(int n) override { return *this; }

			Iterator<T> &operator+(int n) override { return *this; }

			Iterator<T> &operator-(int n) override { return *this; }
			Iterator<T> &operator[](int index) const override { return *this; }

		  private:
			int index;
			const Queue<T> *queue;
		};

		QueueIterator begin() const noexcept { return QueueIterator(this, this->tail); }

		QueueIterator end() const noexcept { return QueueIterator(this, this->head); }

	  private:
		inline int getTypeSize() const noexcept { return sizeof(T); }

		inline T *getData() const noexcept { return this->mdata; }

	  public: /*  */
		Queue &operator=(const Queue &que) {
			this->mdata = static_cast<T *>(malloc(que.getReserved() * que.getTypeSize()));
			memcpy(mdata, que.getData(), que.getReserved());
			this->head = que.head;
			this->tail = que.tail;
			this->reserved = que.getReserved();
			this->nrElements = que.getSize();
			return *this;
		}

	  private:			   /*  */
		size_t reserved;   /*	*/
		size_t nrElements; /*	*/
		size_t tail;	   /*	*/
		size_t head;	   /*	*/
		T *mdata;		   /*	*/
	};
} // namespace fragcore

#endif
