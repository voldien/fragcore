#include "bulletPhysicInterface.h"
#include "debugDrawer.h"
#include "internal_object_type.h"
#include <FragCore.h>
using namespace fragcore;

// void BulletPhysicInterface::initAllocate(InitAllocateTableInfo* table){
//	table->interfacetype = ePhysic;
//	/*  */
//	table->initphyic.collisionsize = sizeof(btCollisionShape);
//	table->initphyic.constraintssize = sizeof(btUniversalConstraint);
//	table->initphyic.rigidbodysize = sizeof(btRigidBody);
//	table->initphyic.characterController = sizeof(btKinematicCharacterController);
//}

BulletPhysicInterface::BulletPhysicInterface(IConfig *config) {
	this->setName("BulletPhysic");

	/*	*/
	this->broadphase = new btDbvtBroadphase();
	assert(this->broadphase);

	/*	*/
	this->collisionConfiguration = new btDefaultCollisionConfiguration();
	this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);

	/*	*/
	this->solver = new btSequentialImpulseConstraintSolver;

	/*	*/
	this->dynamicsWorld =
		new btSoftRigidDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->collisionConfiguration);
	/*	*/
	this->setGravity(Vector3(0.0f, -9.82f, 0.0f));

	/*  Set softbody world info.    */
	this->softBodyWorldInfo.m_broadphase = this->broadphase;
	this->softBodyWorldInfo.m_dispatcher = this->dispatcher;
	this->softBodyWorldInfo.m_gravity = this->dynamicsWorld->getGravity();
	this->softBodyWorldInfo.m_sparsesdf.Initialize();
}

BulletPhysicInterface::BulletPhysicInterface(const BulletPhysicInterface &other) {
	//	*this = other;
}
BulletPhysicInterface::~BulletPhysicInterface() {

	/*	Clean up.	*/
	delete this->dynamicsWorld;
	delete this->solver;
	delete this->dispatcher;
	delete this->collisionConfiguration;
	delete this->broadphase;

	/**/
}

void BulletPhysicInterface::OnInitialization() {}
void BulletPhysicInterface::OnDestruction() {}

void BulletPhysicInterface::simulate(float timeStep, int maxSubSteps, float fixedTimeStep) {

	this->dynamicsWorld->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);
	if (this->debug) {
		this->dynamicsWorld->debugDrawWorld();
	}
}

void BulletPhysicInterface::sync() { this->dynamicsWorld->synchronizeMotionStates(); }

void BulletPhysicInterface::setGravity(const Vector3 &gravity) {

	btVector3 grav = btVector3(gravity.x(), gravity.y(), gravity.z());
	this->dynamicsWorld->setGravity(*(btVector3 *)&gravity);
}

Vector3 BulletPhysicInterface::getGravity() const {

	btVector3 gr = this->dynamicsWorld->getGravity();
	return Vector3(gr.x(), gr.y(), gr.z());
}

void BulletPhysicInterface::addRigidBody(RigidBody *body) {

	btRigidBody *rigid;

	/*	*/
	// rigid = (btRigidBody*)getPointerByIndex(body);

	/*	*/
	this->dynamicsWorld->addRigidBody(rigid);
}
void BulletPhysicInterface::removeRigidBody(RigidBody *body) {

	btRigidBody *rigid;

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
	btCollisionShape *shape;

	switch (desc->Primitive) {
	case CollisionDesc::ePlane: {
		btVector3 normal =
			btVector3(desc->planeshape.normal[0], desc->planeshape.normal[1], desc->planeshape.normal[2]);
		shape = new btStaticPlaneShape(normal, desc->planeshape.d);
	} break;
	case CollisionDesc::eBox: {
		btVector3 size = btVector3(desc->boxshape.boxsize[0], desc->boxshape.boxsize[1], desc->boxshape.boxsize[2]);
		shape = new btBoxShape(size);
	} break;
	case CollisionDesc::eSphere:
		shape = new btSphereShape(desc->sphereshape.radius);
		break;
	case CollisionDesc::eCapsule:
		shape = new btCapsuleShape(desc->capsuleshape.radius, desc->capsuleshape.height);
		break;
	case CollisionDesc::eMesh:
		// btBvhTriangleMeshShape
		//			btTriangleMesh
		//			btTriangleMesh
		//			btBvhTriangleMeshShape
		// shape = nullptr;//btTriangleMeshShape
		break;
	}

	/*	*/
	// return obtainMappedObject<Collision>(shape);
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
}
void BulletPhysicInterface::deleteConstraints(Constraints *constraints) {
	//	btTypedConstraint* typedConstraint = (btTypedConstraint*)constraints->getMappedObject();

	//	delete typedConstraint;
	// this->returnRefObj(constraints);
}

