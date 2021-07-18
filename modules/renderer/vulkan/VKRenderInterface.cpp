#include "VKRenderInterface.h"
#include "Core/IConfig.h"
#include "Exception/InvalidArgumentException.h"
#include "Exception/RuntimeException.h"
#include "Renderer/RenderDesc.h"
#include "Utils/StringUtil.h"
#include "VKCommandList.h"
#include "VKRenderWindow.h"
#include "Window/WindowManager.h"
#include "internal_object_type.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_vulkan.h>
#include <Utils/StringUtil.h>
#include <VKHelper.h>
#include <VKUtil.h>
#include <climits>
#include <fmt/core.h>
#include <iostream>
#include <vector>
using namespace fragcore;

#define RENDER_VULKAN_MAJOR 0
#define RENDER_VULKAN_MINOR 1
#define RENDER_VULKAN_PATCH 0

// TODO enable
static bool validate_object_memeber(VKRenderInterface *renderer, RenderObject *object) {
	return renderer == object->getRenderer();
}

VKRenderInterface::VKRenderInterface(IConfig *config) {

	/*	Minimun requirements.	*/
	std::vector<const char *> instanceExtensionNames = {
		/*	*/
		VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
		VK_KHR_DISPLAY_EXTENSION_NAME,
	};
	const std::vector<const char *> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation",
	};

	VulkanCore *vulkancore = nullptr;
	IConfig setupConfig;
	if (config == nullptr) {
		setupConfig.set("debug", true);
		setupConfig.set("debug-tracer", true);
		setupConfig.set("gamma-correction", true);
	} else {
		// setupConfig = *config
	}

	/*  Initialize video support.   */
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
		throw RuntimeException(fmt::format("SDL_InitSubSystem failed, %s.\n", SDL_GetError()));
	}

	/*	Allocate private data structure for the renderer interface. */
	// vulkancore = new VulkanCore();
	// if (vulkancore == nullptr)
	// 	throw RuntimeException();

	/*  Store reference with the object. */
	// this->pdata = vulkancore;
	const std::unordered_map<const char *, bool> &requested_extensions{};

	this->core = std::make_shared<VulkanCore>(requested_extensions);
	std::vector<std::shared_ptr<PhysicalDevice>> devices = core->createPhysicalDevices();
	this->device = std::make_shared<VKDevice>(devices);

	/*  Determine layer validation. */
	// vulkancore->enableValidationLayers = setupConfig.get<bool>("debug");
	// vulkancore->enableValidationLayers = 1;
	// vulkancore->enableDebugTracer = setupConfig.get<bool>("debug-tracer");
	// vulkancore->useGamma = setupConfig.get<bool>("gamma-correction");

	// /*  Vulkan variables.   */
	// VkResult result;

	// /*  Check for supported extensions.*/
	// uint32_t extensionCount = 0;
	// if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr) != VK_SUCCESS) {
	// 	throw RuntimeException("Failed enumerate Vulkan extension properties");
	// }
	// vulkancore->extension_names = (VkExtensionProperties *)(malloc(sizeof(VkExtensionProperties) * extensionCount));
	// if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
	// 										   (VkExtensionProperties *)vulkancore->extension_names) != VK_SUCCESS) {
	// 	throw RuntimeException("Failed enumerate Vulkan extension properties");
	// }

	// /*  Check for supported validation layers.  */
	// uint32_t layerCount;
	// if (vkEnumerateInstanceLayerProperties(&layerCount, nullptr) != VK_SUCCESS)
	// 	throw RuntimeException("Failed enumerate Vulkan extension properties");
	// std::vector<VkLayerProperties> availableLayers(layerCount);
	// if (vulkancore->enableValidationLayers) {
	// 	if (vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()) != VK_SUCCESS)
	// 		throw RuntimeException("Failed enumerate Vulkan extension properties");

	// 	/*  Check if exists.    */
	// 	for (uint32_t i = 0; i < availableLayers.size(); i++) {
	// 	}
	// }

	// /*  TODO add support for loading requried extensions.   */
	// // TODO improve later.
	// /*  Create Vulkan window.   */
	// SDL_Window *tmpWindow = SDL_CreateWindow("", 0, 0, 1, 1, SDL_WINDOW_VULKAN);
	// if (tmpWindow == nullptr)
	// 	throw RuntimeException(fmt::format("failed create window - %s", SDL_GetError()));
	// unsigned int count;
	// if (!SDL_Vulkan_GetInstanceExtensions(tmpWindow, &count, nullptr))
	// 	throw RuntimeException(fmt::format("%s", SDL_GetError()));
	// unsigned int additional_extension_count = (unsigned int)instanceExtensionNames.size();
	// instanceExtensionNames.resize((size_t)(additional_extension_count + count));
	// if (!SDL_Vulkan_GetInstanceExtensions(tmpWindow, &count, &instanceExtensionNames[additional_extension_count]))
	// 	throw RuntimeException(fmt::format("failed SDL_Vulkan_GetInstanceExtensions - %s", SDL_GetError()));
	// SDL_DestroyWindow(tmpWindow);

	// /*  Get Vulkan version. */
	// uint32_t version;
	// result = vkEnumerateInstanceVersion(&version);
	// if (result != VK_SUCCESS)
	// 	throw RuntimeException(fmt::format("Failed to enumerate instance version - %d", result));

	// /*	Primary Vulkan instance Object. */
	// VkApplicationInfo ai = {};
	// ai.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	// ai.pNext = nullptr;
	// ai.pApplicationName = "FragCore";
	// ai.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	// ai.pEngineName = "FragCore-Engine";
	// ai.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	// ai.apiVersion = version;

	// VkDebugReportCallbackCreateInfoEXT callbackCreateInfoExt = {
	// 	VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT, // sType
	// 	nullptr,													 // pNext
	// 	VK_DEBUG_REPORT_ERROR_BIT_EXT |							 // flags
	// 		VK_DEBUG_REPORT_WARNING_BIT_EXT,
	// 	nullptr, // myOutputDebugString,                                        // pfnCallback
	// 	nullptr  // pUserData
	// };
	// VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoExt = {
	// 	.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
	// 	.pNext = &callbackCreateInfoExt,
	// };
	// VkValidationCheckEXT validationCheckExt[] = {VK_VALIDATION_CHECK_ALL_EXT};
	// VkValidationFlagsEXT validationFlagsExt = {.sType = VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT,
	// 										   .pNext = nullptr, //&debugUtilsMessengerCreateInfoExt,
	// 										   .disabledValidationCheckCount = 1,
	// 										   .pDisabledValidationChecks = validationCheckExt};

	// /*	Prepare the instance object. */
	// VkInstanceCreateInfo ici = {};
	// ici.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	// if (vulkancore->enableValidationLayers)
	// 	ici.pNext = nullptr;
	// else
	// 	ici.pNext = nullptr;
	// ici.flags = 0;
	// ici.pApplicationInfo = &ai;
	// if (vulkancore->enableValidationLayers) {
	// 	ici.enabledLayerCount = validationLayers.size();
	// 	ici.ppEnabledLayerNames = validationLayers.data();
	// } else {
	// 	ici.enabledLayerCount = 0;
	// 	ici.ppEnabledLayerNames = nullptr;
	// }
	// ici.enabledExtensionCount = instanceExtensionNames.size();
	// ici.ppEnabledExtensionNames = instanceExtensionNames.data();

	// /*	Create Vulkan instance.	*/
	// result = vkCreateInstance(&ici, nullptr, &vulkancore->inst);
	// if (result != VK_SUCCESS)
	// 	throw RuntimeException(fmt::format("vkCreateInstance - %d.", result));

	// /*  Set debug mode.  */
	// this->setDebug(setupConfig.get<bool>("debug"));

	// /*	Get number of physical devices. */
	// int num_physica_devices;
	// result = vkEnumeratePhysicalDevices(vulkancore->inst, &vulkancore->num_physical_devices, nullptr);
	// if (result != VK_SUCCESS) {
	// 	throw RuntimeException(fmt::format("Failed to get number physical devices - %d", result));
	// }
	// /*  Get all physical devices.    */
	// vulkancore->physical_devices =
	// 	(VkPhysicalDevice *)malloc(sizeof(VkPhysicalDevice) * vulkancore->num_physical_devices);
	// assert(vulkancore->physical_devices);
	// vulkancore->GPUs.resize(vulkancore->num_physical_devices);
	// result =
	// 	vkEnumeratePhysicalDevices(vulkancore->inst, &vulkancore->num_physical_devices, vulkancore->physical_devices);
	// if (result != VK_SUCCESS)
	// 	throw RuntimeException(fmt::format("Failed to enumerate physical devices - %d.", result));

	// /*	*/
	// uint32_t pPhysicalDeviceGroupCount;
	// result = vkEnumeratePhysicalDeviceGroups(vulkancore->inst, &pPhysicalDeviceGroupCount, nullptr);
	// if (result != VK_SUCCESS)
	// 	throw RuntimeException(fmt::format("vkEnumeratePhysicalDeviceGroups failed query - %d.", result));
	// std::vector<VkPhysicalDeviceGroupProperties> phyiscalDevices;
	// phyiscalDevices.resize(pPhysicalDeviceGroupCount);
	// result = vkEnumeratePhysicalDeviceGroups(vulkancore->inst, &pPhysicalDeviceGroupCount, phyiscalDevices.data());
	// if (result != VK_SUCCESS)
	// 	throw RuntimeException(fmt::format("vkEnumeratePhysicalDeviceGroups failed fetching groups - %d.", result));

	// /*  TODO add selection function. */
	// std::vector<VkPhysicalDevice> gpucandiates;
	// for (int i = 0; i < vulkancore->num_physical_devices; i++) {
	// 	if (isDeviceSuitable(vulkancore->physical_devices[i])) {
	// 		vulkancore->gpu = vulkancore->physical_devices[i];
	// 		gpucandiates.push_back(vulkancore->physical_devices[i]);
	// 		break;
	// 	}
	// }

	// std::vector<VkPhysicalDevice> selectedDevices;
	// selectDefaultDevices(gpucandiates, selectedDevices);

	// /*  */
	// VkPhysicalDeviceFeatures supportedFeatures;
	// vkGetPhysicalDeviceFeatures(vulkancore->gpu, &supportedFeatures);
	// /*  Fetch memory properties.   */
	// vkGetPhysicalDeviceMemoryProperties(vulkancore->gpu, &vulkancore->memProperties);

	// /*  Select queue family.    */
	// /*  TODO improve queue selection.   */
	// vkGetPhysicalDeviceQueueFamilyProperties(vulkancore->gpu, &vulkancore->queue_count, nullptr);

	// vulkancore->queue_props =
	// 	(VkQueueFamilyProperties *)malloc(sizeof(VkQueueFamilyProperties) * vulkancore->queue_count);
	// vkGetPhysicalDeviceQueueFamilyProperties(vulkancore->gpu, &vulkancore->queue_count, vulkancore->queue_props);
	// assert(vulkancore->queue_count >= 1);

	// VkPhysicalDeviceFeatures features;
	// vkGetPhysicalDeviceFeatures(vulkancore->gpu, &features);

	// /*  Select queue with graphic.  */ // TODO add queue for compute and etc.
	// uint32_t graphicsQueueNodeIndex = UINT32_MAX;
	// for (uint32_t i = 0; i < vulkancore->queue_count; i++) {
	// 	if ((vulkancore->queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
	// 		if (graphicsQueueNodeIndex == UINT32_MAX)
	// 			graphicsQueueNodeIndex = i;
	// 	}
	// }
	// vulkancore->graphics_queue_node_index = graphicsQueueNodeIndex;

	// float queue_priorities[1] = {1.0};
	// const VkDeviceQueueCreateInfo queue = {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
	// 									   .pNext = nullptr,
	// 									   .flags = 0,
	// 									   .queueFamilyIndex = vulkancore->graphics_queue_node_index,
	// 									   .queueCount = 1,
	// 									   .pQueuePriorities = queue_priorities};

	// /*  Required extensions.    */
	// std::vector<const char *> deviceExtensions = {
	// 	VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME,
	// 	// VK_NV_RAY_TRACING_EXTENSION_NAME
	// 	// VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME
	// 	// VK_NV_GLSL_SHADER_EXTENSION_NAME
	// };
	// if (vulkancore->enableValidationLayers) {
	// 	// TODO determine
	// 	deviceExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
	// 	// deviceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	// }

	// VkDeviceGroupDeviceCreateInfo deviceGroupDeviceCreateInfo = {
	// 	.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO,
	// };
	// if (phyiscalDevices.size() > 0) {
	// 	if (phyiscalDevices[0].physicalDeviceCount > 1) {
	// 		deviceGroupDeviceCreateInfo.physicalDeviceCount = phyiscalDevices[0].physicalDeviceCount;
	// 		deviceGroupDeviceCreateInfo.pPhysicalDevices = phyiscalDevices[0].physicalDevices;
	// 	}
	// }

	// VkPhysicalDeviceShaderDrawParameterFeatures deviceShaderDrawParametersFeatures = {
	// 	.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETER_FEATURES,
	// 	.pNext = nullptr,
	// 	.shaderDrawParameters = VK_TRUE};
	// VkPhysicalDeviceMultiviewFeatures deviceMultiviewFeatures = {
	// 	.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES,
	// 	.pNext = &deviceShaderDrawParametersFeatures,
	// 	.multiview = VK_TRUE,
	// 	.multiviewGeometryShader = VK_TRUE,
	// 	.multiviewTessellationShader = VK_TRUE};
	// VkPhysicalDeviceConditionalRenderingFeaturesEXT conditionalRenderingFeaturesExt = {
	// 	.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT,
	// 	.pNext = &deviceMultiviewFeatures,
	// 	.conditionalRendering = VK_TRUE,
	// 	.inheritedConditionalRendering = VK_TRUE};

	// VkDeviceCreateInfo device = {};
	// VkPhysicalDeviceFeatures deviceFeatures{};
	// device.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	// device.pNext = &conditionalRenderingFeaturesExt;
	// device.queueCreateInfoCount = 1;
	// device.pQueueCreateInfos = &queue;
	// if (vulkancore->enableValidationLayers) {
	// 	device.enabledLayerCount = vulkancore->enabled_layer_count;
	// 	device.ppEnabledLayerNames =
	// 		(const char *const *)((vulkancore->validate) ? vulkancore->device_validation_layers : nullptr);
	// } else {
	// 	device.enabledLayerCount = 0;
	// }

	// /*	Enable group.	*/
	// if (deviceGroupDeviceCreateInfo.physicalDeviceCount > 1) {
	// 	deviceShaderDrawParametersFeatures.pNext = &deviceGroupDeviceCreateInfo;
	// }
	// device.enabledExtensionCount = deviceExtensions.size();
	// device.ppEnabledExtensionNames = deviceExtensions.data();
	// device.pEnabledFeatures = &deviceFeatures;

	// /*  Create device.  */
	// result = vkCreateDevice(vulkancore->gpu, &device, nullptr, &vulkancore->device);
	// if (result != VK_SUCCESS)
	// 	throw RuntimeException(fmt::format("Failed to create logical Device - %d", result));

	// /*  Get all queues.    */
	// vkGetDeviceQueue(vulkancore->device, vulkancore->graphics_queue_node_index, 0, &vulkancore->queue);
	// vkGetDeviceQueue(vulkancore->device, vulkancore->graphics_queue_node_index, 0, &vulkancore->presentQueue);

	// /*  Create command pool.    */
	// VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
	// cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	// cmdPoolCreateInfo.queueFamilyIndex = vulkancore->graphics_queue_node_index;
	// cmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	// /*  Create command pool.    */
	// result = vkCreateCommandPool(vulkancore->device, &cmdPoolCreateInfo, nullptr, &vulkancore->cmd_pool);
	// if (result != VK_SUCCESS)
	// 	throw RuntimeException(fmt::format("failed to create command pool %d", result));

	/*  TODO determine which languages are supported.   */
	//    uint32_t nExtensions;
	//    result = vkEnumerateDeviceExtensionProperties(vulkancore->device , nullptr, &nExtensions, nullptr);
	this->languageSupport = SPIRV;
	this->getCapability(&this->capability);
}

