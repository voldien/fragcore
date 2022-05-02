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
#ifndef _FRAG_CORE_BULLET_PHYSIC_INTERFACE_H_
#define _FRAG_CORE_BULLET_PHYSIC_INTERFACE_H_ 1
#include "../PhysicDesc.h"
#include "../PhysicInterface.h"
#include "internal_object_type.h"
#include <Core/IConfig.h>
#include <Core/Module.h>
#include <Core/Ref.h>
#include <Core/SmartReference.h>

namespace fragcore {

	/**
	 *	Physic interface.
	 *
	 */
	class FVDECLSPEC BulletPhysicInterface : public PhysicInterface { // TODO add a base object for all plugin based.
	  public:
		BulletPhysicInterface(IConfig *config);
		virtual ~BulletPhysicInterface();

		virtual void OnInitialization();
		virtual void OnDestruction();

		/**
		 * Start simulate.
		 * @param timeStep
		 * @param maxSubSteps
		 * @param fixedTimeStep
		 */
		virtual void simulate(float timeStep, int maxSubSteps = 1, float fixedTimeStep = 1.0f / 60.0f) override;

		/**
		 * synchronize.
		 */
		virtual void sync() override;

		/**
		 * Set gravity.
		 */
		virtual void setGravity(const Vector3 &gravity) override;

		/**
		 *	Get gravity.
		 *	@return gravity vector.
		 */
		virtual Vector3 getGravity() const override;

		/**
		 * Add rigidbody to scene.
		 * @param body
		 */
		virtual void addRigidBody(RigidBody *body) override;

		/**
		 *
		 * @param body
		 */
		virtual void removeRigidBody(RigidBody *body) override;

		/**
		 * Add constraint to scene.
		 * @param constraints
		 */
		virtual void addConstraints(Constraints *constraints) override;

		/**
		 *
		 * @param constraints
		 */
		virtual void removeConstraints(Constraints *constraints) override;

		/**
		 *	Create collision object.
		 *
		 *	@return
		 */
		virtual Collision *createCollision(const CollisionDesc *desc) override;

		/**
		 *
		 */
		virtual void deleteCollision(Collision *collision) override;

		/**
		 *	Create constraint object.
		 *
		 *	@return
		 */
		virtual Constraints *createConstraints(const ConstraintsDesc *desc) override;

		/**
		 *
		 */
		virtual void deleteConstraints(Constraints *constraints) override;

		/**
		 * Create rigidbody.
		 * @param desc
		 * @return
		 */
		virtual RigidBody *createRigibody(const RigidBodyDesc *desc) override;

		/**
		 * Delete rigidbody.
		 * @param rigidbody
		 */
		virtual void deleteRigibody(RigidBody *rigidbody) override;

		/**
		 *
		 * @param softbodyDesc
		 * @return
		 */
		virtual void *createSoftBody(SoftbodyDesc *softbodyDesc) override;
		virtual void deleteSoftBody(void *softbody) override;

		virtual void *createCloth(ClothDesc *clothDesc) override;
		virtual void deleteCloth(void *cloth) override;

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual void *createTerrain(const TerrainDesc *desc) override;
		virtual void deleteTerrain(void *terrain) override;

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual CharacterController *createCharacterController(CharacterControllerDesc *desc) override;

		/**
		 *
		 * @param characterController
		 */
		virtual void deleteCharacterController(CharacterController *characterController) override;

		/**
		 *
		 * @param controller
		 */
		virtual void addCharacterController(CharacterController *controller) override;

		/**
		 *
		 * @param controller
		 */
		virtual void removeCharacterController(CharacterController *controller) override;

		/**
		 * Perform ray test.
		 * @param ray
		 * @param hit
		 * @return
		 */
		virtual bool rayTest(const Ray &ray, RayCastHit *hit) override;

		/**
		 *
		 * @param ray
		 * @param hit
		 * @return
		 */
		virtual bool raySphereTest(const Ray &ray, RayCastHit *hit) override;

		/**
		 *
		 *	@return
		 */
		virtual void *getState(unsigned int *len) override;

		/**
		 *
		 * @param renderer
		 */
		// virtual void setDebugRenderer(Ref<IRenderer> &renderer);

		/**
		 *	Get version of the interface.
		 *	@return non-null terminated string.
		 */
		virtual const char *getVersion() const override;

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
		btSoftRigidDynamicsWorld *dynamicsWorld;
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
