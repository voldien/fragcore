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
#ifndef _FRAGCORE_GL_VIEWPORT_H_
#define _FRAGCORE_GL_VIEWPORT_H_ 1
#include "../ViewPort.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC GLViewPort : public ViewPort {
		friend class IRenderer;

	  public:
		void depthRange(double near, double far) override;

		void setDimensions(int x, int y, int width, int height) override;

		void setscissorView(int x, int y, int width, int height) override;

		void getViewPort(int *x, int *y, int *width, int *height) override;

		void getScissorView(int *x, int *y, int *width, int *height) override;

		void getDepthRange(double *near, double *far) override;

		void enable(IRenderer::State state) override;

		void disable(IRenderer::State state) override;

		bool isStateEnabled(IRenderer::State state) override;

		intptr_t getNativePtr() const override;

	  private:
		unsigned int viewport;
	};
} // namespace fragcore

#endif