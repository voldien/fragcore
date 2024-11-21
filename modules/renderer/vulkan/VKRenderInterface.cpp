#include "VKRenderInterface.h"
#include "Core/IConfig.h"

#include "../RenderDesc.h"

#include "VKRenderWindow.h"
#include "internal_object_type.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_vulkan.h>
#include <SDLWindowManager.h>

#include <VKHelper.h>
#include <VKUtil.h>
#include <climits>
#include <fmt/core.h>
#include <vector>

using namespace fragcore;
using namespace fvkcore;

#define RENDER_VULKAN_MAJOR 0
#define RENDER_VULKAN_MINOR 1
#define RENDER_VULKAN_PATCH 0

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
	// IConfig setupConfig;
	/*	Default config parameters.	*/
	if (config == nullptr) {
		// setupConfig.set("debug", true);
		// setupConfig.set("debug-tracer", true);
		// setupConfig.set("gamma-correction", true);
	} else {
		// setupConfig = *config
	}

	/*  Initialize video support.   */
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
		throw RuntimeException(fmt::format("SDL_InitSubSystem failed, %s.\n", SDL_GetError()));
	}

	/*  Store reference with the object. */
	std::unordered_map<const char *, bool> required_instance_extensions = {{VK_KHR_SURFACE_EXTENSION_NAME, true},
																		   {"VK_KHR_xlib_surface", true}};

	std::unordered_map<const char *, bool> required_instance_layer = {{"VK_LAYER_KHRONOS_validation", true}};
	std::unordered_map<const char *, bool> required_device_extensions = {{VK_KHR_SWAPCHAIN_EXTENSION_NAME, true}};

	/*	*/
	this->core = std::make_shared<VulkanCore>(required_instance_extensions, required_instance_layer);
	std::vector<std::shared_ptr<PhysicalDevice>> devices = core->createPhysicalDevices();
	this->device = std::make_shared<VKDevice>(devices, required_device_extensions);

	this->languageSupport = SPIRV;
	this->getCapability(&this->capability);

	// this->queue = getDevice()->getDefaultGraphicQueue();
}

VKRenderInterface::~VKRenderInterface() {
	/*  Release all vulkan resources.   */
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void VKRenderInterface::onInitialization() {}

void VKRenderInterface::onDestruction() {}

bool VKRenderInterface::supportCompute() noexcept { return true; }

RendererWindow *VKRenderInterface::createWindow(int x, int y, int width, int height) {

	/*	*/
	Ref<VKRenderInterface> rendRef = Ref<VKRenderInterface>(this);
	RendererWindow *window = new VKRenderWindow(rendRef);

	/*	*/
	windows.push_back(window);
	return window;
}

void VKRenderInterface::setCurrentWindow(RendererWindow *window) { /*  */ }

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
													VkDebugUtilsMessageTypeFlagsEXT messageType,
													const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
													void *pUsinst) {
	return VK_FALSE;
}

void VKRenderInterface::setDebug(bool enable) {

	VkResult result;
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT =
		(PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(core->getHandle(), "vkCreateDebugReportCallbackEXT");

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
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(core->getHandle(), "vkCreateDebugUtilsMessengerEXT");
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

		if (CreateDebugUtilsMessengerEXT(core->getHandle(), &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		} // return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void VKRenderInterface::getSupportedTextureCompression(TextureDesc::Compression *pCompressions) {
	if (pCompressions == nullptr) {
		throw InvalidArgumentException("pCompressions may not be a null pointer.");
	}

	unsigned int compressions = 0;
	VkPhysicalDeviceFeatures deviceFeatures;

	// vkGetPhysicalDeviceFeatures(getDevice()-> gpu, &deviceFeatures);

	if (deviceFeatures.textureCompressionBC) {
		compressions |= (unsigned int)TextureDesc::Compression::NoCompression;
	}
	if (deviceFeatures.textureCompressionETC2) {
		compressions |= (unsigned int)TextureDesc::Compression::ETC2;
	}
	if (deviceFeatures.textureCompressionASTC_LDR) {
		compressions |= (unsigned int)TextureDesc::Compression::ASTC_LDR;
	}

	// Add support for default compression format.
	if (compressions != 0) {
		compressions |= (unsigned int)TextureDesc::Compression::Compression;
	}

	*pCompressions = (TextureDesc::Compression)compressions;
}

void VKRenderInterface::getCapability(Capability *capability) {

	assert(capability);

	if (capability == nullptr) {
		throw InvalidArgumentException("Must not be a null pointer.");
	}

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

	VkPhysicalDeviceProperties2 deviceProperties2 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, .pNext = &deviceMultiviewProperties, .properties = {}};
	VkPhysicalDeviceFeatures2 deviceFeatures2 = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext = &conditionalRenderingFeaturesExt,
	};
	this->device->getPhysicalDevice(0)->checkFeature<VkPhysicalDeviceConditionalRenderingFeaturesEXT>(
		VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT, conditionalRenderingFeaturesExt);

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
	// properties.limits.strictLines;
	// properties.limits.maxCullDistances;
	// properties.limits.maxTexelOffset;
	// properties.limits.maxUniformBufferRange;
	// properties.limits.maxImageDimension2D;

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

	if (features == nullptr) {
		throw InvalidArgumentException("Must not be a null pointer.");
	}
}

const char *VKRenderInterface::getShaderVersion(ShaderLanguage language) const {
	static char shaderversion[64];

	return "";
}

ShaderLanguage VKRenderInterface::getShaderLanguage() const { return languageSupport; }

const char *VKRenderInterface::getAPIVersion() const {
	const VkPhysicalDeviceProperties &devicePropertie = this->device->getPhysicalDevice(0)->getProperties();
	/*	Get API version.	*/
	static char apiversion[64];
	sprintf(apiversion, "%d.%d.%d", VK_VERSION_MAJOR(devicePropertie.apiVersion),
			VK_VERSION_MINOR(devicePropertie.apiVersion), VK_VERSION_PATCH(devicePropertie.apiVersion));
	return (const char *)apiversion;
}

const char *VKRenderInterface::getVersion() const {
	return FV_STR_VERSION(RENDER_VULKAN_MAJOR, RENDER_VULKAN_MINOR, RENDER_VULKAN_PATCH);
}

void VKRenderInterface::getStatus(MemoryInfo *memoryInfo) {}

void *VKRenderInterface::getData() const { return nullptr; }

extern "C" IRenderer *createInternalRenderer(IConfig *config) { return new VKRenderInterface(config); }