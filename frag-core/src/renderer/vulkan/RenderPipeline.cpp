#include"Renderer/IRenderer.h"
#include"Renderer/vulkan/internal_object_type.h"
#include"Core/IConfig.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_video.h>
#include<SDL2/SDL_surface.h>
#include<SDL2/SDL_syswm.h>
#include<SDL2/SDL_vulkan.h>
#include"Renderer/vulkan/VKRenderWindow.h"
#include"Window/WindowManager.h"

#include<climits>
#include<vector>
#include<Utils/StringUtil.h>
#include"Utils/StringUtil.h"
#include<iostream>
#include"Renderer/RenderDesc.h"
#include"Exception/InvalidArgumentException.h"
#include "Exception/RuntimeException.h"

using namespace fragcore;

#define RENDER_VULKAN_MAJOR 0
#define RENDER_VULKAN_MINOR 1
#define RENDER_VULKAN_PATCH 0

// TODO enable
static bool validate_object_memeber(IRenderer *renderer, RenderObject *object) {
	return renderer == object->getRenderer();
}


IRenderer::IRenderer(IConfig *config) {
    static const std::vector<const char *> instanceExtensionNames = {
#if defined(FV_LINUX)
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#elif defined(FV_WIN)
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
        VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
		VK_KHR_DISPLAY_EXTENSION_NAME,
    };
	const std::vector<const char *> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation",
	};

	VulkanCore *vulkancore = NULL;
	IConfig setupConfig;
	if(config == NULL){
		setupConfig.set("debug", true);
		setupConfig.set("debug-tracer", true);
		setupConfig.set("gamma-correction", true);
	} else{
		//setupConfig = *config
	}

	/*  Initialize video support.   */
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
		throw RuntimeException(fvformatf("SDL_InitSubSystem failed, %s.\n", SDL_GetError()));
	}

	/*	Allocate private data structure for the renderer interface. */
	vulkancore = new VulkanCore();
	if(vulkancore == NULL)
		throw RuntimeException();

	/*  Store reference with the object. */
	this->pdata = vulkancore;

	/*  Determine layer validation. */
	vulkancore->enableValidationLayers = setupConfig.get<bool>("debug");
	vulkancore->enableValidationLayers = 1;
	vulkancore->enableDebugTracer = setupConfig.get<bool>("debug-tracer");
	vulkancore->useGamma = setupConfig.get<bool>("gamma-correction");

	/*  Vulkan variables.   */
	VkResult result;

	/*  Check for supported extensions.*/
	uint32_t extensionCount = 0;
	if (vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL) != VK_SUCCESS) {
		throw RuntimeException("Failed enumerate Vulkan extension properties");
	}
	vulkancore->extension_names = (VkExtensionProperties *) (malloc(sizeof(VkExtensionProperties) * extensionCount));
	if (vkEnumerateInstanceExtensionProperties(NULL, &extensionCount,
											   (VkExtensionProperties *) vulkancore->extension_names) != VK_SUCCESS) {
		throw RuntimeException("Failed enumerate Vulkan extension properties");
	}

	/*  Check for supported validation layers.  */
	uint32_t layerCount;
	if (vkEnumerateInstanceLayerProperties(&layerCount, NULL) != VK_SUCCESS)
		throw RuntimeException("Failed enumerate Vulkan extension properties");
	std::vector<VkLayerProperties> availableLayers(layerCount);
	if(vulkancore->enableValidationLayers){
		if (vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()) != VK_SUCCESS)
			throw RuntimeException("Failed enumerate Vulkan extension properties");

		/*  Check if exists.    */
		for(uint32_t i = 0; i < availableLayers.size(); i++){

		}
	}

	/*  TODO add support for loading requried extensions.   */
	//TODO improve later.
	/*  Create Vulkan window.   */
	SDL_Window *tmpWindow = SDL_CreateWindow("", 0, 0, 1, 1, SDL_WINDOW_VULKAN);
	if (tmpWindow == NULL)
		throw RuntimeException(fvformatf("failed create window - %s", SDL_GetError()));
	unsigned int count;
	if (!SDL_Vulkan_GetInstanceExtensions(tmpWindow, &count, NULL))
		throw RuntimeException(fvformatf("%s", SDL_GetError()));
	size_t additional_extension_count = instanceExtensionNames.size();
	instanceExtensionNames.resize(additional_extension_count + count);
	if (!SDL_Vulkan_GetInstanceExtensions(tmpWindow, &count, instanceExtensionNames.data() + additional_extension_count))
		throw RuntimeException(fvformatf("failed SDL_Vulkan_GetInstanceExtensions - %s", SDL_GetError()));
	SDL_DestroyWindow(tmpWindow);

	/*  Get Vulkan version. */
	uint32_t  version;
	result = vkEnumerateInstanceVersion(&version);
	if(result != VK_SUCCESS)
		throw RuntimeException(fvformatf("Failed to enumerate instance version - %d", result));

	/*	Primary Vulkan instance Object. */
	VkApplicationInfo ai = {};
	ai.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ai.pNext = NULL;
	ai.pApplicationName = "FragCore";
	ai.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	ai.pEngineName = "FragCore-Engine";
	ai.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	ai.apiVersion = version;

	VkDebugReportCallbackCreateInfoEXT callbackCreateInfoExt = {
		VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,    // sType
		NULL,                                                       // pNext
		VK_DEBUG_REPORT_ERROR_BIT_EXT |                             // flags
		VK_DEBUG_REPORT_WARNING_BIT_EXT,
		NULL,//myOutputDebugString,                                        // pfnCallback
		NULL                                                        // pUserData
	};
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoExt = {
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.pNext = &callbackCreateInfoExt,
	};
	VkValidationCheckEXT validationCheckExt[] = {
		VK_VALIDATION_CHECK_ALL_EXT
	};
	VkValidationFlagsEXT validationFlagsExt = {
		.sType = VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT,
		.pNext = NULL,//&debugUtilsMessengerCreateInfoExt,
		.disabledValidationCheckCount = 1,
		.pDisabledValidationChecks = validationCheckExt
	};

	/*	Prepare the instance object. */
	VkInstanceCreateInfo ici = {};
	ici.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	if(vulkancore->enableValidationLayers)
		ici.pNext = NULL;
	else
		ici.pNext = NULL;
	ici.flags = 0;
	ici.pApplicationInfo = &ai;
	if (vulkancore->enableValidationLayers) {
		ici.enabledLayerCount = validationLayers.size();
		ici.ppEnabledLayerNames = validationLayers.data();
	} else {
		ici.enabledLayerCount = 0;
		ici.ppEnabledLayerNames = NULL;
	}
	ici.enabledExtensionCount = instanceExtensionNames.size();
	ici.ppEnabledExtensionNames = instanceExtensionNames.data();

	/*	Create Vulkan instance.	*/
	result = vkCreateInstance(&ici, NULL, &vulkancore->inst);
	if (result != VK_SUCCESS)
		throw RuntimeException(fvformatf("vkCreateInstance - %d.", result));

	/*  Set debug mode.  */
	this->setDebug(setupConfig.get<bool>("debug"));

	/*	Get number of physical devices. */
	int num_physica_devices;
	result = vkEnumeratePhysicalDevices(vulkancore->inst, &vulkancore->num_physical_devices, NULL);
	if (result != VK_SUCCESS) {
		throw RuntimeException(fvformatf("Failed to get number physical devices - %d", result));
	}
	/*  Get all physical devices.    */
	vulkancore->physical_devices = (VkPhysicalDevice *) malloc(
			sizeof(VkPhysicalDevice) * vulkancore->num_physical_devices);
	assert(vulkancore->physical_devices);
	vulkancore->GPUs.resize(vulkancore->num_physical_devices);
	result = vkEnumeratePhysicalDevices(vulkancore->inst, &vulkancore->num_physical_devices,
										vulkancore->physical_devices);
	if (result != VK_SUCCESS)
		throw RuntimeException(fvformatf("Failed to enumerate physical devices - %d.", result));

	/*	*/
	uint32_t pPhysicalDeviceGroupCount;
	result = vkEnumeratePhysicalDeviceGroups(vulkancore->inst,&pPhysicalDeviceGroupCount, NULL);
	if (result != VK_SUCCESS)
		throw RuntimeException(fvformatf("vkEnumeratePhysicalDeviceGroups failed query - %d.", result));
	std::vector<VkPhysicalDeviceGroupProperties> phyiscalDevices;
	phyiscalDevices.resize(pPhysicalDeviceGroupCount);
	result = vkEnumeratePhysicalDeviceGroups(vulkancore->inst, &pPhysicalDeviceGroupCount, phyiscalDevices.data());
	if (result != VK_SUCCESS)
		throw RuntimeException(fvformatf("vkEnumeratePhysicalDeviceGroups failed fetching groups - %d.", result));
		

	/*  TODO add selection function. */
	std::vector<VkPhysicalDevice> gpucandiates;
	for(int i = 0; i < vulkancore->num_physical_devices; i++) {
		if(isDeviceSuitable(vulkancore->physical_devices[i])) {
			vulkancore->gpu = vulkancore->physical_devices[i];
			gpucandiates.push_back(vulkancore->physical_devices[i]);
			break;
		}
	}

	std::vector<VkPhysicalDevice> selectedDevices;
	selectDefaultDevices(gpucandiates, selectedDevices);

	/*  */
	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(vulkancore->gpu, &supportedFeatures);
	/*  Fetch memory properties.   */
	vkGetPhysicalDeviceMemoryProperties(vulkancore->gpu, &vulkancore->memProperties);

	/*  Select queue family.    */
	/*  TODO improve queue selection.   */
	vkGetPhysicalDeviceQueueFamilyProperties(vulkancore->gpu, &vulkancore->queue_count, NULL);

	vulkancore->queue_props = (VkQueueFamilyProperties *) malloc(
			sizeof(VkQueueFamilyProperties) * vulkancore->queue_count);
	vkGetPhysicalDeviceQueueFamilyProperties(vulkancore->gpu, &vulkancore->queue_count, vulkancore->queue_props);
	assert(vulkancore->queue_count >= 1);

	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(vulkancore->gpu, &features);

	/*  Select queue with graphic.  *///TODO add queue for compute and etc.
	uint32_t graphicsQueueNodeIndex = UINT32_MAX;
	for (uint32_t i = 0; i < vulkancore->queue_count; i++) {
		if ((vulkancore->queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
			if (graphicsQueueNodeIndex == UINT32_MAX)
				graphicsQueueNodeIndex = i;
		}
	}
	vulkancore->graphics_queue_node_index = graphicsQueueNodeIndex;

	float queue_priorities[1] = {1.0};
	const VkDeviceQueueCreateInfo queue = {
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.queueFamilyIndex = vulkancore->graphics_queue_node_index,
			.queueCount = 1,
			.pQueuePriorities = queue_priorities
	};

	/*  Required extensions.    */
	std::vector<const char *> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME,
			//VK_NV_RAY_TRACING_EXTENSION_NAME
			//VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME
			//VK_NV_GLSL_SHADER_EXTENSION_NAME
	};
	if (vulkancore->enableValidationLayers) {
		//TODO determine
		deviceExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
		//deviceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	VkDeviceGroupDeviceCreateInfo deviceGroupDeviceCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO,
	};
	if(phyiscalDevices.size() > 0){
		if (phyiscalDevices[0].physicalDeviceCount > 1) {
			deviceGroupDeviceCreateInfo.physicalDeviceCount = phyiscalDevices[0].physicalDeviceCount;
			deviceGroupDeviceCreateInfo.pPhysicalDevices = phyiscalDevices[0].physicalDevices;
		}
	}

	VkPhysicalDeviceShaderDrawParameterFeatures deviceShaderDrawParametersFeatures = {
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETER_FEATURES,
			.pNext = NULL,
			.shaderDrawParameters = VK_TRUE
	};
	VkPhysicalDeviceMultiviewFeatures deviceMultiviewFeatures = {
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES,
			.pNext = &deviceShaderDrawParametersFeatures,
			.multiview = VK_TRUE,
			.multiviewGeometryShader = VK_TRUE,
			.multiviewTessellationShader = VK_TRUE
	};
	VkPhysicalDeviceConditionalRenderingFeaturesEXT conditionalRenderingFeaturesExt = {
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT,
			.pNext = &deviceMultiviewFeatures,
			.conditionalRendering = VK_TRUE,
			.inheritedConditionalRendering = VK_TRUE
	};

	VkDeviceCreateInfo device = {};
	VkPhysicalDeviceFeatures deviceFeatures{};
	device.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device.pNext = &conditionalRenderingFeaturesExt;
	device.queueCreateInfoCount = 1;
	device.pQueueCreateInfos = &queue;
	if (vulkancore->enableValidationLayers) {
		device.enabledLayerCount = vulkancore->enabled_layer_count;
		device.ppEnabledLayerNames = (const char *const *) ((vulkancore->validate)
															? vulkancore->device_validation_layers : NULL);
	} else {
		device.enabledLayerCount = 0;
	}

	/*	Enable group.	*/
	if (deviceGroupDeviceCreateInfo.physicalDeviceCount > 1) {
        deviceShaderDrawParametersFeatures.pNext = &deviceGroupDeviceCreateInfo;
    }
	device.enabledExtensionCount = deviceExtensions.size();
	device.ppEnabledExtensionNames = deviceExtensions.data();
	device.pEnabledFeatures = &deviceFeatures;


	/*  Create device.  */
	result = vkCreateDevice(vulkancore->gpu, &device, NULL, &vulkancore->device);
	if (result != VK_SUCCESS)
		throw RuntimeException(fvformatf("Failed to create logical Device - %d", result));

	/*  Get all queues.    */
	vkGetDeviceQueue(vulkancore->device, vulkancore->graphics_queue_node_index, 0, &vulkancore->queue);
	vkGetDeviceQueue(vulkancore->device, vulkancore->graphics_queue_node_index, 0, &vulkancore->presentQueue);




	/*  Create command pool.    */
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
	cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolCreateInfo.queueFamilyIndex = vulkancore->graphics_queue_node_index;
	cmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	/*  Create command pool.    */
	result = vkCreateCommandPool(vulkancore->device, &cmdPoolCreateInfo, NULL, &vulkancore->cmd_pool);
	if(result != VK_SUCCESS)
		throw RuntimeException(fvformatf("failed to create command pool %d", result));

	/*  TODO determine which languages are supported.   */
