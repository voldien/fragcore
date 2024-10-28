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

#ifndef _FRAGCORE_RENDERWINDOW_H_
#define _FRAGCORE_RENDERWINDOW_H_ 1
#include "IRenderer.h"
#include "RenderObject.h"
#include "SDLWindow.h"
#include <Core/Ref.h>
#include <Core/SmartReference.h>
#include <Display.h>
#include <FragDef.h>
#include <Prerequisites.h>
#include <Window.h>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC RendererWindow : public SDLWindow {
		friend class IRenderer;

	  public:
		RendererWindow() : SDLWindow(0){};

	  public:
		virtual void swapBuffer() = 0;
		virtual void vsync(bool state) = 0;
		virtual ViewPort *getViewPort() = 0;
		virtual FrameBuffer *getDefaultFrameBuffer() = 0;

	  protected: /*  Renderer methods.   */
		// TODO determine how to resolve and
		virtual bool assertConfigAttributes(const IConfig *iConfig) = 0;
		virtual void createWindow(int x, int y, int width, int height, const char *api) = 0;
		virtual void useWindow(void *data) = 0;

	  protected: /*  */
		const char *api = nullptr;
		Ref<IRenderer> renderer = {nullptr};
	};

} // namespace fragcore

#endif
