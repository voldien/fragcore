#ifndef FRAG_CORE_INTERNAL_IMP
#define FRAG_CORE_INTERNAL_IMP 1
#endif
#include "VKRenderWindow.h"
#include "internal_object_type.h"
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_vulkan.h>

#include <SDLWindowManager.h>
#include <fmt/core.h>
#include <vulkan/vulkan.h>
using namespace fragcore;

VKRenderWindow::VKRenderWindow(Ref<VKRenderInterface> &renderer) : renderer(renderer) {

	// TODO relocate with the modules!
	if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
		throw cxxexcept::RuntimeException("Failed to init subsystem {}", SDL_GetError());
	}

	int width = 800;
	int height = 600;

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE |
													 SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_INPUT_FOCUS);
	this->window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
	if (window == NULL) {
		throw cxxexcept::RuntimeException("failed create window - {}", SDL_GetError());
	}

	/*  Create surface. */
	this->surface = this->createSurface();
	this->swapChain = SwapchainBuffers();

	/*  Create command pool.    */
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
	cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolCreateInfo.queueFamilyIndex = renderer->device->getDefaultGraphicQueueIndex();
	cmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	/*  Create command pool.    */
	VKS_VALIDATE(vkCreateCommandPool(getDevice(), &cmdPoolCreateInfo, NULL, &this->cmd_pool));

	/*	Create swap chain.	*/
	createSwapChain();

	const int MAX_FRAMES_IN_FLIGHT = this->getSwapChainImageCount();
	this->imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	this->renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	this->inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	/*	*/
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	/*	*/
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VKS_VALIDATE(vkCreateSemaphore(getDevice(), &semaphoreInfo, nullptr, &this->imageAvailableSemaphores[i]));
		VKS_VALIDATE(vkCreateSemaphore(getDevice(), &semaphoreInfo, nullptr, &this->renderFinishedSemaphores[i]));
		VKS_VALIDATE(vkCreateFence(getDevice(), &fenceInfo, nullptr, &this->inFlightFences[i]));
	}
}

VKRenderWindow::~VKRenderWindow() {
	/*	Release all the resources.	*/
	// TODO add the swap chain to be part of the window rather than
}

void VKRenderWindow::show() { SDL_ShowWindow(this->window); }

void VKRenderWindow::hide() { SDL_HideWindow(this->window); }

void VKRenderWindow::close() { SDL_DestroyWindow(this->window); }

void VKRenderWindow::setPosition(int x, int y) { SDL_SetWindowPosition(this->window, x, y); }

void VKRenderWindow::setSize(int width, int height) {
	/*	TODO determine if it shall update framebuffera as well.	*/
	SDL_SetWindowSize(this->window, width, height);
}

void VKRenderWindow::vsync(bool state) { SDL_GL_SetSwapInterval(state); }

bool VKRenderWindow::assertConfigAttributes(const fragcore::IConfig *iConfig) { return false; }

float VKRenderWindow::getGamma() const {
	uint16_t ramp[256 * 3];
	int err = SDL_GetWindowGammaRamp(this->window, &ramp[256 * 0], &ramp[256 * 1], &ramp[256 * 2]);
	if (err == -1)
		throw NotSupportedException(SDL_GetError());

	return this->computeGammaExponent<float, uint16_t>(ramp);
}

void VKRenderWindow::setGamma(float gamma) {
	uint16_t ramp[256 * 3] = {0};

	this->calculateGammaLookupTable<float, uint16_t>(gamma, ramp);
	int err = SDL_SetWindowGammaRamp(this->window, &ramp[256 * 0], &ramp[256 * 1], &ramp[256 * 2]);
	if (err == -1) {
		throw NotSupportedException("Failed to set window gamma {}: {}", gamma, SDL_GetError());
	}
}

// VKRenderWindow::~RendererWindow() {}

void VKRenderWindow::getPosition(int *x, int *y) const { SDL_GetWindowPosition(this->window, x, y); }

void VKRenderWindow::getSize(int *width, int *height) const { SDL_GetWindowSize(this->window, width, height); }

int VKRenderWindow::width() const {
	int width, height;
	getSize(&width, &height);
	return width;
}
int VKRenderWindow::height() const {
	int width, height;
	getSize(&width, &height);
	return height;
}

fragcore::Display *VKRenderWindow::getCurrentDisplay() const {
	int index;
	index = SDL_GetWindowDisplayIndex(this->window);
	// return WindowManager::getInstance()->getDisplay(index);
	return nullptr;
}

