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
#ifndef _FRAG_CORE_WINDOW_MANAGER_H_
#define _FRAG_CORE_WINDOW_MANAGER_H_ 1
#include "../Core/Ref.h"
#include "../Core/Singleton.h"
#include "Display.h"
#include "Window.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC WindowManager : public Singleton<WindowManager> {
	  public:
		/**
		 * @brief Get the Display object
		 *
		 * @param index
		 * @return Display*
		 */
		Display *getDisplay(unsigned int index) noexcept(true);

		/**
		 * @brief Get the Num Displays object
		 *
		 * @return unsigned int
		 */
		unsigned int getNumDisplays() const noexcept;

		/**
		 * @brief Get the Associated Display object
		 *
		 * @param window
		 * @return Display*
		 */
		Display *getAssociatedDisplay(Ref<Window> &window);

		/**
		 * @brief Create a Window object
		 *
		 * @param title
		 * @return Window*
		 */
		Window *createWindow(const std::string &title);

		/**
		 * @brief
		 *
		 * @return Display*
		 */
		Display *primaryDisplay() const noexcept;
		// Window* createWindow()
		/*	TODO get the event.	*/

		// protected:
		void onCreation(Ref<WindowManager> &instance) override;
		WindowManager();
	};
} // namespace fragcore

#endif
