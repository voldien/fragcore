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
#ifndef _FRAGCORE_VULKAN_WINDOW_H_
#define _FRAGCORE_VULKAN_WINDOW_H_ 1
#include "VKRenderInterface.h"
#include <../IRenderer.h>
#include <../RendererWindow.h>
#include <vulkan/vulkan.h>
#if defined(FRAG_CORE_INTERNAL_IMP) // TODO resolve to a single file or
									// something later
#include <SDL2/SDL_syswm.h>
#endif

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC VKRenderWindow : public RendererWindow {
	  public:
		VKRenderWindow(Ref<VKRenderInterface> &renderer);
		~VKRenderWindow() override;

		void close() override;

		void show() override;

		void hide() override;

		void focus() override;

		void restore() override;

		void maximize() override;

		void minimize() override;

		void swapBuffer() override;

		void setPosition(int x, int y) override;

		void setSize(int width, int height) override;

		void vsync(bool state) override;

		void getPosition(int *x, int *y) const override;

		void getSize(int *width, int *height) const override;

		int width() const override;
		int height() const override;

		void resizable(bool resizable) override;

		Display *getCurrentDisplay() const override;

		void setFullScreen(bool fullscreen) override;
		void setFullScreen(Display &display) override;

		bool isFullScreen() const override;

		void setBordered(bool bordered) override;

		void setMinimumSize(int width, int height) override;
		void getMinimumSize(int *width, int *height) override;
		void setMaximumSize(int width, int height) override;
		void getMaximumSize(int *width, int *height) override;

		float getGamma() const override;

		void setGamma(float gamma) override;

		void setTitle(const std::string &title) override;
		std::string getTitle() const override;

		intptr_t getNativePtr() const override;

		void createWindow(int x, int y, int width, int height, const char *api) override;
		bool assertConfigAttributes(const IConfig *iConfig) override;
		void useWindow(void *pdata) override;

		/*	Vulkan methods.	*/
		/*	*/
		VkDevice getDevice() const noexcept;
		/*	*/
		uint32_t getCurrentFrameIndex() const noexcept;
		uint32_t getSwapChainImageCount() const noexcept;
		VkFramebuffer getDefaultFrameBuffer() const noexcept;
		VkRenderPass getDefaultRenderPass() const noexcept;

		VkFramebuffer getFrameBuffer(unsigned int index) const noexcept;
		/*	*/
		VkFormat depthStencilFormat() const noexcept;
		VkImage depthStencilImage() const noexcept;
		VkImageView depthStencilImageView() const noexcept;

		/*	*/
		VkImage getDefaultMSSAColorImage() const noexcept;
		VkImageView getDefaultMSSAColorImageView() const noexcept;

		/*	*/
		VkImage getDefaultImage() const;
		VkImageView getDefaultImageView() const;
		VkFormat getDefaultImageFormat() const noexcept;

		/*	*/
		VkCommandBuffer getCurrentCommandBuffer() const noexcept;
		size_t getNrCommandBuffers() const noexcept;
		VkCommandBuffer getCommandBuffers(unsigned int index) const noexcept;
		VkCommandPool getGraphicCommandPool() const noexcept;

		// VkCommandPool getComputeCommandPool() const noexcept;
		const VkPhysicalDeviceProperties &physicalDeviceProperties() const noexcept;

		/*	*/
		uint32_t getGraphicQueueIndex() const;
		VkQueue getDefaultGraphicQueue() const;
		VkQueue getDefaultComputeQueue() const;

		const std::vector<VkImage> &getSwapChainImages() const noexcept;
		const std::vector<VkImageView> &getSwapChainImageViews() const noexcept;

		const std::shared_ptr<fvkcore::VKDevice> &getVKDevice() const noexcept;
		const std::shared_ptr<fvkcore::PhysicalDevice> getPhysicalDevice() const noexcept;

		VkPhysicalDevice physicalDevice() const;
		void setPhysicalDevice(VkPhysicalDevice device);
		std::vector<VkQueue> getQueues() const noexcept;
		const std::vector<VkPhysicalDevice> &availablePhysicalDevices() const;

		VkSurfaceKHR getSurface() { return this->surface; }
		VkSurfaceFormatKHR getSurfaceFormat() { return this->surfaceFormat; }
		VkPresentModeKHR getPresentMode() { return this->presentMode; }

	  protected:
		virtual void createSwapChain();
		virtual void recreateSwapChain();
		virtual void cleanSwapChain();
		VkFormat findDepthFormat();
		VkSurfaceKHR createSurface();

	  private:
		SDL_Window *window;
		Ref<VKRenderInterface> renderer;

		using SwapchainBuffers = struct SwapchainBuffers {
			struct SwapChainSupportDetails {
				VkSurfaceCapabilitiesKHR capabilities;
				std::vector<VkSurfaceFormatKHR> formats;
				std::vector<VkPresentModeKHR> presentModes;
			};

			SwapChainSupportDetails details; /*  */

			std::vector<VkImage> swapChainImages;
			std::vector<VkImageView> swapChainImageViews;
			std::vector<VkFramebuffer> swapChainFramebuffers;
			std::vector<VkCommandBuffer> commandBuffers;

			std::vector<VkFramebuffer *> frameBuffers;

			VkImage depthImage;
			VkDeviceMemory depthImageMemory;
			VkImageView depthImageView;

			/*	*/
			VkFormat swapChainImageFormat;
			VkRenderPass renderPass;
			VkSwapchainKHR swapchain; /*  */
			VkExtent2D chainExtend;	  /*  */
			int currentFrame = 0;
			bool vsync = false;
			int width, height;
		};

		SwapchainBuffers swapChain;
		int currentFrame;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;

		VkQueue queue{VK_NULL_HANDLE}; // TODO rename graphicsQueue
		VkQueue presentQueue{VK_NULL_HANDLE};

		/*  */
		VkPhysicalDeviceProperties gpu_props;
		VkQueueFamilyProperties *queue_props;
		uint32_t graphics_queue_node_index;

		VkSurfaceKHR surface{VK_NULL_HANDLE};
		VkSurfaceFormatKHR surfaceFormat;
		VkPresentModeKHR presentMode;

		VkCommandPool cmd_pool{VK_NULL_HANDLE};
		VkCommandPool compute_pool{VK_NULL_HANDLE};
		VkCommandPool transfer_pool{VK_NULL_HANDLE};

	  public:
		const SwapchainBuffers &getSwapChain() const { return this->swapChain; }
	};
} // namespace fragcore

#endif