VKRenderInterface::~VKRenderInterface(void) {
	/*  Release all vulkan resources.   */
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void VKRenderInterface::OnInitialization(void) {}

void VKRenderInterface::OnDestruction(void) {}

Texture *VKRenderInterface::createTexture(TextureDesc *desc) {

	// Texture *texture = new Texture();
	// VKTextureObject *vktex = new VKTextureObject();
	// vktex->vulkanCore = vulkanCore;
	// vktex->desc = *desc;
	// texture->pdata = vktex;

	// VkDeviceMemory stagingBufferMemory;
	// VkDeviceSize imageSize = desc->pixelSize;

	// /*  Check descriptor.    */
	// if (!desc->pixel)
	// 	throw RuntimeException("failed to load texture image!");

	// VkBuffer stagingBuffer;
	// ::createBuffer(vulkanCore, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	// 			   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
	// 			   stagingBufferMemory);

	// void *data;
	// vkMapMemory(device->getHandle(), stagingBufferMemory, 0, imageSize, 0, &data);
	// memcpy(data, desc->pixel, static_cast<size_t>(imageSize));
	// vkUnmapMemory(device->getHandle(), stagingBufferMemory);

	// /*-------------Create Image --------*/
	// VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	// /*  Image description.  */
	// VkImageCreateInfo imageInfo = {};
	// imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	// imageInfo.imageType = (VkImageType)getTextureTarget((TextureDesc::Target)desc->target);
	// imageInfo.extent.width = static_cast<uint32_t>(desc->width);
	// imageInfo.extent.height = static_cast<uint32_t>(desc->height);
	// imageInfo.extent.depth = desc->depth;
	// imageInfo.mipLevels = desc->numlevel;
	// imageInfo.arrayLayers = desc->depth;

	// /*  */
	// imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	// imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	// imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	// imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	// imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	// /*  Shading multisampling.  */
	// imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	// imageInfo.flags = 0; // Optional

	// /*  Create image.   */
	// VkResult result = vkCreateImage(device->getHandle(), &imageInfo, nullptr, &vktex->texture);
	// if (result != VK_SUCCESS) {
	// 	throw RuntimeException(fmt::format("Failed creating texture image - %d", result));
	// }

	// /*  */
	// VkMemoryRequirements memRequirements;
	// vkGetImageMemoryRequirements(device->getHandle(), vktex->texture, &memRequirements);

	// VkMemoryAllocateInfo allocInfo = {};
	// allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	// allocInfo.allocationSize = memRequirements.size;
	// allocInfo.memoryTypeIndex = findMemoryType(vulkanCore, memRequirements.memoryTypeBits, properties);

	// /*  Allocate memory for the texture.    */
	// result = vkAllocateMemory(device->getHandle(), &allocInfo, nullptr, &stagingBufferMemory);
	// if (result != VK_SUCCESS) {
	// 	throw RuntimeException("failed to allocate image memory!");
	// }
	// vktex->imageMemory = stagingBufferMemory;

	// /*  Bind texture and memory.*/
	// result = vkBindImageMemory(device->getHandle(), vktex->texture, vktex->imageMemory, 0);
	// if (result != VK_SUCCESS) {
	// 	throw RuntimeException("Failed to create ");
	// }

	// /*  Sampler.    */
	// VkSamplerCreateInfo samplerInfo = {};
	// samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	// samplerInfo.magFilter = VK_FILTER_LINEAR;
	// samplerInfo.minFilter = VK_FILTER_LINEAR;
	// samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	// samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	// samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	// samplerInfo.anisotropyEnable = desc->sampler.anisotropy > 0.0 ? VK_TRUE : VK_FALSE;
	// samplerInfo.maxAnisotropy = desc->sampler.anisotropy;
	// samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	// samplerInfo.unnormalizedCoordinates = VK_FALSE;
	// samplerInfo.compareEnable = VK_FALSE;
	// samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	// samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	// samplerInfo.mipLodBias = 0.0f;
	// samplerInfo.minLod = 0.0f;
	// samplerInfo.maxLod = 0.0f;

	// result = vkCreateSampler(device->getHandle(), &samplerInfo, nullptr, &vktex->sampler);
	// if (result != VK_SUCCESS) {
	// 	throw RuntimeException("failed to create texture sampler!");
	// }

	// return texture;
}

void VKRenderInterface::deleteTexture(Texture *texture) {

	// VKTextureObject *vkTextureObject = (VKTextureObject *)texture->pdata;

	// /*  Release resources.  */
	// if (vkTextureObject->texture)
	// 	vkDestroyImage(device->getHandle(), vkTextureObject->texture, nullptr);
	// if (vkTextureObject->sampler)
	// 	vkDestroySampler(device->getHandle(), vkTextureObject->sampler, nullptr);

	// /*  Release objects.    */
	// //delete texture->pdata;
	// delete texture;
}

Sampler *VKRenderInterface::createSampler(SamplerDesc *desc) {

	if (desc == nullptr)
		throw InvalidArgumentException("Invalid sampler description pointer object.");

	/*  */
	Sampler *sampler = new Sampler();
	// sampler->pdata= samplerObject;
	return sampler;
}

void VKRenderInterface::deleteSampler(Sampler *sampler) {}

RenderPipeline *VKRenderInterface::createPipeline(const ProgramPipelineDesc *desc) {

	//    desc.
}

void VKRenderInterface::deletePipeline(RenderPipeline *obj) {}

Shader *VKRenderInterface::createShader(ShaderDesc *desc) {

	/*  Create shader interface object. */
	Shader *shader = new Shader();
	VKShaderObject *shaobj = new VKShaderObject();
	//	shader->pdata = shaobj;
	// shaobj->vulkanCore = vulkanCore;

	VkResult result;
	VkShaderModule vertShaderModule = nullptr;
	VkShaderModule fragShaderModule = nullptr;
	VkShaderModule geomShaderModule = nullptr;
	VkShaderModule tessCShaderModule = nullptr;
	VkShaderModule tessEShaderModule = nullptr;

	/*  */
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	VkPipelineShaderStageCreateInfo geomShaderStageInfo = {};
	VkPipelineShaderStageCreateInfo tessEShaderStageInfo = {};
	VkPipelineShaderStageCreateInfo tessChaderStageInfo = {};

	/*  */
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

	// Validate the shader language.
	ShaderLanguage supportedLanguage = (ShaderLanguage)(~this->getShaderLanguage());
	if (desc->vertex.language & supportedLanguage)
		throw RuntimeException("Vertex shader language is not supported");
	if (desc->fragment.language & supportedLanguage)
		throw RuntimeException("Fragment shader language is not supported");
	if (desc->geometry.language & supportedLanguage)
		throw RuntimeException("Geometry shader language is not supported");
	if (desc->tessellationControl.language & supportedLanguage)
		throw RuntimeException("Tessellation Control shader language is not supported");
	if (desc->tessellationEvolution.language & supportedLanguage)
		throw RuntimeException("Tessellation Evolution shader language is not supported");
	if (desc->Compute.language & supportedLanguage)
		throw RuntimeException("Compute shader language is not supported");

	/*  Create shader module code containers.   */
	// TODO fix the code for the shader desc.
	//	if(desc->vertex.vertexBinary && desc->vertex.language == SPIRV){
	if (1) {
		// vertShaderModule = createShaderModule(device->getHandle(), (const char*)desc->vertex.vertexBinary,
		// desc->vertex.size);
		// vertShaderModule = createShaderModule(device->getHandle(), (const char *)nullptr, 0); // FIXME

		/*  */
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		shaderStages.push_back(vertShaderStageInfo);
	}
	//	if(desc->fragment.fragmentBinary && desc->fragment.language == SPIRV){
	if (1) {
		// fragShaderModule = createShaderModule(device->getHandle(),  (const char*)desc->fragment.fragmentBinary,
		// desc->fragment.size);
		// fragShaderModule = createShaderModule(device->getHandle(), (const char *)nullptr, 0); // FIXME

		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		shaderStages.push_back(fragShaderStageInfo);
	}
	//	if(desc->geometry.geometryBinary && desc->geometry.language == SPIRV){
	//		geomShaderModule = createShaderModule(device->getHandle(), desc->vertexsource[0],
	// strlen(desc->vertexsource[0]));
	//
	//		geomShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//		geomShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
	//		geomShaderStageInfo.module = geomShaderModule;
	//		geomShaderStageInfo.pName = "main";
	//
	//		shaderStages.push_back(geomShaderStageInfo);
	//	}
	//	if(desc->numtesco > 0){
	//		tessCShaderModule = createShaderModule(device->getHandle(), desc->vertexsource[0],
	// strlen(desc->vertexsource[0]));
	//
	//		tessChaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//		tessChaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	//		tessChaderStageInfo.module = tessCShaderModule;
	//		tessChaderStageInfo.pName = "main";
	//
	//		shaderStages.push_back(tessChaderStageInfo);
	//	}
	//	if(desc->numtesev> 0){
	//		tessEShaderModule = createShaderModule(device->getHandle(), desc->vertexsource[0],
	// strlen(desc->vertexsource[0]));
	//
	//		tessEShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//		tessEShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	//		tessEShaderStageInfo.module = tessEShaderModule;
	//		tessEShaderStageInfo.pName = "main";
	//
	//		shaderStages.push_back(tessEShaderStageInfo);
	//	}

	// If using compute shader.
	if (desc->Compute.language == SPIRV) {

		VkDescriptorSetLayoutBinding bindings[2] = {{0}};
		bindings[0].binding = 0;
		bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		bindings[0].descriptorCount = 1;
		bindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
		bindings[1].binding = 1;
		bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		bindings[1].descriptorCount = 1;
		bindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

		//        VkComputePipelineCreateInfo info = { VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO };
		//        info.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		//        info.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		//        info.stage.module = loadShaderModule(device->getHandle(), "shaders/particle.comp.spv");
		//        info.stage.pName = "main";
		//        info.layout = computePipeline.pipelineLayout;

		VkComputePipelineCreateInfo computePipelineCreateInfo = {};
		// result = vkCreateComputePipelines(gpu, nullptr, 1, &computePipelineCreateInfo, nullptr,
		// &shaobj->graphicsPipeline);
	}

	/*  */
	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &samplerLayoutBinding;

	VkDescriptorSetLayout setLayout;
	result = vkCreateDescriptorSetLayout(device->getHandle(), &layoutInfo, nullptr, &setLayout);
	if (result != VK_SUCCESS)
		throw RuntimeException(fmt::format("Failed to create descriptor set layout - %d", result));

	/*  */
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;			  // Optional
	pipelineLayoutInfo.pSetLayouts = &setLayout;	  // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0;	  // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	result = vkCreatePipelineLayout(device->getHandle(), &pipelineLayoutInfo, nullptr, &shaobj->pipelineLayout);
	if (result != VK_SUCCESS)
		throw RuntimeException(fmt::format("failed to create pipeline layout - %d!", result));

	VkAttachmentDescription colorAttachment = {};
	// colorAttachment.format = swapChain->swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	/*  */
	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	/*  */
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPass renderPass;
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	if (vkCreateRenderPass(device->getHandle(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
		throw RuntimeException(fmt::format("failed to create render pass - %d", result));

	/*  */
	VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(float) * 3;
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	/**/
	VkVertexInputAttributeDescription attributeDescriptions = {0};
	attributeDescriptions.binding = 0;
	attributeDescriptions.location = 0;
	attributeDescriptions.format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions.offset = 0;

	/*  */
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = 1;
	vertexInputInfo.pVertexAttributeDescriptions = &attributeDescriptions; // Optional

	/*  */
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	/*  */
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;	 // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;			 // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;	 // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;			 // Optional

	/*  */
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.lineWidth = 1.0f;

	/*  */
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	/*  */
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	/*  Enable dynamic variables in shader configuration.   */
	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,		  VK_DYNAMIC_STATE_LINE_WIDTH,	 VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
		VK_DYNAMIC_STATE_BLEND_CONSTANTS, VK_DYNAMIC_STATE_DEPTH_BOUNDS,
	};

	/*  */
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = sizeof(dynamicStates) / sizeof(dynamicStates[0]);
	dynamicState.pDynamicStates = dynamicStates;

	/*  */
	VkViewport viewport = {};
	// viewport.x = 0.0f;
	// viewport.y = 0.0f;
	// viewport.width = (float) swapChain->chainExtend.width;
	// viewport.height = (float) swapChain->chainExtend.height;
	// viewport.minDepth = 0.0f;
	// viewport.maxDepth = 1.0f;

	/*  */
	VkRect2D scissor = {};
	// scissor.offset = {0, 0};
	// scissor.extent = swapChain->chainExtend;

	/*  */
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	/*  Graphic pipeline descriptor.    */
	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState; // Optional
	pipelineInfo.layout = shaobj->pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1;			  // Optional

	/*  Create graphic pipeline.    */
	result = vkCreateGraphicsPipelines(device->getHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr,
									   &shaobj->graphicsPipeline);
	if (result != VK_SUCCESS)
		throw RuntimeException(fmt::format("vkCreateGraphicsPipelines failed - %d", result));

	/*  Release shader moudles once used.  */
	if (vertShaderModule)
		vkDestroyShaderModule(device->getHandle(), vertShaderModule, nullptr);
	if (fragShaderModule)
		vkDestroyShaderModule(device->getHandle(), fragShaderModule, nullptr);
	if (geomShaderModule)
		vkDestroyShaderModule(device->getHandle(), geomShaderModule, nullptr);
	if (tessCShaderModule)
		vkDestroyShaderModule(device->getHandle(), tessCShaderModule, nullptr);
	if (tessEShaderModule)
		vkDestroyShaderModule(device->getHandle(), tessEShaderModule, nullptr);

	return shader;
}

void VKRenderInterface::deleteShader(Shader *shader) {

	// VKShaderObject *shaobj = (VKShaderObject *)shader->pdata;

	// /*  validate object.   */
	// if (shader->getRenderer() != this)
	// 	throw InvalidArgumentException("");

	// if (shaobj->graphicsPipeline) {
	// 	// vkDestroyPipelineLayout(device->getHandle(), pipelineLayout, nullptr);
	// 	/*  Release all resources.  */
	// 	vkDestroyPipeline(device->getHandle(), shaobj->graphicsPipeline, nullptr);
	// } else
	// 	throw RuntimeException("");

	// //delete shader->pdata;
	// delete shader;
}

Buffer *VKRenderInterface::createBuffer(BufferDesc *desc) {

	VkResult result;
	Buffer *buffer;
	VKBufferObject *vkBufferObject = new VKBufferObject();
	//	vkBufferObject->vulkanCore = (VulkanCore *)this->pdata;
	buffer = new Buffer();
	// buffer->pdata = vkBufferObject;

	VKHelper::createBuffer(device->getHandle(), desc->size, device->getPhysicalDevice(0)->getMemoryProperties(),
						   (VkBufferUsageFlags)getBufferType(desc->type), VK_MEMORY_PROPERTY_HOST_CACHED_BIT, vkBufferObject->buffer,
						   vkBufferObject->vertexBufferMemory);

	// /*  Buffer description. */
	// VkBufferCreateInfo bufferInfo = {};
	// bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	// bufferInfo.size = desc->size;
	// // bufferInfo.usage = (VkBufferUsageFlags)getBufferType((BufferDesc::BufferType)desc->type);
	// bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	// /*  Create buffer.  */
	// result = vkCreateBuffer(device->getHandle(), &bufferInfo, nullptr, &vkBufferObject->buffer);
	// if (result != VK_SUCCESS) {
	// 	throw RuntimeException(fmt::format("failed to create vertex buffer %d", result));
	// }

	// /*  */
	// VkMemoryRequirements memRequirements;
	// vkGetBufferMemoryRequirements(device->getHandle(), vkBufferObject->buffer, &memRequirements);

	// /*  */
	// VkPhysicalDeviceMemoryProperties memProperties;
	// vkGetPhysicalDeviceMemoryProperties(gpu, &memProperties);

	// /*  Allocate memory.    */
	// VkMemoryAllocateInfo allocInfo = {};
	// allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	// allocInfo.allocationSize = memRequirements.size;
	// // allocInfo.memoryTypeIndex =
	// // 	findMemoryType(physical_devices[0], memRequirements.memoryTypeBits,
	// // 				   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// /*  */
	// if (vkAllocateMemory(device->getHandle(), &allocInfo, nullptr, &vkBufferObject->vertexBufferMemory) != VK_SUCCESS) {
	// 	throw RuntimeException(fmt::format("failed to allocate vertex buffer memory!"));
	// }

	// /*  */
	// result = vkBindBufferMemory(device->getHandle(), vkBufferObject->buffer, vkBufferObject->vertexBufferMemory, 0);
	// if (result != VK_SUCCESS)
	// 	throw RuntimeException(fmt::format("failed to allocate vertex buffer memory!"));

	return buffer;
}

void VKRenderInterface::deleteBuffer(Buffer *object) {

	// VKBufferObject *bufferObject = (VKBufferObject *)object->pdata;

	// // vkFreeMemory(device->getHandle(), vertexBufferMemory, nullptr);
	// if (bufferObject->buffer)
	// 	vkDestroyBuffer(device->getHandle(), bufferObject->buffer, nullptr);

	// /*  Release objects.    */
	// //delete object->pdata;
	// delete object;
}

Geometry *VKRenderInterface::createGeometry(GeometryDesc *desc) {
	Geometry *geometryObject;
	VKGeometryObject *glgeoobj = nullptr;
	unsigned int x;

	/*	*/
	geometryObject = new Geometry();
	glgeoobj = new VKGeometryObject();

	/*	Requires array buffer.  */
	if (desc->numVerticecs > 0) {
		BufferDesc abuffer;
		abuffer.size = desc->numVerticecs * desc->vertexStride;
		abuffer.type = BufferDesc::eArray;
		abuffer.data = desc->buffer;
		abuffer.hint = (BufferDesc::BufferHint)(BufferDesc::eWrite | BufferDesc::eStatic);
		glgeoobj->vertexbuffer = createBuffer(&abuffer);
		glgeoobj->vertexbuffer->bind();
	}

	/*	Requires element buffer.    */
	if (desc->numIndices > 0) {
		BufferDesc abuffer;
		abuffer.size = desc->numIndices * desc->indicesStride;
		abuffer.type = BufferDesc::eElementArray;
		abuffer.data = desc->indices;
		abuffer.hint = (BufferDesc::BufferHint)(BufferDesc::eWrite | BufferDesc::eStatic);
		glgeoobj->indicesbuffer = createBuffer(&abuffer);
		glgeoobj->indicesbuffer->bind();

		/*  Determine indices data size type.   */
		switch (desc->indicesStride) {
		case 2:
		case 1:
			glgeoobj->indicesType = VK_INDEX_TYPE_UINT16;
			break;
		case 4:
			glgeoobj->indicesType = VK_INDEX_TYPE_UINT32;
			break;
		}
	}

	/*	*/
	//	for(x = 0; x < desc->numVertexAttributes; x++){
	//		glEnableVertexAttribArray(desc->vertexattribute[x].index);
	//		glVertexAttribPointer(desc->vertexattribute[x].index,
	//		                      desc->vertexattribute[x].size,
	//		                      getAttributeDataType((GeometryDesc::AttributeType)desc->vertexattribute[x].datatype),
	//		                      GL_FALSE,
	//		                      desc->vertexStride,
	//		                      (const void*)desc->vertexattribute[x].offset);
	//	}

	// glgeoobj->mode = getPrimitive((GeometryDesc::Primitive)desc->primitive);
	glgeoobj->desc = *desc;

	//	geometryObject->pdata = glgeoobj;
	return geometryObject;
}

void VKRenderInterface::deleteGeometry(Geometry *obj) {

	// VKGeometryObject *glgeoobj = (VKGeometryObject *)obj->pdata;

	// deleteBuffer(glgeoobj->vertexbuffer);
	// deleteBuffer(glgeoobj->indicesbuffer);

	/*  Release objects.    */
	//	delete obj->pdata;
	delete obj;
}

// void VKRenderInterface::setViewport(int x, int y, int width, int height) {
//
//	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;
//
//	VkViewport viewport = {};
//	viewport.x = x;
//	viewport.y = y;
//	viewport.width = (float) width;
//	viewport.height = (float) height;
//	viewport.minDepth = 0.0f;
//	viewport.maxDepth = 1.0f;
//
//	return;
//
//	/*  */
//	for(uint32_t i = 0; i < swapChain->swapchainImageCount; i++)
//		vkCmdSetViewport(swapChain->commandBuffers[i], 0, 1, &viewport);
//
//	/*  Create swapchain.   */
//}

ViewPort *VKRenderInterface::getView(unsigned int i) {

	/*  Validate the index. */
	if (i >= capability.sMaxViewPorts)
		throw InvalidArgumentException(
			fmt::format("Does not support viewport index %d, max index %d.", i, capability.sMaxViewPorts));

	//	// If the view does not exits. Create it.
	//	if(i == 0)
	//		return glcore->defaultViewport;
	//	return glcore->viewports[i - 1];
}

FrameBuffer *VKRenderInterface::createFrameBuffer(FrameBufferDesc *desc) {
	

	 return nullptr; 
	 
}

void VKRenderInterface::deleteFrameBuffer(FrameBuffer *obj) {}

QueryObject *VKRenderInterface::createQuery(QueryDesc *desc) {}
void VKRenderInterface::deleteQuery(QueryObject *query) {}

RendererWindow *VKRenderInterface::createWindow(int x, int y, int width, int height) {

	/*	*/
	Ref<VKRenderInterface> rendRef = Ref<VKRenderInterface>(this);
	RendererWindow *window = new VKRenderWindow(rendRef);

	/*	*/
	windows.push_back(window);
	return window;
}

void VKRenderInterface::setCurrentWindow(RendererWindow *window) { /*  */
}

FrameBuffer *VKRenderInterface::getDefaultFramebuffer(void *window) {
	static FrameBuffer *defaultFrambuffer = nullptr;
	/*  TODO add support.   */
	if (defaultFrambuffer == nullptr) {
		// FrameBuffer *frameBuffer = new FrameBuffer();
	}

	return defaultFrambuffer;
}

void VKRenderInterface::clear(unsigned int bitflag) {

	const uint32_t currentFrame = currentFrame;
	return;
	if (bitflag & eColor) {
		// VkClearColorValue clearColor = {color[0], color[1], color[2],
		// 								color[3]};
		// /*  */
		// VkImageSubresourceRange imageRange = {};
		// imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		// imageRange.levelCount = 1;
		// imageRange.layerCount = 1;

		// vkCmdClearColorImage(swapChain->commandBuffers[currentFrame],
		// 					 swapChain->swapChainImages[currentFrame], VK_IMAGE_LAYOUT_GENERAL,
		// 					 &clearColor, 1, &imageRange);
	}

	if (bitflag & eDepth || bitflag & eStencil) {
		VkClearDepthStencilValue depthStencilClear = {0.0f, 0};

		/*  */
		VkImageSubresourceRange imageRange = {};
		imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageRange.levelCount = 1;
		imageRange.layerCount = 1;

		// vkCmdClearDepthStencilImage(swapChain->commandBuffers[currentFrame],
		// 							swapChain->swapChainImages[currentFrame],
		// VK_IMAGE_LAYOUT_GENERAL, 							&depthStencilClear, 1, &imageRange);
	}
}

void VKRenderInterface::clearColor(float r, float g, float b, float a) {

	/*  Cache the clear color.  */
	// color[0] = r;
	// color[1] = g;
	// color[2] = b;
	// color[3] = a;
}

void VKRenderInterface::setVSync(int sync) { /*  Change the toolchain.   */
}

void VKRenderInterface::setDepthMask(bool flag) { /*  */
}

void VKRenderInterface::enableState(VKRenderInterface::State state) { /*  */
}

void VKRenderInterface::disableState(VKRenderInterface::State state) { /*  */
}

bool VKRenderInterface::isStateEnabled(VKRenderInterface::State state) {}

void VKRenderInterface::swapBuffer(void) {

	// VkResult result;
	// uint32_t imageIndex;

	// VulkanCore *vulkanCore = (VulkanCore *) this->pdata;
	// result = vkEndCommandBuffer(swapChain->commandBuffers[currentFrame]);
	// if(result  != VK_SUCCESS)
	// 	throw RuntimeException("failed");

	// /*  */
	// result = vkAcquireNextImageKHR(device->getHandle(), swapChain->swapchain, UINT64_MAX,
	// 							   renderFinishedSemaphore, VK_NULL_HANDLE, &imageIndex);
	// if(result == VK_ERROR_OUT_OF_DATE_KHR){
	// 	/*  Recreate.   */
	// }
	// if(result != VK_SUCCESS)
	// 	throw RuntimeException(fmt::format("Failed to acquire next image - %d", result));

	// VkSubmitInfo submitInfo = {};
	// submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	// submitInfo.commandBufferCount   = 1;
	// submitInfo.pCommandBuffers      = &swapChain->commandBuffers[imageIndex];

	// result = vkQueueSubmit(queue, 1, &submitInfo, nullptr);
	// if(result != VK_SUCCESS)
	// 	throw RuntimeException("Failed to submit to queue.");

	// VkPresentInfoKHR presentInfo = {};
	// presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	// presentInfo.swapchainCount     = 1;
	// presentInfo.pSwapchains        = &swapChain->swapchain;
	// presentInfo.pImageIndices      = &imageIndex;

	// result = vkQueuePresentKHR(queue, &presentInfo);
	// if(result != VK_SUCCESS)
	// 	throw RuntimeException(fmt::format("Failed to present - %d", result));

	// vkQueueWaitIdle(queue);

	// /*  Compute current frame.  */
	// currentFrame = (currentFrame + 1) % swapChain->swapChainImages.size();

	// /*  Reset command buffer.    */
	// result = vkResetCommandBuffer(swapChain->commandBuffers[currentFrame],
	// VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT); if(result != VK_SUCCESS) 	throw RuntimeException("failed");

	// /*  */
	// VkCommandBufferBeginInfo beginInfo = {};
	// beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	// beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	// /*  */
	// result = vkBeginCommandBuffer(swapChain->commandBuffers[currentFrame], &beginInfo);
	// if(result != VK_SUCCESS)
	// 	throw RuntimeException("Failed to start the ");
}

void VKRenderInterface::drawInstance(Geometry *geometry, unsigned int num) {

	VKGeometryObject *glgeo;
	assert(geometry && num > 0);

	/*  */
	//	const uint32_t curFrame = currentFrame;
	//	glgeo = (VKGeometryObject *)geometry->pdata;

	// /*  */
	// VKBufferObject *vertexBuffer = (VKBufferObject *)glgeo->vertexbuffer->pdata;
	// VkBuffer vertexBuffers[] = {vertexBuffer->buffer};

	/*  */
	if (glgeo->desc.numIndices > 0) {
		assert(glgeo->vertexbuffer && glgeo->indicesbuffer);
		VkDeviceSize offsets[] = {0};

		// VKBufferObject *indexBuffer = (VKBufferObject *)glgeo->indicesbuffer->pdata;
		// vkCmdBindVertexBuffers(swapChain->commandBuffers[curFrame], 0, 1, vertexBuffers, offsets);
		// vkCmdBindIndexBuffer(swapChain->commandBuffers[curFrame], indexBuffer->buffer, offsets[0],
		// VK_INDEX_TYPE_UINT16);

		// vkCmdDrawIndexed(swapChain->commandBuffers[curFrame],
		// static_cast<uint32_t>(geometry->getIndicesCount()), 1, 0, 0, 0);

	} else {
		assert(glgeo->vertexbuffer);

		VkDeviceSize offsets[] = {0};
		// vkCmdBindVertexBuffers(swapChain->commandBuffers[curFrame], 0, 1, vertexBuffers, offsets);

		// vkCmdDraw(swapChain->commandBuffers[curFrame], static_cast<uint32_t>(geometry->getVertexCount()),
		// 1, 0, 0);
	}
}

void VKRenderInterface::drawMultiInstance(Geometry &geometries, const unsigned int *first, const unsigned int *count,
										  unsigned int num) {}
void VKRenderInterface::drawMultiIndirect(Geometry &geometries, unsigned int offset, unsigned int indirectCount) {}

void VKRenderInterface::drawIndirect(Geometry *geometry) {}

void VKRenderInterface::setLineWidth(float width) {}

void VKRenderInterface::blit(const FrameBuffer *source, FrameBuffer *dest,
							 Texture::FilterMode filterMode) { /*  TODO add filter.    */

	// vkCmdBlitImage()
}

void VKRenderInterface::bindTextures(unsigned int firstUnit, const std::vector<Texture *> &textures) {}

void VKRenderInterface::bindImages(unsigned int firstUnit, const std::vector<Texture *> &textures,
								   const std::vector<Texture::MapTarget> &mapping,
								   const std::vector<Texture::Format> &formats) {}

void VKRenderInterface::copyTexture(const Texture *source, Texture *target) {
	// vkCmdCopyImage()
}

void VKRenderInterface::dispatchCompute(unsigned int *global, unsigned int *local, unsigned int offset) {

	/*  */
	// const uint32_t curFrame = currentFrame;
	//	VkCommandBuffer curBuffer = swapChain->commandBuffers[curFrame];

	/*  */
	// vkCmdDispatch(curBuffer, local[0], local[1], local[2]);
}

void VKRenderInterface::memoryBarrier(void) {}

Sync *VKRenderInterface::createSync(SyncDesc *desc) {}
void VKRenderInterface::deleteSync(Sync *sync) {}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
													VkDebugUtilsMessageTypeFlagsEXT messageType,
													const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
													void *pUserData) {

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

void VKRenderInterface::setDebug(bool enable) {

	VkResult result;
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT =
		(PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(inst, "vkCreateDebugReportCallbackEXT");

	if (vkCreateDebugReportCallbackEXT) {
		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		createInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)debugCallback;
		createInfo.pUserData = nullptr;

		// result = vkCreateDebugReportCallbackEXT(inst, &createInfo, nullptr, &debugReport);
		// if(result != VK_SUCCESS)
		// 	throw RuntimeException(fmt::format("Failed to create debug report callback - %d", result));
	}

	PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT =
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(inst, "vkCreateDebugUtilsMessengerEXT");
	if (CreateDebugUtilsMessengerEXT) {
		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
									 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
									 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;

		if (CreateDebugUtilsMessengerEXT(inst, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		} else {
			// return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}
}

void VKRenderInterface::getSupportedTextureCompression(TextureDesc::Compression *pCompressions) {
	if (pCompressions == nullptr)
		throw InvalidArgumentException("pCompressions may not be a null pointer.");

	unsigned int compressions = 0;
	VkPhysicalDeviceFeatures deviceFeatures;

	vkGetPhysicalDeviceFeatures(gpu, &deviceFeatures);

	if (deviceFeatures.textureCompressionBC)
		compressions |= TextureDesc::Compression::eNoCompression;
	if (deviceFeatures.textureCompressionETC2)
		compressions |= TextureDesc::Compression::eETC2;
	if (deviceFeatures.textureCompressionASTC_LDR)
		compressions |= TextureDesc::Compression::eASTC_LDR;

	// Add support for default compression format.
	if (compressions != 0)
		compressions |= TextureDesc::eCompression;

	*pCompressions = (TextureDesc::Compression)compressions;
}

void VKRenderInterface::getCapability(Capability *capability) {

	assert(capability);

	if (capability == nullptr)
		throw InvalidArgumentException("Must not be a null pointer.");

	/*  */
	VkPhysicalDeviceProperties properties;

	/*  Propertie set.  */
	VkPhysicalDeviceRayTracingPropertiesNV deviceRayTracingPropertiesNv = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV,
		.pNext = nullptr,
	};
	VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT advancedPropertiesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_PROPERTIES_EXT,
		.pNext = &deviceRayTracingPropertiesNv,
	};
	VkPhysicalDeviceConservativeRasterizationPropertiesEXT conservativeRasterizationPropertiesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT,
		.pNext = &advancedPropertiesExt,
	};
	VkPhysicalDeviceMaintenance3Properties deviceMaintenance3Properties = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES,
		.pNext = &conservativeRasterizationPropertiesExt,
	};
	VkPhysicalDevicePushDescriptorPropertiesKHR descriptorPropertiesKhr = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR,
		.pNext = &deviceMaintenance3Properties,
	};
	VkPhysicalDeviceSamplerFilterMinmaxPropertiesEXT deviceSamplerFilterMinmaxPropertiesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_FILTER_MINMAX_PROPERTIES_EXT,
		.pNext = &descriptorPropertiesKhr,
	};
	VkPhysicalDeviceTransformFeedbackPropertiesEXT deviceTransformFeedbackPropertiesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT,
		.pNext = &deviceSamplerFilterMinmaxPropertiesExt,
	};
	VkPhysicalDeviceExternalMemoryHostPropertiesEXT deviceExternalMemoryHostPropertiesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_HOST_PROPERTIES_EXT,
		.pNext = &deviceTransformFeedbackPropertiesExt,
		.minImportedHostPointerAlignment = 0};
	VkPhysicalDeviceMultiviewProperties deviceMultiviewProperties = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES,
		.pNext = &deviceExternalMemoryHostPropertiesExt,
	};

	/*  Feature set.    */
	VkPhysicalDeviceMultiviewFeatures deviceMultiviewFeatures = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES,
		.pNext = nullptr,
	};
	VkPhysicalDeviceTransformFeedbackFeaturesEXT deviceTransformFeedbackFeaturesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT,
		.pNext = &deviceMultiviewFeatures,
	};
	VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV deviceRepresentativeFragmentTestFeaturesNv = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_REPRESENTATIVE_FRAGMENT_TEST_FEATURES_NV,
		.pNext = &deviceTransformFeedbackFeaturesExt,
	};
	VkPhysicalDeviceConditionalRenderingFeaturesEXT conditionalRenderingFeaturesExt = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT,
		.pNext = &deviceRepresentativeFragmentTestFeaturesNv,
	};

	// VkPhysicalDevicePointClippingProperties
	vkGetPhysicalDeviceProperties(gpu, &properties);

	VkPhysicalDeviceProperties2 deviceProperties2 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, .pNext = &deviceMultiviewProperties, .properties = {}};
	VkPhysicalDeviceFeatures2 deviceFeatures2 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext = &conditionalRenderingFeaturesExt,
	};

	vkGetPhysicalDeviceProperties2(gpu, &deviceProperties2);
	vkGetPhysicalDeviceFeatures2(gpu, &deviceFeatures2);

	capability->sDepthStencil = true;
	capability->sFramebuffer = true;
	capability->sFrameBufferMSAA = true;
	capability->sCubeMap = true;
	capability->sCubeMapArray = deviceFeatures2.features.imageCubeArray;
	capability->sQueryOcclusion = true;
	capability->sShaderImageLoadStorage = true;
	capability->sShaderStorageBuffer = true;
	capability->sUniformBuffer = true;
	capability->sMapBuffer = true;

	capability->sGeometryShader = deviceFeatures2.features.geometryShader;
	capability->sTessellation = deviceFeatures2.features.tessellationShader;
	capability->sTransformFeedback = deviceTransformFeedbackPropertiesExt.transformFeedbackDraw;
	capability->sIndirectMultiDraw = deviceFeatures2.features.multiDrawIndirect;
	capability->sIndirectDraw = deviceFeatures2.features.drawIndirectFirstInstance;

	capability->sVertexShader = true;
	capability->sFragmentShader = true;
	capability->sShadow = true;
	capability->sInstancing = true;
	capability->sTextureCompression = deviceFeatures2.features.textureCompressionASTC_LDR |
									  deviceFeatures2.features.textureCompressionETC2 |
									  deviceFeatures2.features.textureCompressionBC;
	capability->sVirtualViewPort = deviceFeatures2.features.multiViewport;
	capability->sConditionalRendering = conditionalRenderingFeaturesExt.conditionalRendering;
	// maxDrawIndirectCount

	//	deviceFeatures.samplerAnisotropy;
	//	deviceFeatures.shaderUniformBufferArrayDynamicIndexing;
	// deviceFeatures.shaderStorageImageMultisample;

	// TODO resolve.
	capability->sMaxViewPorts = properties.limits.maxViewports;
	capability->sMaxViewPortDims = properties.limits.maxViewportDimensions[0];
	capability->sViewPortBoundRange[0] = properties.limits.viewportBoundsRange[0];
	capability->sViewPortBoundRange[1] = properties.limits.viewportBoundsRange[1];
	capability->sMaxUniformBlockSize = properties.limits.maxUniformBufferRange;
	capability->sMaxFrameBufferWidth = properties.limits.maxFramebufferWidth;
	capability->sMaxFrameBufferHeight = properties.limits.maxFramebufferHeight;
	capability->sMaxFrameBufferLayers = properties.limits.maxFramebufferLayers;

	capability->lineWidthRange[0] = properties.limits.lineWidthRange[0];
	capability->lineWidthRange[1] = properties.limits.lineWidthRange[1];
	capability->lineWidthGranularity = properties.limits.lineWidthGranularity;
	properties.limits.strictLines;
	properties.limits.maxCullDistances;
	properties.limits.maxTexelOffset;
	properties.limits.maxUniformBufferRange;
	properties.limits.maxImageDimension2D;

	/*  Compute shader.*/
	capability->sMaxWorKGroupSize[0] = properties.limits.maxComputeWorkGroupCount[0];
	capability->sMaxWorKGroupSize[1] = properties.limits.maxComputeWorkGroupCount[1];
	capability->sMaxWorKGroupSize[2] = properties.limits.maxComputeWorkGroupCount[2];

	capability->sMaxLocalWorkGroupSize[0] = properties.limits.maxComputeWorkGroupSize[0];
	capability->sMaxLocalWorkGroupSize[1] = properties.limits.maxComputeWorkGroupSize[1];
	capability->sMaxLocalWorkGroupSize[2] = properties.limits.maxComputeWorkGroupSize[2];

	capability->sWorkGroupDimensions = 3;
	capability->sComputeShader = true;

	// properties.limits.maxColorAttachments;
	// properties.
	capability->sMaxAnisotropy = properties.limits.maxSamplerAnisotropy;
	capability->rayTracing = deviceRayTracingPropertiesNv.maxTriangleCount > 0;
}

