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
#ifndef _FRAG_CORE_CL_COMMAND_LIST_H_
#define _FRAG_CORE_CL_COMMAND_LIST_H_ 1
#include "../CommandList.h"
#include <vector>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC CLCommandList : public CommandList {

	  public:
		CLCommandList();
		virtual ~CLCommandList();

		virtual void begin() override;
		virtual void end() override;

		virtual void copyTexture(const Texture *src, Texture *dst) override;

		virtual void bindPipeline(RenderPipeline *p) override;
		virtual void bindComputePipeline(RenderPipeline *pipeline) override;

		virtual void bindFramebuffer(Ref<FrameBuffer> &framebuffer) override;
		virtual void setScissor(int x, int y, unsigned int width, unsigned int height) override;
		virtual void setViewport(int x, int y, unsigned int width, unsigned int height) override;

		virtual void clearDepth(float depth) override;
		virtual void clearColorTarget(uint index, const Color &color) override;

		virtual void dispatch(uint groupCountX, uint groupCountY, uint groupCountZ) override;
		virtual void dispatchIndirect(Buffer *buffer, u_int64_t offset) override;

		virtual void setDepthBounds(float min, float max) override;

		virtual void pushDebugGroup(const char *name) override;

		virtual void popDebugGroup() override;
		virtual void insertDebugMarker(const char *name) override;

		virtual void draw(Ref<Buffer> &buffer, uint32_t vertexCount, uint32_t instanceCount) override;
		// virtual void drawIndexed(Ref<Buffer>& buffer, offset,uint32_t drawCount,uint32_t stride) = 0;

	  private:
		std::vector<cl_command_queue> queues;
		cl_command_queue selectqueue;
	};
} // namespace fragcore

#endif
