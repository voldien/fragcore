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
#ifndef _FRAG_CORE_TREE_H_
#define _FRAG_CORE_TREE_H_ 1
#include "../FragDef.h"
#include "Iterator.h"
#include <utility>

namespace fragcore {

	/**
	 *
	 */
	template <typename T> class ITree {
	  public:
		ITree() {
			this->sibling = nullptr;
			this->numChildren = 0;
			this->child = nullptr;
			this->parent = nullptr;
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
			ITree<T> *find;
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
			ITree<T> *sn = getChild(index - 1);
			ITree<T> *n = sn->sibling;

			sn->setSibling(n->sibling);
			n->parent = nullptr;
		}

		virtual ITree<T> *getChild(unsigned int index) const {
			if (index >= this->getNumChildren()) {
				throw InvalidArgumentException("Exceeded {} has {}", index, this->getNumChildren());
			}
			ITree<T> *chi = this->child;
			for (unsigned int x = 0; x <= index; x++) {
				chi = chi->sibling;
			}
			return chi;
		}

		virtual bool isChild(ITree<T> *item) const noexcept {
			for (unsigned int i = 0; i < getNumChildren(); i++) {
				if (item == getChild(i)) {
					return true;
				}
			}
			return false;
		}

		virtual int getNodeChildIndex(ITree<T> *node) noexcept {
			ITree<T> *n = this->child;
			int i = 0;
			while (n) {
				if (n == node) {
					return i;
				}
				i++;
				n = n->child;
			}
			return -1;
		}

		// T *operator->() { return (T *)this; }

		// T *operator*() { return (T *)this; }

		// const T *operator->() const { return (T *)this; }

		virtual const T *ptr() const noexcept { return (T *)this; }
		virtual T *ptr() noexcept { return (T *)this; }

		class TIterator : public Iterator<T> {
		  public:
			/*			TIterator &operator++() override {
							return Iterator::operator++();
						}

						TIterator &operator++(int i) override {
							return Iterator::operator++(i);
						}

						TIterator &operator--() override {
							return Iterator::operator--();
						}

						TIterator &operator+=(int n) override {
							return Iterator::operator+=(n);
						}
		virtual const T *ptr() const { return (T *)this; }
		virtual T *ptr() { return (T *)this; }

						TIterator &operator-=(int n) override {
							return Iterator::operator-=(n);
						}

						TIterator &operator+(int n) override {
							return Iterator::operator+(n);
						}

						TIterator &operator-(int n) override {
							return Iterator::operator-(n);
						}

						TIterator &operator[](int index) const override {
							return Iterator::operator[](index);
						}

						T &operator->() const override {
							return Iterator::operator->();
						}

						T &operator*() const override {
							return Iterator::operator*();
						}

						T &operator*() override {
							return Iterator::operator*();
						}

						bool operator==(const TIterator &iterator) override {
							return Iterator::operator==(iterator);
						}

						bool operator!=(const TIterator &iterator) override {
							return Iterator::operator!=(iterator);
						}

						Iterator<T> &operator=(const TIterator &iterator) override {
							return Iterator::operator=(iterator);
						}*/
		};

		/*  TODO determine if iterator can be added.    */
		//		virtual TIterator<T> begin();
		//		virtual TIterator<T> end();

	  protected: /*  */
		void setSibling(ITree<T> *sibling) noexcept { this->sibling = sibling; }

		void setChild(ITree<T> *child) noexcept { this->child = child; }

	  private:					  /*  */
		ITree<T> *parent;		  /*	parent node.	*/
		ITree<T> *sibling;		  /*	sibling node.	*/
		ITree<T> *child;		  /*	child node.	*/
		unsigned int numChildren; /*	number of children node attached.	*/
	};
} // namespace fragcore
#endif
