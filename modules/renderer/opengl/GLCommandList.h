#ifndef _FRAG_CORE_GL_COMMAND_LIST_H_
#define _FRAG_CORE_GL_COMMAND_LIST_H_ 1
#include<Core/dataStructure/StackAllactor.h>
#include "../CommandList.h"
#include "GLCommandListCommands.h"
#include <vector>

namespace fragcore {
	/**
	 *
	 */
	class GLCommandList : public CommandList {
		friend class GLRendererInterface;

	  public:
		GLCommandList();
		virtual ~GLCommandList();

		virtual void begin();
		virtual void end();

		virtual void copyTexture(const Texture *src, Texture *dst) override;

		virtual void bindPipeline(RenderPipeline *p) override;
		virtual void bindFramebuffer(Ref<FrameBuffer> &framebuffer) override;
		virtual void setviewport(int x, int y, int width, int height) override;

		virtual void clearDepth(float depth);
		virtual void clearColorTarget(uint index, const Color &color);

		virtual void dispatch(uint groupCountX, uint groupCountY, uint groupCountZ) override;
		virtual void dispatchIndirect(Buffer *buffer, u_int64_t offset) override;

		virtual void setDepthBounds(float min, float max) override;

		virtual void pushDebugGroup(const char *name);

		virtual void popDebugGroup();
		virtual void insertDebugMarker(const char *name);

	  private:
		StackAllocator stackAlloc;
		std::vector<GLCommandBase *> commands;
	};
} // namespace fragcore

#endif
