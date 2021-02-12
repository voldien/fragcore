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
#ifndef _FRAGVIEW_FRAMEBUFFER_H_
#define _FRAGVIEW_FRAMEBUFFER_H_ 1
#include"RenderObject.h"
#include "IRenderer.h"
#include<vector>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC FrameBuffer : public RenderObject {
	public:

		enum BlendFunc {
			eZero = 0,    /*  */
			eOne = 1,    /*  */
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

		enum StencilFunc {

		};

		enum DepthFunc {

		};

		enum BufferAttachment {
			eNoAttachment,
			eDepth,
			eStencil,
			eStencilDepth,
			eColor0,
		};

		virtual int attachmentCount();
		virtual std::vector<Texture *> getColorTargets(void);

		virtual void bind(void);

		virtual void unBind(void);

		virtual void write(void);

		virtual void read(void);

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
		virtual Texture *getDepthAttachment(void);

		/**
		 *
		 * @return
		 */
		virtual Texture *getStencilAttachment(void);

		/**
		 *
		 */
		virtual int width(void);

		/**
		 *
		 */
		virtual int height(void);

		/**
		 *
		 */
		virtual int layers(void);

		virtual int nrSamples(void);

		/*  */
		virtual void blend(BlendEqu equ, BlendFunc sfactor, BlendFunc dfactor, BufferAttachment bufferAttachment);

		virtual void blendSeperate(BlendEqu equ, BlendFunc srcRGB,
		                           BlendFunc dstRGB,
		                           BlendFunc srcAlpha,
		                           BlendFunc dstAlpha, BufferAttachment bufferAttachment);


		virtual void clear(unsigned int clear);

		virtual void clearColor(BufferAttachment colorAttachment, const float *color);

		virtual void clearDepthStencil(float depth, int stencil);

		/*  */
		virtual void setDraws(unsigned int nr, BufferAttachment *attachment);

		virtual void setDraw(BufferAttachment attachment);

		virtual intptr_t getNativePtr(void) const;

		void setName(const std::string &name) override;
	};
}

#endif
