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
#ifndef _FRAG_CORE_GL_RENDERER_INTERFACE_H_
#define _FRAG_CORE_GL_RENDERER_INTERFACE_H_ 1
#include "../IRenderer.h"
#include "GLHelper.h"

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC GLRendererInterface : public IRenderer {
	  public:
		// TODO make it less state machine and allow it to become more modern.
		virtual ~GLRendererInterface();

		void OnInitialization() override;
		void OnDestruction() override;

		/**
		 *	Create texture.
		 *
		 *	@return non null texture object if succesfully. Null otherwise.
		 */
		Texture *createTexture(TextureDesc *desc) override;

		void deleteTexture(Texture *texture) override;

		Sampler *createSampler(SamplerDesc *desc) override;

		void deleteSampler(Sampler *texture) override;

		/**
		 *
		 * @param desc
		 * @return
		 */
		RenderPipeline *createRenderPipeline(const RenderPipelineDesc *desc) override;

		/**
		 *
		 * @param obj
		 * @return
		 */
		void deleteRenderPipeline(RenderPipeline *obj) override;

		/**
		 *	Create shader.
		 *
		 *	@return
		 */
		Shader *createShader(ShaderDesc *desc) override;

		void deleteShader(Shader *shader) override;

		/**
		 *
		 * @param desc
		 * @return
		 */
		Buffer *createBuffer(BufferDesc *desc) override;

		void deleteBuffer(Buffer *object) override;

		/**
		 *
		 * @param desc
		 * @return
		 */
		FrameBuffer *createFrameBuffer(
			FrameBufferDesc *desc) override; // TODO determine what to do with the reference objects. Same for all
											 // other object using reference object to GPU resources.
		void deleteFrameBuffer(FrameBuffer *obj) override;

		QueryObject *createQuery(QueryDesc *desc) override;

		void deleteQuery(QueryObject *query) override;

		/**
		 *
		 * @param x
		 * @param y
		 * @param width
		 * @param height
		 * @return
		 */
		RendererWindow *createWindow(int x, int y, int width, int height) override;

		/**
		 *
		 * @param window
		 */
		void setCurrentWindow(RendererWindow *window) override;

		/**
		 * Create swapchain.
		 */
		virtual void createSwapChain();

		/**
		 *
		 * @return
		 */
		FrameBuffer *getDefaultFramebuffer(void *window) override;

		// TODO add viewobject for handling as a object
		ViewPort *getView(unsigned int i) override;
		// TODO relocate to the render pipeline queue
		/**
		 *
		 * @param bitflag
		 */
		virtual void clear(unsigned int bitflag); // TODO relocate to the default framebuffer.

		/**
		 * Set clear color RGBA.
		 * @param r
		 * @param g
		 * @param b
		 * @param a
		 */
		virtual void clearColor(float red, float green, float blue, float alpha); // TODO relocate to the framebuffer.

		/**
		 *	Set depth mask.
		 */
		virtual void setDepthMask(bool flag);

		/**
		 *	Enable state.
		 */
		virtual void enableState(IRenderer::State state);

		/**
		 *	Disable state.
		 */
		virtual void disableState(IRenderer::State state);

		/**
		 *
		 * @param state
		 * @return
		 */
		virtual bool isStateEnabled(IRenderer::State state);

		/**
		 *
		 * @param width
		 */
		virtual void setLineWidth(float width);

		/**
		 *
		 * @param source
		 * @param dest
		 */
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

		/**
		 *
		 * @param source
		 * @param target
		 */
		// TODO add version with regiion specifiction.
		virtual void copyTexture(const Texture *source, Texture *target);
		// virtual void copyTexture(const Texture* source, Texture* target, int* sourceCoord, int* targetCoord);

		/**
		 * Dispatch compute program.
		 * @param global global workgroup.
		 * @param local local workgroup
		 * @param offset offset in indirect buffer in number of bytes.
		 */
		virtual void dispatchCompute(unsigned int *global, unsigned int *local, unsigned int offset = 0);

		// TODO add memory barrier.
		virtual void memoryBarrier();

		Sync *createSync(SyncDesc *desc) override;

		void deleteSync(Sync *sync) override;

		// virtual void execute(CommandList *list);

		/**
		 * Set debug state.
		 * @param enable
		 */
		void setDebug(bool enable) override;

		/**
		 *	Get shader version.
		 *	@return non-null terminated string.
		 */
		const char *getShaderVersion(ShaderLanguage language) const override;

		/**
		 * Get all support shader languages.
		 * @return bitflag of all supported shader languages.
		 */
		ShaderLanguage getShaderLanguage() const override;

		/**
		 * Get the name of the rendering API.
		 * @return non-null terminated string.
		 */
		const char *getAPIVersion() const override;

		/**
		 *	Get version of the interface.
		 *	@return non-null terminated string.
		 */
		const char *getVersion() const override;

		/**
		 * Get all supported texture formats.
		 * @param pCompressions
		 */
		void getSupportedTextureCompression(TextureDesc::Compression *pCompressions) override;

		// virtual void getSupportedTextureFormat(TextureFormat* textureFormat);
		// virtual void getSupportedGraphicTextureFormat(TextureFormat* textureFormat);

		// TODO add debug callback support

		/**
		 * Get capability of the rendering API.
		 * @param capability non-null object.
		 */
		void getCapability(Capability *capability) override;

		/**
		 *
		 */
		void getFeatures(Features *features) override;

		/**
		 * TODO add for reading status such as memory.
		 *  NVX_gpu_memory_info
		 *  ATI_meminfo
		 */
		// TODO imporove later
		void getStatus(MemoryInfo *memoryInfo) override;

		CommandList *createCommandBuffer() override;
		void submittCommand(Ref<CommandList> &list) override;
		void execute(CommandList *list) override;

		GLRendererInterface(IConfig *config);

		void *getData() const override;

	  public:
		const char *getExtensions() const noexcept;
		bool isExtensionSupported(const char *extension) const noexcept;

	  public: /*	OpenGL Specific methods.	*/
		void *getOpenGLContext() noexcept;
		void bindWindowContext(void *window, void *context);

	  protected: /*  */
		void *pdata;

	  private:
		void *openglcontext;
		void *tpmwindow;
		RendererWindow *drawwindow;

		bool useCoreProfile;
		bool useCompatibility;

		/*  */
		// TODO determine if to use pool or something .
		Buffer *pboUnPack;
		Buffer *pboPack;

		/*  Cached internal capabilities.   */
		Capability capability;
		TextureDesc::Compression compression;
		Features features;

		/*  Context version.    */
		int majorVersion;
		int minorVersion;
		int profile;
		int cflag;

		bool debug;
		bool alpha;

		ViewPort *defaultViewport;
		std::vector<ViewPort *> viewports; // TODO remove pointer.
		FrameBuffer *defaultFrameBuffer;
		/*  Texture gamma corrections.  */
		bool gamma;
		ShaderLanguage supportedLanguages;
	};
} // namespace fragcore

#endif
