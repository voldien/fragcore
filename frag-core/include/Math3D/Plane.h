/*
 *	FragCore - Fragment Core
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
#ifndef _FRAGCORE_PLANE_H_
#define _FRAGCORE_PLANE_H_ 1
#include "../FragDef.h"
#include "Math3D.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 * @tparam T
	 */
	template <typename T = float> struct FVDECLSPEC Plane {
	  public:
		Plane() = default;
		Plane(const Vector3 &normal, const T distance = 0) noexcept {
			this->normal = normal.normalized();
			this->d = distance;
		}
		Plane(const Vector3 &point, const Vector3 &normal) noexcept { this->setNormalAndPoint(normal, point); }
		Plane(const Plane &other) noexcept {
			this->normal = other.normal;
			this->d = other.d;
		}
		Plane &operator=(const Plane &other) {
			this->normal = other.normal;
			this->d = other.d;
			return *this;
		}

		/**
		 * Get normal of plane.
		 * @return normalized normal vector.
		 */
		Vector3 getNormal() noexcept { return this->normal; }

		/**
		 * Get normal of plane.
		 * @return normalized normal vector.
		 */
		const Vector3 &getNormal() const noexcept { return this->normal; }

		/**
		 * Set plane normal.
		 * @param normal
		 */
		void setNormal(const Vector3 &normal) noexcept { this->normal = normal; }

		/**
		 * Compute distance.
		 */
		T distance(const Vector3 &point) const noexcept { return normal.dot(point) + d; }
		T distanceSigned(const Vector3 &point) const noexcept { return normal.dot(point) - d; }

		/**
		 * Get distance.
		 */
		T distance() const noexcept { return this->d; }
		T distanceSigned() const noexcept { return -this->d; }

		/**
		 * Get point.
		 */
		Vector3 getPoint() const noexcept { return this->distance() * this->getNormal(); }

		/**
		 * Set normal and point and
		 * compute internal values.
		 */
		void setNormalAndPoint(const Vector3 &normal, const Vector3 &point) noexcept {
			this->normal = normal.normalized();
			this->d = point.dot(this->normal);
		}

		/**
		 * Construct plane.
		 */
		void set3DPoints(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3) noexcept {
			this->normal = (v1 - v2).cross(v1 - v3).normalized();
			this->d = -this->normal.dot(v2);
		}

		friend bool operator==(const Plane &o1, const Plane &o2) noexcept {
			if (&o1 == &o2) {
				return true;
			}
			if (o1.normal == o2.normal && o1.d == o2.d) {
				return true;
			}
			return false;
		}
		friend bool operator!=(const Plane &o1, const Plane &o2) noexcept { return !(o1 == o2); }

	  protected:					/*	Attributes.	*/
		Vector3 normal = {0, 1, 0}; /*	*/
		T d = 0;					/*	*/

	  public: /*	Static methods.	*/
		/**
		 * Create plane from points.
		 */
		static Plane fromPoints(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3) noexcept {
			Plane tmp;
			const Vector3 e1 = v2 - v1;
			const Vector3 e2 = v3 - v1;

			tmp.normal = e1.cross(e2).normalized();
			tmp.d = -tmp.normal.dot(v2);
			return tmp;
		}
	};
} // namespace fragcore
#endif
