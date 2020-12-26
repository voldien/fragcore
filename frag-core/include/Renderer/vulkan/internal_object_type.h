#ifndef _FRAGVIEW_INTERNAL_OBJECT_TYPE_H_
#define _FRAGVIEW_INTERNAL_OBJECT_TYPE_H_ 1

/*	Expose the correct platform specific surface creation functions.	*/
#if defined(FV_UNIX)
	#define VK_USE_PLATFORM_XLIB_KHR
	#define VK_USE_PLATFORM_WAYLAND_KHR
#elif defined(FV_WINDOW)
	#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(FV_ANDROID)
	#define VK_USE_PLATFORM_ANDROID_KHR
#elif defined(FV_MACOSX)
	#define VK_USE_PLATFORM_MACOS_MVK
#endif

#include"Renderer/RenderDesc.h"
#include"../../Core/Ref.h"
#include"Renderer/Buffer.h"
#include"Renderer/Sampler.h"
#include<vulkan/vulkan.h>
#include<SDL2/SDL.h>
#include<vector>

#define ArraySize(a) (sizeof(a)/sizeof(*a))
namespace fragcore {

	typedef struct _SwapchainBuffers {
		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		SwapChainSupportDetails details;    /*  */

		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::vector<VkCommandBuffer> commandBuffers;
		VkFormat swapChainImageFormat;
		VkRenderPass renderPass;
		VkCommandBuffer *currentBuffer;
		VkSwapchainKHR swapchain;           /*  */
		VkExtent2D chainExtend;             /*  */
	} SwapchainBuffers;

	typedef struct vulkan_core_t {
		/*	*/
		std::vector<RendererWindow*> windows;
		SDL_Window *window;	//TODO remove
		Capability capabilityCached;
		/*	*/
		VkInstance inst;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkDebugReportCallbackEXT debugReport;
		/*  Physical device.    */
		VkPhysicalDevice gpu;
		std::vector<VkPhysicalDevice> GPUs;


		/*  */
		VkDevice device;
		VkQueue queue;	//TODO rename graphicsQueue
		VkQueue presentQueue;

		/*  */
		VkPhysicalDeviceProperties gpu_props;
		VkQueueFamilyProperties *queue_props;
		uint32_t graphics_queue_node_index;

		VkSurfaceKHR surface;

		/*  Collection of swap chain variables. */
		SwapchainBuffers *swapChain;

		VkCommandPool cmd_pool;
		VkCommandPool compute_pool;
		VkCommandPool transfer_pool;

		VkCommandBuffer setup_cmd;            /* Command Buffer for initialization commands	*/
		VkCommandBuffer draw_cmd;            /* Command Buffer for drawing commands			*/

		VkPipelineLayout pipeline_layout;
		VkDescriptorSetLayout desc_layout;

		VkPipelineCache pipelineCache;
		VkRenderPass render_pass;

		/*  */
		VkPipeline current_pipeline;

		/*  */
		//VkSemaphore imageAvailableSemaphore;
		//VkSemaphore renderFinishedSemaphore;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		//VkQueueFamilyIndices indices

		/*  */
		uint32_t num_physical_devices;
		VkPhysicalDevice *physical_devices;
		uint32_t queue_count;
		uint32_t enabled_extension_count;
		uint32_t enabled_layer_count;
		VkExtensionProperties *extension_names;
		bool validate;
		void *device_validation_layers;
		bool enableValidationLayers;
		bool enableDebugTracer;
		uint32_t currentFrame;
		uint32_t numBackBufferFrames;
		float color[4];             /*  Clear color.    */
		bool useGamma;
		VkPhysicalDeviceMemoryProperties memProperties;
		ShaderLanguage languageSupport;
		Capability capability;
	} VulkanCore;

	typedef struct opengl_viewport_t {
		unsigned int viewport;
	} VKViewPort;

	/**
	 *
	 */
	typedef struct vulkan_texture_object_t {
		VulkanCore *vulkanCore;
		TextureDesc desc;
		unsigned int target;
		VkImage texture;
		VkDeviceMemory imageMemory;
		VkSampler sampler;
		VkImageView view;
	} VKTextureObject;

	/**
	 *
	 */
	typedef struct vulkan_shader_object_t {
		VulkanCore *vulkanCore;
		VkPipeline graphicsPipeline;
		VkPipelineLayout pipelineLayout;
		unsigned int ver;
		unsigned int fra;
		unsigned int geo;
		unsigned int tesse;
		unsigned int tessc;
	} VKShaderObject;


	/**
	 *
	 */
	typedef struct vulkan_buffer_object_t {
		VulkanCore *vulkanCore;
		BufferDesc desc;
		unsigned int target;
		unsigned int base;
		VkBuffer buffer;
		VkDeviceMemory vertexBufferMemory;
	} VKBufferObject;


	/**
	 *
	 */
	typedef struct vulkan_geometry_object_t {
		VulkanCore *vulkanCore;
		GeometryDesc desc;
		unsigned int mode;
		unsigned int vao;
		unsigned int indicesType;
		Buffer *indicesbuffer;
		Buffer *vertexbuffer;

	} VKGeometryObject;

	/**
	 *
	 */
	typedef struct vulkan_framebuffer_object_t {
		VulkanCore *vulkanCore;
		FrameBufferDesc desc;
		unsigned int framebuffer;
		VKTextureObject *textures;
	} VKFrameBufferObject;
}

// extern void createInstance(fragcore::VulkanCore *vulkanCore, IConfig *config);
// extern void createDebugMessenger(fragcore::VulkanCore *vulkanCore, IConfig  *config);
// extern void createSurface(fragcore::VulkanCore *vulkanCore, IConfig *config);

/*  Helper functions.   */
extern uint32_t findMemoryType(fragcore::VulkanCore *vulkanCore, uint32_t typeFilter, VkMemoryPropertyFlags properties);

extern void
createBuffer(fragcore::VulkanCore *vulkanCore, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
			 VkBuffer &buffer, VkDeviceMemory &bufferMemory);

extern VkImageView createImageView(fragcore::VulkanCore *vulkanCore, VkImage image, VkFormat format);

extern VkShaderModule createShaderModule(VkDevice device, const char *pdata, size_t size);

//
extern bool isDeviceSuitable(VkPhysicalDevice device);

extern void selectDefaultDevices(std::vector<VkPhysicalDevice> &devices, std::vector<VkPhysicalDevice> &selectDevices);

//TODO improve to accomudate the configurations.
extern VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

extern VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

extern VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

struct QueueFamilyIndices {
	uint32_t graphicsFamily = -1;
	uint32_t presentFamily = -1;

	bool isComplete() {
		return graphicsFamily != -1 && presentFamily != -1;
	}
};

extern QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
											VkSurfaceKHR surface);

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

extern SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
// TODO move to the helper file.
extern unsigned int getTextureFormat(fragcore::TextureDesc::Format format);

extern unsigned int getTextureTarget(fragcore::TextureDesc::Target target);

extern unsigned int getTextureType(fragcore::TextureDesc::Type type);

extern unsigned int getBufferType(fragcore::BufferDesc::BufferType type);

extern unsigned int getBufferHint(fragcore::BufferDesc::BufferHint hint);

extern unsigned int getPrimitive(fragcore::GeometryDesc::Primitive primitive);

extern unsigned int getAttributeDataType(
	fragcore::GeometryDesc::AttributeType type);

extern unsigned int getState(unsigned int state);

#endif
