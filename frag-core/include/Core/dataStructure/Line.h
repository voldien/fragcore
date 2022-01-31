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
#ifndef _FRAG_CORE_LINE_H_
#define _FRAG_CORE_LINE_H_ 1
#include "../../Def.h"
#include <iostream>

namespace fragcore {
	/**
	 *
	 */
	struct FVDECLSPEC Line {
	  public:
		Line() = default;
		Line(const Vector2 &normal) noexcept {
			this->normal = normal;
			this->d = 0;
		}
		Line(const Vector2 &point, const Vector2 &normal) noexcept { this->setNormalAndPoint(normal, point); }
		Line(const Line &other) noexcept {
			this->normal = other.normal;
			this->d = other.d;
		}

		Line &operator=(const Line &other) {
			this->normal = other.normal;
			this->d = other.d;
			return *this;
		}

		/**
		 * Get normal of plane.
		 * @return normalized normal vector.
		 */
		inline Vector2 getNormal() noexcept { return this->normal; }

		/**
		 * Get normal of plane.
		 * @return normalized normal vector.
		 */
		inline const Vector2 &getNormal() const noexcept { return this->normal; }

		/**
		 * Set plane normal.
		 * @param normal
		 */
		inline void setNormal(const Vector2 &normal) noexcept { this->normal = normal; }

		/**
		 * Compute distance.
		 * @param point
		 * @return
		 */
		inline float distance(const Vector2 &point) const noexcept { return normal.dot(point) + d; }

		/**
		 * Get distance.
		 * @return
		 */
		inline float distance() const noexcept { return this->d; }

		/**
		 * Get point.
		 * @return
		 */
		inline Vector2 getPoint() const noexcept { return d * this->getNormal(); }

		/**
		 * Set normal and point and
		 * compute internal values.
		 */
		void setNormalAndPoint(const Vector2 &normal, const Vector2 &point) noexcept {
			this->normal = normal.normalized();
			this->d = point.dot(this->normal);
		}

		/**
		 * Construct line from line segment.
		 */
		void set2DPoints(const Vector2 &p1, const Vector2 &p2) noexcept {
			Vector2 delta = (p2 - p1);
			Vec2 peru{delta[1], -delta[0]};
			this->normal = peru.normalize();
			this->d = this->normal.dot(p2);
		}

		/**
		 * Compare if plane equal each other.
		 * @return true if equal.
		 */
		friend bool operator==(const Plane &o1, const Plane &o2) noexcept {
			if (&o1 == &o2)
				return true;
			else if (o1.normal == o2.normal && o1.d == o2.d)
				return true;
			else
				return false;
		}

		/**
		 * Compare if plane not equal each other.
		 * @return true if equal.
		 */
		friend bool operator!=(const Plane &o1, const Plane &o2) noexcept { return !(o1 == o2); }

	  protected:		/*	Attributes.	*/
		Vector2 normal; /*	*/
		float d;		/*	*/

	  public: /*	Static methods.	*/
		/**
		 * Create plane from points.
		 * @param v1
		 * @param v2
		 * @return
		 */
		static Line fromPoints(const Vector3 &v1, const Vector3 &v2) noexcept {
			Plane tmp;
			Vector3 e1 = v2 - v1;
			Vec2 e2{e1[1], -e1[0]};

			tmp.d = -tmp.normal.dot(e2);
			return tmp;
		}
	};
} // namespace fragcore
#endif