void VKRenderWindow::createWindow(int x, int y, int width, int height, const char *api) {
	window = SDL_CreateWindow("", x, y, width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN);

	/*  */
	if (window == nullptr)
		throw RuntimeException("Failed to create window {} for API {}", SDL_GetError(), api);
	this->api = api;
}

void VKRenderWindow::useWindow(void *pdata) {}

void VKRenderWindow::setTitle(const std::string &title) { SDL_SetWindowTitle(this->window, title.c_str()); }

std::string VKRenderWindow::getTitle() const { return SDL_GetWindowTitle(this->window); }

void VKRenderWindow::resizable(bool resizable) { SDL_SetWindowResizable(this->window, (SDL_bool)resizable); }

void VKRenderWindow::setFullScreen(bool fullscreen) {
	// TODO add option for using either of the modes.
	if (fullscreen)
		SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else
		SDL_SetWindowFullscreen(this->window, 0);
}

void VKRenderWindow::setFullScreen(Display &display) {}

bool VKRenderWindow::isFullScreen() const { return false; }

void VKRenderWindow::setBordered(bool bordered) { SDL_SetWindowBordered(this->window, (SDL_bool)bordered); }

void VKRenderWindow::setMinimumSize(int width, int height) { SDL_SetWindowMinimumSize(this->window, width, height); }
void VKRenderWindow::getMinimumSize(int *width, int *height) {}

void VKRenderWindow::setMaximumSize(int width, int height) { SDL_SetWindowMaximumSize(this->window, width, height); }
void VKRenderWindow::getMaximumSize(int *width, int *height) {}

void VKRenderWindow::focus() { SDL_SetWindowInputFocus(this->window); }

void VKRenderWindow::restore() { SDL_RestoreWindow(this->window); }

void VKRenderWindow::maximize() { SDL_MaximizeWindow(this->window); }

void VKRenderWindow::minimize() { SDL_MinimizeWindow(this->window); }

intptr_t VKRenderWindow::getNativePtr() const {
	SDL_SysWMinfo info = {};

	SDL_VERSION(&info.version); /* initialize info structure with SDL version info */

	if (SDL_GetWindowWMInfo(this->window, &info)) { /* the call returns true on success */
		/* success */
		switch (info.subsystem) {
		case SDL_SYSWM_UNKNOWN:
			return 0;
		case SDL_SYSWM_OS2:
			return 0;
		case SDL_SYSWM_WINDOWS:
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
			return info.info.win.window;
#endif
		case SDL_SYSWM_X11:
#if defined(SDL_VIDEO_DRIVER_X11)
			return info.info.x11.window;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 3)
		case SDL_SYSWM_WINRT:
#endif
		case SDL_SYSWM_DIRECTFB:
		case SDL_SYSWM_COCOA:
		case SDL_SYSWM_UIKIT:
#if SDL_VERSION_ATLEAST(2, 0, 2)
		case SDL_SYSWM_WAYLAND:
#if defined(SDL_VIDEO_DRIVER_WAYLAND)
			return (intptr_t)info.info.wl.surface;
#endif
		case SDL_SYSWM_MIR:
#if defined(SDL_VIDEO_DRIVER_MIR)
			return (intptr_t)info.info.mir.surface;
#endif
#endif
#if SDL_VERSION_ATLEAST(2, 0, 4)
		case SDL_SYSWM_ANDROID:
#endif
#if SDL_VERSION_ATLEAST(2, 0, 5)
		case SDL_SYSWM_VIVANTE:
			break;
#endif
		}
	} else
		throw RuntimeException(fmt::format("{}", SDL_GetError()));
	return 0;
}

void VKRenderWindow::setIcon(void *pVoid) {}

void *VKRenderWindow::getIcon() const { return nullptr; }

