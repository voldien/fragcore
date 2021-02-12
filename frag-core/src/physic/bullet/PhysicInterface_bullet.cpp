#include<FragCore.h>
#include"physic/PhysicInterface.h"
#include"physic/PhysicDesc.h"
#include"physic/bullet/debugDrawer.h"
#include"physic/bullet/internal_object_type.h"
using namespace fragcore;


//void PhysicInterface::initAllocate(InitAllocateTableInfo* table){
//	table->interfacetype = ePhysic;
//	/*  */
//	table->initphyic.collisionsize = sizeof(btCollisionShape);
//	table->initphyic.constraintssize = sizeof(btUniversalConstraint);
//	table->initphyic.rigidbodysize = sizeof(btRigidBody);
//	table->initphyic.characterController = sizeof(btKinematicCharacterController);
//}

PhysicInterface::PhysicInterface(IConfig* config){
	this->setName("BulletPhysic");
	/*	*/
	PhysicCore* physicore = new PhysicCore();
	this->pdata = physicore;
	assert(physicore);


	/*	*/
	physicore->broadphase = new btDbvtBroadphase();
	assert(physicore->broadphase);

	/*	*/
	physicore->collisionConfiguration = new btDefaultCollisionConfiguration();
	physicore->dispatcher = new btCollisionDispatcher(physicore->collisionConfiguration);

	/*	*/
	physicore->solver = new btSequentialImpulseConstraintSolver;

	/*	*/
	physicore->dynamicsWorld = new btSoftRigidDynamicsWorld(physicore->dispatcher,
							physicore->broadphase,
							physicore->solver,
							physicore->collisionConfiguration);
	/*	*/
	this->setGravity( PVVector3(0.0f,-9.82f,0.0f) );

	/*  Set softbody world info.    */
	physicore->softBodyWorldInfo.m_broadphase = physicore->broadphase;
	physicore->softBodyWorldInfo.m_dispatcher = physicore->dispatcher;
	physicore->softBodyWorldInfo.m_gravity = physicore->dynamicsWorld->getGravity();
	physicore->softBodyWorldInfo.m_sparsesdf.Initialize();
}

PhysicInterface::PhysicInterface(const PhysicInterface& other){
//	*this = other;
}
PhysicInterface::~PhysicInterface(void) {

	PhysicCore* physicore = (PhysicCore*)this->pdata;

	/*	Clean up.	*/
	delete physicore->dynamicsWorld;
	delete physicore->solver;
	delete physicore->dispatcher;
	delete physicore->collisionConfiguration;
	delete physicore->broadphase;

	/**/
	free(physicore);
}

void PhysicInterface::OnInitialization(void){
	
}
void PhysicInterface::OnDestruction(void){

}


void PhysicInterface::simulate(float timeStep, int maxSubSteps, float fixedTimeStep) {
	PhysicCore *physicore = (PhysicCore *) this->pdata;

	physicore->dynamicsWorld->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);
	if(physicore->debug)
		physicore->dynamicsWorld->debugDrawWorld();
}

void PhysicInterface::sync(void) {
	PhysicCore *physicore = (PhysicCore *) this->pdata;
	assert(physicore);

	physicore->dynamicsWorld->synchronizeMotionStates();
}

void PhysicInterface::setGravity(const PVVector3 &gravity) {
	PhysicCore *physicore = (PhysicCore *) this->pdata;
	assert(physicore);

	btVector3 grav = btVector3(gravity.x(), gravity.y(), gravity.z());
	physicore->dynamicsWorld->setGravity(*(btVector3 *) &gravity);
}

PVVector3 PhysicInterface::getGravity(void) const {
	PhysicCore *physicore = (PhysicCore *) this->pdata;

	btVector3 gr = physicore->dynamicsWorld->getGravity();
	return PVVector3(gr.x(), gr.y(), gr.z());
}

void PhysicInterface::addRigidBody(RigidBody* body){
	PhysicCore* physicore = (PhysicCore*)this->pdata;
	btRigidBody* rigid;

	/*	*/
	//rigid = (btRigidBody*)getPointerByIndex(body);

	/*	*/
	physicore->dynamicsWorld->addRigidBody(rigid);
}
void PhysicInterface::removeRigidBody(RigidBody* body){
	PhysicCore* physicore = (PhysicCore*)this->pdata;
	btRigidBody* rigid;

	/*	*/
	//rigid = (btRigidBody*)getPointerByIndex(body);

	/*	*/
	physicore->dynamicsWorld->removeRigidBody(rigid);
}

void PhysicInterface::addConstraints(Constraints* constraints){
	PhysicCore* physicore = (PhysicCore*)this->pdata;

//	btTypedConstraint* typedConstraint = (btTypedConstraint*)constraints->getMappedObject();

//	physicore->dynamicsWorld->addConstraint(typedConstraint);
}
void PhysicInterface::removeConstraints(Constraints* constraints){
	PhysicCore* physicore = (PhysicCore*)this->pdata;

//	btTypedConstraint* typedConstraint = (btTypedConstraint*)constraints->getMappedObject();

//	physicore->dynamicsWorld->removeConstraint(typedConstraint);
}

