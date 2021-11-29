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
	};

	class GLCommandBase {
	  public:
		GLCommandBase(GLCommandBufferCmd command) { this->cmd = command; }
		inline GLCommandBufferCmd getCommand() const { return this->cmd; }

		template <typename T> size_t getCommandSize() { return sizeof(T); }

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
	};

	class GLPushGroupMarkerCommand : public GLCommandBase {};
	class GLPopGroupMarkerCommand : public GLCommandBase {};
	class GLViewPortCommand : public GLCommandBase {
	  public:
		GLViewPortCommand() : GLCommandBase(GLCommandBufferCmd::ViewPort) {}
		unsigned int index;
		unsigned int width, height;
		unsigned int x, y;
	};
} // namespace fragcore

#endif
