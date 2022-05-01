/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAG_CORE_COMMAND_LIST_H_
#define _FRAG_CORE_COMMAND_LIST_H_ 1
#include "IRenderer.h"
#include <Core/Color.h>
#include <Core/NoCopyable.h>
#include <Core/Ref.h>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC CommandList : public SmartReference {
	  public:
		enum CommandBufferFlag { Single, Continuous };
		virtual ~CommandList() = default;

		virtual void begin() = 0;
		virtual void end() = 0;

		virtual void copyTexture(const Texture *src, Texture *dst) = 0;

		virtual void bindPipeline(RenderPipeline *pipline) = 0;
		virtual void bindComputePipeline(RenderPipeline *pipeline) = 0;

		// virtual void updateBuffer(Ref<Buffer> &buffer, void *p, int size) = 0;
		virtual void bindFramebuffer(Ref<FrameBuffer> &framebuffer) = 0;

		virtual void setViewport(int x, int y, unsigned int width, unsigned int height) = 0;
		virtual void setScissor(int x, int y, unsigned int width, unsigned int height) = 0;
		virtual void clearDepth(float depth) = 0;
		virtual void clearColorTarget(uint index, const Color &color) = 0;

		virtual void setDepthBounds(float min, float max) = 0;

		// virtual void executeCommand(CommandList &list);

		// virtual void setGraphicResourcesSlot() = 0;

		virtual void draw(Ref<Buffer> &buffer, uint32_t vertexCount, uint32_t instanceCount) = 0;
		// virtual void drawIndexed(Ref<Buffer>& buffer, offset,uint32_t drawCount,uint32_t stride) = 0;
		// virtual void drawIndirect() = 0;
		// virtual void drawIndexedIndirect(Ref<Buffer>& buffer) = 0;

		// TOOD add support for raytracing
		//		virtual void generateBVH();
		//		virtual void setupSecene();
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
		virtual void popDebugGroup() = 0;

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
} // namespace fragcore

#endif
