#ifndef _FRAG_CORE_GL_COMMAND_LIST_H_
#define _FRAG_CORE_GL_COMMAND_LIST_H_ 1
#include"../CommandList.h"
#include"GLCommandListCommands.h"
#include<vector>

namespace fragcore {
	/**
	 *
	 */
	class GLCommandList : public CommandList {
		friend class GLRendererInterface;

	  public:
		virtual ~GLCommandList(void);

		virtual void begin(void);
		virtual void end(void);

		virtual void bindPipeline(RenderPipeline *p);
		virtual void bindFramebuffer(Ref<FrameBuffer> &framebuffer);

		virtual void clearDepth(float depth);
		virtual void clearColorTarget(uint index, const Color &color);

		virtual void dispatch(uint groupCountX, uint groupCountY, uint groupCountZ) override;
		virtual void dispatchIndirect(Buffer *buffer, u_int64_t offset) override;

		virtual void pushDebugGroup(const char *name);

		virtual void popDebugGroup(void);
		virtual void insertDebugMarker(const char *name);

	  private:
		std::vector<GLCommandBase *> commands;
	};
}

#endif
