/*
 *	FragCore - Fragment Core
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
#ifndef _FRAGCORE_COMMAND_LIST_H_
#define _FRAGCORE_COMMAND_LIST_H_ 1
#include "IRenderer.h"
#include <Core/NoCopyable.h>
#include <Math3D/Color.h>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC CommandList : public SmartReference {
	  public:
		enum CommandBufferFlag { Single, Continuous };
		~CommandList() override = default;

		virtual void begin() = 0;
		virtual void end() = 0;

		virtual void copyTexture(const Texture *src, Texture *dst) = 0;

		virtual void bindPipeline(RenderPipeline *pipline) = 0;
		virtual void bindComputePipeline(RenderPipeline *pipeline) = 0;

		// virtual void updateBuffer(Ref<Buffer> &buffer, void *p, int size) = 0;
		virtual void bindFramebuffer(Ref<FrameBuffer> &framebuffer) = 0;

		/*	*/
		virtual void setViewport(int x, int y, unsigned int width, unsigned int height) = 0;
		virtual void setScissor(int x, int y, unsigned int width, unsigned int height) = 0;
		virtual void clearDepth(float depth) = 0;
		virtual void clearColorTarget(uint index, const Color &color) = 0;

		virtual void setDepthBounds(float min, float max) = 0;

		/*	*/
		virtual void bindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount,
									   const std::vector<Ref<Buffer>> &buffer, const std::vector<size_t> pOffsets) = 0;
		virtual void bindBindIndexBuffers(Ref<Buffer> &buffer, size_t offset, size_t indexType) = 0;

		/**
		 * @brief
		 *
		 * @param vertexCount
		 * @param instanceCount
		 * @param firstVertex
		 * @param firstInstance
		 */
		virtual void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex,
						  uint32_t firstInstance) = 0;
		virtual void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
								 uint32_t firstInstance) = 0;
		virtual void drawIndirect(Ref<Buffer> &buffer, size_t offset, uint32_t drawCount, uint32_t stride) = 0;
		virtual void drawIndexedIndirect(Ref<Buffer> &buffer, size_t offset, uint32_t drawCount, uint32_t stride) = 0;

		/**
		 * @brief
		 *
		 * @param groupCountX
		 * @param groupCountY
		 * @param groupCountZ
		 */
		virtual void dispatch(uint groupCountX, uint groupCountY, uint groupCountZ) = 0;

		/**
		 * @brief
		 *
		 * @param buffer
		 * @param offset
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
