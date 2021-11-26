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
