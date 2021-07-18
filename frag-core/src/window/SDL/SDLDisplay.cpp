#include "Window/SDL/SDLDisplay.h"
#include <Exception/RuntimeException.h>
#include <Renderer/RenderDesc.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>
#include <Utils/StringUtil.h>
#include <fmt/core.h>
using namespace fragcore;

unsigned int SDLDisplay::x(void) const {
	SDL_Rect bound;
	int err = SDL_GetDisplayBounds(this->index, &bound);
	if (err != 0)
		throw RuntimeException(fmt::format("Failed getting display {} bounds, %s.", this->index, SDL_GetError()));
	return bound.x;
}

unsigned int SDLDisplay::y(void) const {
	SDL_Rect bound;
	int err = SDL_GetDisplayBounds(this->index, &bound);
	if (err != 0)
		throw RuntimeException(fmt::format("Failed getting display {} bounds, %s.", this->index, SDL_GetError()));
	return bound.y;
}

unsigned int SDLDisplay::width(void) const {
	SDL_Rect bound;
	int err = SDL_GetDisplayBounds(this->index, &bound);
	if (err != 0)
		throw RuntimeException(fmt::format("Failed getting display {} bounds, %s.", this->index, SDL_GetError()));
	return bound.w;
}

unsigned int SDLDisplay::height(void) const {
	SDL_Rect bound;
	int err = SDL_GetDisplayBounds(this->index, &bound);
	if (err != 0)
		throw RuntimeException(fmt::format("Failed getting display {} bounds, %s.", this->index, SDL_GetError()));
	return bound.h;
}

unsigned int SDLDisplay::refreshRate(void) const {
	SDL_DisplayMode mode;
	int err = SDL_GetCurrentDisplayMode(this->index, &mode);
	if (err != 0)
		throw RuntimeException(fmt::format("Failed getting display {} refresh rate, %s.", this->index, SDL_GetError()));
	return mode.refresh_rate;
}

std::vector<Display::Mode> SDLDisplay::getModes(void) const {
	int n = SDL_GetNumDisplayModes(this->index);
	SDL_DisplayMode mode;

	std::vector<Display::Mode> modes;
	modes.resize(n);

	for (int i = 0; i < n; i++) {
		SDL_GetDisplayMode(this->index, i, &mode);
		modes[i].width = mode.w;
		modes[i].height = mode.h;
		modes[i].refreshRate = mode.refresh_rate;
	}

	return modes;
}

void SDLDisplay::getDPI(Display::DPI *dpi) {
	int err = SDL_GetDisplayDPI(this->index, &dpi->ddpi, &dpi->hdpi, &dpi->vdpi);
	if (err != 0)
		throw RuntimeException(fmt::format("Failed to retrieve display {} DPI, %s", this->index, SDL_GetError()));
}

TextureFormat SDLDisplay::getFormat(void) {
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(this->index, &mode);
	return (TextureFormat)translateFormat(mode.format);
}

SDLDisplay::SDLDisplay(int index) { this->index = index; }

unsigned int SDLDisplay::translateFormat(unsigned int format) {
	switch (format) {
	case SDL_PIXELFORMAT_INDEX1LSB:
	case SDL_PIXELFORMAT_RGB332:
	case SDL_PIXELFORMAT_RGB24:
	case SDL_PIXELFORMAT_ARGB2101010:
	case SDL_PIXELFORMAT_BGR24:
	case SDL_PIXELFORMAT_RGB888:
	case SDL_PIXELFORMAT_RGBA32:
		return 0;
	}
	return 0;
}

int SDLDisplay::getNumDisplays(void) { return SDL_GetNumVideoDisplays(); }