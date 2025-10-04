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
#ifndef _FRAGCORE_UID_STRUCTURE_H_
#define _FRAGCORE_UID_STRUCTURE_H_ 1
#include <cstddef>
#include <type_traits>

namespace fragcore {

	/**
	 * @brief
	 */
	template <typename T = size_t> struct UIDStruct {
		static_assert(std::is_integral_v<T> || std::is_enum_v<T>, "Must be a integer type.");

	  public:
		/*	*/
		using UIDType = T;
		UIDStruct(const T uid) noexcept : uid(uid) {}

		UIDType get_id() const noexcept { return this->uid; }

	  private:
		UIDType uid;
	};
} // namespace fragcore

#endif