Collision *PhysicInterface::createCollision(const CollisionDesc *desc) {
	btCollisionShape *shape;

	switch (desc->Primitive) {
		case CollisionDesc::ePlane: {
			btVector3 normal = btVector3(desc->planeshape.normal[0], desc->planeshape.normal[1],
			                             desc->planeshape.normal[2]);
			shape = new btStaticPlaneShape(normal, desc->planeshape.d);
		}
			break;
		case CollisionDesc::eBox: {
			btVector3 size = btVector3(desc->boxshape.boxsize[0], desc->boxshape.boxsize[1], desc->boxshape.boxsize[2]);
			shape = new btBoxShape(size);
		}
			break;
		case CollisionDesc::eSphere:
			shape = new btSphereShape(desc->sphereshape.radius);
			break;
		case CollisionDesc::eCapsule:
			shape = new btCapsuleShape(desc->capsuleshape.radius, desc->capsuleshape.height);
			break;
		case CollisionDesc::eMesh:
			//btBvhTriangleMeshShape
//			btTriangleMesh
			btTriangleMesh
//			btBvhTriangleMeshShape
					shape = NULL;//btTriangleMeshShape
			break;
	}

	/*	*/
	//return obtainMappedObject<Collision>(shape);
}
void PhysicInterface::deleteCollision(Collision* collision){

	btCollisionShape* shape;

	//shape = (btCollisionShape*)getPointerByIndex(collision);

	delete shape;
	//returnRefObj(collision);
}


Constraints* PhysicInterface::createConstraints(const ConstraintsDesc* desc){
	PhysicCore* physicore = (PhysicCore*)this->pdata;

	Constraints* constrains;
	btTypedConstraint* constr;
	btRigidBody* b1,*b2;



//	if(desc->body1)
//		b1 = (btRigidBody*)desc->body1->getMappedObject();
//	if(desc->body2)
//		b2 = (btRigidBody*)desc->body2->getMappedObject();


	const btTransform frameA = btTransform();
	const btTransform frameB = btTransform();
	
	switch(desc->type){
		case ConstraintsDesc::eHinge:
			constr = new btHingeConstraint(*b1, *b2, frameA, frameB);
			break;
		case ConstraintsDesc::ePoint:
			//constr = new btPoint2PointConstraint(*b1,*b2);
			break;
		case ConstraintsDesc::eSpring:{
			btGeneric6DofSpringConstraint* spring= new btGeneric6DofSpringConstraint(*b1, *b2, frameA, frameB, true);
			spring->setStiffness(0, desc->spring.stiffness);
			constr = spring;
		}break;
		case ConstraintsDesc::eSlider:
			break;
		default:
			assert(0);
			break;
	}

	constr->setBreakingImpulseThreshold(desc->breakforce);

	//return obtainMappedObject<Constraints>(constr);
}
void PhysicInterface::deleteConstraints(Constraints* constraints){
//	btTypedConstraint* typedConstraint = (btTypedConstraint*)constraints->getMappedObject();

//	delete typedConstraint;
	//this->returnRefObj(constraints);
}

