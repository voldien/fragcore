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
#ifndef _FRAGCORE_RAY_CAST_HIT_H_
#define _FRAGCORE_RAY_CAST_HIT_H_ 1
#include "PhysicInterface.h"
#include "PhysicObject.h"

namespace fragcore {

	/**
	 *	RayCastHit.
	 */
	class FVDECLSPEC RayCastHit : public PhysicObject {
	  public:
		// TODO add parmaters and methods.
		RayCastHit() = default;
		~RayCastHit() override = default;

		// Collider*  collider() const;

		// RigidBody*  rigidBody() const;

		//	VDGameObject*  gameObject() const;

		// VDTransform*  transform() const;

		// Vector3& point() const;

		// Vector3 &normal() const;

		// float distance() const;

		// PVVector2& barycentric() const;
	};
} // namespace fragcore

#endif
