#include"../FrameBuffer.h"

using namespace fragcore;


void FrameBuffer::bind() {
    //vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void FrameBuffer::unBind() {
    //vkCmdEndRenderPass(commandBuffers[i]);
}

void FrameBuffer::write() {

}

void FrameBuffer::read() const {
}

Texture *FrameBuffer::getAttachment(unsigned int index) {

}

Texture *FrameBuffer::getDepthAttachment() {

}

Texture *FrameBuffer::getStencilAttachment() {

}

int FrameBuffer::width() const
{
}

int FrameBuffer::height() const
{
}
int FrameBuffer::layers(){
	
}

int FrameBuffer::nrSamples() {}

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

intptr_t FrameBuffer::getNativePtr() const {
	return 0;
}
