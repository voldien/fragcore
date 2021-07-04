
#ifndef _FV_DYNAMIC_OBJECT_H_
#define _FV_DYNAMIC_OBJECT_H_ 1
#include"../Core/SmartReference.h"


/*  TODO rename.    */
/**
 * Contains all dynamic object references.
 */
namespace fragcore {

	class FVDECLSPEC DynamicObjects {
		friend class PhysicInterface;

	public:
		DynamicObjects(void) {
			this->attachment = nullptr;
		}

		/**
		 * Check if dynamic object is attached.
		 * @return
		 */
		bool isAttached(void) const {
			return attachment != nullptr;
		}

		/*  */
		//RigidBody *attachment;
	};
}

#endif