#ifndef _INTERNAL_OBJECT_TYPE_H_
#define _INTERNAL_OBJECT_TYPE_H_ 1

#include<bullet/btBulletCollisionCommon.h>
#include<bullet/btBulletDynamicsCommon.h>
#include<bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include<bullet/BulletDynamics/Character/btKinematicCharacterController.h>

#include<bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include<bullet/BulletSoftBody/btSoftBodyHelpers.h>
#include<bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>

namespace fragcore {
	typedef struct engine_physic_core_t{
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		btSoftRigidDynamicsWorld* dynamicsWorld;
		btSoftBodyWorldInfo softBodyWorldInfo;
		btIDebugDraw* drawDebugger;
//	std::vector<btKinematicCharacterController*> characterControllers;
		bool debug;
	}PhysicCore;
}

#endif
