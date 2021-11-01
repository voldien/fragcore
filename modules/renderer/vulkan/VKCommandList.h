
#ifndef _FRAG_CORE_VK_COMMAND_LIST_H_
#define _FRAG_CORE_VK_COMMAND_LIST_H_ 1
#include "../CommandList.h"
#include <vulkan/vulkan.h>

namespace fragcore {
	/**
	 *
	 */
	class VKCommandList : public CommandList {
	  public:
		VKCommandList(Ref<IRenderer> &renderer);
		VKCommandList(const VKCommandList &other);
		virtual ~VKCommandList();

		virtual void begin();
		virtual void end();

		virtual void bindPipeline(RenderPipeline *p);
		virtual void bindFramebuffer(Ref<FrameBuffer> &framebuffer);

		virtual void setviewport(int x, int y, int width, int height);
		virtual void clearDepth(float depth);
		virtual void clearColorTarget(uint index, const Color &color);

		virtual void dispatch(uint groupCountX, uint groupCountY, uint groupCountZ);
		virtual void dispatchIndirect(Buffer *buffer, u_int64_t offset);

		virtual void pushDebugGroup(const char *name);
		virtual void popDebugGroup();
		virtual void insertDebugMarker(const char *name);

	  private:
		void beginCurrentRenderPass();
		void endCurrentRenderPass();

	  public:
		VkCommandBuffer cmdBuffer;
		VkCommandPool _pool;

	  protected:
		bool _commandBufferBegun;
		bool _commandBufferEnded;
		std::vector<VkCommandBuffer> cmdbuffers;
	};
} // namespace fragcore

#endif
