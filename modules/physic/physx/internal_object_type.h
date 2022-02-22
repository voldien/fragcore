/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _INTERNAL_OBJECT_TYPE_H_
#define _INTERNAL_OBJECT_TYPE_H_ 1
#include </PxPhysicsAPI.h>
#include <FragCore.h>
#include <PxPhysicsAPI.h>
#include <cooking/PxCooking.h>
#include <extensions/PxExtensionsAPI.h>
#include <foundation/PxFoundation.h>
#include <pxtask/PxCudaContextManager.h>
#include <vehicle/PxVehicleSDK.h>

namespace fragcore {
	typedef struct engine_physic_physx_core_t {
		// PhysicUserControllerHitReport *gPhysiccontrollerreport;
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
} // namespace fragcore

#endif
