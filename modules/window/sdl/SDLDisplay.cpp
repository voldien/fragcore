#include "SDLDisplay.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>

using namespace fragcore;

unsigned int SDLDisplay::x() const {
	SDL_Rect bound;
	int err = SDL_GetDisplayBounds(this->index, &bound);
	if (err != 0) {
		throw RuntimeException("Failed getting display {} x, {}.", this->index, SDL_GetError());
	}
	return bound.x;
}

unsigned int SDLDisplay::y() const {
	SDL_Rect bound;
	int err = SDL_GetDisplayBounds(this->index, &bound);
	if (err != 0) {
		throw RuntimeException("Failed getting display {} y, {}.", this->index, SDL_GetError());
	}
	return bound.y;
}

unsigned int SDLDisplay::width() const {
	SDL_Rect bound;
	int err = SDL_GetDisplayBounds(this->index, &bound);
	if (err != 0) {
		throw RuntimeException("Failed getting display {} width, {}.", this->index, SDL_GetError());
	}
	return bound.w;
}

unsigned int SDLDisplay::height() const {
	SDL_Rect bound;
	int err = SDL_GetDisplayBounds(this->index, &bound);
	if (err != 0) {
		throw RuntimeException("Failed getting display {} height, {}.", this->index, SDL_GetError());
	}
	return bound.h;
}

unsigned int SDLDisplay::refreshRate() const {
	SDL_DisplayMode mode;
	int err = SDL_GetCurrentDisplayMode(this->index, &mode);
	if (err != 0) {
		throw RuntimeException("Failed getting display {} refresh rate, {}.", this->index, SDL_GetError());
	}
	return mode.refresh_rate;
}

std::vector<Display::Mode> SDLDisplay::getModes() const {
	const int n_display_modes = SDL_GetNumDisplayModes(this->index);
	SDL_DisplayMode mode;

	std::vector<Display::Mode> modes;
	modes.resize(n_display_modes);

	for (int i = 0; i < n_display_modes; i++) {
		SDL_GetDisplayMode(this->index, i, &mode);
		modes[i].width = mode.w;
		modes[i].height = mode.h;
		modes[i].refreshRate = mode.refresh_rate;
	}

	return modes;
}

void SDLDisplay::getDPI(Display::DPI *dpi) {
	int err = SDL_GetDisplayDPI(this->index, &dpi->ddpi, &dpi->hdpi, &dpi->vdpi);
	if (err != 0) {
		throw RuntimeException("Failed to retrieve display {} DPI, {}", this->index, SDL_GetError());
	}
}

void SDLDisplay::setMode([[maybe_unused]] const Mode &mode) {}

SDLDisplay::DisplayFormat SDLDisplay::getFormat() {
	SDL_DisplayMode mode;
	int err = SDL_GetCurrentDisplayMode(this->index, &mode);
	if (err != 0) {
		throw RuntimeException("Failed to get display {} Mode, {}", this->index, SDL_GetError());
	}
	return static_cast<DisplayFormat>(translateFormat(mode.format));
}

SDLDisplay::SDLDisplay(int index) : index(index) {}

SDLDisplay::DisplayFormat SDLDisplay::translateFormat(unsigned int format) {
	switch (format) {
	case SDL_PIXELFORMAT_INDEX1LSB:
		return Display::DisplayFormat::PIXELFORMAT_INDEX1LSB;
	case SDL_PIXELFORMAT_RGB332:
		return Display::DisplayFormat::PIXELFORMAT_RGB332;
	case SDL_PIXELFORMAT_RGB24:
		return Display::DisplayFormat::PIXELFORMAT_RGB24;
	case SDL_PIXELFORMAT_ARGB2101010:
		return Display::DisplayFormat::PIXELFORMAT_ARGB2101010;
	case SDL_PIXELFORMAT_BGR24:
		return Display::DisplayFormat::PIXELFORMAT_BGR24;
	case SDL_PIXELFORMAT_RGB888:
		return Display::DisplayFormat::PIXELFORMAT_RGB888;
	case SDL_PIXELFORMAT_RGBA32:
		return Display::DisplayFormat::PIXELFORMAT_RGBA32;
	default:
		return Display::DisplayFormat::PIXELFORMAT_UNKNOWN;
	}
	return Display::DisplayFormat::PIXELFORMAT_UNKNOWN;
}

int SDLDisplay::getNumDisplays() { return SDL_GetNumVideoDisplays(); }

SDLDisplay SDLDisplay::getPrimaryDisplay() { return {0}; }

SDLDisplay SDLDisplay::getDisplay(const int index) { return {index}; }