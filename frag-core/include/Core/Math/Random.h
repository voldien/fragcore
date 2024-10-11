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
#ifndef _FRAG_CORE_RANDOM_H_
#define _FRAG_CORE_RANDOM_H_ 1
#include "../../FragDef.h"
#include "NormalDistribution.h"
#include <random>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Random {
	  public:
		template <typename U> static U rand() {
			static_assert(std::is_floating_point<U>::value || std::is_integral<U>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return static_cast<U>(rand_internal());
		}

		template <typename U> static constexpr U normalizeRand() noexcept {
			static_assert(std::is_floating_point<U>::value || std::is_integral<U>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return rand<U>() / static_cast<U>(RAND_MAX);
		}
		template <typename U> static constexpr U range(const U start, const U end) noexcept {
			static_assert(std::is_floating_point<U>::value || std::is_integral<U>::value,
						  "Must be a decimal type(float/double/half) or integer.");
			return start + (Random::rand<U>() % (start - end));
		}

		template <typename U> static U insideUnitCircle() noexcept { return 0; }
		template <typename U> static U insideUnitSphere() noexcept { return 0; }
		template <typename U> static void setSet() noexcept {}

	  private: /*	Attributes.	*/
		static unsigned int rand_internal() noexcept;

	  public:
		Random() = delete;
		Random(const Random &other) = delete;
		Random(Random &&other) = delete;
		~Random() = delete;
	};

} // namespace fragcore

#endif
