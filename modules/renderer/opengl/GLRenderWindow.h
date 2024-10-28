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
#ifndef _FRAGCORE_OPENGL_WINDOW_H_
#define _FRAGCORE_OPENGL_WINDOW_H_ 1
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
		~GLRenderWindow() override;

		void swapBuffer() override;
		void vsync(bool state) override;

		ViewPort *getViewPort() override;
		FrameBuffer *getDefaultFrameBuffer() override;

		void createWindow(int x, int y, int width, int height, const char *api) override;
		bool assertConfigAttributes(const IConfig *iConfig) override;
		void useWindow(void *pdata) override;

	  protected: /*	*/
		virtual void createSwapChain();

	  private: /*	*/
		// TODO add proxy window.
		Ref<GLRendererInterface> renderer = {nullptr};
		Ref<GLFrameBuffer> defaultFrameBuffer = {nullptr};
	};
} // namespace fragcore

#endif
