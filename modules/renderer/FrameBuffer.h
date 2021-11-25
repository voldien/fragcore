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
#ifndef _FRAGCORE_FRAMEBUFFER_H_
#define _FRAGCORE_FRAMEBUFFER_H_ 1
#include "IRenderer.h"
#include "RenderObject.h"
#include <vector>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC FrameBuffer : public RenderObject {
	  public:
		FrameBuffer() = default;
		virtual ~FrameBuffer() = default;

		/**
		 * @brief
		 *
		 */
		enum class BlendFunc {
			Zero = 0, /*  */
			eOne = 1, /*  */
			eSrcColor = 2,
			eOneMinusSrcColor = 3,
			eSrcAlpha = 4,
			eOneMinusSrcAlpha = 5,
			eConstantAlpha = 6,
		};

		/**
		 * @brief
		 *
		 */
		enum class BlendEqu {
			eNoEqu,
			Addition,
			Subtract,
			ReverseSubtract,
			Min,
			Max,
		};

		enum class StencilFunc {

		};

		enum class DepthFunc {

		};

		enum class BufferAttachment : int {
			NoAttachment,
			Depth,
			Stencil,
			StencilDepth,
			Color0,
		};

		virtual int attachmentCount() = 0;
		virtual std::vector<Texture *> getColorTargets() = 0;

		virtual void bind() = 0;

		virtual void unBind() = 0;

		virtual void write() = 0;

		virtual void read() const = 0;

		/**
		 *	Get texture attached to framebuffer by index.
		 *
		 *	@Return Non null texture pointer if successfully.
		 */
		virtual Texture *getAttachment(unsigned int index) = 0;

		/**
		 *
		 * @return
		 */
		virtual Texture *getDepthAttachment() = 0;

		/**
		 *
		 * @return
		 */
		virtual Texture *getStencilAttachment() = 0;

		/**
		 *
		 */
		virtual int width() const = 0;

		/**
		 *
		 */
		virtual int height() const = 0;

		/**
		 *
		 */
		virtual int layers() = 0;

		virtual int nrSamples() = 0;

		/*  */
		virtual void blend(BlendEqu equ, BlendFunc sfactor, BlendFunc dfactor, BufferAttachment bufferAttachment) = 0;

		virtual void blendSeperate(BlendEqu equ, BlendFunc srcRGB, BlendFunc dstRGB, BlendFunc srcAlpha,
								   BlendFunc dstAlpha, BufferAttachment bufferAttachment) = 0;

		virtual void clear(unsigned int clear) = 0;

		virtual void clearColor(BufferAttachment colorAttachment, const float *color) = 0;

		virtual void clearDepthStencil(float depth, int stencil) = 0;

		/*  */
		virtual void setDraws(unsigned int nr, BufferAttachment *attachment) = 0;

		virtual void setDraw(BufferAttachment attachment) = 0;
	};
} // namespace fragcore

#endif
