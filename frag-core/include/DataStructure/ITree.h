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
#ifndef _FRAGCORE_TREE_H_
#define _FRAGCORE_TREE_H_ 1
#include "../FragDef.h"
#include <utility>

namespace fragcore {

	/**
	 *
	 */
	template <typename T> class FVDECLSPEC ITree {
	  public:
		ITree() {
			this->sibling = nullptr;
			this->numChildren = 0;
			this->child = nullptr;
			this->parent = nullptr;
		}

		ITree(const ITree &other) {
			// TODO: impl
			// this->child = std::exchange(other.child, nullptr);
			// this->parent = std::exchange(other.parent, nullptr);
			// this->sibling = std::exchange(other.sibling, nullptr);
			this->numChildren = other.numChildren;
		}

		ITree(ITree &&other) {
			this->child = std::exchange(other.child, nullptr);
			this->parent = std::exchange(other.parent, nullptr);
			this->sibling = std::exchange(other.sibling, nullptr);
			this->numChildren = other.numChildren;
		}

		virtual ITree<T> *root() const noexcept {
			if (this->getParent() == nullptr) {
				return (ITree<T> *)this;
			}
			return this->getParent()->root();
		}

		virtual bool isEmpty() const noexcept { return this->getNumChildren() == 0; }

		virtual ITree<T> *getParent() const noexcept { return this->parent; }

		virtual void setParent(ITree<T> *parent) noexcept { this->parent = parent; }

		virtual unsigned int getNumChildren() const noexcept { return this->numChildren; }

		virtual void addChild(ITree<T> *pchild) noexcept {
			ITree<T> *find = nullptr;
			assert(pchild);

			this->numChildren++;
			if (!this->child) {
				this->setChild(pchild);
				child->setParent(this);
			} else {

				find = this->child;
				while (find) {
					if (find->sibling) {
						find = find->sibling;
					} else {
						break;
					}
				}
				find->sibling = pchild;
				find->sibling->setParent(this);
			}
		}

		virtual void removeChild(unsigned int index) {
			ITree<T> *node_sibling = getChild(index - 1);
			ITree<T> *sib = node_sibling->sibling;

			node_sibling->setSibling(sib->sibling);
			sib->parent = nullptr;
		}

		virtual ITree<T> *getChild(unsigned int index) const {
			if (index >= this->getNumChildren()) {
				throw InvalidArgumentException("Exceeded {} has {}", index, this->getNumChildren());
			}
			ITree<T> *chi = this->child;
			for (unsigned int nth_child = 0; nth_child <= index; nth_child++) {
				chi = chi->sibling;
			}
			return chi;
		}

		virtual bool isChild(ITree<T> *item) const {
			for (unsigned int i = 0; i < getNumChildren(); i++) {
				if (item == getChild(i)) {
					return true;
				}
			}
			return false;
		}

		virtual int getNodeChildIndex(ITree<T> *node) noexcept {
			ITree<T> *child_node = this->child;
			int index = 0;
			while (child_node) {
				if (child_node == node) {
					return index;
				}
				index++;
				child_node = child_node->child;
			}
			return -1;
		}

		virtual const T *ptr() const noexcept { return (T *)this; }
		virtual T *ptr() noexcept { return (T *)this; }

	  protected: /*  */
		void setSibling(ITree<T> *sibling) noexcept { this->sibling = sibling; }

		void setChild(ITree<T> *child) noexcept { this->child = child; }

	  private:						  /*  */
		ITree<T> *parent = nullptr;	  /*	parent node.	*/
		ITree<T> *sibling = nullptr;  /*	sibling node.	*/
		ITree<T> *child = nullptr;	  /*	child node.	*/
		unsigned int numChildren = 0; /*	number of children node attached.	*/
	};
} // namespace fragcore
#endif
