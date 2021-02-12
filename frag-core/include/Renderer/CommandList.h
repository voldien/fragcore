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
#ifndef _FRAG_CORE_COMMAND_LIST_H_
#define _FRAG_CORE_COMMAND_LIST_H_ 1
#include"../Core/NoCopyable.h"
#include "../Core/Ref.h"
#include"IRenderer.h"


namespace fragcore {
	/**
	 *
	 */
	class CommandList : public SmartReference /*: public NoCopyable */{
	public:
	 enum CommandBufferFlag { Single, Continuous };
	 // virtual ~CommandList() = 0;
	 // virtual void bindPipeline(RenderPipeline *p);

	 virtual void begin(void) = 0;
	 virtual void end(void) = 0;

	 virtual void bindPipeline(RenderPipeline *p) = 0;
	 virtual void updateBuffer(Ref<Buffer> &buffer, void *p, int size) = 0;
	 virtual void bindFramebuffer(Ref<FrameBuffer> &framebuffer) = 0;

	 virtual void setviewport(int x, int y, int width, int height) = 0;
	 virtual void clearDepth(float depth) = 0;
	 virtual void clearColorTarget(uint index, const PVColor &color) = 0;

	 // virtual void setGraphicResourcesSlot(void) = 0;

	 // virtual void draw(void) = 0;
	 // virtual void draw(void) = 0;
	 // virtual void drawIndexed(void) = 0;
	 // virtual void drawIndirect(void) = 0;
	 // virtual void drawIndexedIndirect(void) = 0;
	 // TOOD add support for raytracing
	 //		virtual void generateBVH(void);
	 //		virtual void setupSecene(void);
	 //		virtual void

	 /**
	  *
	  */
	 virtual void dispatch(uint groupCountX, uint groupCountY, uint groupCountZ) = 0;

	 /**
	  *
	  */
	 virtual void dispatchIndirect(Buffer *buffer, u_int64_t offset) = 0;

	 /**
	  *
	  */
	 virtual void pushDebugGroup(const char *name) = 0;

	 /**
	  *
	  */
	 virtual void popDebugGroup(void) = 0;

	 /**
	  *
	  */
	 virtual void insertDebugMarker(const char *name) = 0;

	protected:
	 IRenderer *renderer;
	 ICompute *compute;
	 Features *features;
	 Capability *capability;
	};
}

#endif
