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
#ifndef _FRAGCORE_LINE_H_
#define _FRAGCORE_LINE_H_ 1
#include "Shape.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	template <typename T = float> struct FVDECLSPEC Line : public Shape {
	  public:
		// using LineVec = Eigen::Vector2f

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
		Line(const Vector2 &normal, float distance) noexcept {
			this->normal = normal;
			this->d = distance;
		}

		Line &operator=(const Line &other) noexcept {
			this->normal = other.normal;
			this->d = other.d;
			return *this;
		}

		/**
		 * Get normal of plane.
		 * @return normalized normal vector.
		 */
		Vector2 getNormal() noexcept { return this->normal; }

		/**
		 * Get normal of plane.
		 * @return normalized normal vector.
		 */
		const Vector2 &getNormal() const noexcept { return this->normal; }

		/**
		 * Set plane normal.
		 * @param normal
		 */
		void setNormal(const Vector2 &normal) noexcept { this->normal = normal; }

		/**
		 * Compute distance.
		 * @param point
		 * @return
		 */
		float distance(const Vector2 &point) const noexcept { return this->normal.dot(point) + d; }

		/**
		 * Get distance.
		 * @return
		 */
		float distance() const noexcept { return this->d; }

		/**
		 * Get point.
		 * @return
		 */
		Vector2 getPoint() const noexcept { return d * this->getNormal(); }

		/**
		 * Set normal and point and
		 * compute internal values.
		 */
		void setNormalAndPoint(const Vector2 &normal, const Vector2 &point) noexcept {
			this->normal = normal.normalized();
			this->d = -point.dot(this->normal);
		}

		/**
		 * Construct line from line segment.
		 */
		void set2DPoints(const Vector2 &p1, const Vector2 &p2) noexcept {
			Vector2 delta = (p2 - p1);
			Vector2 peru{delta[1], -delta[0]};
			this->normal = peru.normalized();
			this->d = this->normal.dot(p2);
		}

		/**
		 * Compare if plane equal each other.
		 * @return true if equal.
		 */
		friend bool operator==(const Line &o1, const Line &o2) noexcept {
			if (&o1 == &o2) {
				return true;
			}
			if (o1.normal == o2.normal && o1.d == o2.d) {
				return true;
			}
			return false;
		}

		/**
		 * Compare if plane not equal each other.
		 * @return true if equal.
		 */
		friend bool operator!=(const Line &o1, const Line &o2) noexcept { return !(o1 == o2); }

	  protected:		/*	Attributes.	*/
		Vector2 normal; /*	*/
		float d{};		/*	*/

	  public: /*	Static methods.	*/
		/**
		 * Create plane from line segment.
		 * @param v1
		 * @param v2
		 * @return
		 */
		static Line fromPoints(const Vector2 &v1, const Vector2 &v2) noexcept {
			Line tmp;
			Vector2 e1 = (v2 - v1);
			/*	Corss product.	*/
			Vector2 e2{e1[1], -e1[0]};

			tmp.d = -tmp.normal.dot(v1);
			return tmp;
		}
	};
} // namespace fragcore
#endif
