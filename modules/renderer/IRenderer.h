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
#include "ICompute.h"
#include "RenderDesc.h"
#include "RenderPrerequisites.h"
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
