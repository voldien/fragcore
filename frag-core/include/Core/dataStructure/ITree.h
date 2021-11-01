/*
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
#ifndef _FRAG_CORE_TREE_H_
#define _FRAG_CORE_TREE_H_ 1
#include"../../Def.h"
#include"Iterator.h"

namespace fragcore {

	/**
	 *
	 */
	template <class T> // TODO evoluate.
	class ITree {
	  public:
		ITree() {
			this->sibling = nullptr;
			this->numChildren = 0;
			this->child = nullptr;
			this->parent = nullptr;
		}
		ITree(ITree &&other) {}

		virtual ITree<T> *root() const {
			if (this->getParent() == nullptr)
				return (ITree<T> *)this;
			else
				return this->getParent()->root();
		}

		virtual bool isEmpty() const { return this->getNumChildren() == 0; }

		virtual ITree<T> *getParent() const { return this->parent; }

		virtual void setParent(ITree<T> *parent) { this->parent = parent; }

		virtual unsigned int getNumChildren() const { return this->numChildren; }

		virtual void addChild(ITree<T> *pchild) {
			ITree<T> *find;
			assert(pchild);

			this->numChildren++;
			if (!this->child) {
				this->setChild(pchild);
				child->setParent(this);
			} else {

				find = this->child;
				while (find) {
					if (find->sibling)
						find = find->sibling;
					else
						break;
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
			if (index >= this->getNumChildren())
				throw InvalidArgumentException("Exceeded {} has {}", index, this->getNumChildren());
			ITree<T> *chi = this->child;
			for (unsigned int x = 0; x <= index; x++) {
				chi = chi->sibling;
			}
			return chi;
		}

		virtual int getNodeChildIndex(ITree<T> *node) {
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
		void setSibling(ITree<T> *sibling) { this->sibling = sibling; }

		void setChild(ITree<T> *child) { this->child = child; }

	  private:					  /*  */
		ITree<T> *parent;		  /*	parent node.	*/
		ITree<T> *sibling;		  /*	sibling node.	*/
		ITree<T> *child;		  /*	child node.	*/
		unsigned int numChildren; /*	number of children node attached.	*/
	};
} // namespace fragcore
#endif
