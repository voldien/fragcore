#ifndef _FRAG_CORE_VULKAN_WINDOW_H_
#define _FRAG_CORE_VULKAN_WINDOW_H_ 1
#include "../IRenderer.h"
#include "../RendererWindow.h"
#if defined(FRAG_CORE_INTERNAL_IMP)  // TODO resolve to a single file or
                                     // something later
#include <SDL2/SDL_syswm.h>
#endif

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC GLRenderWindow : public RendererWindow {
	public:
		GLRenderWindow(Ref<IRenderer> &renderer);
		~GLRenderWindow(void);

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

		virtual void createWindow(int x, int y, int width, int height,
								const char *api) override;
		virtual bool assertConfigAttributes(const IConfig *iConfig) override;
		virtual void useWindow(void *pdata) override;

	protected:
		virtual void createSwapChain(void);

	private:
		SDL_Window *window;
		Ref<IRenderer> renderer;
	};
}  // namespace fragcore

#endif