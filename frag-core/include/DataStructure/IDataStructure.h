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
#pragma once

#include <cstddef>
#include <type_traits>

namespace fragcore {

	template <typename T> class IDataStructure {
	  protected:
		static constexpr bool isClass = std::is_class_v<T>;
		using DType = T;
		static constexpr size_t DTypeSize = sizeof(DType);

	  public:
		void clear() noexcept {}

		bool isEmpty() const noexcept { return false; }

		size_t getSize() const noexcept { return 0; }

		size_t getReserved() const noexcept { return 0; }

		constexpr size_t getTypeSize() const noexcept { return 0; }

		T *getData() const noexcept { return nullptr; }
	};
} // namespace fragcore
