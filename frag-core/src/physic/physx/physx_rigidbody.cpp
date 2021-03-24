#define USE_PHYSX
#include <physx/PxPhysicsAPI.h>

using namespace std;
using namespace physx;
using namespace fragcore;

RigidBody::~RigidBody(void) {}

float RigidBody::getMass(void)
{
	btRigidBody *body = (btRigidBody *)this->getObject();
	return 1.0f / body->getInvMass();
}
void RigidBody::setMass(float mass)
{
	btRigidBody *body = (btRigidBody *)this->getObject();

	btVector3 inertia;
	body->getCollisionShape()->calculateLocalInertia(mass, inertia);
}

Vector3 RigidBody::getPosition(void)
{

	btRigidBody *body;
	btTransform trans;
	btMotionState *mot;

	body = (btRigidBody *)this->getObject();

	/**/
	mot = body->getMotionState();
	mot->getWorldTransform(trans);

	return *(Vector3 *)&trans.getOrigin();
}
void RigidBody::setPosition(const Vector3 &position)
{
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	/*  Both internal and rigidbody.    */
}

PVQuaternion RigidBody::getOrientation(void)
{

	btRigidBody *body;
	btQuaternion oritention;
	body = (btRigidBody *)this->getObject();

	oritention = body->getOrientation();

	return PVQuaternion(oritention.getW(),
						oritention.getX(),
						oritention.getY(),
						oritention.getZ());
}
void RigidBody::setOrientation(const PVQuaternion &quat)
{
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();
}

Vector3 RigidBody::getScale(void)
{
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	const btVector3 scale = body->getCollisionShape()->getLocalScaling();

	return Vector3(scale.x(), scale.y(), scale.z());
}
void RigidBody::setScale(const Vector3 &scale)
{
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	btVector3 sc = btVector3(scale.x(), scale.y(), scale.z());
	body->getCollisionShape()->setLocalScaling(sc);
}

void RigidBody::addForce(const Vector3 &force)
{
	physx::PxRigidActor *body;
	body = (physx::PxRigidActor *)this->getObject();
	((PxRigidDynamic *)this->actor)->addForce(*dynamic_cast<const PxVec3 *>&force));
}

float RigidBody::getDrag(void)
{
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	return body->getLinearDamping();
}
void RigidBody::setDrag(float drag)
{
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	body->setDamping(drag, body->getAngularDamping());
}

float RigidBody::getAngularDrag(void)
{
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	return body->getAngularDamping();
}
void RigidBody::setAngularDrag(float angularDrag)
{
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	body->setDamping(body->getLinearDamping(), angularDrag);
}

Vector3 RigidBody::getVelocity(void)
{
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	const btVector3 &linear = body->getLinearVelocity();
	return Vector3(linear.x(), linear.y(), linear.z());
}





VDRigidBody::VDRigidBody(void) : VDPhysicActor()
{
	this->actor = NULL;
}

void VDRigidBody::instanceInitilize(void)
{
	VDPhysic::addActor(this->actor);

	if (getActor())
		VDCAST(PxActor *, getActor())->userData = gameObject();
	rigidBodyCollection.push_back(this);
}

void VDRigidBody::initializeComponent(void)
{
	VDPhysicActor::initializeComponent();
	this->initVDPhysicObject((this->collider() != NULL) ? true : false, true);
	this->kinect(false);
	this->useGraivity(true);
	this->setMass(1.0f);
	this->PxShapeSimulate(true);
	this->GetDynamicActor()->setLinearDamping(0.0f);
	this->GetDynamicActor()->setAngularDamping(0.05f);
	rigidBodyCollection.push_back(this);

	// define gameobject as a rigidbody atteched
	this->gameObject()->setActive(this->gameObject()->getActiveFlag() | VDGameObject::eRigidbody);
}

void VDRigidBody::onEnable(void)
{
	this->GetDynamicActor()->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
}

void VDRigidBody::onDisable(void)
{
	this->GetDynamicActor()->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
}

void VDRigidBody::onDestroy(void)
{
	for (std::vector<VDRigidBody *>::iterator x = rigidBodyCollection.begin(); x != rigidBodyCollection.end(); x++)
	{
		if ((*x) == this)
		{
			rigidBodyCollection.erase(x);
			//TODO Clean Actor Data and Collider Information
			VDPhysic::removeActor(this->getActor());
			// Remove All Data Releated to this.
			if (!this->isColliderAttached())
				this->PxReleaseActor();
			break;
		}
	}
}

VDBehavior *VDRigidBody::copyComponent(unsigned int &dataSize)
{
	VDRigidBody *rigid = (VDRigidBody *)malloc(sizeof(VDRigidBody));
	memcpy(rigid, this, sizeof(VDRigidBody));
	dataSize = sizeof(VDRigidBody);
	VDPhysic::copyInstanceActor(this, rigid);
	return (VDBehavior *)rigid;
}

void VDRigidBody::addForce(const VDVector3 &force)
{
	((PxRigidDynamic *)this->actor)->addForce(*VDCASTP(const PxVec3 *, &force));
}

void VDRigidBody::addForceAtPosition(const VDVector3 &force, const VDVector3 &position)
{
	PxRigidBodyExt::addForceAtPos(*(PxRigidBody *)this->actor, *(PxVec3 *)&force, *(PxVec3 *)&position);
}

void VDRigidBody::addRelativeForce(const VDVector3 &force)
{
}

void VDRigidBody::addRelativeTorque(const VDVector3 &force)
{
	
}

void VDRigidBody::addTorque(const VDVector3 &force)
{
	((PxRigidDynamic *)this->actor)->addTorque(*VDCASTP(const PxVec3 *, &force));
}

void VDRigidBody::setMass(float Mass)
{
	this->GetDynamicActor()->setMass(Mass);
}

float VDRigidBody::getMass(void) const
{
	return ((PxRigidDynamic *)this->actor)->getMass();
}

void VDRigidBody::setDrag(float drag)
{
	((PxRigidDynamic *)this->actor)->setLinearDamping(drag);
}

float VDRigidBody::getDrag(void) const
{
	return ((PxRigidDynamic *)this->actor)->getLinearDamping();
}

void VDRigidBody::sleep(void)
{
	this->GetDynamicActor()->putToSleep();
}

void VDRigidBody::wake(void)
{
	this->GetDynamicActor()->wakeUp();
}

void VDRigidBody::useGraivity(bool useGravity)
{
	this->GetRigidActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, (useGravity == true) ? false : true);
}

void VDRigidBody::kinect(bool kinect)
{
	this->GetDynamicActor()->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, kinect);
}