void VKRenderInterface::getFeatures(Features *features) {

	assert(features);

	if (features == nullptr)
		throw InvalidArgumentException("Must not be a null pointer.");
}

const char *VKRenderInterface::getShaderVersion(ShaderLanguage language) const {
	static char shaderversion[64];

	return "";
}

ShaderLanguage VKRenderInterface::getShaderLanguage(void) const { return languageSupport; }

const char *VKRenderInterface::getAPIVersion(void) const {
	VkPhysicalDeviceProperties devicePropertie;

	/*	Get API version.	*/
	vkGetPhysicalDeviceProperties(gpu, &devicePropertie);
	static char apiversion[64];
	sprintf(apiversion, "%d.%d.%d", VK_VERSION_MAJOR(devicePropertie.apiVersion),
			VK_VERSION_MINOR(devicePropertie.apiVersion), VK_VERSION_PATCH(devicePropertie.apiVersion));
	return (const char *)apiversion;
}

const char *VKRenderInterface::getVersion(void) const {
	return FV_STR_VERSION(RENDER_VULKAN_MAJOR, RENDER_VULKAN_MINOR, RENDER_VULKAN_PATCH);
}

/*const char *VKRenderInterface::getName(void) const {
	return "Vulkan";
}*/

void VKRenderInterface::getStatus(MemoryInfo *memoryInfo) {}

CommandList *VKRenderInterface::createCommandBuffer(void) {}

void VKRenderInterface::submittCommand(Ref<CommandList> &list) {

	VKCommandList *l = (VKCommandList *)*list;

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &l->cmdBuffer;

	VkResult result = vkQueueSubmit(queue, 1, &submitInfo, nullptr);
	if (result != VK_SUCCESS)
		throw RuntimeException("Failed to submit to queue.");
}

void VKRenderInterface::execute(CommandList *list) {}

void *VKRenderInterface::getData(void) const { return nullptr; }

extern "C" IRenderer *createInternalRenderer(IConfig *config) { return new VKRenderInterface(config); }