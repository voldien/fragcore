#include "SDLWindow.h"
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_video.h>

using namespace fragcore;

SDLWindow::SDLWindow() {

	SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

	int width = 800;
	int height = 600;

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
													 SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_INPUT_FOCUS);
	this->window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
}
SDLWindow::~SDLWindow() {
	SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
}

void SDLWindow::show() { SDL_ShowWindow(this->window); }

void SDLWindow::hide() { SDL_HideWindow(this->window); }

void SDLWindow::close() {
	this->hide();
	SDL_DestroyWindow(this->window);
}

void SDLWindow::setTitle(const std::string &title) { SDL_SetWindowTitle(window, title.c_str()); }

std::string SDLWindow::getTitle() const { return SDL_GetWindowTitle(window); }

// TODO change the type to image.
void SDLWindow::setIcon(void *) {}
//		virtual void setIcon(Image& image) = 0;

void *SDLWindow::getIcon() const {}
// virtual Image getIcon() const = 0;
//		virtual Image* setIcon(Image* image) = 0;

void SDLWindow::setPosition(int x, int y) noexcept { SDL_SetWindowPosition(this->window, x, y); }

void SDLWindow::setSize(int width, int height) noexcept { SDL_SetWindowSize(this->window, width, height); }

void SDLWindow::getPosition(int *x, int *y) const { SDL_GetWindowPosition(this->window, x, y); }

void SDLWindow::getSize(int *width, int *height) const { SDL_GetWindowSize(this->window, width, height); }

// int SDLWindow::x() const noexcept {
// 	int x, y;
// 	SDL_GetWindowPosition(this->window, &x, &y);
// 	return x;
// }
// int SDLWindow::y() const noexcept {
// 	int x, y;
// 	SDL_GetWindowPosition(this->window, &x, &y);
// 	return y;
// }
void SDLWindow::resizable(bool resizable) noexcept { SDL_SetWindowResizable(this->window, (SDL_bool)resizable); }

void SDLWindow::setFullScreen(bool fullscreen) {

	if (fullscreen)
		SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else
		SDL_SetWindowFullscreen(this->window, 0);
}
void SDLWindow::setFullScreen(fragcore::Display &display) {}

bool SDLWindow::isFullScreen() const { return false; }

void SDLWindow::setBordered(bool bordered) { SDL_SetWindowBordered(this->window, (SDL_bool)bordered); }

int SDLWindow::width() const {
	int w, h;
	getSize(&w, &h);
	return w;
}
int SDLWindow::height() const {
	int w, h;
	getSize(&w, &h);
	return h;
}

float SDLWindow::getGamma() const { return 0; }

void SDLWindow::setGamma(float gamma) {}

void SDLWindow::setMinimumSize(int width, int height) { SDL_SetWindowMinimumSize(this->window, width, height); }
void SDLWindow::getMinimumSize(int *width, int *height) { SDL_GetWindowMinimumSize(this->window, width, height); }

void SDLWindow::setMaximumSize(int width, int height) { SDL_SetWindowMaximumSize(this->window, width, height); }
void SDLWindow::getMaximumSize(int *width, int *height) { SDL_GetWindowMaximumSize(this->window, width, height); }

void SDLWindow::focus() { SDL_SetWindowInputFocus(this->window); }

void SDLWindow::restore() { SDL_RestoreWindow(this->window); }

void SDLWindow::maximize() { SDL_MaximizeWindow(this->window); }

void SDLWindow::minimize() { SDL_MinimizeWindow(this->window); }

fragcore::Display *SDLWindow::getCurrentDisplay() const { return nullptr; }

intptr_t SDLWindow::getNativePtr() const {
	return (intptr_t)this->window;
	// 	SDL_SysWMinfo info;

	// 	SDL_VERSION(&info.version); /* initialize info structure with SDL version info */

	// 	if (SDL_GetWindowWMInfo(window, &info)) { /* the call returns true on success */
	// 		/* success */
	// 		switch (info.subsystem) {
	// 		case SDL_SYSWM_UNKNOWN:
	// 			return 0;
	// 		case SDL_SYSWM_WINDOWS:
	// #if defined(SDL_VIDEO_DRIVER_WINDOWS)
	// 			return info.info.win.window;
	// #endif
	// 		case SDL_SYSWM_X11:
	// #if defined(SDL_VIDEO_DRIVER_X11)
	// 			return (intptr_t)info.info.x11.window;
	// #endif
	// #if SDL_VERSION_ATLEAST(2, 0, 3)
	// 		case SDL_SYSWM_WINRT:
	// #endif
	// 		case SDL_SYSWM_DIRECTFB:
	// 		case SDL_SYSWM_COCOA:
	// 		case SDL_SYSWM_UIKIT:
	// #if SDL_VERSION_ATLEAST(2, 0, 2)
	// 		case SDL_SYSWM_WAYLAND:
	// #if defined(SDL_VIDEO_DRIVER_WAYLAND)
	// 			return (intptr_t)info.info.wl.surface;
	// #endif
	// 		case SDL_SYSWM_MIR:
	// #if defined(SDL_VIDEO_DRIVER_MIR)
	// 			return (intptr_t)info.info.mir.surface;
	// #endif
	// #endif
	// #if SDL_VERSION_ATLEAST(2, 0, 4)
	// 		case SDL_SYSWM_ANDROID:
	// #endif
	// #if SDL_VERSION_ATLEAST(2, 0, 5)
	// 		case SDL_SYSWM_VIVANTE:
	// 			break;
	// #endif
	// 		default:
	// 			break;
	// 		}
	// 	} // else
	// 	  //     throw RuntimeException(fmt::format("%s", SDL_GetError()));
	// 	  // throw NotImplementedException("Window format not implemented");
}