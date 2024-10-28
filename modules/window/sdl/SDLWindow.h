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
#ifndef _FRAGCORE_SDL_WINDOW_H_
#define _FRAGCORE_SDL_WINDOW_H_ 1
#include "../Display.h"
#include "../Window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <string>

namespace fragcore {

	class FVDECLSPEC SDLWindow : public Window {
	  public:
		SDLWindow();
		SDLWindow(intptr_t reference);
		~SDLWindow() override;

		void show() override;

		void hide() override;

		void close() override;

		void focus() override;

		void restore() override;

		void maximize() override;

		void minimize() override;

		void setTitle(const std::string &title) override;

		std::string getTitle() const override;

		int width() const override;
		int height() const override;

		void getPosition(int *x, int *y) const override;

		void setPosition(int x, int y) override;

		void setSize(int width, int height) override;

		void getSize(int *width, int *height) const override;

		void resizable(bool resizable) override;

		void setFullScreen(bool fullscreen) override;
		void setFullScreen(fragcore::Display &display) override;

		bool isFullScreen() const override;

		void setBordered(bool bordered) override;

		float getGamma() const override;

		void setGamma(float gamma) override;

		void setMinimumSize(int width, int height) override;
		void getMinimumSize(int *width, int *height) override;
		void setMaximumSize(int width, int height) override;
		void getMaximumSize(int *width, int *height) override;

		fragcore::Display *getCurrentDisplay() const override;

		intptr_t getNativePtr() const override; /*  Get native window reference object. */

	  protected:
		SDL_Window *window;
	};
} // namespace fragcore

#endif
