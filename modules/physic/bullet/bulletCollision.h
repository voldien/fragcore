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
#ifndef _FRAG_CORE_BULLET_PHYSIC_COLLISION_H_
#define _FRAG_CORE_BULLET_PHYSIC_COLLISION_H_ 1
#include "../Collision.h"
#include "bulletPhysicInterface.h"

namespace fragcore {

	class FVDECLSPEC BulletCollision : public Collision {
		friend class BulletPhysicInterface;

	  public:
		BulletCollision(btCollisionShape* shape);
		BulletCollision(const BulletCollision &collision) = default;
		~BulletCollision() override;

		/**
		 *	Get collider object.
		 */
		Collider *collider() const override;

		/**
		 *	Get relative velocity magnitude.
		 */
		float relativeVelocity() const override;

		/**
		 *	Get transform object.
		 */
		// VDTransform *transform() const;

		/**
		 *	Get rigidbodt object.
		 */
		RigidBody *rigidBody() const override;

		/**
		 *	Get contact.
		 */
		// Contact contacts() const;

	  private: /*	Attributes.	*/
		void *pair;
		void *shape;
	};
} // namespace fragcore

#endif
