
#ifndef _FRAG_CORE_WINDOW_MANAGER_H_
#define _FRAG_CORE_WINDOW_MANAGER_H_ 1
#include "../Core/Ref.h"
#include "../Core/Singleton.h"
#include "Display.h"
#include "Window.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC WindowManager : public Singleton<WindowManager> {
	  public:

	  	/**
	  	 * @brief Get the Display object
	  	 *
	  	 * @param index
	  	 * @return Display*
	  	 */
		Display *getDisplay(unsigned int index) noexcept(true);

		/**
		 * @brief Get the Num Displays object
		 *
		 * @return unsigned int
		 */
		unsigned int getNumDisplays(void) const noexcept;

		/**
		 * @brief Get the Associated Display object
		 *
		 * @param window
		 * @return Display*
		 */
		Display *getAssociatedDisplay(Ref<Window> &window);

		/**
		 * @brief Create a Window object
		 *
		 * @param title
		 * @return Window*
		 */
		Window *createWindow(const std::string &title);

		/**
		 * @brief
		 *
		 * @return Display*
		 */
		Display *primaryDisplay(void) const noexcept;
		// Window* createWindow()
		/*	TODO get the event.	*/

		// protected:
		void onCreation(Ref<WindowManager> &instance) override;
		WindowManager(void);
	};
} // namespace fragcore

#endif