//    uint32_t nExtensions;
//    result = vkEnumerateDeviceExtensionProperties(vulkancore->device , NULL, &nExtensions, NULL);
	vulkancore->languageSupport = SPIRV;
	this->getCapability(&vulkancore->capability);
}

IRenderer::~IRenderer(void) {
	/*  Release all vulkan resources.   */
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;

	/*  */
	if (vulkanCore->cmd_pool)
		vkDestroyCommandPool(vulkanCore->device, vulkanCore->cmd_pool, NULL);

	/*  */
	if (vulkanCore->swapChain) {
		vkDestroySwapchainKHR(vulkanCore->device, vulkanCore->swapChain->swapchain, NULL);
	}

	if (vulkanCore->device)
		vkDestroyDevice(vulkanCore->device, NULL);

	if (vulkanCore->inst)
		vkDestroyInstance(vulkanCore->inst, NULL);

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	free(this->pdata);
}

void IRenderer::OnInitialization(void){

}

void IRenderer::OnDestruction(void){

}


Texture *IRenderer::createTexture(TextureDesc *desc) {
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;

	Texture *texture = new Texture();
	VKTextureObject *vktex = new VKTextureObject();
	vktex->vulkanCore = vulkanCore;
	vktex->desc = *desc;
	texture->pdata = vktex;

	VkDeviceMemory stagingBufferMemory;
	VkDeviceSize imageSize = desc->pixelSize;

	/*  Check descriptor.    */
	if (!desc->pixel)
		throw RuntimeException("failed to load texture image!");

	VkBuffer stagingBuffer;
	::createBuffer(vulkanCore, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(vulkanCore->device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, desc->pixel, static_cast<size_t>(imageSize));
	vkUnmapMemory(vulkanCore->device, stagingBufferMemory);

	/*-------------Create Image --------*/
	VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;



	/*  Image description.  */
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = (VkImageType)getTextureTarget((TextureDesc::Target)desc->target);
	imageInfo.extent.width = static_cast<uint32_t>(desc->width);
	imageInfo.extent.height = static_cast<uint32_t>(desc->height);
	imageInfo.extent.depth = desc->depth;
	imageInfo.mipLevels = desc->numlevel;
	imageInfo.arrayLayers = desc->depth;

	/*  */
	imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	/*  Shading multisampling.  */
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0; // Optional

	/*  Create image.   */
	VkResult result = vkCreateImage(vulkanCore->device, &imageInfo, NULL, &vktex->texture);
	if(result != VK_SUCCESS){
		throw RuntimeException(fvformatf("Failed creating texture image - %d", result));
	}

	/*  */
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(vulkanCore->device, vktex->texture, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(vulkanCore, memRequirements.memoryTypeBits, properties);

	/*  Allocate memory for the texture.    */
	result = vkAllocateMemory(vulkanCore->device, &allocInfo, NULL, &stagingBufferMemory);
	if(result != VK_SUCCESS) {
		throw RuntimeException("failed to allocate image memory!");
	}
	vktex->imageMemory = stagingBufferMemory;

	/*  Bind texture and memory.*/
	result = vkBindImageMemory(vulkanCore->device, vktex->texture, vktex->imageMemory, 0);
	if(result != VK_SUCCESS){
		throw RuntimeException("Failed to create ");
	}


	/*  Sampler.    */
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = desc->sampler.anisotropy > 0.0 ? VK_TRUE : VK_FALSE;
	samplerInfo.maxAnisotropy = desc->sampler.anisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	result = vkCreateSampler(vulkanCore->device, &samplerInfo, NULL, &vktex->sampler);
	if ( result != VK_SUCCESS) {
		throw RuntimeException("failed to create texture sampler!");
	}

	return texture;
}

void IRenderer::deleteTexture(Texture *texture) {
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;
	VKTextureObject* vkTextureObject = (VKTextureObject*)texture->pdata;

	/*  Release resources.  */
	if(vkTextureObject->texture)
		vkDestroyImage(vulkanCore->device, vkTextureObject->texture, NULL);
	if(vkTextureObject->sampler)
		vkDestroySampler(vulkanCore->device, vkTextureObject->sampler, NULL);

	/*  Release objects.    */
	delete texture->pdata;
	delete texture;
}


Sampler* IRenderer::createSampler(SamplerDesc* desc){

	if(desc == NULL)
		throw InvalidArgumentException("Invalid sampler description pointer object.");

	/*  */
	Sampler* sampler = new Sampler();
	//sampler->pdata= samplerObject;
	return sampler;
}

void IRenderer::deleteSampler(Sampler* sampler){


}


ProgramPipeline *IRenderer::createPipeline(const ProgramPipelineDesc *desc) {

//    desc.
}

void IRenderer::deletePipeline(ProgramPipeline *obj) {

}


Shader *IRenderer::createShader(ShaderDesc *desc) {
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;

	/*  Create shader interface object. */
	Shader *shader = new Shader();
	VKShaderObject *shaobj = new VKShaderObject();
	shader->pdata = shaobj;
	shaobj->vulkanCore = vulkanCore;

	VkResult result;
	VkShaderModule vertShaderModule = NULL;
	VkShaderModule fragShaderModule = NULL;
	VkShaderModule geomShaderModule = NULL;
	VkShaderModule tessCShaderModule = NULL;
	VkShaderModule tessEShaderModule = NULL;

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
	if( desc->vertex.language & supportedLanguage)
		throw RuntimeException("Vertex shader language is not supported");
	if( desc->fragment.language & supportedLanguage)
		throw RuntimeException("Fragment shader language is not supported");
	if( desc->geometry.language & supportedLanguage)
		throw RuntimeException("Geometry shader language is not supported");
	if( desc->tessellationControl.language & supportedLanguage)
		throw RuntimeException("Tessellation Control shader language is not supported");
	if( desc->tessellationEvolution.language & supportedLanguage)
		throw RuntimeException("Tessellation Evolution shader language is not supported");
	if( desc->Compute.language & supportedLanguage)
		throw RuntimeException("Compute shader language is not supported");

	/*  Create shader module code containers.   */
	//TODO fix the code for the shader desc.
//	if(desc->vertex.vertexBinary && desc->vertex.language == SPIRV){
	if(1){
		//vertShaderModule = createShaderModule(vulkanCore->device, (const char*)desc->vertex.vertexBinary, desc->vertex.size);
		vertShaderModule = createShaderModule(vulkanCore->device,  (const char*)NULL, 0);//FIXME

		/*  */
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		shaderStages.push_back(vertShaderStageInfo);
	}
//	if(desc->fragment.fragmentBinary && desc->fragment.language == SPIRV){
	if(1){
		//fragShaderModule = createShaderModule(vulkanCore->device,  (const char*)desc->fragment.fragmentBinary, desc->fragment.size);
		fragShaderModule = createShaderModule(vulkanCore->device,  (const char*)NULL, 0); //FIXME

		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		shaderStages.push_back(fragShaderStageInfo);
	}
//	if(desc->geometry.geometryBinary && desc->geometry.language == SPIRV){
//		geomShaderModule = createShaderModule(vulkanCore->device, desc->vertexsource[0], strlen(desc->vertexsource[0]));
//
//		geomShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//		geomShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
//		geomShaderStageInfo.module = geomShaderModule;
//		geomShaderStageInfo.pName = "main";
//
//		shaderStages.push_back(geomShaderStageInfo);
//	}
//	if(desc->numtesco > 0){
//		tessCShaderModule = createShaderModule(vulkanCore->device, desc->vertexsource[0], strlen(desc->vertexsource[0]));
//
//		tessChaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//		tessChaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
//		tessChaderStageInfo.module = tessCShaderModule;
//		tessChaderStageInfo.pName = "main";
//
//		shaderStages.push_back(tessChaderStageInfo);
//	}
//	if(desc->numtesev> 0){
//		tessEShaderModule = createShaderModule(vulkanCore->device, desc->vertexsource[0], strlen(desc->vertexsource[0]));
//
//		tessEShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//		tessEShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
//		tessEShaderStageInfo.module = tessEShaderModule;
//		tessEShaderStageInfo.pName = "main";
//
//		shaderStages.push_back(tessEShaderStageInfo);
//	}

	// If using compute shader.
	if(desc->Compute.language == SPIRV){

		VkDescriptorSetLayoutBinding bindings[2] = { { 0 } };
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
//        info.stage.module = loadShaderModule(device, "shaders/particle.comp.spv");
//        info.stage.pName = "main";
//        info.layout = computePipeline.pipelineLayout;

		VkComputePipelineCreateInfo computePipelineCreateInfo = {};
		//result = vkCreateComputePipelines(vulkanCore->gpu, NULL, 1, &computePipelineCreateInfo, NULL, &shaobj->graphicsPipeline);
	}

	/*  */
	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = NULL;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount =  1;
	layoutInfo.pBindings = &samplerLayoutBinding;

	VkDescriptorSetLayout setLayout;
	result = vkCreateDescriptorSetLayout(vulkanCore->device, &layoutInfo, NULL, &setLayout);
	if(result != VK_SUCCESS)
		throw RuntimeException(fvformatf("Failed to create descriptor set layout - %d", result));

	/*  */
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1; // Optional
	pipelineLayoutInfo.pSetLayouts = &setLayout; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = NULL; // Optional

	result = vkCreatePipelineLayout(vulkanCore->device, &pipelineLayoutInfo, NULL, &shaobj->pipelineLayout);
	if (result != VK_SUCCESS)
		throw RuntimeException(fvformatf("failed to create pipeline layout - %d!", result));

	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = vulkanCore->swapChain->swapChainImageFormat;
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

	if (vkCreateRenderPass(vulkanCore->device, &renderPassInfo, NULL, &renderPass) != VK_SUCCESS)
		throw RuntimeException(fvformatf("failed to create render pass - %d", result));

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
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

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
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_LINE_WIDTH,
			VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
			VK_DYNAMIC_STATE_BLEND_CONSTANTS,
			VK_DYNAMIC_STATE_DEPTH_BOUNDS,
	};

	/*  */
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = sizeof(dynamicStates) / sizeof(dynamicStates[0]);
	dynamicState.pDynamicStates = dynamicStates;

	/*  */
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) vulkanCore->swapChain->chainExtend.width;
	viewport.height = (float) vulkanCore->swapChain->chainExtend.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	/*  */
	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = vulkanCore->swapChain->chainExtend;

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
	pipelineInfo.pDepthStencilState = NULL; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState; // Optional
	pipelineInfo.layout = shaobj->pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	/*  Create graphic pipeline.    */
	result = vkCreateGraphicsPipelines(vulkanCore->device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL,
									   &shaobj->graphicsPipeline);
	if (result != VK_SUCCESS)
		throw RuntimeException(fvformatf("vkCreateGraphicsPipelines failed - %d", result));

	/*  Release shader moudles once used.  */
	if (vertShaderModule)
		vkDestroyShaderModule(vulkanCore->device, vertShaderModule, NULL);
	if (fragShaderModule)
		vkDestroyShaderModule(vulkanCore->device, fragShaderModule, NULL);
	if (geomShaderModule)
		vkDestroyShaderModule(vulkanCore->device, geomShaderModule, NULL);
	if (tessCShaderModule)
		vkDestroyShaderModule(vulkanCore->device, tessCShaderModule, NULL);
	if (tessEShaderModule)
		vkDestroyShaderModule(vulkanCore->device, tessEShaderModule, NULL);

	return shader;
}

