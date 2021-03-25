#define FRAG_CORE_INTERNAL_IMP
#include "Renderer/vulkan/VKRenderWindow.h"
#include <Exception/NotImplementedException.h>
#include <Exception/NotSupportedException.h>
#include <Exception/RuntimeException.h>
#include <SDL2/SDL_syswm.h>
#include<SDL2/SDL_vulkan.h>
#include <Utils/StringUtil.h>
#include<vulkan/vulkan.h>
#include"Renderer/vulkan/internal_object_type.h"
#include"Window/WindowManager.h"

using namespace fragcore;

VKRenderWindow::VKRenderWindow(Ref<VKRenderInterface> &renderer) {

	this->renderer = renderer;
	VulkanCore *vulkancore = (VulkanCore *)renderer->getData();
	VkResult result;
	SDL_bool surfaceResult;

	/*  Create Vulkan window.   */
	WindowManager::getInstance();
	SDL_Window *windowp = SDL_CreateWindow(
		"", 1, 1, 800, 600, SDL_WINDOW_VULKAN);
	if (windowp == NULL)
		throw RuntimeException(
			fvformatf("failed create window - %s", SDL_GetError()));
	this->window = windowp;
	
	/*  Create surface. */
	surfaceResult = SDL_Vulkan_CreateSurface(
		(SDL_Window *)window, vulkancore->inst, &vulkancore->surface);
	if (surfaceResult == SDL_FALSE)
		throw RuntimeException(
			fvformatf("failed create vulkan surface - %s", SDL_GetError()));
	/*  */
#ifdef VK_USE_PLATFORM_XLIB_KHR

#elif defined(VK_USE_PLATFORM_WIN32_KHR)
//	vkCreateWin32SurfaceKHR
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
	//	// vkCreateAndroidSurfaceKHR
#endif

	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(vulkancore->gpu, vulkancore->surface);
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	/*  Allocate swapchain. */
	currentFrame = 0;

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	QueueFamilyIndices indices = findQueueFamilies(vulkancore->gpu, vulkancore->surface);
	uint32_t queueFamilyIndices[] = {indices.graphicsFamily, indices.presentFamily};

	/*  */
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = vulkancore->surface;
	
	/*  */
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	/*  */
	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	/*  */
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;//(presentModes);
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	/*  Create swapchain.   */
	if (vkCreateSwapchainKHR(vulkancore->device, &createInfo, NULL,
								&swapChain.swapchain) != VK_SUCCESS){
		throw RuntimeException(
			fvformatf("vkCreateSwapchainKHR failed - %d", result));
	}

	/*  Get the image associated with the swap chain.   */
	uint32_t nrChainImageCount = 1;
	result = vkGetSwapchainImagesKHR(vulkancore->device,
										swapChain.swapchain,
										&nrChainImageCount, NULL);
	if(result != VK_SUCCESS){
		throw RuntimeException(fvformatf(
			"vkGetSwapchainImagesKHR failed query count - %d", result));
	}
	swapChain.swapChainImages.resize(nrChainImageCount);
	result = vkGetSwapchainImagesKHR( vulkancore->device, swapChain.swapchain, &nrChainImageCount, swapChain.swapChainImages.data());
	if (result != VK_SUCCESS)
		throw RuntimeException(fvformatf(
			"vkGetSwapchainImagesKHR failed query object - %d", result));
	swapChain.swapChainImageFormat = surfaceFormat.format;
	swapChain.chainExtend = extent;

	/*	*/
	swapChain.swapChainImageViews.resize(swapChain.swapChainImages.size());

	for (size_t i = 0; i < swapChain.swapChainImages.size(); i++) {
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChain.swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChain.swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(vulkancore->device, &createInfo, nullptr,
								&swapChain.swapChainImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views!");
		}
	}

	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChain.swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(vulkancore->device, &renderPassInfo, nullptr, &swapChain.renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}

	//TODO add support.
	swapChain.swapChainFramebuffers.resize(swapChain.swapChainImageViews.size());

	for (size_t i = 0; i < swapChain.swapChainImageViews.size(); i++) {
		VkImageView attachments[] = {
			swapChain.swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = swapChain.renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChain.chainExtend.width;
		framebufferInfo.height = swapChain.chainExtend.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(vulkancore->device, &framebufferInfo,
								nullptr, &swapChain.swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}

	/*  */
	swapChain.commandBuffers.resize(swapChain.swapChainFramebuffers.size());
	VkCommandBufferAllocateInfo cmdBufAllocInfo = {};
	cmdBufAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufAllocInfo.commandPool = vulkancore->cmd_pool;
	cmdBufAllocInfo.commandBufferCount = swapChain.swapChainImages.size();
	cmdBufAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	/*  */
	// swapChain.commandBuffers = (VkCommandBuffer *)malloc(
	// 	swapChain.swapChainImages.size() * sizeof(VkCommandBuffer));
	result = vkAllocateCommandBuffers(vulkancore->device, &cmdBufAllocInfo,
									  swapChain.commandBuffers.data());
	if (result != VK_SUCCESS)
		throw RuntimeException(
			fvformatf("vkAllocateCommandBuffers failed - %d", result));

	// /*  */
	// VkCommandBufferBeginInfo beginInfo = {};
	// beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	// beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	// /*  */
	// result = vkBeginCommandBuffer(
	// 	swapChain.commandBuffers[vulkancore->currentFrame],
	// 	&beginInfo);
	// if (result != VK_SUCCESS)
	// 	throw RuntimeException(
	// 		fvformatf("vkBeginCommandBuffer failed - %d", result));
	swapChain.commandBuffers.resize(swapChain.swapChainFramebuffers.size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = vulkancore->cmd_pool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)swapChain.commandBuffers.size();

	if (vkAllocateCommandBuffers(vulkancore->device, &allocInfo,
									swapChain.commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	//TODO resolve and move the the chain 
	const int MAX_FRAMES_IN_FLIGHT = 2;
	vulkancore->imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	vulkancore->renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	vulkancore->inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	vulkancore->imagesInFlight.resize(swapChain.swapChainImages.size(), VK_NULL_HANDLE); //TODO resolve vulkancore->swapChainImages.size()

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(vulkancore->device, &semaphoreInfo, nullptr, &vulkancore->imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(vulkancore->device, &semaphoreInfo, nullptr, &vulkancore->renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(vulkancore->device, &fenceInfo, nullptr, &vulkancore->inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
	

	for (size_t i = 0; i < swapChain.commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(swapChain.commandBuffers[i], &beginInfo) !=
			VK_SUCCESS) {
			throw std::runtime_error(
				"failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swapChain.renderPass;
		renderPassInfo.framebuffer = swapChain.swapChainFramebuffers[i];
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = swapChain.chainExtend;

		VkClearValue clearColor = {0.0f, 1.0f, 0.0f, 1.0f};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(swapChain.commandBuffers[i], &renderPassInfo,
								VK_SUBPASS_CONTENTS_INLINE);

		// vkCmdBindPipeline(swapChain.cswapChain.ommandBuffers[i],
		// 					VK_PIPELINE_BIND_POINT_GRAPHICS,
		// 					graphicsPipeline);


		//vkCmdDraw(swapChain.commandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(swapChain.commandBuffers[i]);

		VkClearColorValue clearColor_ = {1,0,1,1};
		/*  */
		VkImageSubresourceRange imageRange = {};
		imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageRange.levelCount = 1;
		imageRange.layerCount = 1;


		vkCmdClearColorImage(swapChain.commandBuffers[i],
		 					 swapChain.swapChainImages[i], VK_IMAGE_LAYOUT_GENERAL,
		 					 &clearColor_, 1, &imageRange);

		if (vkEndCommandBuffer(swapChain.commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

VKRenderWindow::~VKRenderWindow(void){
	/*	Release all the resources.	*/
	//TODO add the swap chain to be part of the window rather than 
}

void VKRenderWindow::show() { SDL_ShowWindow(this->window); }

void VKRenderWindow::hide() { SDL_HideWindow(this->window); }

void VKRenderWindow::close(void) { SDL_DestroyWindow(this->window); }

ViewPort *VKRenderWindow::getViewPort(void){
	this->renderer->getView(0);
}
FrameBuffer *VKRenderWindow::getDefaultFrameBuffer(){
    throw NotSupportedException();
}

void VKRenderWindow::swapBuffer() {
	VkResult result;
	uint32_t imageIndex;

	const VKRenderInterface *core = *this->renderer;

	vkWaitForFences(getDevice(), 1, &this->inFlightFences[swapChain.currentFrame], VK_TRUE, UINT64_MAX);

	/*  */
	result = vkAcquireNextImageKHR(getDevice(), swapChain.swapchain, UINT64_MAX,
								   this->imageAvailableSemaphores[swapChain.currentFrame], VK_NULL_HANDLE,
								   &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		/*  Recreate.   */
		recreateSwapChain();
          return;
	}
	else if (result != VK_SUCCESS)
		throw RuntimeException(
			fvformatf("Failed to acquire next image - %d", result));

	if (this->imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(getDevice(), 1, &this->imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	this->imagesInFlight[imageIndex] = this->inFlightFences[swapChain.currentFrame];

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {this->imageAvailableSemaphores[swapChain.currentFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &swapChain.commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = {this->renderFinishedSemaphores[swapChain.currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(getDevice(), 1, &this->inFlightFences[swapChain.currentFrame]);

	if (vkQueueSubmit(queue, 1, &submitInfo, this->inFlightFences[swapChain.currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	/**/
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapChain.swapchain;
	/*	*/
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	/*	*/
	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(core->presentQueue, &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		// framebufferResized = false;
		recreateSwapChain();
	} else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	/*  Compute current frame.  */
	swapChain.currentFrame = (swapChain.currentFrame + 1) % 2;

}

void VKRenderWindow::createSwapChain(void) {
	/*  */
#ifdef VK_USE_PLATFORM_XLIB_KHR

#elif defined(VK_USE_PLATFORM_WIN32_KHR)
//	vkCreateWin32SurfaceKHR
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
	//	// vkCreateAndroidSurfaceKHR
#endif
	VulkanCore *vulkancore = (VulkanCore *)renderer->getData();
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(vulkancore->gpu, this->surface);
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
	/*  Allocate swapchain. */

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	QueueFamilyIndices indices = findQueueFamilies(vulkancore->gpu, this->surface);
	uint32_t queueFamilyIndices[] = {indices.graphicsFamily, indices.presentFamily};

	/*  */
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = this->surface;

	/*  */
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	/*  */
	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	/*  */
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode; //(presentModes);
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	/*  Create swapchain.   */
	vkCreateSwapchainKHR(getDevice(), &createInfo, NULL, &this->swapChain.swapchain);

	/*  Get the image associated with the swap chain.   */
	uint32_t nrChainImageCount = 1;
	vkGetSwapchainImagesKHR(getDevice(), this->swapChain.swapchain, &nrChainImageCount, NULL);

	this->swapChain.swapChainImages.resize(nrChainImageCount);
	vkGetSwapchainImagesKHR(getDevice(), this->swapChain.swapchain, &nrChainImageCount,
									 this->swapChain.swapChainImages.data());
	this->swapChain.swapChainImageFormat = surfaceFormat.format;
	this->swapChain.chainExtend = extent;

	/*	*/
	this->swapChain.swapChainImageViews.resize(this->swapChain.swapChainImages.size());

	for (size_t i = 0; i < this->swapChain.swapChainImages.size(); i++) {
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = this->swapChain.swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = this->swapChain.swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		vkCreateImageView(getDevice(), &createInfo, nullptr, &this->swapChain.swapChainImageViews[i]);
	}

	/*	Renderpass	*/

	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = this->swapChain.swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	vkCreateRenderPass(getDevice(), &renderPassInfo, nullptr, &this->swapChain.renderPass);

	/*	Framebuffer.	*/
	// TODO add support.
	this->swapChain.swapChainFramebuffers.resize(this->swapChain.swapChainImageViews.size());

	for (size_t i = 0; i < this->swapChain.swapChainImageViews.size(); i++) {
		VkImageView attachments[] = {this->swapChain.swapChainImageViews[i]};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = this->swapChain.renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = this->swapChain.chainExtend.width;
		framebufferInfo.height = this->swapChain.chainExtend.height;
		framebufferInfo.layers = 1;

		vkCreateFramebuffer(getDevice(), &framebufferInfo, nullptr, &this->swapChain.swapChainFramebuffers[i]);
	}

	/*	Command buffers*/
	/*  */
	this->swapChain.commandBuffers.resize(this->swapChain.swapChainFramebuffers.size());
	VkCommandBufferAllocateInfo cmdBufAllocInfo = {};
	cmdBufAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufAllocInfo.commandPool = this->cmd_pool;
	cmdBufAllocInfo.commandBufferCount = this->swapChain.swapChainImages.size();
	cmdBufAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	/*  */
	// this->swapChain.commandBuffers = (VkCommandBuffer *)malloc(
	// 	this->swapChain.swapChainImages.size() * sizeof(VkCommandBuffer));
	vkAllocateCommandBuffers(getDevice(), &cmdBufAllocInfo, this->swapChain.commandBuffers.data());
}

void VKRenderWindow::recreateSwapChain(void) {
	vkDeviceWaitIdle(getDevice());

	cleanSwapChain();

	createSwapChain();
}

void VKRenderWindow::cleanSwapChain(void) {
	for (auto framebuffer : swapChain.swapChainFramebuffers) {
		vkDestroyFramebuffer(getDevice(), framebuffer, nullptr);
	}

	vkFreeCommandBuffers(getDevice(), this->cmd_pool, static_cast<uint32_t>(swapChain.commandBuffers.size()),
						 swapChain.commandBuffers.data());

	vkDestroyRenderPass(getDevice(), swapChain.renderPass, nullptr);

	for (auto imageView : swapChain.swapChainImageViews) {
		vkDestroyImageView(getDevice(), imageView, nullptr);
	}

	vkDestroySwapchainKHR(getDevice(), this->swapChain.swapchain, nullptr);
}

VkDevice VKRenderWindow::getDevice(void) const {
	const VKRenderInterface *vulkancore = *this->renderer;
	return vulkancore->device;
}
VkFramebuffer VKRenderWindow::getDefaultFrameBuffer(void) const {
	return this->swapChain.swapChainFramebuffers[this->swapChain.currentFrame];
}

VkCommandBuffer VKRenderWindow::getCurrentCommandBuffer(void) const {
	return this->swapChain.commandBuffers[this->swapChain.currentFrame];
}
VkRenderPass VKRenderWindow::getDefaultRenderPass(void) const { return this->swapChain.renderPass; }
VkCommandPool VKRenderWindow::getGraphicCommadnPool(void) const { return this->cmd_pool; }
VkImage VKRenderWindow::getDefaultImage(void) const {
	return this->swapChain.swapChainImages[this->swapChain.currentFrame];
}

VkPhysicalDevice VKRenderWindow::physicalDevice() const {
	//return renderer->gpu;
	// physicalDevices[0];
	return NULL;
}
std::vector<VkPhysicalDevice> VKRenderWindow::getPhyiscalDevices(void) {}

void VKRenderWindow::setPosition(int x, int y) {
	SDL_SetWindowPosition(this->window, x, y);
}

void VKRenderWindow::setSize(int width, int height) {
	/*	TODO determine if it shall update framebuffera as well.	*/
	SDL_SetWindowSize(this->window, width, height);
}

void VKRenderWindow::vsync(bool state) { SDL_GL_SetSwapInterval(state); }

bool VKRenderWindow::assertConfigAttributes(const fragcore::IConfig *iConfig) {

	return false;
}

float VKRenderWindow::getGamma(void) const {
	ushort ramp[256 * 3];
	int err = SDL_GetWindowGammaRamp(this->window, &ramp[256 * 0],
									 &ramp[256 * 1], &ramp[256 * 2]);
	if (err == -1) throw NotSupportedException(SDL_GetError());

	return this->computeGammaExponent(ramp);
}

void VKRenderWindow::setGamma(float gamma) {
	Uint16 ramp[256 * 3] = {0};

	this->calculateGammaLookupTable(gamma, ramp);
	int err = SDL_SetWindowGammaRamp(this->window, &ramp[256 * 0],
									 &ramp[256 * 1], &ramp[256 * 2]);
	if (err == -1)
		throw NotSupportedException(fvformatf(
			"Failed to set window gamma %f: %s", gamma, SDL_GetError()));
}

//VKRenderWindow::~RendererWindow(void) {}

void VKRenderWindow::getPosition(int *x, int *y) const {
	SDL_GetWindowPosition(this->window, x, y);
}

void VKRenderWindow::getSize(int *width, int *height) const {
	SDL_GetWindowSize(this->window, width, height);
}

fragcore::Display *VKRenderWindow::getCurrentDisplay(void) const {
	int index;
	index = SDL_GetWindowDisplayIndex(this->window);
	return WindowManager::getInstance()->getDisplay(index);
}

void VKRenderWindow::createWindow(int x, int y, int width, int height,
								  const char *api) {
	window = SDL_CreateWindow("", x, y, width, height,
								SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN);

	/*  */
	if (window == NULL)
		throw RuntimeException(fvformatf(
			"Failed to create window %s for API %s", SDL_GetError(), api));
	this->api = api;
}

void VKRenderWindow::useWindow(void *pdata) {

}

void VKRenderWindow::setTitle(const char *title) {
	SDL_SetWindowTitle(this->window, title);
}

const char *VKRenderWindow::getTitle(void) const {
	return SDL_GetWindowTitle(this->window);
}

void VKRenderWindow::resizable(bool resizable) {
	SDL_SetWindowResizable(this->window, (SDL_bool)resizable);
}

void VKRenderWindow::setFullScreen(bool fullscreen) {
	// TODO add option for using either of the modes.
	if (fullscreen)
		SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else
		SDL_SetWindowFullscreen(this->window, 0);
}

void VKRenderWindow::setFullScreen(Display &display){
	
}

bool VKRenderWindow::isFullScreen(void) const {}

void VKRenderWindow::setBordered(bool bordered) {
	SDL_SetWindowBordered(this->window, (SDL_bool)bordered);
}

void VKRenderWindow::setMinimumSize(int width, int height) {
	SDL_SetWindowMinimumSize(this->window, width, height);
}
void VKRenderWindow::getMinimumSize(int *width, int *height){
	
}


void VKRenderWindow::setMaximumSize(int width, int height) {
	SDL_SetWindowMaximumSize(this->window, width, height);
}
void VKRenderWindow::getMaximumSize(int *width, int *height){

}

void VKRenderWindow::focus(void) { SDL_SetWindowInputFocus(this->window); }

void VKRenderWindow::restore(void) { SDL_RestoreWindow(this->window); }

void VKRenderWindow::maximize(void) { SDL_MaximizeWindow(this->window); }

void VKRenderWindow::minimize(void) { SDL_MinimizeWindow(this->window); }

intptr_t VKRenderWindow::getNativePtr(void) const {
//     SDL_SysWMinfo info;

//     SDL_VERSION(
//         &info.version); /* initialize info structure with SDL version info */

//     if (SDL_GetWindowWMInfo(window,
//                             &info)) { /* the call returns true on success */
//         /* success */
//         switch (info.subsystem) {
//             case SDL_SYSWM_UNKNOWN:
//                 return 0;
//             case SDL_SYSWM_WINDOWS:
// #if defined(SDL_VIDEO_DRIVER_WINDOWS)
//                 return info.info.win.window;
// #endif
//             case SDL_SYSWM_X11:
// #if defined(SDL_VIDEO_DRIVER_X11)
//                 return info.info.x11.window;
// #endif
// #if SDL_VERSION_ATLEAST(2, 0, 3)
//             case SDL_SYSWM_WINRT:
// #endif
//             case SDL_SYSWM_DIRECTFB:
//             case SDL_SYSWM_COCOA:
//             case SDL_SYSWM_UIKIT:
// #if SDL_VERSION_ATLEAST(2, 0, 2)
//             case SDL_SYSWM_WAYLAND:
// #if defined(SDL_VIDEO_DRIVER_WAYLAND)
//                 return (intptr_t)info.info.wl.surface;
// #endif
//             case SDL_SYSWM_MIR:
// #if defined(SDL_VIDEO_DRIVER_MIR)
//                 return (intptr_t)info.info.mir.surface;
// #endif
// #endif
// #if SDL_VERSION_ATLEAST(2, 0, 4)
//             case SDL_SYSWM_ANDROID:
// #endif
// #if SDL_VERSION_ATLEAST(2, 0, 5)
//             case SDL_SYSWM_VIVANTE:
//                 break;
// #endif
//         }
//     } else
//         throw RuntimeException(fvformatf("%s", SDL_GetError()));
//     throw NotImplementedException("Window format not implemented");
}

void VKRenderWindow::setIcon(void *pVoid) {}

void *VKRenderWindow::getIcon(void) const { return nullptr; }
