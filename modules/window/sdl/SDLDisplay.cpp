#include "SDLDisplay.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>

using namespace fragcore;

SDLDisplay::SDLDisplay(const int index) : index(index) {}

unsigned int SDLDisplay::x() const {
	SDL_Rect bound;
	const int err = SDL_GetDisplayBounds(this->index, &bound);
	if (err != 0) {
		throw RuntimeException("Failed getting display {} x, {}.", this->index, SDL_GetError());
	}
	return bound.x;
}

unsigned int SDLDisplay::y() const {
	SDL_Rect bound;
	const int err = SDL_GetDisplayBounds(this->index, &bound);
	if (err != 0) {
		throw RuntimeException("Failed getting display {} y, {}.", this->index, SDL_GetError());
	}
	return bound.y;
}

unsigned int SDLDisplay::width() const {
	SDL_Rect bound;
	const int err = SDL_GetDisplayBounds(this->index, &bound);
	if (err != 0) {
		throw RuntimeException("Failed getting display {} width, {}.", this->index, SDL_GetError());
	}
	return bound.w;
}

unsigned int SDLDisplay::height() const {
	SDL_Rect bound;
	const int err = SDL_GetDisplayBounds(this->index, &bound);
	if (err != 0) {
		throw RuntimeException("Failed getting display {} height, {}.", this->index, SDL_GetError());
	}
	return bound.h;
}

unsigned int SDLDisplay::refreshRate() const {
	SDL_DisplayMode mode;
	const int err = SDL_GetCurrentDisplayMode(this->index, &mode);
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

	for (int display_index = 0; display_index < n_display_modes; display_index++) {
		SDL_GetDisplayMode(this->index, display_index, &mode);
		modes[display_index].width = mode.w;
		modes[display_index].height = mode.h;
		modes[display_index].refreshRate = mode.refresh_rate;
	}

	return modes;
}

void SDLDisplay::getDPI(Display::DPI *dpi) {
	const int err = SDL_GetDisplayDPI(this->index, &dpi->ddpi, &dpi->hdpi, &dpi->vdpi);
	if (err != 0) {
		throw RuntimeException("Failed to retrieve display {} DPI, {}", this->index, SDL_GetError());
	}
}

void SDLDisplay::setMode([[maybe_unused]] const Mode &mode) {}

DisplayFormat SDLDisplay::getFormat() {
	SDL_DisplayMode mode;
	const int err = SDL_GetCurrentDisplayMode(this->index, &mode);
	if (err != 0) {
		throw RuntimeException("Failed to get display {} Mode, {}", this->index, SDL_GetError());
	}
	return translateFormat(mode.format);
}

DisplayFormat SDLDisplay::translateFormat(unsigned int format) {
	switch (format) {
	case SDL_PIXELFORMAT_INDEX1LSB:
		return DisplayFormat::PIXELFORMAT_INDEX1LSB;
	case SDL_PIXELFORMAT_RGB332:
		return DisplayFormat::PIXELFORMAT_RGB332;
	case SDL_PIXELFORMAT_RGB24:
		return DisplayFormat::PIXELFORMAT_RGB24;
	case SDL_PIXELFORMAT_ARGB2101010:
		return DisplayFormat::PIXELFORMAT_ARGB2101010;
	case SDL_PIXELFORMAT_BGR24:
		return DisplayFormat::PIXELFORMAT_BGR24;
	case SDL_PIXELFORMAT_RGB888:
		return DisplayFormat::PIXELFORMAT_RGB888;
	case SDL_PIXELFORMAT_RGBA32:
		return DisplayFormat::PIXELFORMAT_RGBA32;
	default:
		return DisplayFormat::PIXELFORMAT_UNKNOWN;
	}
	return DisplayFormat::PIXELFORMAT_UNKNOWN;
}

int SDLDisplay::getNumDisplays() noexcept { return SDL_GetNumVideoDisplays(); }

SDLDisplay SDLDisplay::getPrimaryDisplay() noexcept { return {0}; }

SDLDisplay SDLDisplay::getDisplay(const int index) { return {index}; }