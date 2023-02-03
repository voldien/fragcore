#include "VKFrameBuffer.h"

using namespace fragcore;

void VKFrameBuffer::bind() {
	// vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VKFrameBuffer::unBind() {
	// vkCmdEndRenderPass(commandBuffers[i]);
}

int VKFrameBuffer::attachmentCount() {
	return 0;
}
std::vector<Texture *> VKFrameBuffer::getColorTargets() {
	return {};
}

void VKFrameBuffer::write() {}

void VKFrameBuffer::read() const {}

Texture *VKFrameBuffer::getAttachment(unsigned int index) { return desc.attach[index]; }

Texture *VKFrameBuffer::getDepthAttachment() { return desc.depth; }

Texture *VKFrameBuffer::getStencilAttachment() { return desc.stencil; }

int VKFrameBuffer::width() const { return desc.attach[0]->width(); }

int VKFrameBuffer::height() const { return desc.attach[0]->height(); }
int VKFrameBuffer::layers() { return desc.attach[0]->layers(); }

int VKFrameBuffer::nrSamples() { return 0; }

void VKFrameBuffer::blend(BlendEqu equ, BlendFunc sfactor, BlendFunc dfactor, BufferAttachment bufferAttachment) {}
void VKFrameBuffer::blendSeperate(BlendEqu equ, BlendFunc srcRGB, BlendFunc dstRGB, BlendFunc srcAlpha,
								  BlendFunc dstAlpha, BufferAttachment bufferAttachment) {}

void VKFrameBuffer::clear(unsigned int clear) {
	//  VkImageSubresourceRange ImageSubresourceRange;
	//    ImageSubresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
	//    ImageSubresourceRange.baseMipLevel   = 0;
	//    ImageSubresourceRange.levelCount     = 1;
	//    ImageSubresourceRange.baseArrayLayer = 0;
	//    ImageSubresourceRange.layerCount     = 1;

	//    VkClearColorValue ClearColorValue = { 1.0, 0.0, 0.0, 0.0 };

	//     VkCommandBuffer CommandBuffer = CommandBuffers[i];

	//   Result = vkBeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo);

	//   PrintVkResult("vkBeginCommandBuffer", Result);

	//   vkCmdClearColorImage(CommandBuffer, SwapchainImages[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &ClearColorValue,
	//   1, &ImageSubresourceRange);

	//   Result = vkEndCommandBuffer(CommandBuffer);

	//   PrintVkResult("vkEndCommandBuffer", Result);
}

void VKFrameBuffer::clearColor(BufferAttachment colorAttachment, const float *color) { /*  Assign only.*/
}

void VKFrameBuffer::clearDepthStencil(float depth, int stencil) {}

void VKFrameBuffer::setDraws(unsigned int nr, BufferAttachment *attachment) {}
void VKFrameBuffer::setDraw(BufferAttachment attachment) {}

void VKFrameBuffer::setName(const std::string &name) { Object::setName(name); }

intptr_t VKFrameBuffer::getNativePtr() const { return reinterpret_cast<intptr_t>(this->framebuffer); }
