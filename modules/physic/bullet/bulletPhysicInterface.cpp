#include "bulletPhysicInterface.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcherMt.h"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolverMt.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "Core/SystemInfo.h"
#include "LinearMath/btThreads.h"
#include "LinearMath/btVector3.h"
#include "RigidBody.h"
#include "bulletCollision.h"
#include "bulletRigidBody.h"
#include <FragCore.h>
#include <thread>
using namespace fragcore;

class FragBulletTaskSchedulear : public btITaskScheduler {
  public:
	class CBody : public btIParallelForBody {
	  public:
		CBody(const btIParallelForBody *pBody) : m_pBody(pBody) {}

		void forLoop(int iStart, int iEnd) const override { m_pBody->forLoop(iStart, iEnd); };

	  protected:
		const btIParallelForBody *m_pBody;
	};

  public:
	FragBulletTaskSchedulear() : btITaskScheduler("fragcore - threading") {}

	int getMaxNumThreads() const override { return std::thread::hardware_concurrency(); }
	int getNumThreads() const override { return 64; }
	void setNumThreads(int numThreads) override {}
	void parallelFor(int iBegin, int iEnd, int grainSize, const btIParallelForBody &body) override {
		const int RangeCount = iEnd - iBegin;

		if (iBegin >= iEnd) {
			return;
		}
		if (RangeCount < 1) {
			return;
		}

		/*	*/
		if (RangeCount <= getMaxNumThreads()) {
			// TODO: distribute
			body.forLoop(iBegin, iEnd);
		} else {
			body.forLoop(iBegin, iEnd);
		}
	}
	btScalar parallelSum(int iBegin, int iEnd, int grainSize, const btIParallelSumBody &body) override {
		return body.sumLoop(iBegin, iEnd);
	}
};

BulletPhysicInterface::BulletPhysicInterface(IConfig *config) : broadphase(new btDbvtBroadphase()) {
	this->setName("BulletPhysic");

	/*	*/
	btGhostPairCallback *m_pGHostPairCallback = new btGhostPairCallback();

	// this->broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(m_pGHostPairCallback);
	assert(this->broadphase);

	bool multi_threading = true;

	/*	*/
	this->collisionConfiguration = new btDefaultCollisionConfiguration();

	btITaskScheduler *scheduler = btGetOpenMPTaskScheduler();
	if (scheduler) {
		scheduler->setNumThreads(Math::max<size_t>(1, SystemInfo::getCPUCoreCount() / 3));
		btSetTaskScheduler(scheduler);
	} else {
		multi_threading = false;
	}

	if (multi_threading) {

		FragBulletTaskSchedulear *taskScheduler = new FragBulletTaskSchedulear();

		this->dispatcher = new btCollisionDispatcherMt(this->collisionConfiguration);
		this->solver = new btSequentialImpulseConstraintSolverMt();

		int maxThreadCount = 1;
		btConstraintSolverPoolMt *pSolverPool = new btConstraintSolverPoolMt(maxThreadCount);

		this->dynamicsWorld = new btDiscreteDynamicsWorldMt(this->dispatcher, this->broadphase, pSolverPool,
															this->solver, this->collisionConfiguration);

	} else {

		this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);

		/*	*/
		this->solver = new btSequentialImpulseConstraintSolver();

		/*	*/
		this->dynamicsWorld = new btSoftRigidDynamicsWorld(this->dispatcher, this->broadphase, this->solver,
														   this->collisionConfiguration);
	}

	/*	Default gravity.	*/
	this->setGravity(Vector3(0.0f, -9.82f, 0.0f));

	this->dynamicsWorld->getSolverInfo().m_numIterations = 10;
	this->dynamicsWorld->setForceUpdateAllAabbs(false);

	/*  Set softbody world info.    */
	this->softBodyWorldInfo.m_broadphase = this->broadphase;
	this->softBodyWorldInfo.m_dispatcher = this->dispatcher;
	this->softBodyWorldInfo.m_gravity = this->dynamicsWorld->getGravity();
	this->softBodyWorldInfo.m_sparsesdf.Initialize();
}

BulletPhysicInterface::BulletPhysicInterface(const BulletPhysicInterface &other) {}

BulletPhysicInterface::~BulletPhysicInterface() {

	/*	Clean up.	*/
	delete this->dynamicsWorld;
	delete this->solver;
	delete this->dispatcher;
	delete this->collisionConfiguration;
	delete this->broadphase;

	/**/
}

