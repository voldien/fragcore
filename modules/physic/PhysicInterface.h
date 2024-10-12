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
#ifndef _FRAG_CORE_PHYSIC_INTERFACE_H_
#define _FRAG_CORE_PHYSIC_INTERFACE_H_ 1
#include "PhysicDesc.h"
#include "Prerequisites.h"
#include "SoftBody.h"
#include <Core/IConfig.h>
#include <Core/Module.h>
#include <Core/Ref.h>
#include <Core/SmartReference.h>

namespace fragcore {

	/**
	 *	Physic interface.
	 *
	 */
	class FVDECLSPEC PhysicInterface : public Module {
		friend class PhysicFactory;

	  public:
		PhysicInterface() = default;
		PhysicInterface(const PhysicInterface &other) = delete;
		~PhysicInterface() override = default;

		void OnInitialization() override{};
		void OnDestruction() override{};

		/**
		 * Start simulate.
		 * @param timeStep
		 * @param maxSubSteps
		 * @param fixedTimeStep
		 */
		virtual void simulate(const float timeStep, const int maxSubSteps = 1,
							  const float fixedTimeStep = 1.0f / 60.0f) = 0;

		/**
		 * synchronize.
		 */
		virtual void sync() = 0;

		/**
		 * Set gravity.
		 */
		virtual void setGravity(const Vector3 &gravity) = 0;

		/**
		 *	Get gravity.
		 *	@return gravity vector.
		 */
		virtual Vector3 getGravity() const = 0;

		/**
		 * Add rigidbody to scene.
		 * @param body
		 */
		virtual void addRigidBody(RigidBody *body) = 0;

		/**
		 *
		 * @param body
		 */
		virtual void removeRigidBody(RigidBody *body) = 0;

		/**
		 * Add constraint to scene.
		 * @param constraints
		 */
		virtual void addConstraints(Constraints *constraints) = 0;

		/**
		 *
		 * @param constraints
		 */
		virtual void removeConstraints(Constraints *constraints) = 0;

		/**
		 *	Create collision object.
		 *
		 *	@return
		 */
		virtual Collision *createCollision(const CollisionDesc *desc) = 0;

		/**
		 *
		 */
		virtual void deleteCollision(Collision *collision) = 0;

		/**
		 *	Create constraint object.
		 *
		 *	@return
		 */
		virtual Constraints *createConstraints(const ConstraintsDesc *desc) = 0;

		/**
		 *
		 */
		virtual void deleteConstraints(Constraints *constraints) = 0;

		/**
		 * Create rigidbody.
		 * @param desc
		 * @return
		 */
		virtual RigidBody *createRigibody(const RigidBodyDesc *desc) = 0;

		/**
		 * Delete rigidbody.
		 * @param rigidbody
		 */
		virtual void deleteRigibody(RigidBody *rigidbody) = 0;

		/**
		 *
		 * @param softbodyDesc
		 * @return
		 */
		virtual void *createSoftBody(SoftbodyDesc *softbodyDesc) = 0;
		virtual void deleteSoftBody(void *softbody) = 0;

		virtual void *createCloth(ClothDesc *clothDesc) = 0;
		virtual void deleteCloth(void *cloth) = 0;

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual void *createTerrain(const TerrainDesc *desc) = 0;
		virtual void deleteTerrain(void *terrain) = 0;

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual CharacterController *createCharacterController(CharacterControllerDesc *desc) = 0;

		/**
		 *
		 * @param characterController
		 */
		virtual void deleteCharacterController(CharacterController *characterController) = 0;

		/**
		 *
		 * @param controller
		 */
		virtual void addCharacterController(CharacterController *controller) = 0;

		/**
		 *
		 * @param controller
		 */
		virtual void removeCharacterController(CharacterController *controller) = 0;

		/**
		 * Perform ray test.
		 * @param ray
		 * @param hit
		 * @return
		 */
		virtual bool rayTest(const Ray &ray, RayCastHit *hit) = 0;

		/**
		 *
		 * @param ray
		 * @param hit
		 * @return
		 */
		virtual bool raySphereTest(const Ray &ray, RayCastHit *hit) = 0;

		/**
		 *
		 *	@return
		 */
		virtual void *getState(unsigned int *len) = 0;

		/**
		 *
		 * @param renderer
		 */
		// virtual void setDebugRenderer(Ref<IRenderer> &renderer);

		/**
		 *	Get version of the interface.
		 *	@return non-null terminated string.
		 */
		virtual const char *getVersion() const = 0;

		// virtual intptr_t getNativePtr() const;

	  protected: /*	*/
		void *pdata;
	};

	/**
	 *	Create internal physic interface instance from external
	 *	library.
	 *
	 *	@return non-null physic interface object.
	 */
	extern "C" PhysicInterface *createInternalPhysicInterface(IConfig *options);
} // namespace fragcore

#endif
