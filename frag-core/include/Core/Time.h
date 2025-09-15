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
#ifndef _FRAGCORE_TIME_H_
#define _FRAGCORE_TIME_H_ 1
#include "Object.h"
#include <chrono>

namespace fragcore {

	using namespace std::chrono;
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Time : public Object {
	  public:
		Time() = default;

		void start() noexcept {
			this->start_timestamp = steady_clock::now();
			this->ticks = steady_clock::now();
		}

		template <typename T> T getElapsed() const noexcept {
			static_assert(std::is_floating_point_v<T>, "Must be a decimal type(float/double/half).");
			const duration<T> time_span = duration_cast<duration<T>>(steady_clock::now() - start_timestamp);
			return time_span.count();
		}

		template <typename T> T deltaTime() const noexcept {
			static_assert(std::is_floating_point_v<T>, "Must be a decimal type(float/double/half).");
			return static_cast<T>(delta_data.count());
		}

		void update() noexcept {
			this->delta_data = duration_cast<duration<float>>(steady_clock::now() - this->ticks);
			this->ticks = steady_clock::now();
		}

		template <typename T> void update(const T &additional_time) {
			static_assert(std::is_floating_point_v<T>, "Must be a decimal type(float/double/half).");
			duration_cast<duration<T>>(additional_time);
		}

		size_t getTimeResolution() const noexcept {
			return static_cast<size_t>(1.0 / static_cast<double>(std::chrono::high_resolution_clock::period::num) /
									   static_cast<double>(std::chrono::high_resolution_clock::period::den));
		}

		template <typename T> T now() const noexcept {
			const auto t0 = steady_clock::now();
			return static_cast<T>(t0.time_since_epoch().count());
		}

		template <typename T> T nowHighRes() const noexcept {
			const auto t0 = std::chrono::high_resolution_clock::now();
			return static_cast<T>(t0.time_since_epoch().count());
		}

	  private: /*  */
		steady_clock::time_point start_timestamp;
		steady_clock::time_point ticks;
		duration<float> delta_data{};
	};
} // namespace fragcore

#endif
