#include "VKCommandList.h"
#include "Renderer/RenderPipeline.h"
#include "internal_object_type.h"
#include <Exception/InvalidArgumentException.h>
#include <GL/glew.h>
#include <Utils/StringUtil.h>
#include <stdexcept>
#include <vulkan/vulkan.h>

using namespace fragcore;

PFN_vkCmdDebugMarkerBeginEXT pvkCmdDebugMarkerBeginEXT;
PFN_vkCmdDebugMarkerEndEXT pvkCmdDebugMarkerEndEXT;
PFN_vkCmdDebugMarkerInsertEXT pvkCmdDebugMarkerInsertEXT;

VKCommandList::VKCommandList(Ref<IRenderer> &renderer) {
	// VulkanCore *vulkancore = (VulkanCore *)(*renderer)->getData();

	// /*  Create command pool.    */
	// VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
	// cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	// cmdPoolCreateInfo.queueFamilyIndex = vulkancore->graphics_queue_node_index;
	// cmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	// /*  Create command pool.    */
	// checkError(vkCreateCommandPool(vulkancore->device, &cmdPoolCreateInfo, nullptr, &_pool));

	// VkCommandBufferAllocateInfo cbAI = {};
	// cbAI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	// cbAI.commandPool = _pool;
	// cbAI.commandBufferCount = 1;
	// cbAI.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	// VkResult result = vkAllocateCommandBuffers(vulkancore->device, &cbAI, &cmdbuffers[0]);
	// checkError(result);
}
VKCommandList::VKCommandList(VKCommandList &other) {}

VKCommandList::~VKCommandList(void) {}

void VKCommandList::begin(void) {

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	vkBeginCommandBuffer(cmdBuffer, &beginInfo);
}

void VKCommandList::end(void) { vkEndCommandBuffer(cmdBuffer); }

void VKCommandList::bindPipeline(RenderPipeline *pipeline) {
	VKPipelineObject *vkpipeline = (VKPipelineObject *)pipeline->getObject();
	vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkpipeline->graphicsPipeline);
	vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, vkpipeline->pipelineLayout, 0, 1,
							&vkpipeline->descriptorSet, 0, 0);
}

void VKCommandList::beginCurrentRenderPass() {

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	// renderPassInfo.renderPass = vulkancore->swapChain->renderPass;
	// renderPassInfo.framebuffer = vulkancore->swapChain->swapChainFramebuffers[i];
	// renderPassInfo.renderArea.offset = {0, 0};
	// renderPassInfo.renderArea.extent = vulkancore->swapChain->chainExtend;

	VkClearValue clearColor = {0.0f, 1.0f, 0.0f, 1.0f};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}
void VKCommandList::endCurrentRenderPass(void) { vkCmdEndRenderPass(cmdBuffer); }

void VKCommandList::bindFramebuffer(Ref<FrameBuffer> &framebuffer) { beginCurrentRenderPass(); }

void VKCommandList::setviewport(int x, int y, int width, int height) {
	VkViewport vkViewport = {
		.x = (float)x,
		.y = (float)y,
		.width = (float)width,
		.height = (float)height,
	};

	vkCmdSetViewport(cmdBuffer, 0, 1, &vkViewport);
}
void VKCommandList::clearDepth(float depth) {
	//	this->commmands.push
	// vkCmdClearAttachments
}

void VKCommandList::clearColorTarget(uint index, const Color &color) {}

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
	VKBufferObject *vkBuffer = (VKBufferObject *)buffer->getObject();
	vkCmdDispatchIndirect(cmdBuffer, vkBuffer->buffer, offset);
	// uint global[3] = {groupCountX, groupCountY, groupCountZ};
	// this->compute->dispatchCompute(global, nullptr, 0);
}

void VKCommandList::pushDebugGroup(const char *name) {
	VkDebugMarkerMarkerInfoEXT markerinfo = {};
	markerinfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
	markerinfo.pMarkerName = name;
	markerinfo.pNext = nullptr;

	pvkCmdDebugMarkerBeginEXT(cmdBuffer, &markerinfo);
}
void VKCommandList::popDebugGroup(void) { pvkCmdDebugMarkerEndEXT(cmdBuffer); }

void VKCommandList::insertDebugMarker(const char *name) {
	VkDebugMarkerMarkerInfoEXT markerinfo = {};
	markerinfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
	markerinfo.pMarkerName = name;
	markerinfo.pNext = nullptr;

	pvkCmdDebugMarkerInsertEXT(cmdBuffer, &markerinfo);
}