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
#ifndef _FRAG_CORE_GL_COMMAND_LIST_H_
#define _FRAG_CORE_GL_COMMAND_LIST_H_ 1
#include "../CommandList.h"
#include "GLCommandListCommands.h"
#include <Core/dataStructure/StackAllactor.h>
#include <vector>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC GLCommandList : public CommandList {
		friend class GLRendererInterface;

	  public:
		GLCommandList();
		~GLCommandList() override;

		void begin() override;
		void end() override;

		void copyTexture(const Texture *src, Texture *dst) override;

		void bindPipeline(RenderPipeline *p) override;
		void bindComputePipeline(RenderPipeline *pipeline) override;

		void bindFramebuffer(Ref<FrameBuffer> &framebuffer) override;
		void setScissor(int x, int y, unsigned int width, unsigned int height) override;
		void setViewport(int x, int y, unsigned int width, unsigned int height) override;

		void clearDepth(float depth) override;
		void clearColorTarget(uint index, const Color &color) override;

		void bindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const std::vector<Ref<Buffer>> &buffer,
							   const std::vector<size_t> pOffsets) override;
		void bindBindIndexBuffers(Ref<Buffer> &buffer, size_t offset, size_t indexType) override;

		// virtual void draw(Ref<Buffer> &buffer, uint32_t vertexCount, uint32_t instanceCount) override;
		void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
		void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
						 uint32_t firstInstance) override;
		void drawIndirect(Ref<Buffer> &buffer, size_t offset, uint32_t drawCount, uint32_t stride) override;
		void drawIndexedIndirect(Ref<Buffer> &buffer, size_t offset, uint32_t drawCount, uint32_t stride) override;

		void dispatch(uint groupCountX, uint groupCountY, uint groupCountZ) override;
		void dispatchIndirect(Buffer *buffer, u_int64_t offset) override;

		void setDepthBounds(float min, float max) override;

		void pushDebugGroup(const char *name) override;

		void popDebugGroup() override;
		void insertDebugMarker(const char *name) override;

	  protected:
		// TOOD expand the stack allocator to allow resize on demand.

	  private:
		StackAllocator stackAlloc;
		std::vector<GLCommandBase *> commands;
	};
} // namespace fragcore

#endif
