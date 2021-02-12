#ifndef _FRAG_CORE_GL_COMMAND_LIST_H_
#define _FRAG_CORE_GL_COMMAND_LIST_H_ 1
#include"../CommandList.h"

namespace fragcore {
	/**
	 *
	 */
	class GLCommandList : public CommandList {
		public:
		 virtual ~GLCommandList(void);

		 virtual void begin(void);
		 virtual void end(void);

		 virtual void bindPipeline(RenderPipeline *p);
		 virtual void bindFramebuffer(Ref<FrameBuffer> &framebuffer);

		 virtual void dispatch(uint groupCountX, uint groupCountY, uint groupCountZ);
		 virtual void dispatchIndirect(Buffer* buffer, u_int64_t offset);
	};
}

#endif
