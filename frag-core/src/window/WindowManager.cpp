#include "Window/WindowManager.h"

/*	TODO relocate to be part of the platform specific.	*/
#include"Window/SDL/SDLDisplay.h"
//#include"Window/SDL/SDLRendererWindow.h"
#include "Window/SDL/SDLWindow.h"

using namespace fragcore;

Display* WindowManager::getDisplay(int index){
    return new SDLDisplay(index);	//TODO resolve resource manager.
}
int WindowManager::getNumDisplays(void) { 
	SDLDisplay::getNumDisplays(); 
}
Display* WindowManager::getAssociatedDisplay(Ref<Window>& window) {
    window->getCurrentDisplay();
}

Window* WindowManager::createWindow(const std::string& title) { 
	return NULL; 
}

void WindowManager::onCreation(Ref<WindowManager>& instance) {
	//TODO add event for new window added for handle internal resources and etc.

}

WindowManager::WindowManager(void){

}