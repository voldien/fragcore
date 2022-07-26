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
#ifndef _FRAG_CORE_GL_COMMAND_LIST_COMMANDS_H_
#define _FRAG_CORE_GL_COMMAND_LIST_COMMANDS_H_ 1
#include "Core/math3D/Color.h"
#include <FragDef.h>

namespace fragcore {
	enum class GLCommandBufferCmd {
		ClearImage,
		ClearColor,
		Dispatch,
		DispatchIndirect,
		PushGroupMarker,
		PopGroupMarker,
		InsertGroupMarker,
		ViewPort,
		Scissor,
		BindFrameBuffer,
		CopyTexture,
		Blit,
		UpdateBuffer,
		BindVertexBuffers,
		BindIndiceBuffer,
		Draw,
		DrawIndice,
		DrawIndirect,
		DrawIndirectIndices,
		DepthBounds,
		BindGraphicPipeline,
		BindComputePipeline,

		LineWidth,
		DepthBias,
		BlendConstant,
		StencilCompare,
		StencilWriteMask,
		StencilReference,

		QueryBegin,
		QueryEnd,
		BufferArrayBind,
		BufferIndexBind,
		SetTopology,
		BegingConditionalRendering,
		EndConditionalRendering,
	};

	class FVDECLSPEC GLCommandBase {
	  public:
		GLCommandBase(GLCommandBufferCmd command) : cmd(command) {}
		GLCommandBufferCmd getCommand() const { return this->cmd; }

		template <typename T> static size_t getCommandSize() { return sizeof(T); }
		template <typename T> const T *as() const { return reinterpret_cast<const T *>(this); }

	  private:
		GLCommandBufferCmd cmd;
	};

	class FVDECLSPEC GLCommandClearColor : public GLCommandBase {
	  public:
		GLCommandClearColor(uint index, const Color &color)
			: GLCommandBase(GLCommandBufferCmd::ClearColor), index(index), clear(color) {}
		uint index;
		Color clear;
	};

	class FVDECLSPEC GLCommandClear : public GLCommandBase {
	  public:
		GLCommandClear() : GLCommandBase(GLCommandBufferCmd::ClearImage) {}
		unsigned int mask;
	};

	class FVDECLSPEC GLCommandDispatch : public GLCommandBase {
	  public:
		GLCommandDispatch(int x, int y, int z) : GLCommandBase(GLCommandBufferCmd::Dispatch), x(x), y(y), z(z) {}
		int x, y, z;
	};

	class GLCommandDispatchIndirect : public GLCommandBase {
	  public:
		GLCommandDispatchIndirect(Buffer *buffer, size_t offset)
			: GLCommandBase(GLCommandBufferCmd::DispatchIndirect), buffer(buffer), offset(offset) {}
		Buffer *buffer;
		size_t offset;
	};

	class FVDECLSPEC GLPushGroupMarkerCommand : public GLCommandBase {
	  public:
		GLPushGroupMarkerCommand(const char *name) : GLCommandBase(GLCommandBufferCmd::PushGroupMarker), name(name) {}
		std::string name;
	};

	class FVDECLSPEC GLPopGroupMarkerCommand : public GLCommandBase {
	  public:
		GLPopGroupMarkerCommand() : GLCommandBase(GLCommandBufferCmd::PopGroupMarker) {}
	};

	class FVDECLSPEC GLInsertGroupMarkerCommand : public GLCommandBase {
	  public:
		GLInsertGroupMarkerCommand(const char *name)
			: GLCommandBase(GLCommandBufferCmd::InsertGroupMarker), name(name) {}
		std::string name;
	};

	class FVDECLSPEC GLViewPortCommand : public GLCommandBase {
	  public:
		GLViewPortCommand(int index, int x, int y, int width, int height)
			: GLCommandBase(GLCommandBufferCmd::ViewPort), index(index), x(x), y(y), width(width), height(height) {}
			
		unsigned int index;
		unsigned int x, y;
		unsigned int width, height;
	};

	class FVDECLSPEC GLScissorPortCommand : public GLCommandBase {
	  public:
		GLScissorPortCommand(int index, int x, int y, int width, int height)
			: GLCommandBase(GLCommandBufferCmd::Scissor), index(index), x(x), y(y), width(width), height(height) {}

		unsigned int index;
		unsigned int x, y;
		unsigned int width, height;
	};

	class FVDECLSPEC GLBindFrameBufferCommand : public GLCommandBase {
	  public:
		GLBindFrameBufferCommand(FrameBuffer *framebuffer)
			: GLCommandBase(GLCommandBufferCmd::BindFrameBuffer), framebuffer(framebuffer) {}
		FrameBuffer *framebuffer;
	};

