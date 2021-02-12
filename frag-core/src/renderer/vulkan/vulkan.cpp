#include <limits>
#include "Exception/InvalidArgumentException.h"
#include "Exception/RuntimeException.h"
#include "Renderer/vulkan/internal_object_type.h"
#include "Utils/StringUtil.h"

using namespace fragcore;

// std::vector<const char*> getRequiredExtensions() {
//     uint32_t glfwExtensionCount = 0;
//     const char** glfwExtensions;
//     glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

//     std::vector<const char*> extensions(glfwExtensions, glfwExtensions +
//     glfwExtensionCount);

//     if (enableValidationLayers) {
//         extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//     }

//     return extensions;
// }

static const char* getVKResultString(VkResult result){
	switch (result)
	{
		case VK_SUCCESS:
			return "VK_SUCCESS";
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			return "VK_ERROR_OUT_OF_HOST_MEMORY";
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
		case VK_ERROR_INITIALIZATION_FAILED:
			return "VK_ERROR_INITIALIZATION_FAILED";
		case VK_ERROR_DEVICE_LOST:
			return "VK_ERROR_DEVICE_LOST";
		case VK_ERROR_MEMORY_MAP_FAILED:
			return "VK_ERROR_MEMORY_MAP_FAILED";
		case VK_ERROR_LAYER_NOT_PRESENT:
			return "VK_ERROR_LAYER_NOT_PRESENT";
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			return "VK_ERROR_EXTENSION_NOT_PRESENT";
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			return "VK_ERROR_INCOMPATIBLE_DRIVER";
		case VK_ERROR_TOO_MANY_OBJECTS:
			return "VK_ERROR_TOO_MANY_OBJECTS";
		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			return "VK_ERROR_FORMAT_NOT_SUPPORTED";
		case VK_ERROR_FRAGMENTED_POOL:
			return "VK_ERROR_FRAGMENTED_POOL";
		case VK_ERROR_UNKNOWN:
			return "VK_ERROR_UNKNOWN";
		case VK_ERROR_OUT_OF_POOL_MEMORY:
			return "VK_ERROR_OUT_OF_POOL_MEMORY";
		case VK_ERROR_INVALID_EXTERNAL_HANDLE:
			return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
		case VK_ERROR_FRAGMENTATION:
			return "VK_ERROR_FRAGMENTATION";
	}
}

void checkError(VkResult result){
	if (result != VK_SUCCESS && result < VK_SUCCESS) {
		throw RuntimeException(fvformatf("Unsuccessful VkResult: %s", getVKResultString(result)));
	}
}

uint32_t findMemoryType(fragcore::VulkanCore* vulkanCore, uint32_t typeFilter,
						VkMemoryPropertyFlags properties) {
	for (uint32_t i = 0; i < vulkanCore->memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) &&
			(vulkanCore->memProperties.memoryTypes[i].propertyFlags &
			 properties) == properties) {
			return i;
		}
	}
}

