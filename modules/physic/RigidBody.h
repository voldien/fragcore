/**
	Simple physic simulation with a server-client model support.
	Copyright (C) 2016  Valdemar Lindberg

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_RIGIDBODY_H_
#define _FRAG_CORE_RIGIDBODY_H_ 1
#include "PhysicObject.h"
#include "PhysicPrerequisites.h"

namespace fragcore {
	/**
	 *	Rigidbody.
	 */
	class FVDECLSPEC RigidBody : public PhysicObject {
		friend class DynamicInterface;

	  private:
		inline RigidBody() {}

		virtual ~RigidBody();

	  public: /*	Public methods.	*/
		virtual void useGravity(bool use);
		virtual bool useGravity();
		virtual float getMass();
		virtual void setMass(float mass);

		// virtual void setInertia(void* pobj);

		virtual Vector3 getPosition();
		virtual void setPosition(const Vector3 &position);

		virtual Quaternion getOrientation();
		virtual void setOrientation(const Quaternion &quat);

		virtual Vector3 getScale();
		virtual void setScale(const Vector3 &scale);

		virtual void addForce(const Vector3 &force);

		/**/
		virtual float getDrag();
		virtual void setDrag(float drag);

		virtual float getAngularDrag();
		virtual void setAngularDrag(float angularDrag);

		virtual Vector3 getVelocity();

		intptr_t getNativePtr() const;
	};

} // namespace fragcore

#endif
