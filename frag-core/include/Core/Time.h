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
#ifndef _FRAG_CORE_TIME_H_
#define _FRAG_CORE_TIME_H_ 1
#include "Object.h"
#include <chrono>

namespace fragcore {

	using namespace std::chrono;
	/**
	 * @brief
	 *
	 */
	class Time : public Object {
	  public:
		Time() {}

		void start() noexcept {
			this->start_timestamp = steady_clock::now();
			this->ticks = steady_clock::now();
		}

		template <typename T> T getElapsed() const noexcept {
			duration<T> time_span = duration_cast<duration<T>>(steady_clock::now() - start_timestamp);

			return time_span.count();
		}

		template <typename T> T deltaTime() const noexcept {
			static_assert(std::is_floating_point<T>::value, "Must be a decimal type(float/double/half).");
			duration<T> time_span = duration_cast<duration<T>>(steady_clock::now() - ticks);

			return static_cast<T>(delta_data.count());
		}

		void update() noexcept {

			this->delta_data = duration_cast<duration<float>>(steady_clock::now() - this->ticks);
			this->ticks = steady_clock::now();
		}

		size_t getTimeResolution() const noexcept {
			return static_cast<size_t>(1.0 / static_cast<double>(std::chrono::high_resolution_clock::period::num) /
									   static_cast<double>(std::chrono::high_resolution_clock::period::den));
		}

	  private: /*  */
		steady_clock::time_point start_timestamp;
		steady_clock::time_point ticks;
		duration<float> delta_data;
	};
} // namespace fragcore

#endif
