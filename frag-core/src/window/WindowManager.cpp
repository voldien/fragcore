#include "Window/WindowManager.h"

using namespace fragcore;

Display *WindowManager::getDisplay(unsigned int index) noexcept {
	// return new SDLDisplay(index); // TODO resolve resource manager.
}
unsigned int WindowManager::getNumDisplays() const noexcept { /*SDLDisplay::getNumDisplays();*/
}
Display *WindowManager::getAssociatedDisplay(Ref<Window> &window) { window->getCurrentDisplay(); }

Window *WindowManager::createWindow(const std::string &title) { return nullptr; }

void WindowManager::onCreation(Ref<WindowManager> &instance) {
	// TODO add event for new window added for handle internal resources and etc.
}

WindowManager::WindowManager() {}