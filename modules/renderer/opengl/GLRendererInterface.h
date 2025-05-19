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
#ifndef _FRAGCORE_GL_RENDERER_INTERFACE_H_
#define _FRAGCORE_GL_RENDERER_INTERFACE_H_ 1
#include "../IRenderer.h"

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC GLRendererInterface : public IRenderer {
	  public:
		GLRendererInterface(const IConfig *config);
		~GLRendererInterface() override;

		void onInitialization() override;
		void onDestruction() override;

		bool supportCompute() noexcept override;

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

		void *getData() const override;

		IConfig getDefaultConfig() const noexcept;

		const char *getExtensions() const noexcept;
		bool isExtensionSupported(const char *extension) const noexcept;

		/*	OpenGL Specific methods.	*/
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

		FrameBuffer *defaultFrameBuffer = nullptr;
		/*  Texture gamma corrections.  */
		bool gamma;
		ShaderLanguage supportedLanguages;
	};
} // namespace fragcore

#endif
