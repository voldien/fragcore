#ifndef _FRAG_CORE_GL_COMMAND_LIST_COMMANDS_H_
#	define _FRAG_CORE_GL_COMMAND_LIST_COMMANDS_H_ 1

namespace fragcore {
	enum GLCommandBufferCmd{ Dispatch, DispatchIndirect, PushGroupMarker, PopGroupMarker, InsertGroupMarker };
}

#endif
