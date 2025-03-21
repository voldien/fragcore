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
#ifndef _FRAGCORE_SDL2_WINDOW_MANAGER_H_
#define _FRAGCORE_SDL2_WINDOW_MANAGER_H_ 1
#include "../WindowManager.h"
#include "SDLDisplay.h"

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC SDLWindowManager : public WindowManager {
	  public:
		Display *getDisplay(unsigned int index) override;
		unsigned int getNumDisplays() const noexcept override;
		Display *getAssociatedDisplay(Ref<Window> &window) override;
		Window *createWindow(const std::string &title) override;
		Display *primaryDisplay() const noexcept override;
		const std::vector<Display *> &getDisplays() const override;

		SDLWindowManager();
		~SDLWindowManager() override;

	  private:
		std::vector<SDLDisplay> displays;
	};
} // namespace fragcore

#endif
