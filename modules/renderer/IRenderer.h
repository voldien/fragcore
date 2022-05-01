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
#ifndef _FRAG_CORE_IRENDERER_H_
#define _FRAG_CORE_IRENDERER_H_ 1
#include "Buffer.h"
#include "FrameBuffer.h"
#include "Geometry.h"
#include "ICompute.h"
#include "Query.h"
#include "RenderDesc.h"
#include "RenderPrerequisites.h"
#include "Shader.h"
#include "Texture.h"
#include <Core/IConfig.h>
#include <vector>
namespace fragcore {
	/**
	 *
	 */
	enum class CLEARBITMASK : uint32_t { // TODO rename
		Color = 0x1,					 /*	Clear color.    */
		Depth = 0x2,					 /*	Clear depth.    */
		eStencil = 0x4,					 /*	Clear stencil.  */
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
		virtual ~IRenderer() = default;

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
		 *	Create geometry.
		 *
		 *	@return
		 */
		virtual Geometry *createGeometry(GeometryDesc *desc) = 0;

		virtual void deleteGeometry(Geometry *obj) = 0;

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

		// TODO add viewobject for handling as a object
		virtual ViewPort *getView(unsigned int i) = 0;

		/**
		 *	Set depth mask.
		 */
		virtual void setDepthMask(bool flag) = 0;

		/**
		 *	Enable state.
		 */
		virtual void enableState(IRenderer::State state) = 0;

		/**
		 *	Disable state.
		 */
		virtual void disableState(IRenderer::State state) = 0;

		/**
		 *
		 * @param state
		 * @return
		 */
		virtual bool isStateEnabled(IRenderer::State state) = 0;

		/**
		 *
		 * @param geometry
		 * @param num
		 */
		virtual void drawInstance(Geometry *geometry, unsigned int num) = 0;
		// virtual void drawInstance(Shader* pipeline, GeometryObject* geometry, unsigned int num);

		/**
		 *
		 * @param geometries
		 * @param num
		 */
		virtual void drawMultiInstance(Geometry &geometries, const unsigned int *first, const unsigned int *count,
									   unsigned int num) = 0;

		virtual void drawMultiIndirect(Geometry &geometries, unsigned int offset, unsigned int indirectCount) = 0;

		/**
		 *
		 * @param geometries
		 * @param num
		 */
		virtual void drawIndirect(Geometry *geometry) = 0;

		/**
		 *
		 * @param width
		 */
		virtual void setLineWidth(float width) = 0;

		/**
		 *
		 * @param source
		 * @param dest
		 */
		virtual void
		blit(const FrameBuffer *source, FrameBuffer *dest,
			 Texture::FilterMode filterMode) = 0; // TODO add filter mode.    /*  TODO add filter and buffer bit.    */
		// virtual void blit(const FrameBuffer* source, FrameBuffer* dest, int* source, int* dest, Texture::FilterMode
		// filterMode, FrameBuffer::BufferAttachment attachment);
		// TODO add additional version of the blit for sub image specifiction.

		virtual void bindTextures(unsigned int firstUnit, const std::vector<Texture *> &textures) = 0;

		virtual void bindImages(unsigned int firstUnit, const std::vector<Texture *> &textures,
								const std::vector<Texture::MapTarget> &mapping,
								const std::vector<Texture::Format> &formats) = 0;

		/**
		 *
		 * @param source
		 * @param target
		 */
		// TODO add version with regiion specifiction.
		virtual void copyTexture(const Texture *source, Texture *target) = 0;
		// virtual void copyTexture(const Texture* source, Texture* target, int* sourceCoord, int* targetCoord);

		/**
		 * Dispatch compute program.
		 * @param global global workgroup.
		 * @param local local workgroup
		 * @param offset offset in indirect buffer in number of bytes.
		 */
		virtual void dispatchCompute(unsigned int *global, unsigned int *local, unsigned int offset = 0) = 0;

		// TODO add memory barrier.
		virtual void memoryBarrier() = 0;

		virtual Sync *createSync(SyncDesc *desc) = 0;

		virtual void deleteSync(Sync *sync) = 0;

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
		 *	Get version of the interface.
		 *	@return non-null terminated string.
		 */
		virtual const char *getVersion() const = 0;

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

		virtual CommandList *createCommandBuffer() = 0;
		virtual void submittCommand(Ref<CommandList> &list) = 0;
		virtual void execute(CommandList *list) = 0;

		virtual void *getData() const = 0;

	  private: /*  */
		void *pdata;
	};
} // namespace fragcore

/**
 * @brief Create internal rendering interface.
 * @param config
 * @return non-null renderinginterface object.
 */
extern "C" fragcore::IRenderer *createInternalRenderer(fragcore::IConfig *config);

#endif
