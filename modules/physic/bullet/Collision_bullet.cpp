#include"../Collider.h"

#include<bullet/btBulletCollisionCommon.h>
#include<bullet/btBulletDynamicsCommon.h>
using namespace fragcore;

Collision::~Collision(){}

Vector3 Collision::getCenter(void* pobj){

	btCollisionShape* col = (btCollisionShape*)pobj;

}
void Collision::setCenter(void* pobj, const Vector3& center){

	btCollisionShape* col = (btCollisionShape*)pobj;

}




