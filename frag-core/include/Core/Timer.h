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
#ifndef _FRAG_CORE_TIMER_H_
#define _FRAG_CORE_TIMER_H_ 1
#include "Object.h"
#include <SDL2/SDL_timer.h>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class Time : public Object {
	  public:
		Time() {
			this->_private_level_startup = SDL_GetPerformanceCounter();

			this->timeResolution = SDL_GetPerformanceFrequency();
		}

		void start() { this->ticks = SDL_GetPerformanceCounter(); }
		template <typename T> T getElapsed() const noexcept {
			return static_cast<T>(SDL_GetPerformanceCounter() - this->_private_level_startup) /
				   static_cast<T>(this->timeResolution);
		}
		template <typename T> T deltaTime() const noexcept {
			return static_cast<T>(delta_data) / static_cast<T>(this->timeResolution);
		}
		void update() {
			delta_data = SDL_GetPerformanceCounter() - ticks;
			ticks = SDL_GetPerformanceCounter();
		}

	  private: /*  */
		long int ticks;
		float scale;
		float fixed;

		/*	TODO clean up later by relocating it to the time class.*/
		float gc_fdelta;
		float delta_data;
		// unsigned int nDeltaTime = sizeof(delta_data) / sizeof(delta_data[0]);
		unsigned int idelta;

		/*  */

		unsigned long timeResolution;
		unsigned long _private_level_startup;
	};
} // namespace fragcore

#endif
