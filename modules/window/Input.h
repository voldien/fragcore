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
#ifndef _FRAG_CORE_INPUT_H_
#define _FRAG_CORE_INPUT_H_ 1
#include "Math3D/Math3D.h"
#include <Core/SmartReference.h>
#include <Math/Math.h>

namespace fragcore {

	/**
	 * @brief
	 */
	class FVDECLSPEC Input : public Object {
	  public:
		Input() = default;

	  public:
		virtual void update() noexcept = 0;

		enum class MouseButton : size_t {
			LEFT_BUTTON = 0,
			RIGHT_BUTTON = 1,
			MIDDLE_BUTTON = 2,
		};

		virtual bool anyKey() noexcept = 0;
		// virtual bool getKey() = 0;
		// virtual bool getKeyPressed() = 0;
		// virtual bool getKeyReleased() = 0;

		virtual bool getMousePosition(int *positionX, int *positionY) noexcept = 0;
		virtual Vector2 getMouseScroll() const noexcept = 0;

		virtual bool getMousePressed(const MouseButton button) noexcept = 0;
		virtual bool getMouseDown(const MouseButton button) noexcept = 0;
		virtual bool getMouseReleased(const MouseButton button) noexcept = 0;
	};
} // namespace fragcore

#endif