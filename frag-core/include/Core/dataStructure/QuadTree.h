/**
	FragEngine, A Two layer Game Engine.
    Copyright (C) 2015  Valdemar Lindberg

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
#ifndef _FRAG_CORE_QUAD_TREE_H_
#define _FRAG_CORE_QUAD_TREE_H_ 1
#include<iostream>

namespace fragcore {
	template<class T>
	class QuadTree {
		typedef struct node {
			node *lt;
			node *rt;
			node *lb;
			node *rb;
			T data;

		} Node;


	public:

		QuadTree(void) {

		}

		QuadTree(const QuadTree &quadTree) {

		}

		~QuadTree(void) {

		}


		void insert(void);

		void remove(void);

		void clear(void);

	private:
		Node *nodes;
		unsigned int nrOfElements;
		unsigned int reserved;

	};
}

#endif
