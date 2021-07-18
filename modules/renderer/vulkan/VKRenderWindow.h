#ifndef _FRAG_CORE_VULKAN_WINDOW_H_
#define _FRAG_CORE_VULKAN_WINDOW_H_ 1
#include "VKRenderInterface.h"
#include <Renderer/IRenderer.h>
#include <Renderer/RendererWindow.h>
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
		~VKRenderWindow(void);

		void close(void) override;

		virtual void show(void);

		virtual void hide(void);

		void focus(void) override;

		void restore(void) override;

		void maximize(void) override;

		void minimize(void) override;

		virtual void swapBuffer(void);

		virtual void setPosition(int x, int y);

		virtual void setSize(int width, int height);

		virtual void vsync(bool state) override;

		virtual void getPosition(int *x, int *y) const;

		virtual void getSize(int *width, int *height) const;

		void resizable(bool resizable) override;

		void setIcon(void *pVoid) override;

		void *getIcon(void) const override;

		virtual Display *getCurrentDisplay(void) const;

		void setFullScreen(bool fullscreen) override;
		virtual void setFullScreen(Display &display);

		bool isFullScreen(void) const;

		void setBordered(bool bordered) override;

		void setMinimumSize(int width, int height) override;
		virtual void getMinimumSize(int *width, int *height);
		void setMaximumSize(int width, int height) override;
		virtual void getMaximumSize(int *width, int *height);

		virtual float getGamma(void) const;

		virtual void setGamma(float);

		virtual void setTitle(const char *title);
		virtual const char *getTitle(void) const;

		intptr_t getNativePtr(void) const override;

		virtual ViewPort *getViewPort(void) override;
		virtual FrameBuffer *getDefaultFrameBuffer() override;

		virtual void createWindow(int x, int y, int width, int height, const char *api) override;
		virtual bool assertConfigAttributes(const IConfig *iConfig) override;
		virtual void useWindow(void *pdata) override;

	  public:
		/*	Vulkan methods.	*/
		VkDevice getDevice(void) const;
		int getCurrentFrame(void) const;
		VkFramebuffer getDefaultFrameBuffer(void) const;
		VkFormat depthStencilFormat(void) const;
		VkImage depthStencilImage(void) const;

		VkCommandBuffer getCurrentCommandBuffer(void) const;
		VkRenderPass getDefaultRenderPass(void) const;
		VkCommandPool getGraphicCommadnPool(void) const;
		VkImage getDefaultImage(void) const;
		VkQueue getGraphicQueue(void) const;
		VkPhysicalDevice physicalDevice() const;
		std::vector<VkPhysicalDevice> getPhyiscalDevices(void);
		// virtual void std::vector<SupportedExtensions> getSupportedExtensions(void);

	  protected:
		virtual void createSwapChain(void);
		virtual void recreateSwapChain(void);
		virtual void cleanSwapChain(void);

	  private:
		SDL_Window *window;
		Ref<VKRenderInterface> renderer;

		typedef struct _SwapchainBuffers {
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
			VkFormat swapChainImageFormat;
			VkRenderPass renderPass;
			VkCommandBuffer *currentBuffer;
			VkSwapchainKHR swapchain; /*  */
			VkExtent2D chainExtend;	  /*  */
			int currentFrame;
		} SwapchainBuffers;

		SwapchainBuffers swapChain;
		int currentFrame;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;

		VkQueue queue; // TODO rename graphicsQueue
		VkQueue presentQueue;

		/*  */
		VkPhysicalDeviceProperties gpu_props;
		VkQueueFamilyProperties *queue_props;
		uint32_t graphics_queue_node_index;

		VkSurfaceKHR surface;

		VkCommandPool cmd_pool;
		VkCommandPool compute_pool;
		VkCommandPool transfer_pool;
	};
} // namespace fragcore

#endif
