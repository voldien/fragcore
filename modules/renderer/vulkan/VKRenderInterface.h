/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _LIB_FRAG_CORE_VK_RENDERER_INTERFACE_H_
#define _LIB_FRAG_CORE_VK_RENDERER_INTERFACE_H_ 1
#include "Prerequisites.h"
#include <../Buffer.h>
#include <../IRenderer.h>
#include <../RenderDesc.h>
#include <../Sampler.h>
#include <Core/Ref.h>
#include <VKDevice.h>
#include <vulkan/vulkan.h>

namespace fragcore {

	class VKRenderInterface : public IRenderer {
		friend class VKRenderWindow;

	  public:
		VKRenderInterface() = delete;
		VKRenderInterface(VKRenderInterface &&other) = delete;
		VKRenderInterface(const VKRenderInterface &other) = delete;
		VKRenderInterface(IConfig *config);
		virtual ~VKRenderInterface();

		// TODO make it less state machine and allow it to become more modern.

		virtual void OnInitialization() override;
		virtual void OnDestruction() override;

		virtual Texture *createTexture(TextureDesc *desc) override;

		virtual void deleteTexture(Texture *texture) override;

		virtual Sampler *createSampler(SamplerDesc *desc) override;

		virtual void deleteSampler(Sampler *texture) override;

		virtual RenderPipeline *createPipeline(const ProgramPipelineDesc *desc) override;

		virtual void deletePipeline(RenderPipeline *obj) override;

		virtual Shader *createShader(ShaderDesc *desc) override;

		virtual void deleteShader(Shader *shader) override;

		virtual Buffer *createBuffer(BufferDesc *desc) override;

		virtual void deleteBuffer(Buffer *object) override;

		virtual Geometry *createGeometry(GeometryDesc *desc) override;

		virtual void deleteGeometry(Geometry *obj) override;

		virtual FrameBuffer *createFrameBuffer(
			FrameBufferDesc *desc) override; // TODO determine what to do with the reference objects. Same for
											 // all other object using reference object to GPU resources.
		virtual void deleteFrameBuffer(FrameBuffer *obj) override;

		virtual QueryObject *createQuery(QueryDesc *desc) override;

		virtual void deleteQuery(QueryObject *query) override;

		virtual RendererWindow *createWindow(int x, int y, int width, int height) override;

		virtual void setCurrentWindow(RendererWindow *window) override;

		virtual FrameBuffer *getDefaultFramebuffer(void *window) override;

		// TODO add viewobject for handling as a object
		virtual ViewPort *getView(unsigned int i) override;

		virtual void clear(unsigned int bitflag); // TODO relocate to the default framebuffer.

		virtual void clearColor(float r, float g, float b, float a); // TODO relocate to the framebuffer.

		virtual void setVSync(int sync); // TODO relocate to the render window.

		virtual void swapBuffer(); // TODO relocate to the render window.

		virtual void setDepthMask(bool flag) override;

		virtual void enableState(IRenderer::State state) override;

		virtual void disableState(IRenderer::State state) override;

		virtual bool isStateEnabled(IRenderer::State state) override;

		virtual void drawInstance(Geometry *geometry, unsigned int num) override;
		// virtual void drawInstance(Shader* pipeline, GeometryObject* geometry, unsigned int num);

		virtual void drawMultiInstance(Geometry &geometries, const unsigned int *first, const unsigned int *count,
									   unsigned int num) override;

		virtual void drawMultiIndirect(Geometry &geometries, unsigned int offset, unsigned int indirectCount) override;

		virtual void drawIndirect(Geometry *geometry) override;

		virtual void setLineWidth(float width) override;

		virtual void blit(
			const FrameBuffer *source, FrameBuffer *dest,
			Texture::FilterMode filterMode) override; // TODO add filter mode.    /*  TODO add filter and buffer bit. */
		// virtual void blit(const FrameBuffer* source, FrameBuffer* dest, int* source, int* dest, Texture::FilterMode
		// filterMode, FrameBuffer::BufferAttachment attachment);
		// TODO add additional version of the blit for sub image specifiction.

		virtual void bindTextures(unsigned int firstUnit, const std::vector<Texture *> &textures) override;

		virtual void bindImages(unsigned int firstUnit, const std::vector<Texture *> &textures,
								const std::vector<Texture::MapTarget> &mapping,
								const std::vector<Texture::Format> &formats) override;

		virtual void copyTexture(const Texture *source, Texture *target) override;

		virtual void dispatchCompute(unsigned int *global, unsigned int *local, unsigned int offset = 0) override;

		// TODO add memory barrier.
		virtual void memoryBarrier() override;

		virtual Sync *createSync(SyncDesc *desc) override;

		virtual void deleteSync(Sync *sync) override;

		virtual void setDebug(bool enable) override;

		virtual const char *getShaderVersion(ShaderLanguage language) const override;

		virtual ShaderLanguage getShaderLanguage() const override;

		virtual const char *getAPIVersion() const override;

		virtual const char *getVersion() const override;

		virtual void getSupportedTextureCompression(TextureDesc::Compression *pCompressions) override;
		virtual void getCapability(Capability *capability) override;

		virtual void getFeatures(Features *features) override;

		virtual void getStatus(MemoryInfo *memoryInfo) override;

		virtual CommandList *createCommandBuffer() override;
		virtual void submittCommand(Ref<CommandList> &list) override;
		virtual void execute(CommandList *list) override;

		virtual void *getData() const override;

	  public: /*	*/
		std::shared_ptr<VKDevice> &getDevice() { return this->device; };
		std::shared_ptr<VulkanCore> getInstance() { return this->core; }

	  protected:
		std::shared_ptr<VKDevice> device;
		std::shared_ptr<VulkanCore> core;

		/*	*/
		VkInstance inst;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkDebugReportCallbackEXT debugReport;
		/*  Physical device.    */
		VkPhysicalDevice gpu;
		std::vector<VkPhysicalDevice> GPUs;

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
