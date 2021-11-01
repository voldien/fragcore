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
#ifndef _FRAG_ENGINE_RENDER_TARGET_FACTORY_H_
#define _FRAG_ENGINE_RENDER_TARGET_FACTORY_H_ 1
#include"IRenderer.h"
#include"FrameBuffer.h"

namespace fragcore {

   //TODO determine if shall be relocated to the engine
	/**
	 * @brief Responsible for creating predefined
	 * framebuffer target.
	 */
	class FVDECLSPEC RenderTargetFactory {
	public:

		static FrameBuffer *createShadow(IRenderer *render, int width, int height);

		static FrameBuffer *createDeffered(IRenderer *render, int width, int height, unsigned int options = 0);

		static FrameBuffer *createColor(IRenderer *render, int width, int height);

		static FrameBuffer *createHDR(IRenderer *renderer, int width, int height);

	private:    /*	Prevent one from creating an instance of this class.	*/

		RenderTargetFactory() = delete;
		RenderTargetFactory(const RenderTargetFactory &other) = delete;
		RenderTargetFactory(RenderTargetFactory &&other) = delete;
		~RenderTargetFactory() = delete;
	};
}


#endif