void BulletPhysicInterface::onInitialization() {}
void BulletPhysicInterface::onDestruction() {}

void BulletPhysicInterface::simulate(const float timeStep, const int maxSubSteps, const float fixedTimeStep) {

	this->dynamicsWorld->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);

	/*	*/
	if (this->debug) {
		this->dynamicsWorld->debugDrawWorld();
	}
}

void BulletPhysicInterface::sync() { this->dynamicsWorld->getSynchronizeAllMotionStates(); }

void BulletPhysicInterface::setGravity(const Vector3 &gravity) {

	btVector3 grav = btVector3(gravity.x(), gravity.y(), gravity.z());
	this->dynamicsWorld->setGravity(*(btVector3 *)&gravity);
}

Vector3 BulletPhysicInterface::getGravity() const {

	btVector3 gr = this->dynamicsWorld->getGravity();
	return Vector3(gr.x(), gr.y(), gr.z());
}

void BulletPhysicInterface::addRigidBody(RigidBody *body) {

	BulletRigidBody *bullet_rigidbody = dynamic_cast<BulletRigidBody *>(body);
	btRigidBody *rigid = static_cast<btRigidBody *>(bullet_rigidbody->getObject());
	/*	*/
	this->dynamicsWorld->addRigidBody(rigid);
}
void BulletPhysicInterface::removeRigidBody(RigidBody *body) {

	btRigidBody *rigid = (btRigidBody *)body->getObject();

	/*	*/
	// rigid = (btRigidBody*)getPointerByIndex(body);

	/*	*/
	this->dynamicsWorld->removeRigidBody(rigid);
}

void BulletPhysicInterface::addConstraints(Constraints *constraints) {

	//	btTypedConstraint* typedConstraint = (btTypedConstraint*)constraints->getMappedObject();

	//	this->dynamicsWorld->addConstraint(typedConstraint);
}
void BulletPhysicInterface::removeConstraints(Constraints *constraints) {

	//	btTypedConstraint* typedConstraint = (btTypedConstraint*)constraints->getMappedObject();

	//	this->dynamicsWorld->removeConstraint(typedConstraint);
}

Collision *BulletPhysicInterface::createCollision(const CollisionDesc *desc) {
	btCollisionShape *shape = nullptr;

	switch (desc->Primitive) {
	case CollisionDesc::Plane: {
		btVector3 normal =
			btVector3(desc->planeshape.normal[0], desc->planeshape.normal[1], desc->planeshape.normal[2]);
		shape = new btStaticPlaneShape(normal, desc->planeshape.d);
	} break;
	case CollisionDesc::Box: {
		btVector3 size = btVector3(desc->boxshape.boxsize[0], desc->boxshape.boxsize[1], desc->boxshape.boxsize[2]);
		shape = new btBoxShape(size);
	} break;
	case CollisionDesc::Sphere:
		shape = new btSphereShape(desc->sphereshape.radius);
		break;
	case CollisionDesc::Capsule:
		shape = new btCapsuleShape(desc->capsuleshape.radius, desc->capsuleshape.height);
		break;
	case CollisionDesc::Mesh:
		// btBvhTriangleMeshShape
		//			btTriangleMesh
		//			btTriangleMesh
		//			btBvhTriangleMeshShape
		// shape = nullptr;//btTriangleMeshShape
		break;
	case collision_desc_t::Terrain:
		break;
	}

	/*	*/
	BulletCollision *collision = new BulletCollision(shape);
	return collision;
}
void BulletPhysicInterface::deleteCollision(Collision *collision) {

	btCollisionShape *shape = nullptr;

	// shape = (btCollisionShape*)getPointerByIndex(collision);

	delete shape;
	// returnRefObj(collision);
}

