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
#ifndef _FRAG_CORE_QUEUE_H_
#define _FRAG_CORE_QUEUE_H_ 1
//#include "../../Exception/InvalidArgumentException.h"
#include "Iterator.h"
#include <memory>

namespace fragcore {
	/**
	 *	Rotating Queue data struct.
	 */
	template <class T, int U = 0> class Queue : public std::allocator<T> {
	  public:
		Queue(void) {
			this->mdata = nullptr;
			this->reserved = 0;
			this->nrElements = 0;
			clear();
			this->resize(16);
		}

		Queue(unsigned int nrOfElements) {
			if (nrOfElements <= 0)
				throw InvalidArgumentException("");
			this->mdata = nullptr;
			this->reserved = 0;
			this->nrElements = 0;
			clear();
			this->resize(nrOfElements);
		}

		Queue(const Queue &queue) { *this = queue; }

		~Queue(void) { delete[] this->mdata; }

		T &front(void) const { return getData()[head]; }

		T &back(void) const { return getData()[tail]; }

		T &dequeue(void) {
			if (isEmpty())
				throw InvalidArgumentException("");
			T *obj = &this->getData()[head];
			head = (head + 1) % getReserved();
			this->nrElements--;
			return *obj;
		}

		// TODO resolve circluar queue for enqueue and dequeue.
		T &enqueue(const T &item) {
			if (getSize() >= getReserved() - 1)
				resize(getReserved() * 2);
			T *obj = &this->getData()[tail];

			memcpy(obj, &item, getTypeSize());
			tail = (tail + 1) % getReserved();
			this->nrElements++;
			return *obj;
		}

		/**
		 *
		 */
		inline void clear(void) {
			this->head = 0;
			this->tail = 0;
		}

		/**
		 *	@Return true if queue empty.
		 */
		inline bool isEmpty(void) const { return (this->getSize() == 0); }

		/**
		 *	@Return true if full, false otherwise.
		 */
		inline bool isFull(void) const { return (this->getSize() == this->getReserved()); }

		/**
		 *	Resize queue.
		 */
		void resize(unsigned int size) {
			if (this->mdata == nullptr) {
				this->mdata = (T *)malloc(this->getTypeSize() * size);
			} else {
				this->mdata = (T *)realloc(this->mdata, this->getTypeSize() * size);

				/*	Relocate.	*/
				for (unsigned int i = 0; i < getSize(); i++)
					this->mdata[i] = this->mdata[(this->head + i) % getReserved()];
				this->head = 0;
				this->tail = this->getSize() - 1;
			}
			this->reserved = size;
		}

		unsigned int getSize(void) const { return nrElements; }

		unsigned int getReserved(void) const { return this->reserved; }

		/*  */
		class QueueIterator : public Iterator<T> {
		  public:
			QueueIterator(Queue<T> *queue);
		};

		Iterator<T> begin(void) {}

		Iterator<T> end(void) {}

	  private:
		inline int getTypeSize(void) const { return sizeof(T); }

		inline T *getData(void) const { return this->mdata; }

	  public: /*  */
		/**
		 *	@Return
		 */
		Queue &operator=(const Queue &que) {
			this->mdata = (T *)malloc(que.getReserved() * que.getTypeSize());
			memcpy(mdata, que.getData(), que.getReserved());
			this->head = que.head;
			this->tail = que.tail;
			this->reserved = que.getReserved();
			this->nrElements = que.getSize();
			return *this;
		}

	  private:					 /*  */
		unsigned int reserved;	 /*	*/
		unsigned int nrElements; /*	*/
		unsigned int tail;		 /*	*/
		unsigned int head;		 /*	*/
		T *mdata;				 /*	*/
	};
} // namespace fragcore

#endif
