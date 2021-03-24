#ifndef _FRAG_CORE_VK_RENDERER_INTERFACE_H_
#define _FRAG_CORE_VK_RENDERER_INTERFACE_H_ 1
#include "../../Core/Ref.h"
#include "../IRenderer.h"
#include "Renderer/Buffer.h"
#include "Renderer/RenderDesc.h"
#include "Renderer/Sampler.h"
#include <vector>
#include <vulkan/vulkan.h>

namespace fragcore {
	class VKRenderInterface : public IRenderer{
		public:
		  std::vector<RendererWindow *> windows;
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
		  VkQueue queue; // TODO rename graphicsQueue
		  VkQueue presentQueue;

		  /*  */
		  VkPhysicalDeviceProperties gpu_props;
		  VkQueueFamilyProperties *queue_props;
		  uint32_t graphics_queue_node_index;

		  /*  */
		  uint32_t num_physical_devices;
		  VkPhysicalDevice *physical_devices;
		  uint32_t queue_count;
		  uint32_t enabled_extension_count;
		  uint32_t enabled_layer_count;

		  std::vector<VkExtensionProperties> extensionsProperties;
		  bool validate;
		  void *device_validation_layers;
		  bool enableValidationLayers;
		  bool enableDebugTracer;

		  bool useGamma;

		  VkPhysicalDeviceMemoryProperties memProperties;
		  ShaderLanguage languageSupport;
		  Capability capability;
	};
} // namespace fragcore

#endif
