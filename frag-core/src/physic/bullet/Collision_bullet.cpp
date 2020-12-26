#include"physic/Collider.h"

#include<bullet/btBulletCollisionCommon.h>
#include<bullet/btBulletDynamicsCommon.h>
using namespace fragcore;

Collision::~Collision(void){}

PVVector3 Collision::getCenter(void* pobj){

	btCollisionShape* col = (btCollisionShape*)pobj;

}
void Collision::setCenter(void* pobj, const PVVector3& center){

	btCollisionShape* col = (btCollisionShape*)pobj;

}




