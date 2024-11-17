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
#ifndef _FRAGCORE_OPENGL_FRAMEBUFFER_H_
#define _FRAGCORE_OPENGL_FRAMEBUFFER_H_ 1
#include "../IRenderer.h"
#include <vector>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC GLFrameBuffer : public FrameBuffer {
		friend class GLRendererInterface;

	  public:
		int attachmentCount() override;
		std::vector<Texture *> getColorTargets() override;

		void bind() override;

		void unBind() override;

		void write() override;

		void read() const override;

		/**
		 *	Get texture attached to framebuffer by index.
		 *
		 *	@return Non null texture pointer if successfully.
		 */
		Texture *getAttachment(unsigned int index) override;

		/**
		 *
		 * @return
		 */
		Texture *getDepthAttachment() override;

		/**
		 *
		 * @return
		 */
		Texture *getStencilAttachment() override;

		/**
		 *
		 */
		int width() const override;

		/**
		 *
		 */
		int height() const override;

		/**
		 *
		 */
		int layers() override;

		int nrSamples() override;

		/*  */
		void blend(BlendEqu equ, BlendFunc sfactor, BlendFunc dfactor, BufferAttachment bufferAttachment) override;

		void blendSeperate(BlendEqu equ, BlendFunc srcRGB, BlendFunc dstRGB, BlendFunc srcAlpha, BlendFunc dstAlpha,
						   BufferAttachment bufferAttachment) override;

		void clear(unsigned int clear) override;

		void clearColor(BufferAttachment colorAttachment, const float *color) override;

		void clearDepthStencil(float depth, int stencil) override;

		/*  */
		void setDraws(unsigned int nr, BufferAttachment *attachment) override;

		void setDraw(BufferAttachment attachment) override;

		intptr_t getNativePtr() const override;

		void setName(const std::string &name) override;

	  private:
		FrameBufferDesc desc;
		Texture *textures;
		unsigned int numtextures;
		unsigned int framebuffer;

	  public:
		GLFrameBuffer();
		~GLFrameBuffer() override = default;
	};
} // namespace fragcore
#endif
