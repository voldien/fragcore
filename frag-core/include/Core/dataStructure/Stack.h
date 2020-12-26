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
#ifndef _FRAG_CORE_STACK_H_
#define _FRAG_CORE_STACK_H_ 1
#include "../../Def.h"
#include"Iterator.h"

namespace fragcore {
	/**
	 *	Data structure for
	 *	putting data on stack.
	 */
	template<class T>
	class Stack : public std::allocator<T>{
	public:

		Stack(void){
			this->data = NULL;
			this->msize = 0;
			this->mreserved = 0;
		}

		Stack(const Stack& stack){
			this->data = NULL;
			this->msize = 0;
			this->mreserved = 0;
			reserve(stack.getReserved());
			memcpy(this->data, stack.data, stack.getSize() * sizeof(T));
		}

		~Stack(void){
			delete data;
		}

		void push(const T& p){
			this->data[this->msize] = p;
			this->msize++;
		}

		T& peek(void)const{
			return data[this->msize - 1];
		}


		inline void pop(void){
			this->msize--;
		}

		inline T& operator[](int index){return this->data[index];}


		inline T& operator[](int index)const{return this->data[index];}

		void reserve(int nrOfElement){
			this->data = realloc(this->data, nrOfElement * sizeof(T));
			assert(this->data != NULL);
		}


		inline void clear(void){
			this->msize = 0;
		}

		inline bool isEmpty(void)const{
			return this->msize != 0;
		}

		inline int getSize(void)const{
			return this->msize;
		}


		inline int getReserved(void)const{
			return this->mreserved;
		}

		class StackIterator : public Iterator<T> {

		};

		StackIterator begin(void){}
		StackIterator end(void){}

	private:	/*	Attributes.	*/

		unsigned int msize;			/*	Number of elements in the stack.	*/
		unsigned int mreserved;		/*	Number of reserved elements in the stack.	*/
		T* data;
	};

}

#endif
