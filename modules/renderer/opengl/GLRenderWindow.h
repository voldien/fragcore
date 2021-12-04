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
#ifndef _FRAG_CORE_OPENGL_WINDOW_H_
#define _FRAG_CORE_OPENGL_WINDOW_H_ 1
#include "GLRendererInterface.h"
#include "../RendererWindow.h"
#if defined(FRAG_CORE_INTERNAL_IMP) // TODO resolve to a single file or
									// something later
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_video.h>
#endif

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC GLRenderWindow : public RendererWindow {
	  public:
		GLRenderWindow(Ref<GLRendererInterface> &renderer);
		~GLRenderWindow();

		void close() override;

		virtual void show();

		virtual void hide();

		void focus() override;

		void restore() override;

		void maximize() override;

		void minimize() override;

		virtual void swapBuffer();

		virtual void setPosition(int x, int y);

		virtual void setSize(int width, int height);

		virtual void vsync(bool state) override;

		virtual void getPosition(int *x, int *y) const;

		virtual void getSize(int *width, int *height) const;

		void resizable(bool resizable) override;

		void setIcon(void *pVoid) override;

		void *getIcon() const override;

		virtual Display *getCurrentDisplay() const;

		void setFullScreen(bool fullscreen) override;
		virtual void setFullScreen(Display &display);

		bool isFullScreen() const;

		void setBordered(bool bordered) override;

		void setMinimumSize(int width, int height) override;
		virtual void getMinimumSize(int *width, int *height);
		void setMaximumSize(int width, int height) override;
		virtual void getMaximumSize(int *width, int *height);

		virtual float getGamma() const;

		virtual void setGamma(float);

		virtual void setTitle(const std::string& title);
		virtual std::string getTitle() const;

		intptr_t getNativePtr() const override;

		virtual ViewPort *getViewPort() override;
		virtual FrameBuffer *getDefaultFrameBuffer() override;

		virtual void createWindow(int x, int y, int width, int height, const char *api) override;
		virtual bool assertConfigAttributes(const IConfig *iConfig) override;
		virtual void useWindow(void *pdata) override;

	  protected:
		virtual void createSwapChain();

	  private:
		SDL_Window *window;
		Ref<GLRendererInterface> renderer;
	};
} // namespace fragcore

#endif
