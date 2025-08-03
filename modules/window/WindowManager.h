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
#ifndef _FRAGCORE_WINDOW_MANAGER_H_
#define _FRAGCORE_WINDOW_MANAGER_H_ 1
#include "Display.h"
#include "Window.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC WindowManager : public SmartReference {
	  public:
		/**
		 * @brief Get the Display object
		 */
		virtual Display *getDisplay(unsigned int index) = 0;

		/**
		 * @brief Get the Num Displays object
		 */
		virtual unsigned int getNumDisplays() const noexcept = 0;

		/**
		 * @brief Get the Associated Display object
		 */
		virtual Display *getAssociatedDisplay(Ref<Window> &window) = 0;

		/**
		 * @brief Create a Window object
		 */
		virtual Window *createWindow(const std::string &title) = 0;

		/**
		 * @brief
		 */
		virtual Display *primaryDisplay() const noexcept = 0;

		/**
		 * @brief
		 */
		virtual const std::vector<Display *> &getDisplays() const = 0;

		/*	TODO get the event.	*/

	  protected:
		WindowManager() = default;
	};
} // namespace fragcore

#endif
