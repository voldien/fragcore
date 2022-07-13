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
#ifndef _FRAG_CORE_RAY_H_
#define _FRAG_CORE_RAY_H_ 1
#include "../../FragDef.h"

namespace fragcore {
	/**
	 * Ray data type. Contains
	 * a position and a direction.
	 */
	struct FVDECLSPEC Ray {
	  public:
		Ray() = default;
		Ray(const Vector3 &origin, const Vector3 &direction) {
			this->setOrigin(origin);
			this->setDirection(direction);
		}
		Ray(const Ray &ray) {
			this->setOrigin(ray.getOrigin());
			this->setDirection(ray.getDirection());
		}

		Ray &operator=(const Ray &other) {
			this->setOrigin(other.getOrigin());
			this->setDirection(other.getDirection());
			return *this;
		}

		/**
		 * @return get origin position.
		 */
		inline const Vector3 &getOrigin() const noexcept { return this->m[0]; }

		/**
		 * Set origin position.
		 */
		inline void setOrigin(const Vector3 &origin) noexcept { this->m[0] = origin; }

		/**
		 * Get direction.
		 * @return normalized direction.
		 */
		inline const Vector3 &getDirection() const noexcept { return this->m[1]; }

		/**
		 * Set direction vector.
		 * @param direction has to be a normalized vector.
		 */
		inline void setDirection(const Vector3 &direction) noexcept { this->m[1] = direction; }

		/**
		 * Get position from distance from point
		 * in respect to direction.
		 * @return point on the ray.
		 */
		inline Vector3 pointAtParameter(float t) const noexcept { return this->getOrigin() + t * this->getDirection(); }

	  private: /*	Attributes.	*/
		/**
		 * #0 origin
		 * #1 direction
		 */
		Vector3 m[2];
	};
} // namespace fragcore

#endif