RigidBody *PhysicInterface::createRigibody(const RigidBodyDesc *desc) {

	RigidBody *body = NULL;            /*	*/
	btCollisionShape *shape = NULL;    /*	*/

	//assert(desc->node && desc->collision);

	// if (desc->node == NULL)
	// 	throw RuntimeException("Requires a non-null node reference");

	if (desc->collision == NULL)
		throw RuntimeException("Requires a non-null collision reference");

//	/*	Get collsion object.	*/
//	//shape = (btCollisionShape *) getPointerByIndex(desc->collision);
//
//	/*	Get world space variables.  */
////	Node *node = desc->node;
////	PVQuaternion nodeRotation = node->getRotation();
////	const PVVector3 pos = node->getPosition();
////	btQuaternion rotation = btQuaternion(nodeRotation.x(), nodeRotation.y(), nodeRotation.z(), nodeRotation.w());
////	btVector3 position = btVector3(pos.x(),
////	                               pos.y(),
////	                               pos.z());
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
//		groundRigidBody->setCollisionFlags(groundRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
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

void PhysicInterface::deleteRigibody(RigidBody* rigidbody){

	btRigidBody* body = NULL;

	/*  */
	//body = (btRigidBody*)getPointerByIndex(rigidbody);
	assert(body);

	/*	*/
	delete body;
	//returnRefObj(rigidbody);
}

void* PhysicInterface::createSoftBody(SoftbodyDesc* softbodyDesc){
	PhysicCore* physicore = (PhysicCore*)this->pdata;
	//btSoftBodyFloatData

	btSoftBody* softBody = btSoftBodyHelpers::CreateFromTriMesh(
			physicore->softBodyWorldInfo,
			softbodyDesc->vertices,
	                                     softbodyDesc->indices,
	                                     softbodyDesc->nVertices / 3);



}
void PhysicInterface::deleteSoftBody(void* softbody){

}

void* PhysicInterface::createCloth(ClothDesc* clothDesc){
	PhysicCore* physicore = (PhysicCore*)this->pdata;


//
//	btSoftBody* cloth = btSoftBodyHelpers::CreatePatch(physicore->dynamicsWorld,
//	                                                   btVector3(-s / 2, s + 1, 0),
//	                                                   btVector3(+s / 2, s + 1, 0),
//	                                                   btVector3(-s / 2, s + 1, +s),
//	                                                   btVector3(+s / 2, s + 1, +s),
//	                                                   numX, numY,
//	                                                   fixed, true);



}
void PhysicInterface::deleteCloth(void* cloth){

}

void* PhysicInterface::createTerrain(const TerrainDesc* desc){
	//btHeightfieldTerrainShape
	//btHeightfieldTerrainShape = new
}

void PhysicInterface::deleteTerrain(void* terrain){
	
}


CharacterController *PhysicInterface::createCharacterController(CharacterControllerDesc *desc) {
	PhysicCore *physicore = (PhysicCore *) this->pdata;
	btPairCachingGhostObject *ghostObject = NULL;//new btPairCachingGhostObject();

	btCapsuleShape *capsuleShape = new btCapsuleShape(2.0f, 1.0f);
	btKinematicCharacterController *m_pTommyController = new btKinematicCharacterController(ghostObject, capsuleShape,
	                                                                                        1);

	//m_pTommyController->updateAction(m_dynamicsWorld, timeSinceLast);

	/*	*/
	//return obtainMappedObject<CharacterController>(m_pTommyController);
}

void PhysicInterface::deleteCharacterController(CharacterController *characterController) {

}

void PhysicInterface::addCharacterController(CharacterController *controller) {
	PhysicCore *physicore = (PhysicCore *) this->pdata;
//	btKinematicCharacterController *characterController = (btKinematicCharacterController *) controller->getMappedObject();

//	physicore->dynamicsWorld->addAction(characterController);
}

void PhysicInterface::removeCharacterController(CharacterController *controller) {
	PhysicCore *physicore = (PhysicCore *) this->pdata;
//	btKinematicCharacterController *characterController = (btKinematicCharacterController *) controller->getMappedObject();

	//physicore->dynamicsWorld->removeAction(characterController);
}


void* PhysicInterface::getState(unsigned int* len){
	PhysicCore* physicore = (PhysicCore*)this->pdata;
	
	btDefaultSerializer* serializer = new btDefaultSerializer();
	physicore->dynamicsWorld->serialize(serializer);

	if(len){
		*len = serializer->getCurrentBufferSize();
	}

	return (void*)serializer->getBufferPointer();
}

bool PhysicInterface::rayTest(const PVRay& ray, RayCastHit* hit){
	/*	*/
	PhysicCore* physicore = (PhysicCore*)this->pdata;
	btSoftRigidDynamicsWorld* world = physicore->dynamicsWorld;

	struct	AllRayResultCallback : public btCollisionWorld::RayResultCallback
	{
		AllRayResultCallback(const btVector3& rayFromWorld,const btVector3& rayToWorld)
				:m_rayFromWorld(rayFromWorld),
				 m_rayToWorld(rayToWorld) {
		}

		btVector3	m_rayFromWorld;//used to calculate hitPointWorld from hitFraction
		btVector3	m_rayToWorld;

		btVector3	m_hitNormalWorld;
		btVector3	m_hitPointWorld;

		virtual	btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
		{

			//caller already does the filter on the m_closestHitFraction
			btAssert(rayResult.m_hitFraction <= m_closestHitFraction);

			m_closestHitFraction = rayResult.m_hitFraction;
			m_collisionObject = rayResult.m_collisionObject;

			if (normalInWorldSpace) {
				m_hitNormalWorld = rayResult.m_hitNormalLocal;
			} else {
				///need to transform normal into worldspace
				m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis()*rayResult.m_hitNormalLocal;
			}
			m_hitPointWorld.setInterpolate3(m_rayFromWorld,m_rayToWorld,rayResult.m_hitFraction);
			
			return 1.0f;
		}
	};

	/*	TODO add support.	*/
	btVector3 from;//= ray.getOrigin();
	btVector3 dir; //ray.getDirection();
	AllRayResultCallback result(from, dir);
	world->rayTest(from, dir, result);

	return result.hasHit();
}
bool PhysicInterface::raySphereTest(const PVRay& ray, RayCastHit* hit){
	PhysicCore* physicore = (PhysicCore*)this->pdata;
	btSoftRigidDynamicsWorld* world = physicore->dynamicsWorld;

	//world->convexSweepTest
}

void PhysicInterface::setDebugRenderer(Ref <IRenderer>& renderer) {
	PhysicCore* physicore = (PhysicCore*)this->pdata;

	physicore->drawDebugger = new DebugDrawer(renderer);
	physicore->dynamicsWorld->setDebugDrawer(physicore->drawDebugger);
}

const char* PhysicInterface::getVersion(void) const {
	return FV_STR_VERSION(1,0,0);
}

extern "C" PhysicInterface* createInternalPhysicInterface(IConfig* options){
    return new PhysicInterface(options);
}

