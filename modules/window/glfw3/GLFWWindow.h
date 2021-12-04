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
#ifndef _FRAG_CORE_GLFW_WINDOW_H_
#define _FRAG_CORE_GLFW_WINDOW_H_ 1
#include <GLFW/glfw3.h>
#include <Window/Window.h>
#include <string>

namespace fragcore {

	class FVDECLSPEC GLFWWindow : public Window {
	  public:
		GLFWWindow();
		virtual ~GLFWWindow();

		virtual void show() override;

		virtual void hide() override;

		virtual void close() override;

		virtual void focus() override;

		virtual void restore() override;

		virtual void maximize() override;

		virtual void minimize() override;

		virtual void setTitle(const std::string &title) override;

		virtual std::string getTitle() const override;

		// virtual int x() const noexcept override;
		// virtual int y() const noexcept override;

		// virtual int width() const override;
		// virtual int height() const override;

		virtual void getPosition(int *x, int *y) const override;

		virtual void setPosition(int x, int y) noexcept override;

		virtual void setSize(int width, int height) noexcept override;

		virtual void getSize(int *width, int *height) const override;

		virtual void resizable(bool resizable) noexcept;

		virtual void setFullScreen(bool fullscreen);

		virtual bool isFullScreen() const;

		virtual void setBordered(bool borded);

		virtual float getGamma() const override;

		virtual void setGamma(float gamma) override;

		virtual void setMinimumSize(int width, int height) override;
		virtual void getMinimumSize(int *width, int *height) override;
		virtual void setMaximumSize(int width, int height) override;
		virtual void getMaximumSize(int *width, int *height) override;

		virtual intptr_t getNativePtr() const override; /*  Get native window reference object. */

	  protected:
		std::string title;
		GLFWwindow *window;
	};
} // namespace fragcore

#endif