Constraints *BulletPhysicInterface::createConstraints(const ConstraintsDesc *desc) {

	Constraints *constrains = nullptr;
	btTypedConstraint *constr = nullptr;
	btRigidBody *b1 = nullptr;
	btRigidBody *b2 = nullptr;

	//	if(desc->body1)
	//		b1 = (btRigidBody*)desc->body1->getMappedObject();
	//	if(desc->body2)
	//		b2 = (btRigidBody*)desc->body2->getMappedObject();

	const btTransform frameA = btTransform();
	const btTransform frameB = btTransform();

	switch (desc->type) {
	case ConstraintsDesc::eHinge:
		constr = new btHingeConstraint(*b1, *b2, frameA, frameB);
		break;
	case ConstraintsDesc::ePoint:
		// constr = new btPoint2PointConstraint(*b1,*b2);
		break;
	case ConstraintsDesc::eSpring: {
		btGeneric6DofSpringConstraint *spring = new btGeneric6DofSpringConstraint(*b1, *b2, frameA, frameB, true);
		spring->setStiffness(0, desc->spring.stiffness);
		constr = spring;
	} break;
	case ConstraintsDesc::eSlider:
		break;
	default:
		assert(0);
		break;
	}

	constr->setBreakingImpulseThreshold(desc->breakforce);

	// return obtainMappedObject<Constraints>(constr);
	return nullptr;
}

void BulletPhysicInterface::deleteConstraints(Constraints *constraints) {
	//	btTypedConstraint* typedConstraint = (btTypedConstraint*)constraints->getMappedObject();

	//	delete typedConstraint;
	// this->returnRefObj(constraints);
}

RigidBody *BulletPhysicInterface::createRigibody(const RigidBodyDesc *desc) {

	BulletRigidBody *body = nullptr; /*	*/

	assert(desc && desc->collision);

	if (desc == nullptr) {
		throw RuntimeException("Requires a non-null descriptor");
	}

	if (desc->collision == nullptr) {
		throw RuntimeException("Requires a non-null collision reference");
	}

	/*	Get collsion object.	*/
	const BulletCollision *bulletCollision = dynamic_cast<BulletCollision *>(desc->collision);
	btCollisionShape *shape = reinterpret_cast<btCollisionShape *>(bulletCollision->shape);

	/*	Get world space variables.  */
	btQuaternion rotation = btQuaternion(desc->quat.x(), desc->quat.y(), desc->quat.z(), desc->quat.w());
	btVector3 position = btVector3(desc->position.x(), desc->position.y(), desc->position.z());
	/*  */
	btTransform initTransform(rotation, position);
	btDefaultMotionState *fallMotionState = new btDefaultMotionState(initTransform);
	btVector3 fallInertia(0, 0, 0);

	/*	*/
	btVector3 scale = btVector3(1, 1, 1);
	shape->setLocalScaling(scale);
	if (desc->mass > 0) {
		shape->calculateLocalInertia(desc->mass, fallInertia);
	}

	/*  Create rigidbody.   */
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(desc->mass, fallMotionState, shape, fallInertia);
	btRigidBody *rigidBody = new btRigidBody(fallRigidBodyCI);

	/*	*/
	if (desc->isKinematic) {
		rigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
	} else {
		rigidBody->forceActivationState(DISABLE_DEACTIVATION);
	}

	//
	// groundRigidBody->setCcdSweptSphereRadius(0);
	// groundRigidBody->setContactStiffnessAndDamping(300, 10);
	rigidBody->setUserIndex(-1);

	/*	Create rigidbody.   */
	body = new BulletRigidBody(rigidBody);

	/*  Final properties.  */
	body->setAngularDrag(desc->angularDrag);
	body->setDrag(desc->drag);
	body->useGravity(desc->useGravity);

	return body;
}

void BulletPhysicInterface::deleteRigibody(RigidBody *rigidbody) {

	btRigidBody *body = nullptr;
	/*  */
	// body = (btRigidBody*)getPointerByIndex(rigidbody);
	assert(body);
	/*	*/
	delete body;
}

void *BulletPhysicInterface::createSoftBody(SoftbodyDesc *softbodyDesc) {

	// btSoftBodyFloatData

	btSoftBody *softBody = btSoftBodyHelpers::CreateFromTriMesh(this->softBodyWorldInfo, softbodyDesc->vertices,
																softbodyDesc->indices, softbodyDesc->nVertices / 3);
	return nullptr;
}

void BulletPhysicInterface::deleteSoftBody(void *softbody) {}

void *BulletPhysicInterface::createCloth(ClothDesc *clothDesc) {

	//
	//	btSoftBody* cloth = btSoftBodyHelpers::CreatePatch(this->dynamicsWorld,
	//	                                                   btVector3(-s / 2, s + 1, 0),
	//	                                                   btVector3(+s / 2, s + 1, 0),
	//	                                                   btVector3(-s / 2, s + 1, +s),
	//	                                                   btVector3(+s / 2, s + 1, +s),
	//	                                                   numX, numY,
	//	                                                   fixed, true);
	return nullptr;
}
void BulletPhysicInterface::deleteCloth(void *cloth) {}

