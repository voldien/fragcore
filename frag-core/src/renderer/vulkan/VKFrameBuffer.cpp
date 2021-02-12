#include"Renderer/FrameBuffer.h"

using namespace fragcore;


void FrameBuffer::bind(void) {
    //vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void FrameBuffer::unBind(void) {
    //vkCmdEndRenderPass(commandBuffers[i]);
}

void FrameBuffer::write(void) {

}

void FrameBuffer::read(void) {

}

Texture *FrameBuffer::getAttachment(unsigned int index) {

}

Texture *FrameBuffer::getDepthAttachment(void) {

}

Texture *FrameBuffer::getStencilAttachment(void) {

}


int FrameBuffer::width(void) {}

int FrameBuffer::height(void) {}
int FrameBuffer::layers(void){
	
}

int FrameBuffer::nrSamples(void) {}

void FrameBuffer::blend(BlendEqu equ, BlendFunc sfactor, BlendFunc dfactor, BufferAttachment bufferAttachment) {

}
void FrameBuffer::blendSeperate(BlendEqu equ, BlendFunc srcRGB,
                                BlendFunc dstRGB, BlendFunc srcAlpha,
                                BlendFunc dstAlpha,
                                BufferAttachment bufferAttachment) {}

void FrameBuffer::clear(unsigned int clear) {
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

    //   vkCmdClearColorImage(CommandBuffer, SwapchainImages[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &ClearColorValue, 1, &ImageSubresourceRange);

    //   Result = vkEndCommandBuffer(CommandBuffer);

    //   PrintVkResult("vkEndCommandBuffer", Result);
}

void FrameBuffer::clearColor(BufferAttachment colorAttachment, const float *color){
    /*  Assign only.*/
}

void FrameBuffer::clearDepthStencil(float depth, int stencil){

}

void FrameBuffer::setDraws(unsigned int nr, BufferAttachment *attachment){}
void FrameBuffer::setDraw(BufferAttachment attachment){}

void FrameBuffer::setName(const std::string &name) {
	Object::setName(name);
}

intptr_t FrameBuffer::getNativePtr(void) const {
	return 0;
}
