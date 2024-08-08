/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _FRAG_CORE_BULLET_RIGIDBODY_H_
#define _FRAG_CORE_BULLET_RIGIDBODY_H_ 1
#include "../RigidBody.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "bulletPhysicInterface.h"

namespace fragcore {

	class FVDECLSPEC BulletRigidBody : public RigidBody {
		friend class DynamicInterface;
		friend class BulletPhysicInterface;

	  public:
		BulletRigidBody() = default;
		BulletRigidBody(btRigidBody *body);
		~BulletRigidBody() override = default;

	  public: /*	Public methods.	*/
		void useGravity(bool use) override;
		bool useGravity() override;
		float getMass() override;
		void setMass(float mass) override;

		//  void setInertia(void* pobj);

		Vector3 getPosition() override;
		void setPosition(const Vector3 &position) override;

		Quaternion getOrientation() override;
		void setOrientation(const Quaternion &quat) override;

		Vector3 getScale() override;
		void setScale(const Vector3 &scale) override;

		void addForce(const Vector3 &force) override;

		float getDrag() override;
		void setDrag(float drag) override;

		float getAngularDrag() override;
		void setAngularDrag(float angularDrag) override;

		Vector3 getVelocity() override;

	  public:
		intptr_t getNativePtr() const override;
	};

} // namespace fragcore

#endif
