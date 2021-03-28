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
#ifndef _FRAG_CORE_DISJOINT_SET_H_
#define _FRAG_CORE_DISJOINT_SET_H_ 1
#include "../../Def.h"
#include "Iterator.h"

namespace fragcore {

	/**
	 *
	 */
	template <class T> class FVDECLSPEC DisjointSet {

		typedef struct partition {
			int parent; /*	*/
			T data;		/*	*/
		} Partition;

	  public:
		/**
		 *
		 */
		int getNumberSets(void) const {
			int i = 0;
			for (i = 0; i < getSize(); i++) {
				if (this->set[i] == -1)
					i++;
			}
			return i;
		}

		/**
		 *
		 */
		int height(int x) const {
			int i = x;
			while (i > 0) {
				i = this->set[i].parent;
			}
			return i;
		}

		/**
		 *
		 */
		int maxHeight(void) const {
			int max = 0;
			int m;
			for (int i = 0; i < this->getSize(); i++) {
				m = this->height(i);
				if (m > max)
					max = m;
			}
			return max;
		}

	  public:
		DisjointSet(void) {
			this->set = NULL;
			this->reserved = 0;
			this->size = 0;
		}

		DisjointSet(int size) {
			this->set = NULL;
			this->size = 0;
			this->reserved = 0;
			this->resize(size);
			this->clear();
		}
		DisjointSet(const DisjointSet &disjointSet) { *this = disjointSet; }

		/**
		 *
		 */
		int find(int x) const {
			int i;
			for (i = x; i > -1; i = this->set[i].parent) {
				continue;
			}
			return i;
		}

		/**
		 *
		 */
		int findCompression(int x) {
			if (this->set[x].parent < 0)
				return x;
			else {
				return this->set[x].parent = this->findCompression(this->set[x].parent);
			}
		}

		/**
		 *
		 */
		void unions(int root1, int root2) {
			if (root1 != root2) {
				this->set[root2].parent = root1;
			}
		}

		/**
		 *
		 */
		void unionRank(int root1, int root2) {
			if (root1 != root2) {

				if (this->set[root1].parent <= this->set[root2].parent) {
					// first tree has more nodes
					this->set[root1].parent += this->set[root2].parent;
					this->set[root2].parent = root1;
				} else { // second tree has more nodes
					this->set[root2].parent += this->set[root1].parent;
					this->set[root1].parent = root2;
				}
			}
		}

		/**
		 *
		 */
		inline T &operator[](int index) { return this->set[index].data; }

		/**
		 *
		 */
		inline T &operator[](int index) const { return this->set[index].data; }

		/**
		 *
		 */
		void resize(int size) {
			int tmpSize = this->getSize();
			/*allocate size and partitons new allocated address*/
			this->set = realloc(set, size * sizeof(Partition));
			this->size = size;

			if ((size - tmpSize) > 0)
				clear(tmpSize);
		}

		/**
		 *
		 */
		void clear(int x = 0) {
			int i;
			for (i = x; i < this->getSize(); i++) {
				this->set[i].parent = -1;
			}
		}

		inline int getSize(void) const { return this->size; }

		inline int getReserved(void) const { return this->reserved; }

		inline int getTypeSize(void) const { return sizeof(Partition); }

		inline void *getData(void) const { return this->set; }

		/**
		 *
		 */
		DisjointSet &operator=(const DisjointSet &disjointSet) {
			this->resize(disjointSet.getSize());
			memcpy(this->set, disjointSet.getData(), disjointSet.getSize() * sizeof(Partition));

			return *this;
		}

	  private:				   /*	Attributes.	*/
		Partition *set;		   /*	*/
		unsigned int size;	   /*	*/
		unsigned int reserved; /*	*/
	};
} // namespace fragcore

#endif
