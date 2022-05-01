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
#ifndef _FRAG_CORE_UID_GENERATOR_H_
#define _FRAG_CORE_UID_GENERATOR_H_ 1
#include "../FragDef.h"

namespace fragcore {
	/**
	 *	@brief Unique Identifier generator.
	 *	This is a simple UID generator.
	 *	Each next will be an increment of previosuly.
	 */
	template <class T> class UIDGenerator {
	  public:
		static_assert(std::is_arithmetic<T>::value, "Must support artithmetic operations");
		UIDGenerator() noexcept { this->nextUID = 0; }
		UIDGenerator(UIDGenerator &&other) = default;
		UIDGenerator(const UIDGenerator &other) = default;

		UIDGenerator &operator=(const UIDGenerator &other) = default;
		UIDGenerator &operator=(UIDGenerator &&other) = default;

		/**
		 * @brief Get the Next U I D object
		 *
		 * @return next unique id.
		 */
		T getNextUID() noexcept { return this->nextUID++; }

		bool operator==(const UIDGenerator &other) noexcept {
			if (this == &other)
				return true;
			else
				return this->uid == other.uid;
		}
		bool operator!=(const UIDGenerator &other) noexcept { return !(*this == other); }

	  private:	   /*	Attributes.	*/
		T nextUID; /*	*/
		T uid;	   // TODO resolve.
	};

} // namespace fragcore

#endif
