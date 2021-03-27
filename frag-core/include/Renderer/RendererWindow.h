/**
	FragEngine, A Two layer Game Engine.
    Copyright (C) 2018  Valdemar Lindberg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_RENDERWINDOW_H_
#define _FRAG_CORE_RENDERWINDOW_H_ 1
#include "../Def.h"
#include "../Window/Display.h"
#include "../Core/Ref.h"
#include "../Prerequisites.h"
#include "../Core/SmartReference.h"
#include "../Window/Window.h"
#include "IRenderer.h"
#include"RenderObject.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC RendererWindow : public Window {
		friend class IRenderer;
	public:
		virtual void swapBuffer(void) = 0;
		virtual void vsync(bool state) = 0;
		virtual ViewPort* getViewPort(void) = 0;
		virtual FrameBuffer* getDefaultFrameBuffer() = 0;

		//virtual const char* getRendererAPI(void);
	protected:  /*  Renderer methods.   */
		//TODO determine how to resolve and
		virtual bool assertConfigAttributes(const IConfig *iConfig) = 0;
		virtual void createWindow(int x, int y, int width, int height, const char *api) = 0;
		virtual void useWindow(void *data) = 0;

	protected:  /*  */
		const char *api;
		Ref<IRenderer> renderer;
	};

}

#endif
