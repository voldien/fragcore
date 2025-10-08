/*
 *	FragCore - Fragment Core
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
	class FVDECLSPEC IRenderer : public ICompute {
	  public:
		// TODO make it less state machine and allow it to become more modern.
		IRenderer() = default;
		IRenderer(const IRenderer &other) = delete;
		IRenderer(IRenderer &&other) = delete;
		~IRenderer() override = default;

		/**
		 * @brief
		 */
		virtual RendererWindow *createWindow(int x, int y, int width, int height) = 0;

		/**
		 * @brief
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
		 * @brief
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
