#include "bulletCollision.h"
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
using namespace fragcore;

BulletCollision::BulletCollision() {}
BulletCollision::~BulletCollision() {}

BulletCollision::BulletCollision(const BulletCollision &collision) {}

Collider *BulletCollision::collider() const { return nullptr; }

float BulletCollision::relativeVelocity() const { return 0.0f; }

RigidBody *BulletCollision::rigidBody() const { return nullptr; }

// Contact BulletCollision::contacts() const { return Contract(); }