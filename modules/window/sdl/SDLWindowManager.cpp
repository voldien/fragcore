#include "SDLWindowManager.h"
#include "SDLDisplay.h"
#include "SDLWindow.h"
#include <FragCore.h>
#include <SDL2/SDL.h>

using namespace fragcore;

SDLWindowManager::SDLWindowManager() {

	int err = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER);
	if (err != 0) {
		throw RuntimeException("Failed to init SDL Window Manager: {}", SDL_GetError());
	}

	this->displayPool.resize(16, SDLDisplay(0));

	/*	Populate Displays.	*/
	this->activeDisplays.clear();
	for (size_t i = 0; i < this->getNumDisplays(); i++) {
		this->activeDisplays.push_back((SDLDisplay *)this->getDisplay(i));
	}
}
SDLWindowManager::~SDLWindowManager() { SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER); }

Display *SDLWindowManager::getDisplay(unsigned int index) {
	this->displayPool[index] = SDLDisplay(index);
	return &this->displayPool[index];
}
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
Display *SDLWindowManager::primaryDisplay() const noexcept { return (Display *)&this->displayPool[0]; }

const std::vector<Display *> &SDLWindowManager::getDisplays() const {

	return (const std::vector<Display *> &)activeDisplays;
}