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
#ifndef _FRAGCORE_PHYSIC_COLLIDER_H_
#define _FRAGCORE_PHYSIC_COLLIDER_H_ 1
#include "PhysicObject.h"
#include <FragCore.h>

namespace fragcore {
	/**
	 *	Collision object.
	 *
	 */
	class FVDECLSPEC Collider : public PhysicObject {
		friend class DynamicInterface;

	  public:
		Collider() = default;
		~Collider() override = default;

	  public: /*	Public methods.	*/
		virtual Vector3 getCenter() = 0;

		/**
		 *
		 * @param center
		 */
		virtual void setCenter(const Vector3 &center) = 0;
	};
} // namespace fragcore

#endif
