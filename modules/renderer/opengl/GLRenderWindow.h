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
#ifndef _FRAG_CORE_OPENGL_WINDOW_H_
#define _FRAG_CORE_OPENGL_WINDOW_H_ 1
#include "../RendererWindow.h"
#include "GLFrameBuffer.h"
#include "GLRendererInterface.h"
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
		virtual ~GLRenderWindow();

		void close() override;

		void show() override;

		void hide() override;

		void focus() override;

		void restore() override;

		void maximize() override;

		void minimize() override;

		void swapBuffer() override;

		void setPosition(int x, int y) override;

		void setSize(int width, int height) override;

		void vsync(bool state) override;

		void getPosition(int *x, int *y) const override;

		void getSize(int *width, int *height) const override;

		int width() const override;
		int height() const override;

		void resizable(bool resizable) override;

		void setIcon(void *pVoid) override;

		void *getIcon() const override;

		Display *getCurrentDisplay() const override;

		void setFullScreen(bool fullscreen) override;
		void setFullScreen(Display &display) override;

		bool isFullScreen() const override;

		void setBordered(bool bordered) override;

		void setMinimumSize(int width, int height) override;
		void getMinimumSize(int *width, int *height) override;
		void setMaximumSize(int width, int height) override;
		void getMaximumSize(int *width, int *height) override;

		float getGamma() const override;

		void setGamma(float gamma) override;

		void setTitle(const std::string &title) override;
		std::string getTitle() const override;

		intptr_t getNativePtr() const override;

	  public:
		ViewPort *getViewPort() override;
		FrameBuffer *getDefaultFrameBuffer() override;

		void createWindow(int x, int y, int width, int height, const char *api) override;
		bool assertConfigAttributes(const IConfig *iConfig) override;
		void useWindow(void *pdata) override;

	  protected: /*	*/
		virtual void createSwapChain();

	  private: /*	*/
		// TODO add proxy window.
		SDL_Window *window;
		Ref<GLRendererInterface> renderer;
		Ref<GLFrameBuffer> defaultFrameBuffer;
	};
} // namespace fragcore

#endif
