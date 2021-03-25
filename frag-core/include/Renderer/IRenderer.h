/**
	FragEngine, A Two layer Game Engine.
    Copyright (C) 2018  Valdemar Lindberg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_IRENDERER_H_
#define _FRAG_CORE_IRENDERER_H_ 1
#include "../Prerequisites.h"
#include "ICompute.h"
#include "Texture.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "RenderDesc.h"
#include "Buffer.h"
#include "Geometry.h"
#include "Query.h"
#include <vector>
namespace fragcore {
	/**
	 *
	 */
	enum CLEARBITMASK {	//TODO rename
		eColor = 0x1,		/*	Clear color.    */
		eDepth = 0x2,		/*	Clear depth.    */
		eStencil = 0x4,		/*	Clear stencil.  */
	};

	/**
	 *
	 */
	class FVDECLSPEC IRenderer : public ICompute {
	public:

		/*	TODO rename and fix enum names.	*/
		enum State {
			eDepthTest = 0x1,           /*	Perform depth test on pixels.   */
			eStencilTest,               /*	Perform stencil test.   */
			eScissorTest,               /*  */
			eAlphaTest,                 /*  */
			eDepthMask,                 /*	Perform .	*/  //TODO determine how to deal with depth, stencil and etc.
			eBlend,                     /*	Set Color Blend. */
			eCullface,                  /*	Set Culling face. */
			eDither,                    /*	Set Color dithering.    */
			eSRGB,
			eMultiSampling,
			eSampleShading,             /*   */
			//GL_SAMPLE_COVERAGE
			eSampleAlphaCoverage,       /*    */
			eDiscardRasterization,      /*  */

		};

		//TODO make it less state machine and allow it to become more modern.
		virtual ~IRenderer(void) = default;
		IRenderer(void) = default;

		/**
		 *	Create texture.
		 *
		 *	@Return non null texture object if succesfully. Null otherwise.
		 */
		virtual Texture *createTexture(TextureDesc *desc) noexcept(false) = 0;

		virtual void deleteTexture(Texture *texture) noexcept(false) = 0;

		virtual Sampler *createSampler(SamplerDesc *desc) = 0;

		virtual void deleteSampler(Sampler *texture) = 0;

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual RenderPipeline *createPipeline(const ProgramPipelineDesc *desc) = 0;

		/**
		 *
		 * @param obj
		 * @return
		 */
		virtual void deletePipeline(RenderPipeline *obj) = 0;

		/**
		 *	Create shader.
		 *
		 *	@Return
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
		 *	@Return
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

		//TODO add viewobject for handling as a object
		virtual ViewPort *getView(unsigned int i) = 0;

		// /**
		//  *
		//  * @param bitflag
		//  */
		// virtual void clear(unsigned int bitflag);   //TODO relocate to the default framebuffer.

		// /**
		//  * Set clear color RGBA.
		//  * @param r
		//  * @param g
		//  * @param b
		//  * @param a
		//  */
		// virtual void clearColor(float r, float g, float b, float a);    // TODO relocate to the framebuffer.

		// /**
		//  *	Enable VSync.
		//  */
		// virtual void setVSync(int sync);	//TODO relocate to the render window.

		/**
		 *	Swap current window buffer.
		 */
		//virtual void swapBuffer(void);	//TODO relocate to the render window.

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
		//virtual void drawInstance(Shader* pipeline, GeometryObject* geometry, unsigned int num);

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
		virtual void blit(const FrameBuffer *source, FrameBuffer *dest,
		                  Texture::FilterMode filterMode);    //TODO add filter mode.    /*  TODO add filter and buffer bit.    */
		//virtual void blit(const FrameBuffer* source, FrameBuffer* dest, int* source, int* dest, Texture::FilterMode filterMode, FrameBuffer::BufferAttachment attachment);
		//TODO add additional version of the blit for sub image specifiction.


		virtual void bindTextures(unsigned int firstUnit, const std::vector<Texture *> &textures);

		virtual void bindImages(unsigned int firstUnit, const std::vector<Texture *> &textures,
		                        const std::vector<Texture::MapTarget> &mapping,
		                        const std::vector<Texture::Format> &formats);

		/**
		 *
		 * @param source
		 * @param target
		 */
		//TODO add version with regiion specifiction.
		virtual void copyTexture(const Texture *source, Texture *target);
		//virtual void copyTexture(const Texture* source, Texture* target, int* sourceCoord, int* targetCoord);

		/**
		 * Dispatch compute program.
		 * @param global global workgroup.
		 * @param local local workgroup
		 * @param offset offset in indirect buffer in number of bytes.
		 */
		virtual void dispatchCompute(unsigned int *global, unsigned int *local, unsigned int offset = 0);

		//TODO add memory barrier.
		virtual void memoryBarrier(void);

		virtual Sync *createSync(SyncDesc *desc);

		virtual void deleteSync(Sync *sync);

		//virtual void execute(CommandList *list);

		/**
		 * Set debug state.
		 * @param enable
		 */
		virtual void setDebug(bool enable);

		/**
		 *	Get shader version.
		 *	@Return non-null terminated string.
		 */
		virtual const char *getShaderVersion(ShaderLanguage language) const;

		/**
		 * Get all support shader languages.
		 * @return bitflag of all supported shader languages.
		 */
		virtual ShaderLanguage getShaderLanguage(void) const;

		/**
		 * Get the name of the rendering API.
		 * @return non-null terminated string.
		 */
		virtual const char *getAPIVersion(void) const;

		/**
		 *	Get version of the interface.
		 *	@Return non-null terminated string.
		 */
		virtual const char *getVersion(void) const;

		/**
		 * Get all supported texture formats.
		 * @param pCompressions
		 */
		virtual void getSupportedTextureCompression(TextureDesc::Compression *pCompressions);

		//virtual void getSupportedTextureFormat(TextureFormat* textureFormat);
		//virtual void getSupportedGraphicTextureFormat(TextureFormat* textureFormat);

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
		//TODO imporove later
		virtual void getStatus(MemoryInfo *memoryInfo);

		virtual CommandList *createCommandBuffer(void);
		virtual void submittCommand(Ref<CommandList> &list);
		virtual void execute(CommandList *list);

		virtual void *getData(void) const;

	private:    /*  */
		void *pdata;
	};
}

/**
 * Create internal rendering interface.
 * @param config
 * @return non-null renderinginterface object.
 */
extern "C" fragcore::IRenderer* createInternalRenderer(fragcore::IConfig* config);

#endif