RigidBody *BulletPhysicInterface::createRigibody(const RigidBodyDesc *desc) {

	RigidBody *body = nullptr;		   /*	*/
	btCollisionShape *shape = nullptr; /*	*/

	// assert(desc->node && desc->collision);

	// if (desc->node == nullptr)
	// 	throw RuntimeException("Requires a non-null node reference");

	if (desc->collision == nullptr) {
		throw RuntimeException("Requires a non-null collision reference");
	}

	//	/*	Get collsion object.	*/
	//	//shape = (btCollisionShape *) getPointerByIndex(desc->collision);
	//
	//	/*	Get world space variables.  */
	////	Node *node = desc->node;
	////	Quaternion nodeRotation = node->getRotation();
	////	const Vector3 pos = node->getPosition();
	////	btQuaternion rotation = btQuaternion(nodeRotation.x(), nodeRotation.y(), nodeRotation.z(),
	/// nodeRotation.w()); /	btVector3 position = btVector3(pos.x(), /	                               pos.y(), /
	/// pos.z());
	////
	//	/*  */
	////	btDefaultMotionState *fallMotionState =
	////			new btDefaultMotionState(btTransform(rotation, position));
	////	btVector3 fallInertia(0, 0, 0);
	//
	//	/*	*/
	////	btVector3 scale = btVector3(node->getScale().x(), node->getScale().y(), node->getScale().z());
	////	shape->setLocalScaling(scale);
	//
	//	shape->calculateLocalInertia(desc->mass, fallInertia);
	//
	//	/*  Create rigidbody.   */
	//	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(desc->mass, fallMotionState, shape, fallInertia);
	//	btRigidBody *groundRigidBody = new btRigidBody(fallRigidBodyCI);
	//	if (desc->isKinematic)
	//		groundRigidBody->setCollisionFlags(groundRigidBody->getCollisionFlags() |
	// btCollisionObject::CF_KINEMATIC_OBJECT);
	//
	//	//groundRigidBody->setCcdSweptSphereRadius()
	//	//groundRigidBody->setContactStiffnessAndDamping(300, 10);
	//
	//	/*	Create rigidbody.   */
	//	//body = obtainMappedObject<RigidBody>(groundRigidBody);
	//
	//	/*  Bind rigidbody and node bidirectional.  */
	//	//body->attachNode(desc->node);
	//	//desc->node->getDynamicRef()->attachment = body;
	//
	//	/*  Final properties.  */
	//	//body->setAngularDrag(desc->angularDrag);
	//	//body->setDrag(desc->drag);
	//	//body->useGravity(desc->useGravity);
	//

	return body;
}

void BulletPhysicInterface::deleteRigibody(RigidBody *rigidbody) {

	btRigidBody *body = nullptr;

	/*  */
	// body = (btRigidBody*)getPointerByIndex(rigidbody);
	assert(body);

	/*	*/
	delete body;
	// returnRefObj(rigidbody);
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

	btPairCachingGhostObject *ghostObject = nullptr; // new btPairCachingGhostObject();

	btCapsuleShape *capsuleShape = new btCapsuleShape(2.0f, 1.0f);
	btKinematicCharacterController *m_pTommyController =
		new btKinematicCharacterController(ghostObject, capsuleShape, 1);

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

	btSoftRigidDynamicsWorld *world = this->dynamicsWorld;

	struct AllRayResultCallback : public btCollisionWorld::RayResultCallback {
		AllRayResultCallback(const btVector3 &rayFromWorld, const btVector3 &rayToWorld)
			: m_rayFromWorld(rayFromWorld), m_rayToWorld(rayToWorld) {}

		btVector3 m_rayFromWorld; // used to calculate hitPointWorld from hitFraction
		btVector3 m_rayToWorld;

		btVector3 m_hitNormalWorld;
		btVector3 m_hitPointWorld;

		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult &rayResult, bool normalInWorldSpace) {

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
	btSoftRigidDynamicsWorld *world = this->dynamicsWorld;

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
