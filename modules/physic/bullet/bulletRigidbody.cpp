#include "bulletRigidBody.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

using namespace fragcore;

BulletRigidBody::~BulletRigidBody() {}

float BulletRigidBody::getMass() {
	btRigidBody *body = (btRigidBody *)this->getObject();
	return 1.0f / body->getInvMass();
}
void BulletRigidBody::setMass(float mass) {
	btRigidBody *body = (btRigidBody *)this->getObject();

	btVector3 inertia;
	body->getCollisionShape()->calculateLocalInertia(mass, inertia);
}

Vector3 BulletRigidBody::getPosition() {

	btRigidBody *body;
	btTransform trans;
	btMotionState *mot;

	body = (btRigidBody *)this->getObject();

	/**/
	mot = body->getMotionState();
	mot->getWorldTransform(trans);

	return *(Vector3 *)&trans.getOrigin();
}
void BulletRigidBody::setPosition(const Vector3 &position) {
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	/*  Both internal and BulletRigidBody.    */
}

Quaternion BulletRigidBody::getOrientation() {

	btRigidBody *body;
	btQuaternion oritention;
	body = (btRigidBody *)this->getObject();

	oritention = body->getOrientation();

	return Quaternion(oritention.getW(), oritention.getX(), oritention.getY(), oritention.getZ());
}
void BulletRigidBody::setOrientation(const Quaternion &quat) {
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();
}

Vector3 BulletRigidBody::getScale() {
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	const btVector3 scale = body->getCollisionShape()->getLocalScaling();

	return Vector3(scale.x(), scale.y(), scale.z());
}
void BulletRigidBody::setScale(const Vector3 &scale) {
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	btVector3 sc = btVector3(scale.x(), scale.y(), scale.z());
	body->getCollisionShape()->setLocalScaling(sc);
}

void BulletRigidBody::addForce(const Vector3 &force) {
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();
}

float BulletRigidBody::getDrag() {
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	return body->getLinearDamping();
}
void BulletRigidBody::setDrag(float drag) {
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	body->setDamping(drag, body->getAngularDamping());
}

float BulletRigidBody::getAngularDrag() {
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	return body->getAngularDamping();
}
void BulletRigidBody::setAngularDrag(float angularDrag) {
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	body->setDamping(body->getLinearDamping(), angularDrag);
}

Vector3 BulletRigidBody::getVelocity() {
	btRigidBody *body;
	body = (btRigidBody *)this->getObject();

	const btVector3 &linear = body->getLinearVelocity();
	return Vector3(linear.x(), linear.y(), linear.z());
}
