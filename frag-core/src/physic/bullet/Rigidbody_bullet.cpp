#include"physic/RigidBody.h"

#include<bullet/btBulletCollisionCommon.h>
#include<bullet/btBulletDynamicsCommon.h>
using namespace fragcore;

RigidBody::~RigidBody(void){}

float RigidBody::getMass(void){
	btRigidBody *body = (btRigidBody *)this->getObject();
	return 1.0f / body->getInvMass();
}
void RigidBody::setMass(float mass){
	btRigidBody *body = (btRigidBody *)this->getObject();

	btVector3 inertia;
	body->getCollisionShape()->calculateLocalInertia(mass, inertia);
}

PVVector3 RigidBody::getPosition(void){

	btRigidBody* body;
	btTransform trans;
	btMotionState* mot;

	body = (btRigidBody *)this->getObject();

	/**/
	mot = body->getMotionState();
	mot->getWorldTransform(trans);

	return *(PVVector3*)&trans.getOrigin();
}
void RigidBody::setPosition(const PVVector3& position){
	btRigidBody* body;
	body = (btRigidBody *)this->getObject();

	/*  Both internal and rigidbody.    */

}

PVQuaternion RigidBody::getOrientation(void){

	btRigidBody* body;
	btQuaternion oritention;
	body = (btRigidBody *)this->getObject();

	oritention = body->getOrientation();

	return PVQuaternion(oritention.getW(),
			oritention.getX(),
			oritention.getY(),
			oritention.getZ());
}
void RigidBody::setOrientation(const PVQuaternion& quat){
	btRigidBody* body;
	body = (btRigidBody *)this->getObject();
}


PVVector3 RigidBody::getScale(void){
	btRigidBody* body;
	body = (btRigidBody *)this->getObject();

	const btVector3 scale = body->getCollisionShape()->getLocalScaling();

	return PVVector3(scale.x(),scale.y(),scale.z());
}
void RigidBody::setScale(const PVVector3& scale){
	btRigidBody* body;
	body = (btRigidBody *)this->getObject();

	btVector3 sc = btVector3(scale.x(),scale.y(),scale.z());
	body->getCollisionShape()->setLocalScaling(sc);
}

void RigidBody::addForce(const PVVector3& force){
	btRigidBody* body;
	body = (btRigidBody *)this->getObject();
}

float RigidBody::getDrag(void){
	btRigidBody* body;
	body = (btRigidBody *)this->getObject();

	return body->getLinearDamping();
}
void RigidBody::setDrag(float drag){
	btRigidBody* body;
	body = (btRigidBody *)this->getObject();

	body->setDamping(drag, body->getAngularDamping());
}

float RigidBody::getAngularDrag(void){
	btRigidBody* body;
	body = (btRigidBody *)this->getObject();

	return body->getAngularDamping();
}
void RigidBody::setAngularDrag(float angularDrag){
	btRigidBody* body;
	body = (btRigidBody *)this->getObject();

	body->setDamping(body->getLinearDamping(), angularDrag);
}


PVVector3 RigidBody::getVelocity(void){
	btRigidBody* body;
	body = (btRigidBody *)this->getObject();

	const btVector3& linear = body->getLinearVelocity();
	return PVVector3(linear.x(),linear.y(),linear.z());
}

