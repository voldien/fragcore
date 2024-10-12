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
#ifndef _FRAG_CORE_GLFW_WINDOW_H_
#define _FRAG_CORE_GLFW_WINDOW_H_ 1
#include "../Window.h"
#include <GLFW/glfw3.h>
#include <string>

namespace fragcore {

	class FVDECLSPEC GLFWWindow : public Window {
	  public:
		GLFWWindow();
		~GLFWWindow() override;

		void show() override;

		void hide() override;

		void close() override;

		void focus() override;

		void restore() override;

		void maximize() override;

		void minimize() override;

		void setTitle(const std::string &title) override;

		std::string getTitle() const override;

		// virtual int x() const noexcept override;
		// virtual int y() const noexcept override;

		// virtual int width() const override;
		// virtual int height() const override;

		void getPosition(int *x, int *y) const override;

		void setPosition(int x, int y) noexcept override;

		void setSize(int width, int height) noexcept override;

		void getSize(int *width, int *height) const override;

		void resizable(bool resizable) noexcept override;

		void setFullScreen(bool fullscreen) override;

		bool isFullScreen() const override;

		void setBordered(bool borded) override;

		float getGamma() const override;

		void setGamma(float gamma) override;

		void setMinimumSize(int width, int height) override;
		void getMinimumSize(int *width, int *height) override;
		void setMaximumSize(int width, int height) override;
		void getMaximumSize(int *width, int *height) override;

		intptr_t getNativePtr() const override; /*  Get native window reference object. */

	  protected:
		std::string title;
		GLFWwindow *window;
	};
} // namespace fragcore

#endif
