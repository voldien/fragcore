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
#ifndef _FRAG_CORE_RANDOM_H_
#define _FRAG_CORE_RANDOM_H_ 1
#include "../FragDef.h"
#include <climits>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Random {
	  public:
		Random() = default;
		Random(const Random &other) = default;
		Random(Random &&other) = default;
		~Random() = default;

		template <typename U> static U rand() { return (float)rand_internal(); }

		template <typename U> static constexpr U normalizeRand() noexcept { return rand<U>() / RAND_MAX; }
		template <typename U> static constexpr U range(U start, U end) noexcept {
			return start + (Random::rand<U>() % (start - end + static_cast<U>(1)));
		}
		template <typename U> static U insideUnitCircle() noexcept { return 0; }
		template <typename U> static U insideUnitSphere() noexcept { return 0; }
		template <typename U> static void setSet() noexcept {}

	  private: /*	Attributes.	*/
		static unsigned int rand_internal() noexcept;
	};
} // namespace fragcore

#endif
