#include "Window/Display.h"
#include <SDL2/SDL_video.h>

#include <stdexcept>

////SDL_ShowSimpleMessageBox
////SDL_ShowMessageBox
////SDL_GetNumVideoDisplays
// using namespace fragview;
//
// unsigned int Display::x() const {
//	SDL_Rect bound;
//	int err = SDL_GetDisplayBounds(this->index, &bound);
//	if (err != 0)
//		throw RuntimeException(fmt::format("Failed getting display %d bounds, {}.", this->index, SDL_GetError()));
//	return bound.x;
//}
//
// unsigned int Display::y() const {
//	SDL_Rect bound;
//	int err = SDL_GetDisplayBounds(this->index, &bound);
//	if (err != 0)
//		throw RuntimeException(fmt::format("Failed getting display %d bounds, {}.", this->index, SDL_GetError()));
//	return bound.y;
//}
//
// unsigned int Display::width() const {
//	SDL_Rect bound;
//	int err = SDL_GetDisplayBounds(this->index, &bound);
//	if (err != 0)
//		throw RuntimeException(fmt::format("Failed getting display %d bounds, {}.", this->index, SDL_GetError()));
//	return bound.w;
//}
//
// unsigned int Display::height() const {
//	SDL_Rect bound;
//	int err = SDL_GetDisplayBounds(this->index, &bound);
//	if (err != 0)
//		throw RuntimeException(fmt::format("Failed getting display %d bounds, {}.", this->index, SDL_GetError()));
//	return bound.h;
//}
//
// unsigned int Display::refreshRate() const {
//	SDL_DisplayMode mode;
//	int err = SDL_GetCurrentDisplayMode(this->index, &mode);
//	if (err != 0)
//		throw RuntimeException(fmt::format("Failed getting display %d refresh rate, {}.", this->index, SDL_GetError()));
//	return mode.refresh_rate;
//}
//
// std::vector<Display::Mode> Display::getModes() const {
//	int n = SDL_GetNumDisplayModes(this->index);
//	SDL_DisplayMode mode;
//
//	std::vector<Display::Mode> modes;
//	modes.resize(n);
//
//	for (int i = 0; i < n; i++) {
//		SDL_GetDisplayMode(this->index, i, &mode);
//		modes[i].width = mode.w;
//		modes[i].height = mode.h;
//		modes[i].refreshRate = mode.refresh_rate;
//	}
//
//	return modes;
//}
//
////float Display::getGamma() const {
////	//TODO determine how to compute gamma exponent.
////	ushort ramp[256 * 3];
////	//SDL_CalculateGammaRamp()
////	//SDL_GetWindowGammaRamp
////	return 0;
////}
//
////float Display::setGamma(float gamma) {
////	ushort ramp[256 * 3];
////
////	float exponent = 1.0f / gamma;
////
////	//SDL_CalculateGammaRamp
////
////	for (uint i = 0; i < 256; ++i) {
////		float linear = float(i) * 1.0f / 255u;
////		float corrected = (float) pow(linear, exponent);
////		ushort entry = ushort(corrected * 65535);
////		ramp[i] = ramp[i + 256] = ramp[i + 512] = entry;
////	}
////
////	//SDL_SetWindowGammaRamp()
////}
//
// void Display::getDPI(Display::DPI *dpi) {
//	int err = SDL_GetDisplayDPI(this->index, &dpi->ddpi, &dpi->hdpi, &dpi->vdpi);
//	if (err != 0)
//		throw RuntimeException(fmt::format("Failed to retrieve display %d DPI, {}", this->index, SDL_GetError()));
//}
//
// float Display::getFormat() {
//	return 0;
//}

// int Display::getNumDisplays() {
//	int n = SDL_GetNumVideoDisplays();
//	if (n <= 0)
//		throw RuntimeException(fmt::format("Failed to retrieve number of displays, {}", SDL_GetError()));
//}
//
// Display Display::getDisplay(unsigned int index) {
//	Display display;
//	display.setName(SDL_GetDisplayName(index));
//	display.index = index;
//	return display;
//}
