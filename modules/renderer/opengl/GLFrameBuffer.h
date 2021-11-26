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
#ifndef _FRAGCORE_OPENGL_FRAMEBUFFER_H_
#define _FRAGCORE_OPENGL_FRAMEBUFFER_H_ 1
#include "../IRenderer.h"
#include "../RenderObject.h"
#include <vector>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC GLFrameBuffer : public FrameBuffer {
		friend class GLRendererInterface;
	  public:
		virtual int attachmentCount() override;
		virtual std::vector<Texture *> getColorTargets() override;

		virtual void bind() override;

		virtual void unBind() override;

		virtual void write() override;

		virtual void read() const override;

		/**
		 *	Get texture attached to framebuffer by index.
		 *
		 *	@return Non null texture pointer if successfully.
		 */
		virtual Texture *getAttachment(unsigned int index) override;

		/**
		 *
		 * @return
		 */
		virtual Texture *getDepthAttachment() override;

		/**
		 *
		 * @return
		 */
		virtual Texture *getStencilAttachment() override;

		/**
		 *
		 */
		virtual int width() const override;

		/**
		 *
		 */
		virtual int height() const override;

		/**
		 *
		 */
		virtual int layers() override;

		virtual int nrSamples() override;

		/*  */
		virtual void blend(BlendEqu equ, BlendFunc sfactor, BlendFunc dfactor,
						   BufferAttachment bufferAttachment) override;

		virtual void blendSeperate(BlendEqu equ, BlendFunc srcRGB, BlendFunc dstRGB, BlendFunc srcAlpha,
								   BlendFunc dstAlpha, BufferAttachment bufferAttachment) override;

		virtual void clear(unsigned int clear) override;

		virtual void clearColor(BufferAttachment colorAttachment, const float *color) override;

		virtual void clearDepthStencil(float depth, int stencil) override;

		/*  */
		virtual void setDraws(unsigned int nr, BufferAttachment *attachment) override;

		virtual void setDraw(BufferAttachment attachment) override;

		virtual intptr_t getNativePtr() const override;

		virtual void setName(const std::string &name) override;

	  private:
        FrameBufferDesc desc;
		Texture *textures;
		unsigned int numtextures;
		unsigned int framebuffer;

	  public:
		GLFrameBuffer();
		virtual ~GLFrameBuffer() = default;
	};
} // namespace fragcore
#endif