	class FVDECLSPEC GLCopyTextureCommand : public GLCommandBase {
	  public:
		// TODO add src and dest size
		GLCopyTextureCommand(const Ref<Texture> &src, Ref<Texture> &dst)
			: GLCommandBase(GLCommandBufferCmd::CopyTexture) {}
	};

	class FVDECLSPEC GLBlitCommand : public GLCommandBase {
	  public:
		// TODO add src and dest size
		GLBlitCommand(const Ref<FrameBuffer> &src, Ref<FrameBuffer> &dst) : GLCommandBase(GLCommandBufferCmd::Blit) {}

		FrameBuffer *src;
		FrameBuffer *dst;
		// TODO add size, filtermode
	};

	class FVDECLSPEC GLBindVertexBuffersCommand : public GLCommandBase {
	  public:
		GLBindVertexBuffersCommand(const Ref<Buffer> &buffer, uint32_t nrVertices, uint32_t nrInstances)
			: GLCommandBase(GLCommandBufferCmd::BindVertexBuffers), buffer(buffer), nrVertices(nrVertices),
			  nrInstances(nrInstances) {}

		Ref<Buffer> buffer;
		uint32_t nrVertices;
		uint32_t nrInstances;
	};

	class FVDECLSPEC GLBindIndiceBufferCommand : public GLCommandBase {
	  public:
		GLBindIndiceBufferCommand(const Ref<Buffer> &buffer, uint32_t nrVertices, uint32_t nrInstances)
			: GLCommandBase(GLCommandBufferCmd::BindIndiceBuffer) {}

		Buffer *src;
		uint32_t nrVertices;
		uint32_t nrInstances;
	};

	class FVDECLSPEC GLDrawCommand : public GLCommandBase {
	  public:
		GLDrawCommand(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
			: GLCommandBase(GLCommandBufferCmd::Draw), vertexCount(vertexCount), instanceCount(instanceCount),
			  firstVertex(firstVertex), firstInstance(firstInstance) {}

		uint32_t vertexCount;
		uint32_t instanceCount;
		uint32_t firstVertex;
		uint32_t firstInstance;
	};

	class FVDECLSPEC GLDrawIndicesCommand : public GLCommandBase {
	  public:
		// TODO add the rest of require param
		GLDrawIndicesCommand(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
							 uint32_t firstInstance)
			: GLCommandBase(GLCommandBufferCmd::DrawIndirect), indexCount(indexCount), instanceCount(instanceCount),
			  firstIndex(firstIndex), vertexOffset(vertexOffset), firstInstance(firstInstance) {}

		uint32_t indexCount;
		uint32_t instanceCount;
		uint32_t firstIndex;
		int32_t vertexOffset;
		uint32_t firstInstance;
	};

	class FVDECLSPEC GLDrawIndirectCommand : public GLCommandBase {
	  public:
		// TODO add the rest of require param
		GLDrawIndirectCommand(const Ref<Buffer> &buffer, const Ref<Buffer> &indices, uint32_t nrVertices,
							  uint32_t nrInstances)
			: GLCommandBase(GLCommandBufferCmd::DrawIndirect) {}

		Buffer *src;
		uint32_t nrVertices;
		uint32_t nrInstances;
	};

	class FVDECLSPEC GLDrawIndexIndirectCommand : public GLCommandBase {
	  public:
		// TODO add the rest of require param
		GLDrawIndexIndirectCommand(const Ref<Buffer> &buffer, const Ref<Buffer> &indices, uint32_t nrVertices,
								   uint32_t nrInstances)
			: GLCommandBase(GLCommandBufferCmd::DrawIndirectIndices) {
			this->nrVertices = nrVertices;
			this->nrInstances = nrInstances;
		}

		Buffer *src;
		uint32_t nrVertices;
		uint32_t nrInstances;
	};

	class FVDECLSPEC GLDepthBoundsCommand : public GLCommandBase {
	  public:
		// TODO add the rest of require param
		GLDepthBoundsCommand(float min, float max)
			: GLCommandBase(GLCommandBufferCmd::DepthBounds), min(min), max(max) {}

		float min, max;
	};

	class FVDECLSPEC GLGraphicPipelineCommand : public GLCommandBase {
	  public:
		// TODO add the rest of require param
		GLGraphicPipelineCommand(RenderPipeline *pipeline)
			: GLCommandBase(GLCommandBufferCmd::BindGraphicPipeline), pipeline(pipeline) {}

		RenderPipeline *pipeline;
	};

	class FVDECLSPEC GLComputePipelineCommand : public GLCommandBase {
	  public:
		// TODO add the rest of require param
		GLComputePipelineCommand(RenderPipeline *pipeline)
			: GLCommandBase(GLCommandBufferCmd::BindComputePipeline), pipeline(pipeline) {
			this->pipeline = pipeline;
		}

		RenderPipeline *pipeline;
	};

} // namespace fragcore

#endif
