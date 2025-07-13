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
#ifndef _FRAGCORE_VIEWPORT_H_
#define _FRAGCORE_VIEWPORT_H_ 1
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
		virtual void depthRange(double near, double far) = 0;

		virtual void setDimensions(int x, int y, int width, int height) = 0;

		virtual void setscissorView(int x, int y, int width, int height) = 0;

		virtual void getViewPort(int *x, int *y, int *width, int *height) = 0;

		virtual void getScissorView(int *x, int *y, int *width, int *height) = 0;

		virtual void getDepthRange(double *near, double *far) = 0;

		virtual void enable(IRenderer::State state) = 0;

		virtual void disable(IRenderer::State state) = 0;

		virtual bool isStateEnabled(IRenderer::State state) = 0;
	};
} // namespace fragcore

#endif