void *BulletPhysicInterface::createTerrain(const TerrainDesc *desc) {
	// btHeightfieldTerrainShape
	// btHeightfieldTerrainShape = new
	return nullptr;
}

void BulletPhysicInterface::deleteTerrain(void *terrain) {}

CharacterController *BulletPhysicInterface::createCharacterController(CharacterControllerDesc *desc) {

	// btPairCachingGhostObject *ghostObject = nullptr; // new btPairCachingGhostObject();
	//
	// btCapsuleShape *capsuleShape = new btCapsuleShape(2.0f, 1.0f);
	// btKinematicCharacterController *m_pTommyController =
	//	new btKinematicCharacterController(ghostObject, capsuleShape, 1);

	// m_pTommyController->updateAction(m_dynamicsWorld, timeSinceLast);

	/*	*/
	// return obtainMappedObject<CharacterController>(m_pTommyController);
	return nullptr;
}

void BulletPhysicInterface::deleteCharacterController(CharacterController *characterController) {}

void BulletPhysicInterface::addCharacterController(CharacterController *controller) {

	//	btKinematicCharacterController *characterController = (btKinematicCharacterController *)
	// controller->getMappedObject();

	//	this->dynamicsWorld->addAction(characterController);
}

void BulletPhysicInterface::removeCharacterController(CharacterController *controller) {

	//	btKinematicCharacterController *characterController = (btKinematicCharacterController *)
	// controller->getMappedObject();

	// this->dynamicsWorld->removeAction(characterController);
}

void *BulletPhysicInterface::getState(unsigned int *len) {

	btDefaultSerializer *serializer = new btDefaultSerializer();
	this->dynamicsWorld->serialize(serializer);

	if (len) {
		*len = serializer->getCurrentBufferSize();
	}

	return (void *)serializer->getBufferPointer();
}

bool BulletPhysicInterface::rayTest(const Ray &ray, RayCastHit *hit) {

	/*	*/
	btDiscreteDynamicsWorld *world = this->dynamicsWorld;

	struct AllRayResultCallback : public btCollisionWorld::RayResultCallback {
		AllRayResultCallback(const btVector3 &rayFromWorld, const btVector3 &rayToWorld)
			: m_rayFromWorld(rayFromWorld), m_rayToWorld(rayToWorld) {}

		btVector3 m_rayFromWorld; // used to calculate hitPointWorld from hitFraction
		btVector3 m_rayToWorld;

		btVector3 m_hitNormalWorld;
		btVector3 m_hitPointWorld;

		btScalar addSingleResult(btCollisionWorld::LocalRayResult &rayResult, bool normalInWorldSpace) override {

			// caller already does the filter on the m_closestHitFraction
			btAssert(rayResult.m_hitFraction <= m_closestHitFraction);

			m_closestHitFraction = rayResult.m_hitFraction;
			m_collisionObject = rayResult.m_collisionObject;

			if (normalInWorldSpace) {
				m_hitNormalWorld = rayResult.m_hitNormalLocal;
			} else {
				/// need to transform normal into worldspace
				m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis() * rayResult.m_hitNormalLocal;
			}
			m_hitPointWorld.setInterpolate3(m_rayFromWorld, m_rayToWorld, rayResult.m_hitFraction);

			return 1.0f;
		}
	};

	/*	TODO add support.	*/
	btVector3 from; //= ray.getOrigin();
	btVector3 dir;	// ray.getDirection();
	AllRayResultCallback result(from, dir);
	world->rayTest(from, dir, result);

	return result.hasHit();
}

bool BulletPhysicInterface::raySphereTest(const Ray &ray, RayCastHit *hit) {
	btDiscreteDynamicsWorld *world = this->dynamicsWorld;

	// world->convexSweepTest
	return false;
}

// void BulletPhysicInterface::setDebugRenderer(Ref <IRenderer>& renderer) {
// 	PhysicCore* physicore = (PhysicCore*)this->pdata;

// 	this->drawDebugger = new DebugDrawer(renderer);
// 	this->dynamicsWorld->setDebugDrawer(this->drawDebugger);
// }

const char *BulletPhysicInterface::getVersion() const { return FV_STR_VERSION(1, 0, 0); }

extern "C" PhysicInterface *createInternalPhysicInterface(IConfig *options) {
	return new BulletPhysicInterface(options);
}
