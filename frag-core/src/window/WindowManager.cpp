#include "Window/WindowManager.h"

using namespace fragcore;

Display *WindowManager::getDisplay(unsigned int index) noexcept {
	// return new SDLDisplay(index); // TODO resolve resource manager.
	return nullptr;
}
unsigned int WindowManager::getNumDisplays() const noexcept { /*SDLDisplay::getNumDisplays();*/
	return 0;
}
Display *WindowManager::getAssociatedDisplay(Ref<Window> &window) { return window->getCurrentDisplay(); }

Window *WindowManager::createWindow(const std::string &title) { return nullptr; }

void WindowManager::onCreation(Ref<WindowManager> &instance) {
	// TODO add event for new window added for handle internal resources and etc.
}

WindowManager::WindowManager() {}