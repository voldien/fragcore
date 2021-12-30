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

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC GLRendererInterface : public IRenderer {
	  public:
		// TODO make it less state machine and allow it to become more modern.
		virtual ~GLRendererInterface();

		virtual void OnInitialization();
		virtual void OnDestruction();

		/**
		 *	Create texture.
		 *
		 *	@return non null texture object if succesfully. Null otherwise.
		 */
		virtual Texture *createTexture(TextureDesc *desc);

		virtual void deleteTexture(Texture *texture);

		virtual Sampler *createSampler(SamplerDesc *desc);

		virtual void deleteSampler(Sampler *texture);

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual RenderPipeline *createPipeline(const ProgramPipelineDesc *desc);

		/**
		 *
		 * @param obj
		 * @return
		 */
		virtual void deletePipeline(RenderPipeline *obj);

		/**
		 *	Create shader.
		 *
		 *	@return
		 */
		virtual Shader *createShader(ShaderDesc *desc);

		virtual void deleteShader(Shader *shader);

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual Buffer *createBuffer(BufferDesc *desc);

		virtual void deleteBuffer(Buffer *object);

		/**
		 *	Create geometry.
		 *
		 *	@return
		 */
		virtual Geometry *createGeometry(GeometryDesc *desc);

		virtual void deleteGeometry(Geometry *obj);

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual FrameBuffer *
		createFrameBuffer(FrameBufferDesc *desc); // TODO determine what to do with the reference objects. Same for all
												  // other object using reference object to GPU resources.
		virtual void deleteFrameBuffer(FrameBuffer *obj);

		virtual QueryObject *createQuery(QueryDesc *desc);

		virtual void deleteQuery(QueryObject *query);

		/**
		 *
		 * @param x
		 * @param y
		 * @param width
		 * @param height
		 * @return
		 */
		virtual RendererWindow *createWindow(int x, int y, int width, int height);

		/**
		 *
		 * @param window
		 */
		virtual void setCurrentWindow(RendererWindow *window);

		/**
		 * Create swapchain.
		 */
		virtual void createSwapChain();

		/**
		 *
		 * @return
		 */
		virtual FrameBuffer *getDefaultFramebuffer(void *window);

		// TODO add viewobject for handling as a object
		virtual ViewPort *getView(unsigned int i);

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
		virtual void clearColor(float r, float g, float b, float a); // TODO relocate to the framebuffer.

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
		 * @param geometry
		 * @param num
		 */
		virtual void drawInstance(Geometry *geometry, unsigned int num);
		// virtual void drawInstance(Shader* pipeline, GeometryObject* geometry, unsigned int num);

		/**
		 *
		 * @param geometries
		 * @param num
		 */
		virtual void drawMultiInstance(Geometry &geometries, const unsigned int *first, const unsigned int *count,
									   unsigned int num);

		virtual void drawMultiIndirect(Geometry &geometries, unsigned int offset, unsigned int indirectCount);

		/**
		 *
		 * @param geometries
		 * @param num
		 */
		virtual void drawIndirect(Geometry *geometry);

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

		virtual Sync *createSync(SyncDesc *desc);

		virtual void deleteSync(Sync *sync);

		// virtual void execute(CommandList *list);

		/**
		 * Set debug state.
		 * @param enable
		 */
		virtual void setDebug(bool enable);

		/**
		 *	Get shader version.
		 *	@return non-null terminated string.
		 */
		virtual const char *getShaderVersion(ShaderLanguage language) const;

		/**
		 * Get all support shader languages.
		 * @return bitflag of all supported shader languages.
		 */
		virtual ShaderLanguage getShaderLanguage() const;

		/**
		 * Get the name of the rendering API.
		 * @return non-null terminated string.
		 */
		virtual const char *getAPIVersion() const;

		/**
		 *	Get version of the interface.
		 *	@return non-null terminated string.
		 */
		virtual const char *getVersion() const;

		/**
		 * Get all supported texture formats.
		 * @param pCompressions
		 */
		virtual void getSupportedTextureCompression(TextureDesc::Compression *pCompressions);

		// virtual void getSupportedTextureFormat(TextureFormat* textureFormat);
		// virtual void getSupportedGraphicTextureFormat(TextureFormat* textureFormat);

		/**
		 * Get capability of the rendering API.
		 * @param capability non-null object.
		 */
		virtual void getCapability(Capability *capability);

		/**
		 *
		 */
		virtual void getFeatures(Features *features);

		/**
		 * TODO add for reading status such as memory.
		 *  NVX_gpu_memory_info
		 *  ATI_meminfo
		 */
		// TODO imporove later
		virtual void getStatus(MemoryInfo *memoryInfo);

		virtual CommandList *createCommandBuffer();
		virtual void submittCommand(Ref<CommandList> &list);
		virtual void execute(CommandList *list);

		GLRendererInterface(IConfig *config);

		virtual void *getData() const;

	  public: /*	OpenGL Specific methods.	*/
		void *getOpenGLContext() noexcept;
		void bindWindowContext(void* window, void* context);

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
