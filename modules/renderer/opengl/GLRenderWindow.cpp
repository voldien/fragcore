#define FRAG_CORE_INTERNAL_IMP 1
#include "GLRenderWindow.h"
#include "internal_object_type.h"

#include <SDL2/SDL_egl.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_syswm.h>
#include <SDLWindowManager.h>

#include <fmt/core.h>

using namespace fragcore;

GLRenderWindow::GLRenderWindow(Ref<GLRendererInterface> &renderer) {
	this->renderer = renderer;

	this->window = SDL_CreateWindow("", 0, 0, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);

	/*  */
	if (window == nullptr) {
		throw RuntimeException("Failed to create window {} for API {}", SDL_GetError(), api);
	}

	// TODO make it part of the framebuffer bind
	if (SDL_GL_MakeCurrent(this->window, renderer->getOpenGLContext()) != 0) {
		throw RuntimeException("{}", SDL_GetError());
	}
}

GLRenderWindow::~GLRenderWindow() {
	/*	Release all the resources.	*/
	// TODO add the swap chain to be part of the window rather than
}

void GLRenderWindow::show() { SDL_ShowWindow(this->window); }

void GLRenderWindow::hide() { SDL_HideWindow(this->window); }

void GLRenderWindow::close() { SDL_DestroyWindow(this->window); }

ViewPort *GLRenderWindow::getViewPort() { return this->renderer->getView(0); }
FrameBuffer *GLRenderWindow::getDefaultFrameBuffer() {
	return this->renderer->getDefaultFramebuffer(nullptr);
	//    throw NotSupportedException();
}

void GLRenderWindow::swapBuffer() { SDL_GL_SwapWindow(this->window); }

void GLRenderWindow::setPosition(int x, int y) { SDL_SetWindowPosition(this->window, x, y); }

void GLRenderWindow::setSize(int width, int height) {
	/*	TODO determine if it shall update framebuffera as well.	*/
	SDL_SetWindowSize(this->window, width, height);
}

int GLRenderWindow::width() const {
	int width, height;
	getSize(&width, &height);
	return width;
}
int GLRenderWindow::height() const {
	int width, height;
	getSize(&width, &height);
	return height;
}

void GLRenderWindow::vsync(bool state) { SDL_GL_SetSwapInterval(state); }

bool GLRenderWindow::assertConfigAttributes(const fragcore::IConfig *iConfig) { return false; }

float GLRenderWindow::getGamma() const {
	uint16_t ramp[256 * 3];
	int err = SDL_GetWindowGammaRamp(this->window, &ramp[256 * 0], &ramp[256 * 1], &ramp[256 * 2]);
	if (err == -1) {
		throw NotSupportedException(SDL_GetError());
	}

	return this->computeGammaExponent<float, uint16_t>(ramp);
}

void GLRenderWindow::setGamma(float gamma) {
	uint16_t ramp[256 * 3] = {0};

	this->calculateGammaLookupTable<float, uint16_t>(gamma, ramp);
	int err = SDL_SetWindowGammaRamp(this->window, &ramp[256 * 0], &ramp[256 * 1], &ramp[256 * 2]);

	if (err == -1) {
		throw NotSupportedException("Failed to set window gamma {}: {}", gamma, SDL_GetError());
	}
}

// GLRenderWindow::~RendererWindow() {}

void GLRenderWindow::getPosition(int *x, int *y) const { SDL_GetWindowPosition(this->window, x, y); }

void GLRenderWindow::getSize(int *width, int *height) const { SDL_GetWindowSize(this->window, width, height); }

fragcore::Display *GLRenderWindow::getCurrentDisplay() const {
	int index;
	index = SDL_GetWindowDisplayIndex(this->window);
	return nullptr;
	// WindowManager::getInstance()->getDisplay(index);
}

void GLRenderWindow::createWindow(int x, int y, int width, int height, const char *api) {
	window = SDL_CreateWindow("", x, y, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);

	/*  */
	if (window == nullptr) {
		throw RuntimeException("Failed to create window {} for API {}", SDL_GetError(), api);
	}
	// this->api = api;
}

