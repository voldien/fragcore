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
		enum class BlendFunc {
			Zero = 0, /*  */
			eOne = 1,  /*  */
			eSrcColor = 2,
			eOneMinusSrcColor = 3,
			eSrcAlpha = 4,
			eOneMinusSrcAlpha = 5,
			eConstantAlpha = 6,
		};

		enum BlendEqu {
			eNoEqu,
			eAddition,
			eSubtract,
			eReverseSubtract,
			eMin,
			eMax,
		};

		enum class StencilFunc {

		};

		enum class DepthFunc {

		};

		enum class BufferAttachment : int {
			eNoAttachment,
			eDepth,
			eStencil,
			eStencilDepth,
			eColor0,
		};

		virtual int attachmentCount();
		virtual std::vector<Texture *> getColorTargets();

		virtual void bind();

		virtual void unBind();

		virtual void write();

		virtual void read() const;

		/**
		 *	Get texture attached to framebuffer by index.
		 *
		 *	@Return Non null texture pointer if successfully.
		 */
		virtual Texture *getAttachment(unsigned int index);

		/**
		 *
		 * @return
		 */
		virtual Texture *getDepthAttachment();

		/**
		 *
		 * @return
		 */
		virtual Texture *getStencilAttachment();

		/**
		 *
		 */
		virtual int width() const;

		/**
		 *
		 */
		virtual int height() const;

		/**
		 *
		 */
		virtual int layers();

		virtual int nrSamples();

		/*  */
		virtual void blend(BlendEqu equ, BlendFunc sfactor, BlendFunc dfactor, BufferAttachment bufferAttachment);

		virtual void blendSeperate(BlendEqu equ, BlendFunc srcRGB, BlendFunc dstRGB, BlendFunc srcAlpha,
								   BlendFunc dstAlpha, BufferAttachment bufferAttachment);

		virtual void clear(unsigned int clear);

		virtual void clearColor(BufferAttachment colorAttachment, const float *color);

		virtual void clearDepthStencil(float depth, int stencil);

		/*  */
		virtual void setDraws(unsigned int nr, BufferAttachment *attachment);

		virtual void setDraw(BufferAttachment attachment);

		virtual intptr_t getNativePtr() const override;

		void setName(const std::string &name) override;
	};
} // namespace fragcore

#endif
