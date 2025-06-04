/*
 *	FragCore - Fragment Core - Engine Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FRAGCORE_BULLET_PHYSIC_INTERFACE_H_
#define _FRAGCORE_BULLET_PHYSIC_INTERFACE_H_ 1
#include "../PhysicDesc.h"
#include "../PhysicInterface.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletSoftBody/btSoftBody.h"
#include <Core/IConfig.h>
#include <Core/Module.h>
#include <Core/SmartReference.h>

namespace fragcore {

	/**
	 *	Physic interface.
	 *
	 */
	class FVDECLSPEC BulletPhysicInterface : public PhysicInterface { // TODO add a base object for all plugin based.
	  public:
		BulletPhysicInterface(IConfig *config);
		~BulletPhysicInterface() override;

		void onInitialization() override;
		void onDestruction() override;

		/**
		 * Start simulate.
		 * @param timeStep
		 * @param maxSubSteps
		 * @param fixedTimeStep
		 */
		void simulate(const float timeStep, const int maxSubSteps = 1,
					  const float fixedTimeStep = 1.0f / 60.0f) override;

		/**
		 * synchronize.
		 */
		void sync() override;

		/**
		 * Set gravity.
		 */
		void setGravity(const Vector3 &gravity) override;

		/**
		 *	Get gravity.
		 *	@return gravity vector.
		 */
		Vector3 getGravity() const override;

		/**
		 * Add rigidbody to scene.
		 * @param body
		 */
		void addRigidBody(RigidBody *body) override;

		/**
		 *
		 * @param body
		 */
		void removeRigidBody(RigidBody *body) override;

		/**
		 * Add constraint to scene.
		 * @param constraints
		 */
		void addConstraints(Constraints *constraints) override;

		/**
		 *
		 * @param constraints
		 */
		void removeConstraints(Constraints *constraints) override;

		/**
		 *	Create collision object.
		 *
		 *	@return
		 */
		Collision *createCollision(const CollisionDesc *desc) override;

		/**
		 *
		 */
		void deleteCollision(Collision *collision) override;

		/**
		 *	Create constraint object.
		 *
		 *	@return
		 */
		Constraints *createConstraints(const ConstraintsDesc *desc) override;

		/**
		 *
		 */
		void deleteConstraints(Constraints *constraints) override;

		/**
		 * Create rigidbody.
		 * @param desc
		 * @return
		 */
		RigidBody *createRigibody(const RigidBodyDesc *desc) override;

		/**
		 * Delete rigidbody.
		 * @param rigidbody
		 */
		void deleteRigibody(RigidBody *rigidbody) override;

		/**
		 *
		 * @param softbodyDesc
		 * @return
		 */
		void *createSoftBody(SoftbodyDesc *softbodyDesc) override;
		void deleteSoftBody(void *softbody) override;

		void *createCloth(ClothDesc *clothDesc) override;
		void deleteCloth(void *cloth) override;

		/**
		 *
		 * @param desc
		 * @return
		 */
		void *createTerrain(const TerrainDesc *desc) override;
		void deleteTerrain(void *terrain) override;

		/**
		 *
		 * @param desc
		 * @return
		 */
		CharacterController *createCharacterController(CharacterControllerDesc *desc) override;

		/**
		 *
		 * @param characterController
		 */
		void deleteCharacterController(CharacterController *characterController) override;

		/**
		 *
		 * @param controller
		 */
		void addCharacterController(CharacterController *controller) override;

		/**
		 *
		 * @param controller
		 */
		void removeCharacterController(CharacterController *controller) override;

		/**
		 * Perform ray test.
		 * @param ray
		 * @param hit
		 * @return
		 */
		bool rayTest(const Ray &ray, RayCastHit *hit) override;

		/**
		 *
		 * @param ray
		 * @param hit
		 * @return
		 */
		bool raySphereTest(const Ray &ray, RayCastHit *hit) override;

		/**
		 *
		 *	@return
		 */
		void *getState(unsigned int *len) override;

		/**
		 *
		 * @param renderer
		 */
		// virtual void setDebugRenderer(Ref<IRenderer> &renderer);

		/**
		 *	Get version of the interface.
		 *	@return non-null terminated string.
		 */
		const char *getVersion() const override;

	  protected: /*	*/
				 /**
				  *	Initialize Allocation.
				  *	Can be used for resources to allocate.
				  */
				 // virtual void initAllocate(InitAllocateTableInfo* table);

	  protected: /*	*/
		BulletPhysicInterface(const BulletPhysicInterface &other);

	  private:
		void *pdata;
		btBroadphaseInterface *broadphase;
		btDefaultCollisionConfiguration *collisionConfiguration;
		btCollisionDispatcher *dispatcher;
		btSequentialImpulseConstraintSolver *solver;
		btDiscreteDynamicsWorld *dynamicsWorld;
		btSoftBodyWorldInfo softBodyWorldInfo;
		btIDebugDraw *drawDebugger;
		//	std::vector<btKinematicCharacterController*> characterControllers;
		bool debug;
	};

	/**
	 *	Create internal physic interface instance from external
	 *	library.
	 *
	 *	@return non-null physic interface object.
	 */
	// extern "C" PhysicInterface *createInternalPhysicInterface(IConfig *config);
} // namespace fragcore

#endif
