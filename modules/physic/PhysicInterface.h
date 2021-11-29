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
#ifndef _FRAG_CORE_PHYSIC_INTERFACE_H_
#define _FRAG_CORE_PHYSIC_INTERFACE_H_ 1
#include "PhysicDesc.h"
#include "Prerequisites.h"
#include <Core/IConfig.h>
#include <Core/Module.h>
#include <Core/Ref.h>
#include <Core/SmartReference.h>

namespace fragcore {

	/**
	 *	Physic interface.
	 *
	 */
	class FVDECLSPEC PhysicInterface : public Module { // TODO add a base object for all plugin based.
		friend class PhysicFactory;

	  public:
		PhysicInterface(IConfig *config);
		virtual ~PhysicInterface();

		virtual void OnInitialization();
		virtual void OnDestruction();

		/**
		 * Start simulate.
		 * @param timeStep
		 * @param maxSubSteps
		 * @param fixedTimeStep
		 */
		virtual void simulate(float timeStep, int maxSubSteps = 1, float fixedTimeStep = 1.0f / 60.0f);

		/**
		 * synchronize.
		 */
		virtual void sync();

		/**
		 * Set gravity.
		 */
		virtual void setGravity(const Vector3 &gravity);

		/**
		 *	Get gravity.
		 *	@return gravity vector.
		 */
		virtual Vector3 getGravity() const;

		/**
		 * Add rigidbody to scene.
		 * @param body
		 */
		virtual void addRigidBody(RigidBody *body);

		/**
		 *
		 * @param body
		 */
		virtual void removeRigidBody(RigidBody *body);

		/**
		 * Add constraint to scene.
		 * @param constraints
		 */
		virtual void addConstraints(Constraints *constraints);

		/**
		 *
		 * @param constraints
		 */
		virtual void removeConstraints(Constraints *constraints);

		/**
		 *	Create collision object.
		 *
		 *	@return
		 */
		virtual Collision *createCollision(const CollisionDesc *desc);

		/**
		 *
		 */
		virtual void deleteCollision(Collision *collision);

		/**
		 *	Create constraint object.
		 *
		 *	@return
		 */
		virtual Constraints *createConstraints(const ConstraintsDesc *desc);

		/**
		 *
		 */
		virtual void deleteConstraints(Constraints *constraints);

		/**
		 * Create rigidbody.
		 * @param desc
		 * @return
		 */
		virtual RigidBody *createRigibody(const RigidBodyDesc *desc);

		/**
		 * Delete rigidbody.
		 * @param rigidbody
		 */
		virtual void deleteRigibody(RigidBody *rigidbody);

		/**
		 *
		 * @param softbodyDesc
		 * @return
		 */
		virtual void *createSoftBody(SoftbodyDesc *softbodyDesc);
		virtual void deleteSoftBody(void *softbody);

		virtual void *createCloth(ClothDesc *clothDesc);
		virtual void deleteCloth(void *cloth);

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual void *createTerrain(const TerrainDesc *desc);
		virtual void deleteTerrain(void *terrain);

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual CharacterController *createCharacterController(CharacterControllerDesc *desc);

		/**
		 *
		 * @param characterController
		 */
		virtual void deleteCharacterController(CharacterController *characterController);

		/**
		 *
		 * @param controller
		 */
		virtual void addCharacterController(CharacterController *controller);

		/**
		 *
		 * @param controller
		 */
		virtual void removeCharacterController(CharacterController *controller);

		/**
		 * Perform ray test.
		 * @param ray
		 * @param hit
		 * @return
		 */
		virtual bool rayTest(const Ray &ray, RayCastHit *hit);

		/**
		 *
		 * @param ray
		 * @param hit
		 * @return
		 */
		virtual bool raySphereTest(const Ray &ray, RayCastHit *hit);

		/**
		 *
		 *	@return
		 */
		virtual void *getState(unsigned int *len);

		/**
		 *
		 * @param renderer
		 */
		// virtual void setDebugRenderer(Ref<IRenderer> &renderer);

		/**
		 *	Get version of the interface.
		 *	@return non-null terminated string.
		 */
		virtual const char *getVersion() const;

		// virtual intptr_t getNativePtr() const;

	  protected: /*	*/
				 /**
				  *	Initialize Allocation.
				  *	Can be used for resources to allocate.
				  */
				 // virtual void initAllocate(InitAllocateTableInfo* table);

	  protected: /*	*/
		PhysicInterface(const PhysicInterface &other);
		void *pdata;
	};

	/**
	 *	Create internal physic interface instance from external
	 *	library.
	 *
	 *	@return non-null physic interface object.
	 */
	extern "C" PhysicInterface *createInternalPhysicInterface(IConfig *config);
} // namespace fragcore

#endif
