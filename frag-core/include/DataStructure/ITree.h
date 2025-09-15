/*
 *	FragCore - Fragment Core
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
			this->child = other.child;
			this->parent = other.parent;
			this->sibling = other.sibling;
			this->numChildren = other.numChildren;
		}

		ITree(ITree &&other) {
			this->child = std::exchange(other.child, nullptr);
			this->parent = std::exchange(other.parent, nullptr);
			this->sibling = std::exchange(other.sibling, nullptr);
			this->numChildren = other.numChildren;
		}

		ITree &operator=(const ITree &other) {
			this->child = other.child;
			this->parent = other.parent;
			this->sibling = other.sibling;
			this->numChildren = other.numChildren;
			return *this;
		}

		ITree &operator=(ITree &&other) {
			this->child = std::exchange(other.child, nullptr);
			this->parent = std::exchange(other.parent, nullptr);
			this->sibling = std::exchange(other.sibling, nullptr);
			this->numChildren = other.numChildren;
			return *this;
		}

		virtual ITree<T> *root() const noexcept {
			if (this->getParent() == nullptr) {
				return (ITree<T> *)this;
			}
			return this->getParent()->root();
		}

		virtual bool isEmpty() const noexcept { return this->getNumChildren() == 0; }

		virtual ITree<T> *getParent() const noexcept { return this->parent; }

		virtual void setParent(ITree<T> *parent) noexcept {
			/*	Add Child if not exists.	*/
			ITree<T> *myParent = getParent();

			/*	If new parent is not the same as current parent.	*/
			if (myParent != parent) {

				/*	Remove if existing parent.	*/
				if (myParent) {
					myParent->removeChild(this);
					myParent = nullptr;
				}

				/*	*/
				parent->addChild(this);
			}
		}

		virtual unsigned int getNumChildren() const noexcept { return this->numChildren; }

		virtual void addChild(ITree<T> *pchild) noexcept {
			ITree<T> *find = nullptr;
			assert(pchild);

			this->numChildren++;
			if (!this->child) {
				this->setChild(pchild);	/*	Assign Child.	*/
				child->parent = this;	/*	Assign parent*/
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
				find->sibling->parent = this;
			}
		}

		virtual void removeChild(unsigned int index) {
			ITree<T> *node_sibling = getChild(index - 1);
			ITree<T> *sib = node_sibling->sibling;

			node_sibling->setSibling(sib->sibling);
			sib->parent = nullptr;
		}

		virtual void removeChild(ITree<T> *child) {
			const int child_index = getNodeChildIndex(child);
			if (child_index >= 0) {
				removeChild(getNodeChildIndex(child));
			}
		}

		virtual ITree<T> *getChild(const unsigned int index) const {
			if (index >= this->getNumChildren()) {
				throw InvalidArgumentException("Exceeded {} has {}", index, this->getNumChildren());
			}
			ITree<T> *chi = this->child;
			for (unsigned int nth_child = 1; nth_child <= index; nth_child++) {
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

			return -1; /*	No Child Found.	*/
		}

		virtual const T *ptr() const noexcept { return (T *)this; }
		virtual T *ptr() noexcept { return (T *)this; }

		T *operator->() { return (T *)this; }
		T *operator*() { return (T *)this; }

		const T *operator->() const { return (T *)this; }

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
