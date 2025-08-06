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
#ifndef _FRAGCORE_OPENGL_WINDOW_H_
#define _FRAGCORE_OPENGL_WINDOW_H_ 1
#include "../RendererWindow.h"
#include "GLRendererInterface.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC GLRenderWindow : public RendererWindow {
	  public:
		GLRenderWindow(const Ref<GLRendererInterface> &renderer);
		~GLRenderWindow() override;

		/*	*/
		void swapBuffer() override;
		void vsync(bool state) override;

		/*	*/
		void createWindow(int x, int y, int width, int height, const char *api) override;
		bool assertConfigAttributes(const IConfig *iConfig) override;
		void useWindow(void *pdata) override;

	  protected: /*	*/
		virtual void createSwapChain();

	  private: /*	*/
		Ref<GLRendererInterface> renderer = {nullptr};
	};
} // namespace fragcore

#endif
