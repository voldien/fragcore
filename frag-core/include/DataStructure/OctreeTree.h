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
#ifndef _FRAGCORE_OCTREE_H_
#define _FRAGCORE_OCTREE_H_ 1
#include <FragCore.h>

namespace fragcore {

	template <class T> class Octree {
		using Node = struct node {
			Octree *m[8];
			T element;
		};

	  public:
		Octree() {
			nodes = nullptr;
			nrOfElements = 0;
			reserved = 0;
		}

		Octree(const Octree &octree) {}

		~Octree() {}

		void resize(int n) { nodes = realloc(nodes, sizeof(Node) * n); }

		Node *nodes;
		unsigned int nrOfElements;
		unsigned int reserved;
	};
} // namespace fragcore

#endif
