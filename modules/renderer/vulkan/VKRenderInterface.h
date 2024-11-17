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
#include "../Buffer.h"
#include "../IRenderer.h"
#include "../RenderDesc.h"
#include "../Sampler.h"
#include "Prerequisites.h"
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

		void OnInitialization() override;
		void OnDestruction() override;

		Texture *createTexture(TextureDesc *desc) override;

		void deleteTexture(Texture *texture) override;

		Sampler *createSampler(SamplerDesc *desc) override;

		void deleteSampler(Sampler *texture) override;

		RenderPipeline *createRenderPipeline(const RenderPipelineDesc *desc) override;

		void deleteRenderPipeline(RenderPipeline *obj) override;

		Shader *createShader(ShaderDesc *desc) override;

		void deleteShader(Shader *shader) override;

		Buffer *createBuffer(BufferDesc *desc) override;

		void deleteBuffer(Buffer *object) override;

		FrameBuffer *createFrameBuffer(
			FrameBufferDesc *desc) override; // TODO determine what to do with the reference objects. Same for
											 // all other object using reference object to GPU resources.
		void deleteFrameBuffer(FrameBuffer *obj) override;

		QueryObject *createQuery(QueryDesc *desc) override;

		void deleteQuery(QueryObject *query) override;

		Sync *createSync(SyncDesc *desc) override;

		void deleteSync(Sync *sync) override;

		RendererWindow *createWindow(int x, int y, int width, int height) override;

		void setCurrentWindow(RendererWindow *window) override;

		FrameBuffer *getDefaultFramebuffer(void *window) override;

		ViewPort *getView(unsigned int i) override;

		void setDebug(bool enable) override;

		const char *getShaderVersion(ShaderLanguage language) const override;

		ShaderLanguage getShaderLanguage() const override;

		const char *getAPIVersion() const override;

		const char *getVersion() const override;

		void getSupportedTextureCompression(TextureDesc::Compression *pCompressions) override;
		void getCapability(Capability *capability) override;

		void getFeatures(Features *features) override;

		void getStatus(MemoryInfo *memoryInfo) override;

	  public: /*	Command buffer methods.	*/
		CommandList *createCommandBuffer() override;

		// TODO add array support .
		void submittCommand(Ref<CommandList> &list) override;
		void execute(CommandList *list) override;

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