void IRenderer::deleteShader(Shader *shader) {
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;
	VKShaderObject* shaobj = (VKShaderObject*)shader->pdata;

	/*  validate object.   */
	if(shader->getRenderer() != this)
		throw InvalidArgumentException("");

	if(shaobj->graphicsPipeline){
		//vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
		/*  Release all resources.  */
		vkDestroyPipeline(vulkanCore->device, shaobj->graphicsPipeline, nullptr);
	} else
		throw RuntimeException("");

	delete shader->pdata;
	delete shader;
}


Buffer *IRenderer::createBuffer(BufferDesc *desc) {
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;

	VkResult result;
	Buffer* buffer;
	VKBufferObject* vkBufferObject = new VKBufferObject();
	vkBufferObject->vulkanCore = (VulkanCore*)this->pdata;
	buffer = new Buffer();
	buffer->pdata = vkBufferObject;

	/*  Buffer description. */
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = desc->size;
	bufferInfo.usage = (VkBufferUsageFlags)getBufferType((BufferDesc::BufferType)desc->type);
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	/*  Create buffer.  */
	result = vkCreateBuffer(vulkanCore->device, &bufferInfo, NULL, &vkBufferObject->buffer);
	if (result != VK_SUCCESS) {
		throw RuntimeException(fvformatf("failed to create vertex buffer %d", result));
	}

	/*  */
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(vulkanCore->device, vkBufferObject->buffer, &memRequirements);

	/*  */
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(vulkanCore->gpu, &memProperties);

	/*  Allocate memory.    */
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(vulkanCore, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	/*  */
	if (vkAllocateMemory(vulkanCore->device, &allocInfo, nullptr, &vkBufferObject->vertexBufferMemory) != VK_SUCCESS) {
		throw RuntimeException(fvformatf("failed to allocate vertex buffer memory!"));
	}

	/*  */
	result = vkBindBufferMemory(vulkanCore->device, vkBufferObject->buffer, vkBufferObject->vertexBufferMemory, 0);
	if (result != VK_SUCCESS)
		throw RuntimeException(fvformatf("failed to allocate vertex buffer memory!"));

	return buffer;
}

void IRenderer::deleteBuffer(Buffer *object) {
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;
	VKBufferObject* bufferObject = (VKBufferObject*)object->pdata;

	//vkFreeMemory(vulkanCore->device, vertexBufferMemory, nullptr);
	if(bufferObject->buffer)
		vkDestroyBuffer(vulkanCore->device, bufferObject->buffer, NULL);

	/*  Release objects.    */
	delete object->pdata;
	delete object;
}

Geometry *IRenderer::createGeometry(GeometryDesc *desc) {
	Geometry* geometryObject;
	VKGeometryObject* glgeoobj = NULL;
	unsigned int x;

	/*	*/
	geometryObject = new Geometry();
	glgeoobj = new VKGeometryObject();

	/*	Requires array buffer.  */
	if(desc->numVerticecs > 0){
		BufferDesc abuffer;
		abuffer.size = desc->numVerticecs * desc->vertexStride;
		abuffer.type = BufferDesc::eArray;
		abuffer.data = desc->buffer;
		abuffer.hint = (BufferDesc::BufferHint)(BufferDesc::eWrite | BufferDesc::eStatic);
		glgeoobj->vertexbuffer = createBuffer(&abuffer);
		glgeoobj->vertexbuffer->bind();

	}

	/*	Requires element buffer.    */
	if(desc->numIndices > 0){
		BufferDesc abuffer;
		abuffer.size = desc->numIndices * desc->indicesStride;
		abuffer.type = BufferDesc::eElementArray;
		abuffer.data = desc->indices;
		abuffer.hint = (BufferDesc::BufferHint)(BufferDesc::eWrite | BufferDesc::eStatic);
		glgeoobj->indicesbuffer = createBuffer(&abuffer);
		glgeoobj->indicesbuffer->bind();

		/*  Determine indices data size type.   */
		switch(desc->indicesStride){
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


	glgeoobj->mode = getPrimitive((GeometryDesc::Primitive)desc->primitive);
	glgeoobj->desc = *desc;

	geometryObject->pdata = glgeoobj;
	return geometryObject;
}

void IRenderer::deleteGeometry(Geometry *obj) {
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;
	VKGeometryObject* glgeoobj = (VKGeometryObject*)obj->pdata;

	deleteBuffer(glgeoobj->vertexbuffer);
	deleteBuffer(glgeoobj->indicesbuffer);

	/*  Release objects.    */
	delete obj->pdata;
	delete obj;
}

//void IRenderer::setViewport(int x, int y, int width, int height) {
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
//	for(uint32_t i = 0; i < vulkanCore->swapChain->swapchainImageCount; i++)
//		vkCmdSetViewport(vulkanCore->swapChain->commandBuffers[i], 0, 1, &viewport);
//
//	/*  Create swapchain.   */
//}

ViewPort* IRenderer::getView(unsigned int i){
	VulkanCore *vulkancore = (VulkanCore *) this->pdata;

	/*  Validate the index. */
	if(i >= vulkancore->capability.sMaxViewPorts)
		throw InvalidArgumentException(
				fvformatf("Does not support viewport index %d, max index %d.", i, vulkancore->capability.sMaxViewPorts));


//	// If the view does not exits. Create it.
//	if(i == 0)
//		return glcore->defaultViewport;
//	return glcore->viewports[i - 1];
}

FrameBuffer *IRenderer::createFrameBuffer(FrameBufferDesc *desc) {



	return nullptr;
}

void IRenderer::deleteFrameBuffer(FrameBuffer *obj) {

}

QueryObject* IRenderer::createQuery(QueryDesc* desc){}
void IRenderer::deleteQuery(QueryObject* query){}

RendererWindow *IRenderer::createWindow(int x, int y, int width, int height) {
    VulkanCore *vulkancore = (VulkanCore *)this->pdata;

	/*	*/
    Ref<IRenderer> rendRef = Ref<IRenderer>(this);
    RendererWindow *window = new VKRenderWindow(rendRef);

	/*	*/
    vulkancore->windows.push_back(window);
    return window;
}

void IRenderer::createSwapChain(void) {
	/*  Make sure previous swap chain is removed.   */
	VulkanCore *vulkancore = (VulkanCore *) this->pdata;

	vkDeviceWaitIdle(vulkancore->device);
//    for (size_t i = 0; i < vulkancore->swapChain->swapchainImageCount; i++) {
//        VkImageView attachments[] = {
//                vulkancore->swapChain->view[i]
//        };
//
//        VkFramebufferCreateInfo framebufferInfo = {};
//        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//        framebufferInfo.renderPass = renderPass;
//        framebufferInfo.attachmentCount = 1;
//        framebufferInfo.pAttachments = attachments;
//        framebufferInfo.width = vulkancore->swapChain->chainExtend.width;
//        framebufferInfo.height = vulkancore->swapChain->chainExtend.height;
//        framebufferInfo.layers = 1;
//
//        if (vkCreateFramebuffer(vulkancore->device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
//            throw RuntimeException("failed to create framebuffer!");
//
//    }
}

void IRenderer::setCurrentWindow(RendererWindow *window) {
	/*  */
}


FrameBuffer *IRenderer::getDefaultFramebuffer(void *window) {
	static FrameBuffer *defaultFrambuffer = NULL;
	/*  TODO add support.   */
	if (defaultFrambuffer == NULL) {
		FrameBuffer *frameBuffer = new FrameBuffer();
	}

	return defaultFrambuffer;
}

void IRenderer::clear(unsigned int bitflag) {
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;
	const uint32_t currentFrame = vulkanCore->currentFrame;
	return ;
	if(bitflag & eColor){
		VkClearColorValue clearColor = {vulkanCore->color[0], vulkanCore->color[1], vulkanCore->color[2], vulkanCore->color[3]};
		/*  */
		VkImageSubresourceRange imageRange = {};
		imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageRange.levelCount = 1;
		imageRange.layerCount = 1;


		vkCmdClearColorImage(vulkanCore->swapChain->commandBuffers[vulkanCore->currentFrame],
							 vulkanCore->swapChain->swapChainImages[vulkanCore->currentFrame], VK_IMAGE_LAYOUT_GENERAL,
							 &clearColor, 1, &imageRange);
	}

	if(bitflag & eDepth || bitflag & eStencil){
		VkClearDepthStencilValue depthStencilClear = {0.0f, 0};

		/*  */
		VkImageSubresourceRange imageRange = {};
		imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageRange.levelCount = 1;
		imageRange.layerCount = 1;


		vkCmdClearDepthStencilImage(vulkanCore->swapChain->commandBuffers[vulkanCore->currentFrame],
									vulkanCore->swapChain->swapChainImages[vulkanCore->currentFrame], VK_IMAGE_LAYOUT_GENERAL,
									&depthStencilClear, 1, &imageRange);
	}

}


void IRenderer::clearColor(float r, float g, float b, float a) {

	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;
	/*  Cache the clear color.  */
	vulkanCore->color[0] = r;
	vulkanCore->color[1] = g;
	vulkanCore->color[2] = b;
	vulkanCore->color[3] = a;
}


void IRenderer::setVSync(int sync) {
	/*  Change the toolchain.   */
}

void IRenderer::setDepthMask(bool flag) {
	/*  */
}

void IRenderer::enableState(IRenderer::State state) {
	/*  */
}

void IRenderer::disableState(IRenderer::State state) {
	/*  */
}

bool IRenderer::isStateEnabled(IRenderer::State state){}

void IRenderer::swapBuffer(void) {

	// VkResult result;
	// uint32_t imageIndex;

	// VulkanCore *vulkanCore = (VulkanCore *) this->pdata;
	// result = vkEndCommandBuffer(vulkanCore->swapChain->commandBuffers[vulkanCore->currentFrame]);
	// if(result  != VK_SUCCESS)
	// 	throw RuntimeException("failed");

	// /*  */
	// result = vkAcquireNextImageKHR(vulkanCore->device, vulkanCore->swapChain->swapchain, UINT64_MAX,
	// 							   vulkanCore->renderFinishedSemaphore, VK_NULL_HANDLE, &imageIndex);
	// if(result == VK_ERROR_OUT_OF_DATE_KHR){
	// 	/*  Recreate.   */
	// }
	// if(result != VK_SUCCESS)
	// 	throw RuntimeException(fvformatf("Failed to acquire next image - %d", result));

	// VkSubmitInfo submitInfo = {};
	// submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	// submitInfo.commandBufferCount   = 1;
	// submitInfo.pCommandBuffers      = &vulkanCore->swapChain->commandBuffers[imageIndex];

	// result = vkQueueSubmit(vulkanCore->queue, 1, &submitInfo, NULL);
	// if(result != VK_SUCCESS)
	// 	throw RuntimeException("Failed to submit to queue.");

	// VkPresentInfoKHR presentInfo = {};
	// presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	// presentInfo.swapchainCount     = 1;
	// presentInfo.pSwapchains        = &vulkanCore->swapChain->swapchain;
	// presentInfo.pImageIndices      = &imageIndex;

	// result = vkQueuePresentKHR(vulkanCore->queue, &presentInfo);
	// if(result != VK_SUCCESS)
	// 	throw RuntimeException(fvformatf("Failed to present - %d", result));

	// vkQueueWaitIdle(vulkanCore->queue);

	// /*  Compute current frame.  */
	// vulkanCore->currentFrame = (vulkanCore->currentFrame + 1) % vulkanCore->swapChain->swapChainImages.size();

	// /*  Reset command buffer.    */
	// result = vkResetCommandBuffer(vulkanCore->swapChain->commandBuffers[vulkanCore->currentFrame], VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	// if(result != VK_SUCCESS)
	// 	throw RuntimeException("failed");

	// /*  */
	// VkCommandBufferBeginInfo beginInfo = {};
	// beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	// beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	// /*  */
	// result = vkBeginCommandBuffer(vulkanCore->swapChain->commandBuffers[vulkanCore->currentFrame], &beginInfo);
	// if(result != VK_SUCCESS)
	// 	throw RuntimeException("Failed to start the ");
}


void IRenderer::drawInstance(Geometry *geometry, unsigned int num) {

	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;

	VKGeometryObject* glgeo;
	assert(geometry && num > 0);

	/*  */
	const uint32_t curFrame = vulkanCore->currentFrame;
	glgeo = (VKGeometryObject*)geometry->pdata;

	/*  */
	VKBufferObject* vertexBuffer = (VKBufferObject*)glgeo->vertexbuffer->pdata;
	VkBuffer vertexBuffers[] = {vertexBuffer->buffer};

	/*  */
	if(glgeo->desc.numIndices > 0){
		assert(glgeo->vertexbuffer && glgeo->indicesbuffer);
		VkDeviceSize offsets[] = {0};

		VKBufferObject* indexBuffer = (VKBufferObject*)glgeo->indicesbuffer->pdata;
		vkCmdBindVertexBuffers(vulkanCore->swapChain->commandBuffers[curFrame], 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(vulkanCore->swapChain->commandBuffers[curFrame], indexBuffer->buffer, offsets[0], VK_INDEX_TYPE_UINT16);

		vkCmdDrawIndexed(vulkanCore->swapChain->commandBuffers[curFrame], static_cast<uint32_t>(geometry->getIndicesCount()), 1, 0, 0, 0);

	}else{
		assert(glgeo->vertexbuffer);

		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(vulkanCore->swapChain->commandBuffers[curFrame], 0, 1, vertexBuffers, offsets);

		vkCmdDraw(vulkanCore->swapChain->commandBuffers[curFrame], static_cast<uint32_t>(geometry->getVertexCount()), 1, 0, 0);
	}
}

void IRenderer::drawMultiInstance(Geometry& geometries, const unsigned int* first, const unsigned int* count, unsigned int num){}
void IRenderer::drawMultiIndirect(Geometry& geometries, unsigned int offset, unsigned int indirectCount){}

void IRenderer::drawIndirect(Geometry *geometry){

}

void IRenderer::setLineWidth(float width){
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;
	vkCmdSetLineWidth(vulkanCore->draw_cmd, width);
}

void IRenderer::blit(const FrameBuffer *source, FrameBuffer *dest, Texture::FilterMode filterMode) {    /*  TODO add filter.    */

	//vkCmdBlitImage()

}

void IRenderer::bindTextures(unsigned int firstUnit, const std::vector<Texture*>& textures){

}

void IRenderer::bindImages(unsigned int firstUnit, const std::vector<Texture *> &textures,
						   const std::vector<Texture::MapTarget> &mapping,
						   const std::vector<Texture::Format> &formats){

}

void IRenderer::copyTexture(const Texture *source, Texture *target) {
	//vkCmdCopyImage()
}

void IRenderer::dispatchCompute(unsigned int* global, unsigned int* local, unsigned int offset){
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;

	/*  */
	const uint32_t curFrame = vulkanCore->currentFrame;
	VkCommandBuffer curBuffer = vulkanCore->swapChain->commandBuffers[curFrame];

	/*  */
	vkCmdDispatch(curBuffer, local[0], local[1], local[2]);
}

void IRenderer::memoryBarrier(void){

}

Sync* IRenderer::createSync(SyncDesc* desc){

}
void IRenderer::deleteSync(Sync* sync){

}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
		void *pUserData) {

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

void IRenderer::setDebug(bool enable) {
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;

	VkResult result;
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(
			vulkanCore->inst, "vkCreateDebugReportCallbackEXT");

	if(vkCreateDebugReportCallbackEXT){
		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		createInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)debugCallback;
		createInfo.pUserData = NULL;


		// result = vkCreateDebugReportCallbackEXT(vulkanCore->inst, &createInfo, NULL, &vulkanCore->debugReport);
		// if(result != VK_SUCCESS)
		// 	throw RuntimeException(fvformatf("Failed to create debug report callback - %d", result));
	}

    PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vulkanCore->inst, "vkCreateDebugUtilsMessengerEXT");
    if (CreateDebugUtilsMessengerEXT) {
		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;

		if (CreateDebugUtilsMessengerEXT(vulkanCore->inst, &createInfo, nullptr,
											&vulkanCore->debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		} else {
			//return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}
}

void IRenderer::getSupportedTextureCompression(TextureDesc::Compression* pCompressions){
	if (pCompressions == NULL)
		throw InvalidArgumentException("pCompressions may not be a null pointer.");

	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;
	unsigned int compressions = 0;
	VkPhysicalDeviceFeatures deviceFeatures;

	vkGetPhysicalDeviceFeatures(vulkanCore->gpu, &deviceFeatures);

	if(deviceFeatures.textureCompressionBC)
		compressions |= TextureDesc::Compression::eNoCompression;
	if(deviceFeatures.textureCompressionETC2)
		compressions |= TextureDesc::Compression::eETC2;
	if(deviceFeatures.textureCompressionASTC_LDR)
		compressions |= TextureDesc::Compression::eASTC_LDR;

	// Add support for default compression format.
	if(compressions != 0)
		compressions |= TextureDesc::eCompression;

	*pCompressions = (TextureDesc::Compression) compressions;
}

void IRenderer::getCapability(Capability *capability) {
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;
	assert(capability);

	if(capability == NULL)
		throw InvalidArgumentException("Must not be a null pointer.");

	/*  */
	VkPhysicalDeviceProperties properties;

	/*  Propertie set.  */
	VkPhysicalDeviceRayTracingPropertiesNV deviceRayTracingPropertiesNv = {
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV,
			.pNext = NULL,
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
			.minImportedHostPointerAlignment = 0
	};
	VkPhysicalDeviceMultiviewProperties deviceMultiviewProperties = {
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES,
			.pNext = &deviceExternalMemoryHostPropertiesExt,
	};


	/*  Feature set.    */
	VkPhysicalDeviceMultiviewFeatures deviceMultiviewFeatures = {
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES,
			.pNext = NULL,
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

	//VkPhysicalDevicePointClippingProperties
	vkGetPhysicalDeviceProperties(vulkanCore->gpu, &properties);

	VkPhysicalDeviceProperties2 deviceProperties2 = {
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
			.pNext = &deviceMultiviewProperties,
			.properties = {}
	};
	VkPhysicalDeviceFeatures2 deviceFeatures2 = {
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
			.pNext = &conditionalRenderingFeaturesExt,
	};

	vkGetPhysicalDeviceProperties2(vulkanCore->gpu, &deviceProperties2);
	vkGetPhysicalDeviceFeatures2(vulkanCore->gpu, &deviceFeatures2);

	capability->sDepthStencil = VK_TRUE;
	capability->sFramebuffer = VK_TRUE;
	capability->sFrameBufferMSAA = VK_TRUE;
	capability->sCubeMap = VK_TRUE;
	capability->sCubeMapArray = deviceFeatures2.features.imageCubeArray;
	capability->sQueryOcclusion = VK_FALSE;
	capability->sShaderImageLoadStorage = VK_TRUE;
	capability->sShaderStorageBuffer = VK_TRUE;
	capability->sUniformBuffer = VK_TRUE;
	capability->sMapBuffer = VK_TRUE;

	capability->sGeometryShader = deviceFeatures2.features.geometryShader;
	capability->sTessellation = deviceFeatures2.features.tessellationShader;
	capability->sTransformFeedback = deviceTransformFeedbackPropertiesExt.transformFeedbackDraw;
	capability->sIndirectMultiDraw = deviceFeatures2.features.multiDrawIndirect;
	capability->sIndirectDraw = deviceFeatures2.features.drawIndirectFirstInstance;

	capability->sVertexShader = VK_TRUE;
	capability->sFragmentShader = VK_TRUE;
	capability->sShadow = VK_TRUE;
	capability->sInstancing = VK_TRUE;
	capability->sTextureCompression = deviceFeatures2.features.textureCompressionASTC_LDR | deviceFeatures2.features.textureCompressionETC2 |deviceFeatures2.features.textureCompressionBC;
	capability->sVirtualViewPort = deviceFeatures2.features.multiViewport;
	capability->sConditionalRendering = conditionalRenderingFeaturesExt.conditionalRendering;
	// maxDrawIndirectCount

//	deviceFeatures.samplerAnisotropy;
//	deviceFeatures.shaderUniformBufferArrayDynamicIndexing;
	//deviceFeatures.shaderStorageImageMultisample;

	//TODO resolve.
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
	capability->sComputeShader = VK_TRUE;

	//properties.limits.maxColorAttachments;
	//properties.
	capability->sMaxAnisotropy  = properties.limits.maxSamplerAnisotropy;
	capability->rayTracing = deviceRayTracingPropertiesNv.maxTriangleCount > 0;

}



const char *IRenderer::getShaderVersion(ShaderLanguage language) const {
	static char shaderversion[64];

	return "";
}

ShaderLanguage IRenderer::getShaderLanguage(void) const {
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;

	return vulkanCore->languageSupport;
}

const char *IRenderer::getAPIVersion(void) const {
	VkPhysicalDeviceProperties devicePropertie;
	VulkanCore *vulkanCore = (VulkanCore *) this->pdata;

	/*	Get API version.	*/
	vkGetPhysicalDeviceProperties(vulkanCore->gpu, &devicePropertie);
	static char apiversion[64];
	sprintf(apiversion, "%d.%d.%d", VK_VERSION_MAJOR(devicePropertie.apiVersion),
			VK_VERSION_MINOR(devicePropertie.apiVersion),
			VK_VERSION_PATCH(devicePropertie.apiVersion));
	return (const char *) apiversion;
}

const char *IRenderer::getVersion(void) const {
	return FV_STR_VERSION(RENDER_VULKAN_MAJOR, RENDER_VULKAN_MINOR, RENDER_VULKAN_PATCH);
}

/*const char *IRenderer::getName(void) const {
	return "Vulkan";
}*/

void IRenderer::getStatus(MemoryInfo* memoryInfo){}

void* IRenderer::getData(void) {
	return this->pdata;
}


extern "C" IRenderer *createInternalRenderer(IConfig *config) {
	return new IRenderer(config);
}