void GLRenderWindow::useWindow(void *pdata) {}

void GLRenderWindow::setTitle(const std::string &title) { SDL_SetWindowTitle(this->window, title.c_str()); }

std::string GLRenderWindow::getTitle() const { return SDL_GetWindowTitle(this->window); }

void GLRenderWindow::resizable(bool resizable) { SDL_SetWindowResizable(this->window, (SDL_bool)resizable); }

void GLRenderWindow::setFullScreen(bool fullscreen) {
	// TODO add option for using either of the modes.
	if (fullscreen)
		SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else
		SDL_SetWindowFullscreen(this->window, 0);
}

void GLRenderWindow::setFullScreen(Display &display) {}

bool GLRenderWindow::isFullScreen() const { return false; }

void GLRenderWindow::setBordered(bool bordered) { SDL_SetWindowBordered(this->window, (SDL_bool)bordered); }

void GLRenderWindow::setMinimumSize(int width, int height) { SDL_SetWindowMinimumSize(this->window, width, height); }
void GLRenderWindow::getMinimumSize(int *width, int *height) {}

void GLRenderWindow::setMaximumSize(int width, int height) { SDL_SetWindowMaximumSize(this->window, width, height); }
void GLRenderWindow::getMaximumSize(int *width, int *height) {}

void GLRenderWindow::focus() { SDL_SetWindowInputFocus(this->window); }

void GLRenderWindow::restore() { SDL_RestoreWindow(this->window); }

void GLRenderWindow::maximize() { SDL_MaximizeWindow(this->window); }

void GLRenderWindow::minimize() { SDL_MinimizeWindow(this->window); }

intptr_t GLRenderWindow::getNativePtr() const {
	return reinterpret_cast<intptr_t>(this->window);
	//     SDL_SysWMinfo info;

	//     SDL_VERSION(
	//         &info.version); /* initialize info structure with SDL version
	//         info */

	//     if (SDL_GetWindowWMInfo(window,
	//                             &info)) { /* the call returns true on success
	//                             */
	//         /* success */
	//         switch (info.subsystem) {
	//             case SDL_SYSWM_UNKNOWN:
	//                 return 0;
	//             case SDL_SYSWM_WINDOWS:
	// #if defined(SDL_VIDEO_DRIVER_WINDOWS)
	//                 return info.info.win.window;
	// #endif
	//             case SDL_SYSWM_X11:
	// #if defined(SDL_VIDEO_DRIVER_X11)
	//                 return info.info.x11.window;
	// #endif
	// #if SDL_VERSION_ATLEAST(2, 0, 3)
	//             case SDL_SYSWM_WINRT:
	// #endif
	//             case SDL_SYSWM_DIRECTFB:
	//             case SDL_SYSWM_COCOA:
	//             case SDL_SYSWM_UIKIT:
	// #if SDL_VERSION_ATLEAST(2, 0, 2)
	//             case SDL_SYSWM_WAYLAND:
	// #if defined(SDL_VIDEO_DRIVER_WAYLAND)
	//                 return (intptr_t)info.info.wl.surface;
	// #endif
	//             case SDL_SYSWM_MIR:
	// #if defined(SDL_VIDEO_DRIVER_MIR)
	//                 return (intptr_t)info.info.mir.surface;
	// #endif
	// #endif
	// #if SDL_VERSION_ATLEAST(2, 0, 4)
	//             case SDL_SYSWM_ANDROID:
	// #endif
	// #if SDL_VERSION_ATLEAST(2, 0, 5)
	//             case SDL_SYSWM_VIVANTE:
	//                 break;
	// #endif
	//         }
	//     } else
	//         throw RuntimeException(fmt::format("{}", SDL_GetError()));
	//     throw NotImplementedException("Window format not implemented");
}

void GLRenderWindow::setIcon(void *pVoid) {}

void *GLRenderWindow::getIcon() const { return nullptr; }

void GLRenderWindow::createSwapChain() {}