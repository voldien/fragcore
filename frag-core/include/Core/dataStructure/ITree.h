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
#include"../../Exception/InvalidArgumentException.h"
#include"../../Def.h"
#include"Iterator.h"

namespace fragcore {

	/**
	 * 
	 */
	template<class T>	//TODO evoluate.
	class ITree {
	public:
		ITree(void) {
			this->sibling = NULL;
			this->numChildren = 0;
			this->child = NULL;
			this->parent = NULL;
		}

		virtual ITree<T> *root(void) const {
			if (this->getParent())
				return (ITree<T>*)this;
			else
				return this->getParent()->root();
		}

		virtual bool isEmpty(void) const {
			return this->getNumChildren() == 0;
		}

		virtual ITree<T> *getParent(void) const {
			return this->parent;
		}

		virtual void setParent(ITree<T> *parent) {
			this->parent = parent;
		}

		virtual unsigned int getNumChildren(void) const {
			this->numChildren;
		}

		virtual void addChild(ITree<T> *pchild)
		{
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
					else break;
				}
				find->sibling = pchild;
				find->sibling->setParent(this);
			}
		}

		virtual void removeChild(int index) {
			ITree<T> *sn = getChild(index - 1);
			ITree<T> *n = sn->sibling;
			sn->setSibling(n->sibling);
			n->parent = NULL;
		}

		virtual ITree<T> *getChild(int index) const
		{
			if(index < 0 || index >= this->getNumChildren())
				throw InvalidArgumentException("");
			ITree<T> *chi = this->child;
			for (int x = 0; x <= index; x++) {
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

		class TIterator : public Iterator<T>{
		public:
/*			TIterator &operator++(void) override {
				return Iterator::operator++();
			}

			TIterator &operator++(int i) override {
				return Iterator::operator++(i);
			}

			TIterator &operator--(void) override {
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

			T &operator->(void) const override {
				return Iterator::operator->();
			}

			T &operator*(void) const override {
				return Iterator::operator*();
			}

			T &operator*(void) override {
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
//		virtual TIterator begin(void);
//		virtual TIterator end(void);

	protected:  /*  */
		void setSibling(ITree<T> *sibling) {
			this->sibling = sibling;
		}

		void setChild(ITree<T> *child) {
			this->child = child;
		}

	private:    /*  */
		ITree<T> *parent;				/*	parent node.	*/
		ITree<T> *sibling;				/*	sibling node.	*/
		ITree<T> *child;					/*	child node.	*/
		unsigned int numChildren;   /*	number of children node attached.	*/
	};
}
#endif
