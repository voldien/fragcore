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
#ifndef _FRAG_CORE_SDL_WINDOW_MANAGER_H_
#define _FRAG_CORE_SDL_WINDOW_MANAGER_H_ 1
#include "../WindowManager.h"

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC SDLWindowManager : public WindowManager {
	  public:
		virtual Display *getDisplay(unsigned int index) override;
		virtual unsigned int getNumDisplays() const noexcept override;
		virtual Display *getAssociatedDisplay(Ref<Window> &window) override;
		virtual Window *createWindow(const std::string &title) override;
		virtual Display *primaryDisplay() const noexcept override;

		/*	TODO get the event.	*/

		// protected:

		SDLWindowManager();
		virtual ~SDLWindowManager();
	};
} // namespace fragcore

#endif
