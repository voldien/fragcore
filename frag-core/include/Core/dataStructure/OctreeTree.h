/**
    Copyright (C) 2015  Valdemar Lindberg

    This getProgram() is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This getProgram() is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this getProgram().  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _VD_OCTREE_H_
#define _VD_OCTREE_H_ 1
#include<iostream>

namespace fragcore {
	template<class T>
	class Octree {
		typedef struct node {
			Octree *m[8];
			T element;

		} Node;

	public:
		Octree(void) {
			nodes = nullptr;
			nrOfElements = 0;
			reserved = 0;
		}

		Octree(const Octree &octree) {

		}

		~Octree(void) {

		}


		void resize(int n) {
			nodes = realloc(nodes, sizeof(Node) * n);
		}


		Node *nodes;
		unsigned int nrOfElements;
		unsigned int reserved;

	};
}


#endif
