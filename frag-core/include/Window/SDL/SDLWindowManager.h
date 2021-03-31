
#ifndef _FRAG_CORE_WINDOW_MANAGER_H_
#define _FRAG_CORE_WINDOW_MANAGER_H_ 1
#include "../WindowManager.h"

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC SDLWindowManager : public WindowManager {
	  public:
		Display *getDisplay(unsigned int index) noexcept(true);
		unsigned int getNumDisplays(void) const noexcept;
		Display *getAssociatedDisplay(Ref<Window> &window);
		Window *createWindow(const std::string &title);
		Display *primaryDisplay(void) const noexcept;
		// Window* createWindow()
		/*	TODO get the event.	*/

		// protected:
		void onCreation(Ref<WindowManager> &instance) override;
		WindowManager(void);
	};
} // namespace fragcore

#endif
