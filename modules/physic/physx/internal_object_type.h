#ifndef _INTERNAL_OBJECT_TYPE_H_
#define _INTERNAL_OBJECT_TYPE_H_ 1
#include"../../Def.h"
#include <PxPhysicsAPI.h>
#include </PxPhysicsAPI.h>
#include <cooking/PxCooking.h>
#include <extensions/PxExtensionsAPI.h>
#include <foundation/PxFoundation.h>
#include <pxtask/PxCudaContextManager.h>
#include <vehicle/PxVehicleSDK.h>

namespace fragcore
{
	typedef struct engine_physic_physx_core_t
	{
		//PhysicUserControllerHitReport *gPhysiccontrollerreport;
		physx::PxPhysics *gPhysic;
		physx::PxFoundation *gFoundation;
		physx::PxCooking *gCooking;
		physx::PxScene *gPhysxScene;
		physx::PxProfileZoneManager *gProfileManager;
		physx::PxControllerManager *gControlManager;
		physx::PxCudaContextManager *gCudaContextManager;
		//	std::vector<btKinematicCharacterController*> characterControllers;
		bool debug;
	} PhysicPhysxCore;
}

#endif
