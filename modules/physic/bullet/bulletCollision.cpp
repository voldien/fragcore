#include "bulletCollision.h"
#include <btBulletDynamicsCommon.h>
using namespace fragcore;

BulletCollision::BulletCollision(btCollisionShape *shape) { this->shape = shape; }
BulletCollision::~BulletCollision() {}

Collider *BulletCollision::collider() const { return nullptr; }

float BulletCollision::relativeVelocity() const { return 0.0f; }

RigidBody *BulletCollision::rigidBody() const { return nullptr; }

// Contact BulletCollision::contacts() const { return Contract(); }