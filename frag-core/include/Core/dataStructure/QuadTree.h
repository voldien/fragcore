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
#ifndef _FRAG_CORE_QUAD_TREE_H_
#define _FRAG_CORE_QUAD_TREE_H_ 1
#include <iostream>

namespace fragcore {
	
	/**
	 * @brief 
	 * 
	 * @tparam T 
	 */
	template <typename T> class QuadTree {
		using Node = struct node {
			struct node *lt;
			struct node *rt;
			struct node *lb;
			struct node *rb;
			T data;

		};

	  public:
		QuadTree() {}

		QuadTree(const QuadTree &quadTree) {}

		~QuadTree() {}

		void insert();

		void remove();

		void clear();

	  private:
		Node *nodes;
		unsigned int nrOfElements;
		unsigned int reserved;
	};
} // namespace fragcore

#endif
