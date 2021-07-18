#ifndef _FRAG_CORE_GL_COMMAND_LIST_COMMANDS_H_
#define _FRAG_CORE_GL_COMMAND_LIST_COMMANDS_H_ 1
#include<Def.h>

namespace fragcore {
	enum GLCommandBufferCmd{ ClearImage, ClearColor, Dispatch, DispatchIndirect, PushGroupMarker, PopGroupMarker, InsertGroupMarker };

	class GLCommandBase {
		public:
		  GLCommandBase(GLCommandBufferCmd command) { this->cmd = command; }
		  inline GLCommandBufferCmd getCommand(void) const { return this->cmd; }

		private:
		  GLCommandBufferCmd cmd;
	};

	class GLCommandClearColor : public GLCommandBase {
		public:
		  GLCommandClearColor(uint index, const Color &color) : GLCommandBase(ClearColor) {
			  this->index = index;
			  this->clear = color;
		  }
		uint index;
		Color clear;
	};
	class GLCommandClear : public GLCommandBase {
	  public:
		GLCommandClear(void) : GLCommandBase(ClearImage) {}
	};

	class GLPushGroupMarkerCommand : public GLCommandBase {};
	class GLPopGroupMarkerCommand : public GLCommandBase {};
} // namespace fragcore

#endif
