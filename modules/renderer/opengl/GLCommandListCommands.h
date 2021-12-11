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
		DrawIndex,
		DrawIndexIndices,
		DepthBounds,
		
	};

	class GLCommandBase {
	  public:
		GLCommandBase(GLCommandBufferCmd command) { this->cmd = command; }
		inline GLCommandBufferCmd getCommand() const { return this->cmd; }

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
		GLPushGroupMarkerCommand(const char *name) : GLCommandBase(GLCommandBufferCmd::PushGroupMarker) {}
	};
	class GLPopGroupMarkerCommand : public GLCommandBase {
		GLPopGroupMarkerCommand() : GLCommandBase(GLCommandBufferCmd::PopGroupMarker) {}
	};
	class GLViewPortCommand : public GLCommandBase {
	  public:
		GLViewPortCommand(int index, int x, int y, int width, int height)
			: GLCommandBase(GLCommandBufferCmd::ViewPort), index(index), x(x), y(y), width(width), height(height) {}
		unsigned int index;
		unsigned int width, height;
		unsigned int x, y;
	};
	class GLScissorPortCommand : public GLCommandBase {
	  public:
		GLScissorPortCommand() : GLCommandBase(GLCommandBufferCmd::Scissor) {}
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

} // namespace fragcore

#endif
