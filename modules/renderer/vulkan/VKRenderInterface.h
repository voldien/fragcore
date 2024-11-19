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
#ifndef _LIB_FRAGCORE_VK_RENDERER_INTERFACE_H_
#define _LIB_FRAGCORE_VK_RENDERER_INTERFACE_H_ 1
#include "../IRenderer.h"
#include "../RenderDesc.h"
#include <VKDevice.h>
#include <vulkan/vulkan.h>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC VKRenderInterface : public IRenderer {
		friend class VKRenderWindow;

	  public:
		VKRenderInterface() = delete;
		VKRenderInterface(VKRenderInterface &&other) = delete;
		VKRenderInterface(const VKRenderInterface &other) = delete;
		VKRenderInterface(IConfig *config);
		~VKRenderInterface() override;

		// TODO make it less state machine and allow it to become more modern.

		void onInitialization() override;
		void onDestruction() override;


		RendererWindow *createWindow(int x, int y, int width, int height) override;

		void setCurrentWindow(RendererWindow *window) override;


		void setDebug(bool enable) override;

		const char *getShaderVersion(ShaderLanguage language) const override;

		ShaderLanguage getShaderLanguage() const override;

		const char *getAPIVersion() const override;

		const char *getVersion() const override;

		void getSupportedTextureCompression(TextureDesc::Compression *pCompressions) override;
		void getCapability(Capability *capability) override;

		void getFeatures(Features *features) override;

		void getStatus(MemoryInfo *memoryInfo) override;


	  public:
		void *getData() const override;

	  public: /*	*/
		std::shared_ptr<fvkcore::VKDevice> &getDevice() { return this->device; };
		std::shared_ptr<fvkcore::VulkanCore> getInstance() { return this->core; }

	  protected:
		std::shared_ptr<fvkcore::VKDevice> device;
		std::shared_ptr<fvkcore::VulkanCore> core;

		/*	*/
		// VkInstance inst;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkDebugReportCallbackEXT debugReport;
		/*  Physical device.    */
		// VkPhysicalDevice gpu;
		// std::vector<VkPhysicalDevice> GPUs;

		std::vector<RendererWindow *> windows;
		Capability capabilityCached;

		/*  */
		// VkDevice device;
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
