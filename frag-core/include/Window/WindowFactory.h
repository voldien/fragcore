
#ifndef _FRAG_CORE_WINDOW_FACTORY_H_
#define _FRAG_CORE_WINDOW_FACTORY_H_ 1
#include "../Core/Module.h"
#include "WindowManager.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC WindowFactory : public Module {
	  public:
		WindowManager *createWindowManager(void);
	};
} // namespace fragcore

#endif