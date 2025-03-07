#include "SDLWindowManager.h"
#include "SDLDisplay.h"
#include "SDLWindow.h"
#include <FragCore.h>
#include <SDL2/SDL.h>

using namespace fragcore;

SDLWindowManager::SDLWindowManager() {
	int err = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
	if (err != 0) {
		throw RuntimeException("Failed to init SDL Window Manager: {}", SDL_GetError());
	}
}
SDLWindowManager::~SDLWindowManager() {
	SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
}

Display *SDLWindowManager::getDisplay(unsigned int index) { return new SDLDisplay(index); }
unsigned int SDLWindowManager::getNumDisplays() const noexcept { return SDLDisplay::getNumDisplays(); }
Display *SDLWindowManager::getAssociatedDisplay(Ref<Window> &window) {
	intptr_t p = window->getNativePtr();
	return nullptr;
}
Window *SDLWindowManager::createWindow(const std::string &title) {
	Window *window = new SDLWindow();
	window->setTitle(title);
	return window;
}
Display *SDLWindowManager::primaryDisplay() const noexcept { return new SDLDisplay(0); }

const std::vector<Display *> &SDLWindowManager::getDisplays() const { return {}; }