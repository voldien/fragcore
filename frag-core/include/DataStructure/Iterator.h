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
#ifndef _FRAGCORE_ITERATOR_H_
#define _FRAGCORE_ITERATOR_H_ 1
#include "../FragDef.h"

namespace fragcore {

	/**
	 * @brief Simple abstract Iterator class for
	 * generic classes.
	 * @tparam T
	 */
	template <typename T> class FVDECLSPEC Iterator {
	  public:
		/**
		 *	@return
		 */
		virtual Iterator &operator++() = 0;

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
		virtual T &operator->() const { return reinterpret_cast<T &>(*this->iterator); }

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

	  protected:	 /*	Protected attributes.	*/
		T *iterator; /**/
	};

} // namespace fragcore

#endif
