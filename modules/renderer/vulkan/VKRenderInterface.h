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
		VKRenderInterface(const VKRenderInterface &other) = default;
		VKRenderInterface(IConfig *config);
		virtual ~VKRenderInterface();

		// TODO make it less state machine and allow it to become more modern.

		virtual void OnInitialization();
		virtual void OnDestruction();

		virtual Texture *createTexture(TextureDesc *desc);

		virtual void deleteTexture(Texture *texture);

		virtual Sampler *createSampler(SamplerDesc *desc);

		virtual void deleteSampler(Sampler *texture);

		virtual RenderPipeline *createPipeline(const ProgramPipelineDesc *desc);

		virtual void deletePipeline(RenderPipeline *obj);

		virtual Shader *createShader(ShaderDesc *desc);

		virtual void deleteShader(Shader *shader);

		virtual Buffer *createBuffer(BufferDesc *desc);

		virtual void deleteBuffer(Buffer *object);

		virtual Geometry *createGeometry(GeometryDesc *desc);

		virtual void deleteGeometry(Geometry *obj);

		virtual FrameBuffer *
		createFrameBuffer(FrameBufferDesc *desc); // TODO determine what to do with the reference objects. Same for
												  // all other object using reference object to GPU resources.
		virtual void deleteFrameBuffer(FrameBuffer *obj);

		virtual QueryObject *createQuery(QueryDesc *desc);

		virtual void deleteQuery(QueryObject *query);

		virtual RendererWindow *createWindow(int x, int y, int width, int height);

		virtual void setCurrentWindow(RendererWindow *window);

		virtual FrameBuffer *getDefaultFramebuffer(void *window);

		// TODO add viewobject for handling as a object
		virtual ViewPort *getView(unsigned int i);

		virtual void clear(unsigned int bitflag); // TODO relocate to the default framebuffer.

		virtual void clearColor(float r, float g, float b, float a); // TODO relocate to the framebuffer.

		virtual void setVSync(int sync); // TODO relocate to the render window.

		virtual void swapBuffer(); // TODO relocate to the render window.

		virtual void setDepthMask(bool flag);

		virtual void enableState(IRenderer::State state);

		virtual void disableState(IRenderer::State state);

		virtual bool isStateEnabled(IRenderer::State state);

		virtual void drawInstance(Geometry *geometry, unsigned int num);
		// virtual void drawInstance(Shader* pipeline, GeometryObject* geometry, unsigned int num);

		virtual void drawMultiInstance(Geometry &geometries, const unsigned int *first, const unsigned int *count,
									   unsigned int num);

		virtual void drawMultiIndirect(Geometry &geometries, unsigned int offset, unsigned int indirectCount);

		virtual void drawIndirect(Geometry *geometry);

		virtual void setLineWidth(float width);

		virtual void
		blit(const FrameBuffer *source, FrameBuffer *dest,
			 Texture::FilterMode filterMode); // TODO add filter mode.    /*  TODO add filter and buffer bit.    */
		// virtual void blit(const FrameBuffer* source, FrameBuffer* dest, int* source, int* dest, Texture::FilterMode
		// filterMode, FrameBuffer::BufferAttachment attachment);
		// TODO add additional version of the blit for sub image specifiction.

		virtual void bindTextures(unsigned int firstUnit, const std::vector<Texture *> &textures);

		virtual void bindImages(unsigned int firstUnit, const std::vector<Texture *> &textures,
								const std::vector<Texture::MapTarget> &mapping,
								const std::vector<Texture::Format> &formats);

		virtual void copyTexture(const Texture *source, Texture *target);

		virtual void dispatchCompute(unsigned int *global, unsigned int *local, unsigned int offset = 0);

		// TODO add memory barrier.
		virtual void memoryBarrier();

		virtual Sync *createSync(SyncDesc *desc);

		virtual void deleteSync(Sync *sync);

		virtual void setDebug(bool enable);

		virtual const char *getShaderVersion(ShaderLanguage language) const;

		virtual ShaderLanguage getShaderLanguage() const;

		virtual const char *getAPIVersion() const;

		virtual const char *getVersion() const;

		virtual void getSupportedTextureCompression(TextureDesc::Compression *pCompressions);
		virtual void getCapability(Capability *capability);

		virtual void getFeatures(Features *features);

		virtual void getStatus(MemoryInfo *memoryInfo);

		virtual CommandList *createCommandBuffer();
		virtual void submittCommand(Ref<CommandList> &list);
		virtual void execute(CommandList *list);

		virtual void *getData() const override;

		std::shared_ptr<VKDevice> &getDevice() { return this->device; };

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
