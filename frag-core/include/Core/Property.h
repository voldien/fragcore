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

#ifndef _FRAGCORE_PROPERTY_H_
#define _FRAGCORE_PROPERTY_H_ 1
#include "Core/UIDStruct.h"
#include "Math/Math.h"

#include <cstddef>
#include <string>
#include <typeinfo>

namespace fragcore {

	/**
	 * @brief
	 *
	 * @tparam T
	 * @tparam U
	 */
	template <typename T, typename U = size_t> struct Property : public UIDStruct<U> {
	  public:
		using DType = T;

		Property() noexcept : UIDStruct<U>(reinterpret_cast<U>(this)) {
			this->value = T();
			this->overrideValue = T();
			this->override_status = false;
		}

		Property(const std::string &name, const T value) noexcept
			: UIDStruct<U>(reinterpret_cast<U>(this)), name(name), value(value), overrideValue(value) { /*	*/ }
		Property(const std::string &name, const T value, U uid) noexcept
			: UIDStruct<U>(uid), name(name), value(value), overrideValue(value) { /**/ }

		constexpr const DType *operator->() const { return std::__addressof(this->get_current_value()); }
		constexpr DType *operator->() { return std::__addressof(this->get_current_value()); }

		operator DType() const noexcept { return this->get_current_value(); }
		// explicit conversion
		explicit operator const DType() const noexcept { return this->value; }

		operator DType &() noexcept { return this->get_current_value(); }
		// explicit conversion
		explicit operator const DType &() const noexcept { return this->get_current_value(); }

		constexpr const DType &operator*() const & { return this->get_current_value(); }

		constexpr DType &operator*() & { return this->get_current_value(); }

		constexpr DType &&operator*() && { return std::move(this->get_current_value()); }

		constexpr const DType &&operator*() const && { return std::move(this->get_current_value()); }

		auto &operator=(const DType &other) noexcept {
			this->value = other;
			return *this;
		}

		const std::string &get_name() const noexcept { return this->name; }

		constexpr const DType &get_value() const noexcept { return this->get_current_value(); }
		constexpr DType &get_value() noexcept { return this->get_current_value(); }
		void set_value(const DType newValue) {
			if (this->is_override()) {
				this->overrideValue = newValue;
			} else {
				this->value = newValue;
			}
		}

		const std::type_info &getDType() const noexcept { return typeid(T); }

		void override(const bool status) noexcept { this->override_status = status; }
		void set_override_value(const DType &value) { this->overrideValue = value; }
		void set_none_override_value(const DType &value) { this->value = value; }
		bool is_override() const noexcept { return this->override_status; }

	  protected:
		const DType &get_current_value() const { return this->is_override() ? this->overrideValue : this->value; }
		DType &get_current_value() { return this->is_override() ? this->overrideValue : this->value; }

		std::string name;
		DType value;
		DType overrideValue;
		bool override_status = false;
	};

	/**
	 * @brief
	 *
	 * @tparam T
	 */
	template <typename T> struct PropertyMinMaxDefault : public Property<T> {
	  public:
		PropertyMinMaxDefault(const std::string &name, const T &value, const T min, const T max)
			: Property<T>(name, value), min(min), max(max) {
			this->value = Math::clamp<T>(value, min, max);
		}

		operator T() const noexcept { return this->value; }
		// explicit conversion
		explicit operator const T() const noexcept { return this->value; }

		auto &operator=(const T &other) noexcept {
			this->value = Math::clamp<T>(other, min, max);
			return *this;
		}

	  protected:
		T min;
		T max;
	};
} // namespace fragcore

#endif