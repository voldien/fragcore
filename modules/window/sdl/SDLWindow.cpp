#include "SDLWindow.h"
#include "SDLDisplay.h"
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_video.h>

using namespace fragcore;

SDLWindow::SDLWindow() {

	int width = 800;
	int height = 600;

	SDL_WindowFlags window_flags =
		(SDL_WindowFlags)(SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_INPUT_FOCUS);
	this->window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);

	if (window == nullptr) {
		throw RuntimeException("Failed to create window {}", SDL_GetError());
	}
}

SDLWindow::SDLWindow(intptr_t reference) {
	/*	*/
	this->window = reinterpret_cast<SDL_Window *>(reference);
}

SDLWindow::~SDLWindow() {

	/*	*/
	SDL_DestroyWindow(this->window);
	this->window = nullptr;
}

void SDLWindow::show() { SDL_ShowWindow(this->window); }

void SDLWindow::hide() { SDL_HideWindow(this->window); }

void SDLWindow::close() { this->hide(); }

void SDLWindow::setTitle(const std::string &title) { SDL_SetWindowTitle(window, title.c_str()); }

std::string SDLWindow::getTitle() const { return SDL_GetWindowTitle(window); }

void SDLWindow::setPosition(int x, int y) { SDL_SetWindowPosition(this->window, x, y); }

void SDLWindow::setSize(int width, int height) { SDL_SetWindowSize(this->window, width, height); }

void SDLWindow::getPosition(int *x, int *y) const { SDL_GetWindowPosition(this->window, x, y); }

void SDLWindow::getSize(int *width, int *height) const { SDL_GetWindowSize(this->window, width, height); }

void SDLWindow::resizable(bool resizable) { SDL_SetWindowResizable(this->window, (SDL_bool)resizable); }

void SDLWindow::setFullScreen(bool fullscreen) {
	if (fullscreen) {
		SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	} else {
		SDL_SetWindowFullscreen(this->window, 0);
	}
}
void SDLWindow::setFullScreen(fragcore::Display &display) {
	this->setPosition(display.x(), display.y());
	this->setSize(display.width(), display.height());
	SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

bool SDLWindow::isFullScreen() const {
	int x, y;
	this->getPosition(&x, &y);

	return (int)this->getCurrentDisplay()->width() == this->width() &&
		   (int)this->getCurrentDisplay()->height() == this->height() && (int)this->getCurrentDisplay()->x() == x &&
		   (int)this->getCurrentDisplay()->y() == y;
}

void SDLWindow::setBordered(bool bordered) { SDL_SetWindowBordered(this->window, (SDL_bool)bordered); }

int SDLWindow::width() const {
	int width, height;
	this->getSize(&width, &height);
	return width;
}
int SDLWindow::height() const {
	int width, height;
	this->getSize(&width, &height);
	return height;
}

float SDLWindow::getGamma() const {
	uint16_t ramp[256 * 3];
	int err = SDL_GetWindowGammaRamp(this->window, &ramp[256l * 0], &ramp[256l * 1], &ramp[256l * 2]);
	if (err == -1) {
		throw NotSupportedException(SDL_GetError());
	}

	return this->computeGammaExponent<float, uint16_t>(ramp);
}

void SDLWindow::setGamma(float gamma) {
	uint16_t ramp[256 * 3] = {0};

	this->calculateGammaLookupTable<float, uint16_t>(gamma, ramp);
	int err = SDL_SetWindowGammaRamp(this->window, &ramp[256l * 0], &ramp[256l * 1], &ramp[256l * 2]);

	if (err == -1) {
		throw NotSupportedException("Failed to set window gamma {}: {}", gamma, SDL_GetError());
	}
}

void SDLWindow::setMinimumSize(int width, int height) { SDL_SetWindowMinimumSize(this->window, width, height); }
void SDLWindow::getMinimumSize(int *width, int *height) { SDL_GetWindowMinimumSize(this->window, width, height); }

void SDLWindow::setMaximumSize(int width, int height) { SDL_SetWindowMaximumSize(this->window, width, height); }
void SDLWindow::getMaximumSize(int *width, int *height) { SDL_GetWindowMaximumSize(this->window, width, height); }

void SDLWindow::focus() {
	int error_code = SDL_SetWindowInputFocus(this->window);
	if (error_code != 0) {
		
	}
}

void SDLWindow::restore() { SDL_RestoreWindow(this->window); }

void SDLWindow::maximize() { SDL_MaximizeWindow(this->window); }

void SDLWindow::minimize() { SDL_MinimizeWindow(this->window); }

fragcore::Display *SDLWindow::getCurrentDisplay() const {
	int index;
	index = SDL_GetWindowDisplayIndex(this->window);
	return new SDLDisplay(index);
}

intptr_t SDLWindow::getNativePtr() const {
	return (intptr_t)this->window;

	SDL_SysWMinfo info;

	SDL_VERSION(&info.version); /* initialize info structure with SDL version info */

	if (SDL_GetWindowWMInfo(window, &info)) { /* the call returns true on success */
		/* success */
		switch (info.subsystem) {
		case SDL_SYSWM_UNKNOWN:
			return 0;
		case SDL_SYSWM_WINDOWS:
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
			return info.info.win.window;
#endif
		case SDL_SYSWM_X11:
#if defined(SDL_VIDEO_DRIVER_X11)
			return (intptr_t)info.info.x11.window;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 3)
		case SDL_SYSWM_WINRT:
#endif
		case SDL_SYSWM_DIRECTFB:
		case SDL_SYSWM_COCOA:
		case SDL_SYSWM_UIKIT:
#if SDL_VERSION_ATLEAST(2, 0, 2)
		case SDL_SYSWM_WAYLAND:
#if defined(SDL_VIDEO_DRIVER_WAYLAND)
			return (intptr_t)info.info.wl.surface;
#endif
		case SDL_SYSWM_MIR:
#if defined(SDL_VIDEO_DRIVER_MIR)
			return (intptr_t)info.info.mir.surface;
#endif
#endif
#if SDL_VERSION_ATLEAST(2, 0, 4)
		case SDL_SYSWM_ANDROID:
#endif
#if SDL_VERSION_ATLEAST(2, 0, 5)
		case SDL_SYSWM_VIVANTE:
			break;
#endif
		default:
			break;
		}
	} else {
		throw RuntimeException(fmt::format("%s", SDL_GetError()));
	}
}