/*
 *	FragCore - Fragment Core - Engine Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FRAGCORE_VULKAN_INTERNAL_OBJECT_TYPE_H_
#define _FRAGCORE_VULKAN_INTERNAL_OBJECT_TYPE_H_ 1
/*	Expose the correct platform specific surface creation functions.	*/

// #if defined(FV_UNIX)
// #define VK_USE_PLATFORM_XLIB_KHR
// #define VK_USE_PLATFORM_WAYLAND_KHR
// #elif defined(FV_WINDOW)
// #define VK_USE_PLATFORM_WIN32_KHR
// #elif defined(FV_ANDROID)
// #define VK_USE_PLATFORM_ANDROID_KHR
// #elif defined(FV_MACOSX)
// #define VK_USE_PLATFORM_MACOS_MVK
// #endif

#include "../Buffer.h"
#include "../RenderDesc.h"
#include "../Sampler.h"
#include <SDL2/SDL.h>
#include <VKDevice.h>
#include <vector>
#include <vulkan/vulkan.h>

namespace fragcore {
	class VKBuffer;
	class VKTexture;
	class VKSampler;
	class VKShader;
	class VKFrameBuffer;
	class VKViewport;
	class VKQuery;
	class VKCommandList;
	class VKRenderPipeline;
	class VKRenderInterface;
} // namespace fragcore

#define ArraySize(a) (sizeof(a) / sizeof(*a))
namespace fragcore {

	// typedef struct _SwapchainBuffers {
	// 	struct SwapChainSupportDetails {
	// 		VkSurfaceCapabilitiesKHR capabilities;
	// 		std::vector<VkSurfaceFormatKHR> formats;
	// 		std::vector<VkPresentModeKHR> presentModes;
	// 	};

	// 	SwapChainSupportDetails details;    /*  */

	// 	std::vector<VkImage> swapChainImages;
	// 	std::vector<VkImageView> swapChainImageViews;
	// 	std::vector<VkFramebuffer> swapChainFramebuffers;
	// 	std::vector<VkCommandBuffer> commandBuffers;
	// 	VkFormat swapChainImageFormat;
	// 	VkRenderPass renderPass;
	// 	VkCommandBuffer *currentBuffer;
	// 	VkSwapchainKHR swapchain;           /*  */
	// 	VkExtent2D chainExtend;             /*  */
	// } SwapchainBuffers;

	// typedef struct vulkan_core_t {
	// 	/*	*/
	// 	std::vector<RendererWindow*> windows;
	// 	SDL_Window *window;	//TODO remove
	// 	Capability capabilityCached;
	// 	/*	*/
	// 	VkInstance inst;
	// 	VkDebugUtilsMessengerEXT debugMessenger;
	// 	VkDebugReportCallbackEXT debugReport;
	// 	/*  Physical device.    */
	// 	VkPhysicalDevice gpu;
	// 	std::vector<VkPhysicalDevice> GPUs;

	// 	/*  */
	// 	VkDevice device;
	// 	VkQueue queue;	//TODO rename graphicsQueue
	// 	VkQueue presentQueue;

	// 	/*  */
	// 	VkPhysicalDeviceProperties gpu_props;
	// 	VkQueueFamilyProperties *queue_props;
	// 	uint32_t graphics_queue_node_index;

	// 	VkSurfaceKHR surface;

	// 	/*  Collection of swap chain variables. */
	// 	//SwapchainBuffers *swapChain;

	// 	VkCommandPool cmd_pool;
	// 	VkCommandPool compute_pool;
	// 	VkCommandPool transfer_pool;

	// 	VkCommandBuffer setup_cmd;            /* Command Buffer for initialization commands	*/
	// 	VkCommandBuffer draw_cmd;            /* Command Buffer for drawing commands			*/

	// 	VkPipelineLayout pipeline_layout;
	// 	VkDescriptorSetLayout desc_layout;

	// 	VkPipelineCache pipelineCache;
	// 	VkRenderPass render_pass;

	// 	/*  */
	// 	VkPipeline current_pipeline;

	// 	/*  */
	// 	//VkSemaphore imageAvailableSemaphore;
	// 	//VkSemaphore renderFinishedSemaphore;
	// 	std::vector<VkSemaphore> imageAvailableSemaphores;
	// 	std::vector<VkSemaphore> renderFinishedSemaphores;
	// 	std::vector<VkFence> inFlightFences;
	// 	std::vector<VkFence> imagesInFlight;
	// 	//VkQueueFamilyIndices indices

	// 	/*  */
	// 	uint32_t num_physical_devices;
	// 	VkPhysicalDevice *physical_devices;
	// 	uint32_t queue_count;
	// 	uint32_t enabled_extension_count;
	// 	uint32_t enabled_layer_count;
	// 	VkExtensionProperties *extension_names;
	// 	bool validate;
	// 	void *device_validation_layers;
	// 	bool enableValidationLayers;
	// 	bool enableDebugTracer;
	// 	uint32_t currentFrame;
	// 	uint32_t numBackBufferFrames;
	// 	float color[4];             /*  Clear color.    */
	// 	bool useGamma;
	// 	VkPhysicalDeviceMemoryProperties memProperties;
	// 	ShaderLanguage languageSupport;
	// 	Capability capability;
	// } VulkanCore;

	// typedef struct vulkan_viewport_t {
	// 	unsigned int viewport;
	// } VKViewPort;


	// /**
	//  *
	//  */
	// typedef struct vulkan_shader_object_t {
	// 	std::shared_ptr<fvkcore::VKDevice> device;
	// 	// VulkanCore *vulkanCore;
	// 	VkPipeline graphicsPipeline;
	// 	VkPipelineLayout pipelineLayout;
	// 	unsigned int ver;
	// 	unsigned int fra;
	// 	unsigned int geo;
	// 	unsigned int tesse;
	// 	unsigned int tessc;
	// } VKShaderObject;

	// typedef struct vulkan_pipeline_object_t {
	// 	std::shared_ptr<fvkcore::VKDevice> device;
	// 	// VulkanCore *vulkanCore;
	// 	VkPipeline graphicsPipeline;
	// 	VkPipelineLayout pipelineLayout;
	// 	VkDescriptorSet descriptorSet;
	// } VKPipelineObject;

	extern FVDECLSPEC unsigned int getTextureFormat(fragcore::TextureDesc::Format format);

	extern FVDECLSPEC unsigned int getTextureTarget(fragcore::TextureDesc::Target target);

	extern FVDECLSPEC unsigned int getTextureType(fragcore::TextureDesc::Type type);

	extern FVDECLSPEC unsigned int getBufferType(fragcore::BufferDesc::BufferType type);

	extern FVDECLSPEC unsigned int getBufferHint(fragcore::BufferDesc::BufferHint hint);

	extern FVDECLSPEC unsigned int getPrimitive(fragcore::GeometryDesc::Primitive primitive);

	extern FVDECLSPEC unsigned int getAttributeDataType(fragcore::GeometryDesc::AttributeType type);

	extern FVDECLSPEC unsigned int getState(unsigned int state);

} // namespace fragcore


#endif
