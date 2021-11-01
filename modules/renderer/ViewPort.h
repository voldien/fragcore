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
#ifndef _FRAG_CORE_VIEWPORT_H_
#define _FRAG_CORE_VIEWPORT_H_ 1
#include "IRenderer.h"
#include "RenderObject.h"

namespace fragcore {

	/**
	 * @brief Abstraction layar of the viewport.
	 *
	 */
	class FVDECLSPEC ViewPort : public RenderObject {
		friend class IRenderer;

	  public:
		virtual void depthRange(double near, double far);

		virtual void setDimensions(int x, int y, int width, int height);

		virtual void setscissorView(int x, int y, int width, int height);

		virtual void getViewPort(int *x, int *y, int *width, int *height);

		virtual void getScissorView(int *x, int *y, int *width, int *height);

		virtual void getDepthRange(double *near, double *far);

		virtual void enable(IRenderer::State state);

		virtual void disable(IRenderer::State state);

		virtual bool isStateEnabled(IRenderer::State state);

		/*	TODO add index.	*/

		intptr_t getNativePtr() const override;
	};
} // namespace fragcore

#endif