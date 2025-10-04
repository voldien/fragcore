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
#ifndef _FRAGCORE_BITWISE_H_
#define _FRAGCORE_BITWISE_H_ 1
#include "FragDef.h"
#include <cfloat>
#include <cmath>

namespace fragcore {

	class FVDECLSPEC Bitwise {
	  public:
		/*	*/
		template <typename T> static constexpr T removeFlag(const T value, const T flag) noexcept {
			static_assert(std::is_integral_v<T> || std::is_enum_v<T>, "Must be a integer or enum type.");
			return static_cast<T>(static_cast<size_t>(value) & static_cast<size_t>(~flag));
		}

		/*	*/
		template <typename T> static constexpr T addFlag(const T value, const T flag) noexcept {
			static_assert(std::is_integral_v<T> || std::is_enum_v<T>, "Must be a integer or enum type.");
			return static_cast<T>(static_cast<size_t>(value) | static_cast<size_t>(flag));
		}

		/*	*/
		template <typename T> static constexpr bool isFlagSet(const T value, const T flag) noexcept {
			static_assert(std::is_integral_v<T> || std::is_enum_v<T>, "Must be a integer or enum type.");
			return static_cast<T>((static_cast<size_t>(value) & static_cast<size_t>(flag))) == flag;
		}
	};

} // namespace fragcore

#endif