void VKRenderWindow::swapBuffer() {
	VkResult result;

	vkWaitForFences(this->getDevice(), 1, &this->inFlightFences[this->swapChain.currentFrame], VK_TRUE, UINT64_MAX);

	int width, height;
	this->getSize(&width, &height);
	// Check if the size is correct.
	if (this->getSwapChain().width != width || this->getSwapChain().height != height) {
		this->recreateSwapChain();
		return;
	}

	/*  */
	uint32_t imageIndex;
	result = vkAcquireNextImageKHR(getDevice(), this->swapChain.swapchain, UINT64_MAX,
								   this->imageAvailableSemaphores[this->swapChain.currentFrame], VK_NULL_HANDLE,
								   &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	} else {
		VKS_VALIDATE(result);
	}

	if (this->imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(getDevice(), 1, &this->imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	/*	*/
	this->imagesInFlight[imageIndex] = this->inFlightFences[this->swapChain.currentFrame];

	/*	*/
	VkSemaphore signalSemaphores[] = {this->renderFinishedSemaphores[this->swapChain.currentFrame]};

	vkResetFences(getDevice(), 1, &this->inFlightFences[this->swapChain.currentFrame]);

	this->getVKDevice()->submitCommands(getDefaultGraphicQueue(), {this->swapChain.commandBuffers[imageIndex]},
										{this->imageAvailableSemaphores[this->swapChain.currentFrame]},
										{this->renderFinishedSemaphores[this->swapChain.currentFrame]},
										this->inFlightFences[this->swapChain.currentFrame],
										{VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT});

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	/*	*/
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &this->swapChain.swapchain;

	/*	*/
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	/*	*/
	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(this->renderer->device->getDefaultPresent(), &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		// framebufferResized = false;
		recreateSwapChain();
	} else if (result != VK_SUCCESS) {
		throw cxxexcept::RuntimeException("failed to present swap chain image!");
	}

	/*  Compute current frame.  */
	this->swapChain.currentFrame =
		(this->swapChain.currentFrame + 1) % std::min((uint32_t)this->inFlightFences.size(), getSwapChainImageCount());
}

void VKRenderWindow::createSwapChain() {
	const std::shared_ptr<PhysicalDevice> &physicalDevice = renderer->device->getPhysicalDevice(0);

	/*  */
	VKHelper::SwapChainSupportDetails swapChainSupport =
		VKHelper::querySwapChainSupport(physicalDevice->getHandle(), this->surface);

	/*	TODO add option support.	*/
	this->surfaceFormat = VKHelper::selectSurfaceFormat(swapChainSupport.formats, swapChainSupport.formats,
														VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
	// TODO add support to determine which present mode.
	std::vector<VkPresentModeKHR> requestedPresentModes = {VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_MAILBOX_KHR,
														   VK_PRESENT_MODE_FIFO_KHR};
	this->presentMode = VKHelper::chooseSwapPresentMode(swapChainSupport.presentModes, requestedPresentModes);
	int width, height;
	this->getSize(&width, &height);
	const VkExtent2D extent =
		VKHelper::chooseSwapExtent(swapChainSupport.capabilities, {(uint32_t)width, (uint32_t)height});

	/*	Reset frame counter.	*/
	this->swapChain.currentFrame = 0;

	/*	TODO evoluate if this is thec correct.	*/
	/*	Compute number of image to use in the swapchain.	*/
	uint32_t imageCount =
		std::max((uint32_t)swapChainSupport.capabilities.minImageCount, (uint32_t)1); /*	Atleast one.	*/
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		/*	Clamp it to number of semaphores/fences.	*/
		imageCount = std::max(swapChainSupport.capabilities.maxImageCount, (uint32_t)imagesInFlight.size());
	}

	/*	*/
	const VKHelper::QueueFamilyIndices indices =
		VKHelper::findQueueFamilies(physicalDevice->getHandle(), this->surface);
	std::vector<uint32_t> queueFamilyIndices = {(uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily};

	/*  */
	VkSwapchainCreateInfoKHR createSwapChainInfo = {};
	createSwapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createSwapChainInfo.pNext = nullptr;
	createSwapChainInfo.surface = this->surface;

	/*  Image configurations.	*/
	createSwapChainInfo.minImageCount = imageCount;
	createSwapChainInfo.imageFormat = surfaceFormat.format;
	createSwapChainInfo.imageColorSpace = surfaceFormat.colorSpace;
	createSwapChainInfo.imageExtent = extent;
	createSwapChainInfo.imageArrayLayers = 1;
	createSwapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	/*  */
	if (indices.graphicsFamily != indices.presentFamily) {
		createSwapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createSwapChainInfo.queueFamilyIndexCount = queueFamilyIndices.size();
		createSwapChainInfo.pQueueFamilyIndices = queueFamilyIndices.data();
	} else {
		createSwapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	/*  */
	createSwapChainInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createSwapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createSwapChainInfo.presentMode = presentMode;
	createSwapChainInfo.clipped = VK_TRUE;

	/*	TODO add support */
	createSwapChainInfo.oldSwapchain = VK_NULL_HANDLE;

	/*  Create swapchain.   */
	VKS_VALIDATE(vkCreateSwapchainKHR(getDevice(), &createSwapChainInfo, nullptr, &this->swapChain.swapchain));

	/*  Get the image associated with the swap chain.   */
	uint32_t nrChainImageCount = 1;
	VKS_VALIDATE(vkGetSwapchainImagesKHR(getDevice(), this->swapChain.swapchain, &nrChainImageCount, nullptr));

	this->swapChain.swapChainImages.resize(nrChainImageCount);
	VKS_VALIDATE(vkGetSwapchainImagesKHR(getDevice(), this->swapChain.swapchain, &nrChainImageCount,
										 this->swapChain.swapChainImages.data()));

	this->swapChain.swapChainImageFormat = surfaceFormat.format;
	this->swapChain.chainExtend = extent;
	this->imagesInFlight.resize(this->swapChain.swapChainImages.size(), VK_NULL_HANDLE);
	this->swapChain.width = width;
	this->swapChain.height = height;

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

		VKS_VALIDATE(vkCreateImageView(getDevice(), &createInfo, nullptr, &this->swapChain.swapChainImageViews[i]));
	}

	VkFormat depthFormat = findDepthFormat();

	const VkPhysicalDeviceMemoryProperties &memProps = getVKDevice()->getPhysicalDevices()[0]->getMemoryProperties();

	VKHelper::createImage(getDevice(), this->swapChain.chainExtend.width, this->swapChain.chainExtend.height, 1,
						  depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
						  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memProps, this->swapChain.depthImage,
						  this->swapChain.depthImageMemory);
	this->swapChain.depthImageView = VKHelper::createImageView(
		getDevice(), this->swapChain.depthImage, VK_IMAGE_VIEW_TYPE_2D, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

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

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = depthFormat;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask =
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask =
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = attachments.size();
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VKS_VALIDATE(vkCreateRenderPass(getDevice(), &renderPassInfo, nullptr, &this->swapChain.renderPass));

	/*	Framebuffer.	*/
	// TODO add support.
	this->swapChain.swapChainFramebuffers.resize(this->swapChain.swapChainImageViews.size());

	for (size_t i = 0; i < this->swapChain.swapChainImageViews.size(); i++) {
		std::array<VkImageView, 2> attachments = {this->swapChain.swapChainImageViews[i],
												  this->swapChain.depthImageView};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = this->swapChain.renderPass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = this->swapChain.chainExtend.width;
		framebufferInfo.height = this->swapChain.chainExtend.height;
		framebufferInfo.layers = 1;

		VKS_VALIDATE(
			vkCreateFramebuffer(getDevice(), &framebufferInfo, nullptr, &this->swapChain.swapChainFramebuffers[i]));
	}

	/*	Command buffers*/
	this->swapChain.commandBuffers.resize(this->swapChain.swapChainFramebuffers.size());
	VkCommandBufferAllocateInfo cmdBufAllocInfo = {};
	cmdBufAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufAllocInfo.commandPool = this->cmd_pool;
	cmdBufAllocInfo.commandBufferCount = this->swapChain.swapChainImages.size();
	cmdBufAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	VKS_VALIDATE(vkAllocateCommandBuffers(getDevice(), &cmdBufAllocInfo, this->swapChain.commandBuffers.data()));
}

void VKRenderWindow::recreateSwapChain() {

	vkDeviceWaitIdle(getDevice());

	cleanSwapChain();

	createSwapChain();
}

void VKRenderWindow::cleanSwapChain() {
	for (auto framebuffer : swapChain.swapChainFramebuffers) {
		vkDestroyFramebuffer(getDevice(), framebuffer, nullptr);
	}
	swapChain.swapChainFramebuffers.clear();

	/*	*/
	vkFreeCommandBuffers(getDevice(), this->cmd_pool, static_cast<uint32_t>(swapChain.commandBuffers.size()),
						 swapChain.commandBuffers.data());
	swapChain.commandBuffers.clear();

	vkDestroyRenderPass(getDevice(), swapChain.renderPass, nullptr);

	/*	*/
	for (auto imageView : swapChain.swapChainImageViews) {
		vkDestroyImageView(getDevice(), imageView, nullptr);
	}
	swapChain.swapChainImageViews.clear();

	/*	Release depth/stencil.	*/
	vkDestroyImageView(getDevice(), swapChain.depthImageView, nullptr);
	vkDestroyImage(getDevice(), swapChain.depthImage, nullptr);
	vkFreeMemory(getDevice(), swapChain.depthImageMemory, nullptr);

	vkDestroySwapchainKHR(getDevice(), this->swapChain.swapchain, nullptr);
}

VkFormat VKRenderWindow::findDepthFormat() {
	return VKHelper::findSupportedFormat(
		physicalDevice(), {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
		VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

VkSurfaceKHR VKRenderWindow::createSurface() {
	VkSurfaceKHR surface;
	bool surfaceResult = SDL_Vulkan_CreateSurface(this->window, this->renderer->getInstance()->getHandle(), &surface);
	if (surfaceResult == SDL_FALSE)
		throw cxxexcept::RuntimeException("failed create vulkan surface - {}", SDL_GetError());
	return surface;
	// VkXlibSurfaceCreateInfoKHR createInfo{};
	// createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
	// //createInfo.window = proxyWindow->getNativePtr();
	// createInfo.pNext = nullptr;
	// // createInfo.dpy = proxyWindow->getNativePtr();
	// createInfo.flags = 0;

	// VkSurfaceKHR surface;
	// VKS_VALIDATE(vkCreateXlibSurfaceKHR(core->getHandle(), &createInfo, nullptr, &surface));

	return surface;
}
ViewPort *VKRenderWindow::getViewPort() { this->renderer->getView(0); }
FrameBuffer *VKRenderWindow::getDefaultFrameBuffer() { this->swapChain.frameBuffers[this->swapChain.currentFrame]; }

uint32_t VKRenderWindow::getSwapChainImageCount() const noexcept { return this->swapChain.swapChainImages.size(); }

uint32_t VKRenderWindow::getCurrentFrameIndex() const noexcept { return this->swapChain.currentFrame; }

VkDevice VKRenderWindow::getDevice() const noexcept { return renderer->device->getHandle(); }

VkFramebuffer VKRenderWindow::getDefaultFrameBuffer() const noexcept {
	return this->swapChain.swapChainFramebuffers[this->swapChain.currentFrame];
}

VkRenderPass VKRenderWindow::getDefaultRenderPass() const noexcept { return this->swapChain.renderPass; }
VkCommandPool VKRenderWindow::getGraphicCommandPool() const noexcept { return this->cmd_pool; }
VkImage VKRenderWindow::getDefaultImage() const {
	return this->swapChain.swapChainImages[this->swapChain.currentFrame];
}
VkImageView VKRenderWindow::getDefaultImageView() const {
	return this->swapChain.swapChainImageViews[this->swapChain.currentFrame];
}

VkFormat VKRenderWindow::getDefaultImageFormat() const noexcept { return this->swapChain.swapChainImageFormat; }

VkQueue VKRenderWindow::getDefaultGraphicQueue() const { return this->renderer->device->getDefaultGraphicQueue(); }

VkQueue VKRenderWindow::getDefaultComputeQueue() const { return this->renderer->device->getDefaultCompute(); }

const std::vector<VkImage> &VKRenderWindow::getSwapChainImages() const noexcept {
	return this->swapChain.swapChainImages;
}
const std::vector<VkImageView> &VKRenderWindow::getSwapChainImageViews() const noexcept {
	return this->swapChain.swapChainImageViews;
}

const std::shared_ptr<VKDevice> &VKRenderWindow::getVKDevice() const noexcept { return this->renderer->device; }
const std::shared_ptr<PhysicalDevice> VKRenderWindow::getPhysicalDevice() const noexcept {
	// return this->renderer->device->getPhysicalDevice();
}

VkPhysicalDevice VKRenderWindow::physicalDevice() const {

	return renderer->device->getPhysicalDevices()[0]->getHandle();
}

void VKRenderWindow::setPhysicalDevice(VkPhysicalDevice device) {
	// TODO recreate pipeline.
}
std::vector<VkQueue> VKRenderWindow::getQueues() const noexcept { return {}; }

const std::vector<VkPhysicalDevice> &VKRenderWindow::availablePhysicalDevices() const { return {}; }

VkCommandBuffer VKRenderWindow::getCurrentCommandBuffer() const noexcept {
	return this->swapChain.commandBuffers[getCurrentFrameIndex()];
}
size_t VKRenderWindow::getNrCommandBuffers() const noexcept { return this->swapChain.commandBuffers.size(); }

VkCommandBuffer VKRenderWindow::getCommandBuffers(unsigned int index) const noexcept {
	return this->swapChain.commandBuffers[index];
}

VkFramebuffer VKRenderWindow::getFrameBuffer(unsigned int index) const noexcept {
	return this->swapChain.swapChainFramebuffers[index];
}