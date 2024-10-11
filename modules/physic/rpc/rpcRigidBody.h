/*
 *	FragCore - Fragment Core - Engine Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FRAG_CORE_RPC_RIGIDBODY_H_
#define _FRAG_CORE_RPC_RIGIDBODY_H_ 1
#include "../RigidBody.h"

namespace fragcore {

	class FVDECLSPEC RPCRigidBody : public RigidBody {
		friend class DynamicInterface;

	  private:
		RPCRigidBody() = default;

		virtual ~RPCRigidBody() = delete;

	  public: /*	Public methods.	*/
		virtual void useGravity(bool use) = 0;
		virtual bool useGravity() = 0;
		virtual float getMass() = 0;
		virtual void setMass(float mass) = 0;

		// virtual void setInertia(void* pobj);

		virtual Vector3 getPosition() = 0;
		virtual void setPosition(const Vector3 &position) = 0;

		virtual Quaternion getOrientation() = 0;
		virtual void setOrientation(const Quaternion &quat) = 0;

		virtual Vector3 getScale() = 0;
		virtual void setScale(const Vector3 &scale) = 0;

		virtual void addForce(const Vector3 &force) = 0;

		virtual float getDrag() = 0;
		virtual void setDrag(float drag) = 0;

		virtual float getAngularDrag() = 0;
		virtual void setAngularDrag(float angularDrag) = 0;

		virtual Vector3 getVelocity() = 0;

	  public:
		intptr_t getNativePtr() const = 0;
	};

} // namespace fragcore

#endif
