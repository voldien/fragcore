#include "VKCommandList.h"
#include "../RenderPipeline.h"
#include "VKBuffer.h"
#include "VKFrameBuffer.h"
#include "VKRenderInterface.h"
#include "VKTexture.h"
#include "internal_object_type.h"

#include <vulkan/vulkan.h>

using namespace fragcore;

PFN_vkCmdDebugMarkerBeginEXT pvkCmdDebugMarkerBeginEXT;
PFN_vkCmdDebugMarkerEndEXT pvkCmdDebugMarkerEndEXT;
PFN_vkCmdDebugMarkerInsertEXT pvkCmdDebugMarkerInsertEXT;

VKCommandList::VKCommandList(Ref<VKRenderInterface> &renderer) : renderer(renderer) {

	/*  Create command pool.    */
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
	cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolCreateInfo.queueFamilyIndex = renderer->getDevice()->getDefaultGraphicQueueIndex();
	cmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	/*  Create command pool.    */
	VKS_VALIDATE(vkCreateCommandPool(renderer->getDevice()->getHandle(), &cmdPoolCreateInfo, nullptr, &_pool));

	cmdbuffers.resize(1);

	VkCommandBufferAllocateInfo cbAI = {};
	cbAI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cbAI.commandPool = _pool;
	cbAI.commandBufferCount = 1;
	cbAI.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	VkResult result = vkAllocateCommandBuffers(renderer->getDevice()->getHandle(), &cbAI, &cmdbuffers[0]);
	VKS_VALIDATE(result);
	cmdBuffer = cmdbuffers[0];
}
VKCommandList::VKCommandList(const VKCommandList &other) : renderer(other.renderer) {}

VKCommandList::~VKCommandList() {
	// Delete
	vkFreeCommandBuffers(renderer->getDevice()->getHandle(), this->_pool, 1, &cmdBuffer);
}

void VKCommandList::begin() {

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	VKS_VALIDATE(vkBeginCommandBuffer(cmdBuffer, &beginInfo));
}

void VKCommandList::end() { vkEndCommandBuffer(cmdBuffer); }

void VKCommandList::copyTexture(const Texture *src, Texture *dst) {}

void VKCommandList::bindPipeline(RenderPipeline *pipeline) {
	VKPipelineObject *vkpipeline = (VKPipelineObject *)pipeline->getObject();
	vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkpipeline->graphicsPipeline);
	vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, vkpipeline->pipelineLayout, 0, 1,
							&vkpipeline->descriptorSet, 0, nullptr);
}

void VKCommandList::beginCurrentRenderPass() {

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	// renderPassInfo.renderPass = vulkancore->swapChain->renderPass;
	// renderPassInfo.framebuffer = vulkancore->swapChain->swapChainFramebuffers[i];
	// renderPassInfo.renderArea.offset = {0, 0};
	// renderPassInfo.renderArea.extent = vulkancore->swapChain->chainExtend;

	VkClearValue clearColor = {{0.0f, 1.0f, 0.0f, 1.0f}};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}
void VKCommandList::endCurrentRenderPass() { vkCmdEndRenderPass(cmdBuffer); }

void VKCommandList::bindFramebuffer(Ref<FrameBuffer> &framebuffer) {
	VKFrameBuffer *vkFrame = static_cast<VKFrameBuffer *>(framebuffer.ptr());

	/*	*/
	beginCurrentRenderPass();
}

void VKCommandList::setViewport(int x, int y, unsigned int width, unsigned int height) {
	VkViewport vkViewport = {
		.x = (float)x,
		.y = (float)y,
		.width = (float)width,
		.height = (float)height,
	};

	vkCmdSetViewport(cmdBuffer, 0, 1, &vkViewport);
}

void VKCommandList::setScissor(int x, int y, unsigned int width, unsigned int height) {
	VkRect2D sissor = {
		.offset = {x, y},
		.extent = {width, height},
	};
	vkCmdSetScissor(cmdBuffer, 0, 1, &sissor);
}

void VKCommandList::clearDepth(float depth) {
	//	this->commmands.push
	// vkCmdClearAttachments
}

void VKCommandList::clearColorTarget(uint index, const Color &color) {}

void VKCommandList::setDepthBounds(float min, float max) {}

void VKCommandList::dispatch(uint groupCountX, uint groupCountY, uint groupCountZ) {
	uint global[3] = {groupCountX, groupCountY, groupCountZ};
	// this->compute->dispatchCompute(global, nullptr, 0);
	// vkCmdBindPipeline
	vkCmdDispatch(cmdBuffer, groupCountX, groupCountY, groupCountZ);

	// Add memory barrier to ensure that the computer shader has finished writing to the buffer
	// VkBufferMemoryBarrier bufferBarrier = {};
	// bufferBarrier.buffer = compute.storageBuffer.buffer;
	// bufferBarrier.size = compute.storageBuffer.descriptor.range;
	// bufferBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
	// bufferBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	// // Transfer ownership if compute and graphics queue family indices differ
	// bufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	// bufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

	// vkCmdPipelineBarrier(cmdBuffer,
	// 					 VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
	// 					 VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
	// 					 VK_FLAGS_NONE,
	// 					 0,
	// 					 nullptr,
	// 					 1,
	// 					 &bufferBarrier,
	// 					 0,
	// 					 nullptr);
}
void VKCommandList::dispatchIndirect(Buffer *buffer, u_int64_t offset) {
	VKBuffer *vkBuffer = (VKBuffer *)buffer;
	vkCmdDispatchIndirect(cmdBuffer, vkBuffer->getBuffer(), offset);
}

void VKCommandList::pushDebugGroup(const char *name) {
	VkDebugMarkerMarkerInfoEXT markerinfo = {};
	markerinfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
	markerinfo.pMarkerName = name;
	markerinfo.pNext = nullptr;

	pvkCmdDebugMarkerBeginEXT(cmdBuffer, &markerinfo);
}
void VKCommandList::popDebugGroup() { pvkCmdDebugMarkerEndEXT(cmdBuffer); }

void VKCommandList::insertDebugMarker(const char *name) {
	VkDebugMarkerMarkerInfoEXT markerinfo = {};
	markerinfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
	markerinfo.pMarkerName = name;
	markerinfo.pNext = nullptr;

	pvkCmdDebugMarkerInsertEXT(cmdBuffer, &markerinfo);
}

void VKCommandList::draw(Ref<Buffer> &buffer, uint32_t vertexCount, uint32_t instanceCount) {}
