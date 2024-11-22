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
#ifndef _FRAGCORE_SDL_INPUT_H_
#define _FRAGCORE_SDL_INPUT_H_ 1
#include "../Input.h"
#include <Core/SmartReference.h>
#include <Math/Math.h>
#include <bitset>

namespace fragcore {

	/**
	 * @brief
	 */
	class FVDECLSPEC SDLInput : public Input {
	  public:
		SDLInput() = default;

		void update() noexcept override;

		bool anyKey() noexcept override;
		bool getKey(const unsigned int key) override;
		bool getKeyPressed(const unsigned int key_down) override;
		bool getKeyReleased(const unsigned int key_released) override;

		bool getMousePosition(int *positionX, int *positionY) noexcept override;
		Vector2 getMouseScroll() const noexcept override;

		bool getMousePressed(const MouseButton button) noexcept override;
		bool getMouseDown(const MouseButton button) noexcept override;
		bool getMouseReleased(const MouseButton button) noexcept override;

	  private:
		std::bitset<3> mousePressed = {0};
		std::bitset<3> mouseDown = {0};
		std::bitset<3> mouseReleased = {0};
		int x{}, y{};
	};
} // namespace fragcore

#endif