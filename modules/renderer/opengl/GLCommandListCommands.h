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
#include "Core/Color.h"
#include <Def.h>

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
		Draw,
		DrawIndice,
		DrawIndirect,
		DrawIndirectIndices,
		DepthBounds,
		BindPipeline,

	};

	class GLCommandBase {
	  public:
		GLCommandBase(GLCommandBufferCmd command) : cmd(command) {}
		GLCommandBufferCmd getCommand() const { return this->cmd; }

		template <typename T> static size_t getCommandSize() { return sizeof(T); }

	  private:
		GLCommandBufferCmd cmd;
	};

	class GLCommandClearColor : public GLCommandBase {
	  public:
		GLCommandClearColor(uint index, const Color &color)
			: GLCommandBase(GLCommandBufferCmd::ClearColor), index(index), clear(color) {}
		uint index;
		Color clear;
	};
	class GLCommandClear : public GLCommandBase {
	  public:
		GLCommandClear() : GLCommandBase(GLCommandBufferCmd::ClearImage) {}
		unsigned int mask;
	};

	class GLCommandDispatch : public GLCommandBase {
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

	class GLPushGroupMarkerCommand : public GLCommandBase {
		GLPushGroupMarkerCommand(const char *name) : GLCommandBase(GLCommandBufferCmd::PushGroupMarker), name(name) {}
		std::string name;
	};
	class GLPopGroupMarkerCommand : public GLCommandBase {
		GLPopGroupMarkerCommand() : GLCommandBase(GLCommandBufferCmd::PopGroupMarker) {}

	};
	class GLInsertGroupMarkerCommand : public GLCommandBase {
		GLInsertGroupMarkerCommand(const char *name)
			: GLCommandBase(GLCommandBufferCmd::InsertGroupMarker), name(name) {}
		std::string name;
	};

	class GLViewPortCommand : public GLCommandBase {
	  public:
		GLViewPortCommand(int index, int x, int y, int width, int height)
			: GLCommandBase(GLCommandBufferCmd::ViewPort), index(index), x(x), y(y), width(width), height(height) {}
		unsigned int index;
		unsigned int x, y;
		unsigned int width, height;
	};
	class GLScissorPortCommand : public GLCommandBase {
	  public:
		GLScissorPortCommand(int index, int x, int y, int width, int height)
			: GLCommandBase(GLCommandBufferCmd::Scissor), index(index), x(x), y(y), width(width), height(height) {}
		unsigned int index;
		unsigned int width, height;
		unsigned int x, y;
	};
	class GLBindFrameBufferCommand : public GLCommandBase {
	  public:
		GLBindFrameBufferCommand(FrameBuffer *framebuffer)
			: GLCommandBase(GLCommandBufferCmd::BindFrameBuffer), framebuffer(framebuffer) {}
		FrameBuffer *framebuffer;
	};
	class GLCopyTextureCommand : public GLCommandBase {
	  public:
		// TODO add src and dest size
		GLCopyTextureCommand(const Ref<Texture> &src, Ref<Texture> &dst)
			: GLCommandBase(GLCommandBufferCmd::CopyTexture) {}
	};

	class GLBlitCommand : public GLCommandBase {
	  public:
		// TODO add src and dest size
		GLBlitCommand(const Ref<FrameBuffer> &src, Ref<FrameBuffer> &dst) : GLCommandBase(GLCommandBufferCmd::Blit) {}

		FrameBuffer *src;
		FrameBuffer *dst;
		// TODO add size, filtermode
	};

	class GLDrawCommand : public GLCommandBase {
	  public:

		GLDrawCommand(const Ref<Buffer> &buffer, uint32_t nrVertices, uint32_t nrInstances)
			: GLCommandBase(GLCommandBufferCmd::Draw) {}

		Buffer *src;
		uint32_t nrVertices;
		uint32_t nrInstances;
	};

	class GLDrawIndicesCommand : public GLCommandBase {
	  public:
		// TODO add the rest of require param
		GLDrawIndicesCommand(const Ref<Buffer> &buffer, const Ref<Buffer> &indices, uint32_t nrVertices,
							 uint32_t nrInstances)
			: GLCommandBase(GLCommandBufferCmd::DrawIndirect) {}

		Buffer *src;
		uint32_t nrVertices;
		uint32_t nrInstances;
	};

	class GLDrawIndirectCommand : public GLCommandBase {
	  public:
		// TODO add the rest of require param
		GLDrawIndirectCommand(const Ref<Buffer> &buffer, const Ref<Buffer> &indices, uint32_t nrVertices,
							  uint32_t nrInstances)
			: GLCommandBase(GLCommandBufferCmd::DrawIndirect) {}

		Buffer *src;
		uint32_t nrVertices;
		uint32_t nrInstances;
	};
	class GLDrawIndexIndirectCommand : public GLCommandBase {
	  public:
		// TODO add the rest of require param
		GLDrawIndexIndirectCommand(const Ref<Buffer> &buffer, const Ref<Buffer> &indices, uint32_t nrVertices,
								   uint32_t nrInstances)
			: GLCommandBase(GLCommandBufferCmd::DrawIndirectIndices) {}

		Buffer *src;
		uint32_t nrVertices;
		uint32_t nrInstances;
	};

} // namespace fragcore

#endif
