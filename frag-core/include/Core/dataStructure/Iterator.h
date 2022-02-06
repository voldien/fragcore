/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAG_CORE_ITERATOR_H_
#define _FRAG_CORE_ITERATOR_H_ 1
#include "../../Def.h"
#include "../Ref.h"

namespace fragcore {

	/**
	 * @brief Simple abstract Iterator class for
	 * generic classes.
	 * @tparam T
	 */
	template <class T> class Iterator {
	  public:
		/**
		 *	@return
		 */
		virtual Iterator &operator++() = 0;

		/**
		 *	@return
		 */
		virtual Iterator &operator++(int) = 0;

		/**
		 *	@return
		 */
		virtual Iterator &operator--() = 0;

		/**
		 *	@return
		 */
		virtual Iterator &operator+=(int n) = 0;

		/**
		 *	@return
		 */
		virtual Iterator &operator-=(int n) = 0;

		/**
		 *	@return
		 */
		virtual Iterator &operator+(int n) = 0;

		/**
		 *	@return
		 */
		virtual Iterator &operator-(int n) = 0;

		/**
		 *	@return
		 */
		virtual Iterator &operator[](int index) const = 0;

		/**
		 *	@return
		 */
		virtual T &operator->() const { return (T) * this->iterator; }

		/**
		 *	@return
		 */
		virtual T &operator*() const { return *this->iterator; }

		/**
		 *	@return
		 */
		virtual T &operator*() { return *this->iterator; }

		/**
		 *	@return
		 */
		virtual inline bool operator==(const Iterator &iterator) noexcept {
			return this->iterator == iterator.iterator;
		}

		/**
		 *	@return
		 */
		virtual inline bool operator!=(const Iterator &iterator) noexcept {
			return this->iterator != iterator.iterator;
		}

		/**
		 *	@return
		 */
		virtual Iterator &operator=(const Iterator &iterator) {
			this->iterator = iterator.iterator;
			return *this;
		}
		/**
		 * @brief
		 *
		 * @param other
		 * @return Iterator&
		 */
		Iterator &operator=(Iterator &&other) {
			this->iterator = std::exchange(other.iterator, nullptr);
			return *this;
		}

	  public:		 /*	Protected attributes.	*/
		T *iterator; /**/
	};

} // namespace fragcore

#endif
