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
#ifndef _FRAG_CORE_DISJOINT_SET_H_
#define _FRAG_CORE_DISJOINT_SET_H_ 1
#include "../../FragDef.h"
#include "Iterator.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 * @tparam T
	 */
	template <class T> class FVDECLSPEC DisjointSet {

		using Partition = struct partition {
			int parent; /*	*/
			T data;		/*	*/
		};

	  public:
		/**
		 *
		 */
		int getNumberSets() const noexcept {
			int index = 0;
			for (index = 0; index < getSize(); index++) {
				if (this->set[index] == -1)
					index++;
			}
			return index;
		}

		/**
		 *
		 */
		int height(const int x) const noexcept {
			int i = x;
			while (i > 0) {
				i = this->set[i].parent;
			}
			return i;
		}

		/**
		 *
		 */
		int maxHeight() const noexcept {
			int max = 0;
			int m;
			for (int i = 0; i < this->getSize(); i++) {
				m = this->height(i);
				if (m > max) {
					max = m;
				}
			}
			return max;
		}

	  public:
		DisjointSet() {
			this->set = nullptr;
			this->reserved = 0;
			this->size = 0;
		}

		DisjointSet(int size) {
			this->set = nullptr;
			this->size = 0;
			this->reserved = 0;
			this->resize(size);
			this->clear();
		}
		DisjointSet(const DisjointSet &disjointSet) { *this = disjointSet; }

		/**
		 *
		 */
		int find(const int x) const noexcept {
			int i;
			for (i = x; i > -1; i = this->set[i].parent) {
			}
			return i;
		}

		/**
		 *
		 */
		int findCompression(const int x) {
			if (this->set[x].parent < 0) {
				return x;
			}
			return this->set[x].parent = this->findCompression(this->set[x].parent);
		}

		/**
		 *
		 */
		void unions(int root1, int root2) noexcept {
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

			if ((size - tmpSize) > 0) {
				this->clear(tmpSize);
			}
		}

		/**
		 *
		 */
		void clear(const int x = 0) noexcept {
			int i;
			for (i = x; i < this->getSize(); i++) {
				this->set[i].parent = -1;
			}
		}

		inline int getSize() const noexcept { return this->size; }

		inline int getReserved() const noexcept { return this->reserved; }

		inline int getTypeSize() const noexcept { return sizeof(Partition); }

		inline void *getData() const noexcept { return this->set; }

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