void createBuffer(VulkanCore* vulkanCore, VkDeviceSize size,
				  VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
				  VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
	VkResult result;

	/**/
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	/**/
	if (vkCreateBuffer(vulkanCore->device, &bufferInfo, NULL, &buffer) !=
		VK_SUCCESS) {
		throw RuntimeException("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(vulkanCore->device, buffer, &memRequirements);

	/**/
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex =
		findMemoryType(vulkanCore, memRequirements.memoryTypeBits, properties);

	/**/
	if (vkAllocateMemory(vulkanCore->device, &allocInfo, NULL, &bufferMemory) !=
		VK_SUCCESS) {
		throw RuntimeException("failed to allocate buffer memory!");
	}

	/**/
	vkBindBufferMemory(vulkanCore->device, buffer, bufferMemory, 0);
}

VkImageView createImageView(VulkanCore* vulkanCore, VkImage image,
							VkFormat format) {
	/**/
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(vulkanCore->device, &viewInfo, nullptr, &imageView) !=
		VK_SUCCESS) {
		throw RuntimeException("failed to create texture image view!");
	}

	return imageView;
}

VkShaderModule createShaderModule(VkDevice device, const char* pdata,
								  size_t size) {
	VkResult result;
	VkShaderModule module;

	/**/
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pCode = (const uint32_t*)pdata;
	createInfo.codeSize = size;

	/**/
	result = vkCreateShaderModule(device, &createInfo, NULL, &module);
	if (result != VK_SUCCESS)
		throw RuntimeException(
			fvformatf("Failed to create shader module - %d", result));

	return module;
}

bool isDeviceSuitable(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	VkQueueFamilyProperties a;

	/*  */
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	bool swapChainAdequate = false;
	//	vkGetPhysicalDeviceMemoryProperties()
	//	vkGetPhysicalDeviceQueueFamilyProperties()
	//  vkGetPhysicalDeviceQueueFamilyProperties(vulkancore->gpu,
	//  &vulkancore->queue_count, NULL);
	/*  Check if device is good enough as a GPU candidates.  */
	
	//TODO determine, since it adds the requirement of a display in order to use the device.
	uint32_t displayCount;
	std::vector<VkDisplayPropertiesKHR> displayProperties;
	vkGetPhysicalDeviceDisplayPropertiesKHR(device, &displayCount, NULL);
	/*  */

	return (deviceProperties.deviceType ==
				VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
			deviceProperties.deviceType ==
				VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ||
			deviceProperties.deviceType ==
				VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) &&
		   deviceFeatures.geometryShader && deviceFeatures.samplerAnisotropy && displayCount>0;
}

void selectDefaultDevices(std::vector<VkPhysicalDevice>& devices,
						  std::vector<VkPhysicalDevice>& selectDevices) {
	/*  Check for matching. */
	// VK_KHR_device_group_creation
	// VK_KHR_device_group_creation
	/*	Check for the device with the best specs and can display.	*/

	for (int i = 0; i < devices.size(); i++) {
		const VkPhysicalDevice& device = devices[i];
		VkPhysicalDeviceProperties props = {};
		vkGetPhysicalDeviceProperties(device, &props);
	

		uint32_t count;
		std::vector<VkDisplayPropertiesKHR> displayProperties;
		vkGetPhysicalDeviceDisplayPropertiesKHR(device, &count, NULL);
		displayProperties.resize(count);
		vkGetPhysicalDeviceDisplayPropertiesKHR(device, &count, displayProperties.data());

		// Determine the type of the physical device
		if (props.deviceType ==
			VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			selectDevices.push_back(device);
		} else if (props.deviceType ==
				   VkPhysicalDeviceType::
					   VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
			selectDevices.push_back(device);
			// You've got yourself an integrated GPU.
		} else {
			// I don't even...
		}

		// Determine the available device local memory.
		VkPhysicalDeviceMemoryProperties memoryProps = {};
		vkGetPhysicalDeviceMemoryProperties(device, &memoryProps);

		VkMemoryHeap* heapsPointer = memoryProps.memoryHeaps;
		std::vector<VkMemoryHeap> heaps = std::vector<VkMemoryHeap>(
			heapsPointer, heapsPointer + memoryProps.memoryHeapCount);

		for (int j = 0; j < heaps.size(); j++) {
			VkMemoryHeap& heap = heaps[j];
			if (heap.flags &
				VkMemoryHeapFlagBits::VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
				// Device local heap, should be size of total GPU VRAM.
				// heap.size will be the size of VRAM in bytes. (bigger
				// is better)
			}
		}
	}
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(
	const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	if (availableFormats.size() == 1 &&
		availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	}

	for (const auto& availableFormat : availableFormats)
			{
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
					availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return availableFormat;
				}
			}
	return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(
	const std::vector<VkPresentModeKHR>& availablePresentModes) {
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		} else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			bestMode = availablePresentMode;
		}
	}

	return bestMode;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width !=
		std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	} else {
		// TODO determine what
		int width = 800;
		int height = 600;

		VkExtent2D actualExtent = {static_cast<uint32_t>(width),
								   static_cast<uint32_t>(height)};

		actualExtent.width = std::max(
			capabilities.minImageExtent.width,
			std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(
			capabilities.minImageExtent.height,
			std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
									 VkSurfaceKHR surface) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
											 nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
											 queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface,
											 &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
											  VkSurfaceKHR surface) {
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
											  &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
										 nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
											 details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
											  &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

// TODO move to the helper file.
unsigned int getTextureFormat(TextureDesc::Format format) {
	switch (format) {
		case TextureDesc::eRGB:
			return VK_FORMAT_R8G8B8_UNORM;
		case TextureDesc::eRGBA:
			return VK_FORMAT_R8G8B8A8_UNORM;
		case TextureDesc::eBGR:
			return VK_FORMAT_B8G8R8_UNORM;
		case TextureDesc::eBGRA:
			return VK_FORMAT_B8G8R8A8_UNORM;
		case TextureDesc::eSRGB:
			return VK_FORMAT_B8G8R8_SRGB;
		case TextureDesc::eSRGBA:
			return VK_FORMAT_B8G8R8A8_UNORM;
		case TextureDesc::eSingleColor:
			return VK_FORMAT_R8_UNORM;
		case TextureDesc::eDepth:
			return VK_FORMAT_R8_UNORM;
		case TextureDesc::eStencil:
			return VK_FORMAT_R8_UNORM;
		case TextureDesc::eDepthStencil:
			return VK_FORMAT_R8G8_UNORM;
		default:
			throw InvalidArgumentException("Invalid texture fvformatf.");
	}
}

unsigned int getTextureTarget(TextureDesc::Target target) {
	switch (target) {
		case TextureDesc::eTexture1D:
			return VK_IMAGE_TYPE_1D;
		case TextureDesc::eTexture2D:
			return VK_IMAGE_TYPE_2D;
		case TextureDesc::eTexture3D:
			return VK_IMAGE_TYPE_3D;
		case TextureDesc::eCubeMap:
		default:
			throw InvalidArgumentException("Invalid Texture target");
	}
}

unsigned int getTextureType(TextureDesc::Type type) {}

unsigned int getBufferType(BufferDesc::BufferType type) {
	switch (type) {
		case BufferDesc::eArray:
			return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		case BufferDesc::eElementArray:
			return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		case BufferDesc::eUniform:
			return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		case BufferDesc::eShaderStorage:
			return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		case BufferDesc::ePixelUnpack:
			return VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		case BufferDesc::ePixelPack:
			return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		default:
			assert(0);
	}
}

unsigned int getBufferHint(BufferDesc::BufferHint hint) { return 0; }

unsigned int getPrimitive(GeometryDesc::Primitive primitive) {
	switch (primitive) {
		case GeometryDesc::ePoint:
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		case GeometryDesc::eLines:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case GeometryDesc::eTriangles:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case GeometryDesc::eTriangleStrips:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		default:
			return -1;
	}
}

unsigned int getAttributeDataType(GeometryDesc::AttributeType type) {
	return 0;
}

unsigned int getState(unsigned int state) { return 0; }

const char* getVulkanErrorCodeMsg(unsigned int code) {
	switch (code) {
		case VK_SUCCESS:
		case VK_ERROR_DEVICE_LOST:
			return "";
		default:
			"unknown";
	}
}