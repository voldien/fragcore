#include "../RayCastHit.h"
#include <bullet/btBulletCollisionCommon.h>
using namespace fragcore;

// RayCastHit::~RayCastHit() {

// }

// Collider *RaycastHit::collider() const
// {
// 	//return VDCASTP(VDGameObject *, this->hitdata.actor->userData)->collider();
// }

// RigidBody *RaycastHit::rigidBody() const
// {
// 	//return VDCASTP(VDGameObject *, this->hitdata.actor->userData)->rigidBody();
// }

// // VDGameObject *RaycastHit::gameObject() const
// // {
// // 	return VDCASTP(VDGameObject *, this->hitdata.actor->userData);
// // }

// // VDTransform *RaycastHit::transform() const
// // {
// // 	return VDCASTP(VDGameObject *, this->hitdata.actor->userData)->transform();
// // }

// Vector3& RaycastHit::point() const
// {
// //	return *VDCASTP(const VDVector3 *, &this->hitdata.position);
// }

// Vector3& RaycastHit::normal() const
// {
// 	//return *VDCASTP(const VDVector3 *, &this->hitdata.normal);
// }

// float RaycastHit::distance() const
// {
// //	return this->hitdata.distance;
// }

// PVVector2& RaycastHit::barycentric() const
// {
// 	//return VDVector2(this->hitdata.u, this->hitdata.v);
// }