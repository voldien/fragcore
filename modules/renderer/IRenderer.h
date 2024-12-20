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
#ifndef _FRAGCORE_IRENDERER_H_
#define _FRAGCORE_IRENDERER_H_ 1
#include "Buffer.h"
#include "FrameBuffer.h"
#include "ICompute.h"
#include "Query.h"
#include "RenderDesc.h"
#include "RenderPrerequisites.h"
#include "Shader.h"
#include "Texture.h"
#include <Core/IConfig.h>

namespace fragcore {
	/**
	 *
	 */
	enum class CLEARBITMASK : uint32_t { // TODO rename
		Color = 0x1,					 /*	Clear color.    */
		Depth = 0x2,					 /*	Clear depth.    */
		Stencil = 0x4,					 /*	Clear stencil.  */
	};

	/**
	 *
	 */
	class FVDECLSPEC IRenderer : public ICompute {
	  public:
		/*	TODO rename and fix enum names.	*/
		enum class State {
			DepthTest = 0x1, /*	Perform depth test on pixels.   */
			StencilTest,	 /*	Perform stencil test.   */
			ScissorTest,	 /*  */
			AlphaTest,		 /*  */
			DepthMask,
			/*	Perform .	*/ // TODO determine how to deal with depth, stencil and etc.
			Blend,			   /*	Set Color Blend. */
			Cullface,		   /*	Set Culling face. */
			Dither,			   /*	Set Color dithering.    */
			SRGB,
			MultiSampling,
			SampleShading, /*   */
			// GL_SAMPLE_COVERAGE
			SampleAlphaCoverage,  /*    */
			DiscardRasterization, /*  */

		};

		// TODO make it less state machine and allow it to become more modern.
		IRenderer() = default;
		IRenderer(const IRenderer &other) = delete;
		IRenderer(IRenderer &&other) = delete;
		~IRenderer() override = default;

		/**
		 *	Create texture.
		 *
		 *	@return non null texture object if succesfully. Null otherwise.
		 */
		virtual Texture *createTexture(TextureDesc *desc) noexcept(false) = 0;

		/**
		 * @brief
		 *
		 * @param texture
		 */
		virtual void deleteTexture(Texture *texture) noexcept(false) = 0;

		/**
		 * @brief Create a Sampler object
		 *
		 * @param desc
		 * @return Sampler*
		 */
		virtual Sampler *createSampler(SamplerDesc *desc) = 0;

		/**
		 * @brief
		 *
		 * @param texture
		 */
		virtual void deleteSampler(Sampler *texture) = 0;

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual RenderPipeline *createRenderPipeline(const RenderPipelineDesc *desc) = 0;

		/**
		 *
		 * @param obj
		 * @return
		 */
		virtual void deleteRenderPipeline(RenderPipeline *obj) = 0;

		/**
		 *	Create shader.
		 *
		 *	@return
		 */
		virtual Shader *createShader(ShaderDesc *desc) = 0;

		virtual void deleteShader(Shader *shader) = 0;

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual Buffer *createBuffer(BufferDesc *desc) = 0;

		virtual void deleteBuffer(Buffer *object) = 0;

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual FrameBuffer *
		createFrameBuffer(FrameBufferDesc *desc) = 0; // TODO determine what to do with the reference objects. Same for
													  // all other object using reference object to GPU resources.
		virtual void deleteFrameBuffer(FrameBuffer *obj) = 0;

		virtual QueryObject *createQuery(QueryDesc *desc) = 0;

		virtual void deleteQuery(QueryObject *query) = 0;

		virtual Sync *createSync(SyncDesc *desc) = 0;

		virtual void deleteSync(Sync *sync) = 0;

		/**
		 *
		 * @param x
		 * @param y
		 * @param width
		 * @param height
		 * @return
		 */
		virtual RendererWindow *createWindow(int x, int y, int width, int height) = 0;

		/**
		 *
		 * @param window
		 */
		virtual void setCurrentWindow(RendererWindow *window) = 0;

		/**
		 *
		 * @return
		 */
		virtual FrameBuffer *getDefaultFramebuffer(void *window) = 0;

		virtual ViewPort *getView(unsigned int i) = 0;

		// virtual void bindTextures(unsigned int firstUnit, const std::vector<Texture *> &textures) = 0;

		// virtual void bindImages(unsigned int firstUnit, const std::vector<Texture *> &textures,
		// 						const std::vector<Texture::MapTarget> &mapping,
		// 						const std::vector<Texture::Format> &formats) = 0;

		// virtual void execute(CommandList *list);

		/**
		 * Set debug state.
		 * @param enable
		 */
		virtual void setDebug(bool enable) = 0;

		/**
		 *	Get shader version.
		 *	@return non-null terminated string.
		 */
		virtual const char *getShaderVersion(ShaderLanguage language) const = 0;

		/**
		 * Get all support shader languages.
		 * @return bitflag of all supported shader languages.
		 */
		virtual ShaderLanguage getShaderLanguage() const = 0;

		/**
		 * Get the name of the rendering API.
		 * @return non-null terminated string.
		 */
		virtual const char *getAPIVersion() const = 0;

		/**
		 * Get all supported texture formats.
		 * @param pCompressions
		 */
		virtual void getSupportedTextureCompression(TextureDesc::Compression *pCompressions) = 0;

		// virtual void getSupportedTextureFormat(TextureFormat* textureFormat);
		// virtual void getSupportedGraphicTextureFormat(TextureFormat* textureFormat);

		/**
		 * Get capability of the rendering API.
		 * @param capability non-null object.
		 */
		virtual void getCapability(Capability *capability) = 0;

		/**
		 *
		 */
		virtual void getFeatures(Features *features) = 0;

		/**
		 * TODO add for reading status such as memory.
		 *  NVX_gpu_memory_info
		 *  ATI_meminfo
		 */
		// TODO imporove later
		virtual void getStatus(MemoryInfo *memoryInfo) = 0;

		CommandList *createCommandBuffer() override = 0;
		void submittCommand(Ref<CommandList> &list) override = 0;
		void execute(CommandList *list) override = 0;

		virtual void *getData() const = 0;
	};
} // namespace fragcore

/**
 * @brief Create internal rendering interface.
 * @param config
 * @return non-null renderinginterface object.
 */
extern "C" fragcore::IRenderer *createInternalRenderer(fragcore::IConfig *config);

#endif
