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
#ifndef _FRAGCORE_BOUNDING_SPHERE_H_
#define _FRAGCORE_BOUNDING_SPHERE_H_ 1
#include "Ray.h"
#include <cmath>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	// TODO add template for precisous
	// template<typename T = Vector3>
	struct FVDECLSPEC BoundingSphere {
	  public:
		BoundingSphere() = default;
		BoundingSphere(const Vector3 &center, float radius) noexcept {
			this->setCenter(center);
			this->setRadius(radius);
		}
		BoundingSphere(const BoundingSphere &boundingSphere) noexcept { *this = boundingSphere; }

		/**
		 * Get radius size.
		 * @return non-negative size.
		 */
		constexpr float getRadius() const noexcept { return this->radius; }

		/**
		 * set radius size of the sphere.
		 * @param radius non-negative radius size.
		 */
		void setRadius(float radius) noexcept { this->radius = radius; }

		/**
		 * Get center position.
		 * @return world position.
		 */
		const Vector3 &getCenter() const noexcept { return this->center; }

		/**
		 * Set center position.
		 * @param center in world position.
		 */
		void setCenter(const Vector3 &center) { this->center = center; }

		/**
		 * Check if object intersects with another sphere.
		 * @param sphere
		 * @return true if object intersects, false otherwise.
		 */
		bool
		intersect(const BoundingSphere &sphere) const noexcept { // get box closest point to sphere center by clamping
			// we are using multiplications because it's faster than calling Math.pow
			float distance = std::sqrt(
				((this->getCenter().x - sphere.getCenter().x) * (this->getCenter().x - sphere.getCenter().x)) +
				((this->getCenter().y - sphere.getCenter().y) * (this->getCenter().y - sphere.getCenter().y)) +
				((this->getCenter().z - sphere.getCenter().z) * (this->getCenter().z - sphere.getCenter().z)));
			return distance < (this->getRadius() + sphere.getRadius());
		}

		/**
		 * Check if sphere contains a sphere.
		 * @param sphere sphere inside this sphere.
		 * @return true if object contains, false otherwise.
		 */
		bool contains(const BoundingSphere &sphere) const noexcept {

			// TODO: add within radius.
			if (this->contains(sphere.getCenter())) {
				return true;
			}
			return false;
		}

		bool contains(const Vector3 &point) const noexcept {
			return glm::length(point - getCenter()) < this->getRadius();
		}

		/**
		 * @brief
		 */
		template <typename T> bool intersect(const Ray &ray) const noexcept {
			const Vector3 tmp = ray.getOrigin() - getCenter();
			T t;

			/*	*/
			const T a = glm::dot(ray.getDirection(), ray.getDirection());
			const T b = static_cast<T>(2.0) * glm::dot(ray.getDirection(), tmp);
			const T c = glm::dot(tmp, tmp) - (radius * radius);

			/*	*/
			const T discriminant = (b * b) - (static_cast<T>(4) * c * a);
			if (discriminant >= 0) {
				t = (-b - std::sqrt(discriminant)) / (a * static_cast<T>(2));
				if (t < static_cast<T>(0)) {
					t = (-b + discriminant) / static_cast<T>(2.0) * a;
				}
				return true;
			}
			return false;
		}
		bool intersect(const Ray &ray) const noexcept { return intersect<float>(ray); }

		BoundingSphere &operator=(const BoundingSphere &bounds) {
			this->setRadius(bounds.getRadius());
			this->setCenter(bounds.getCenter());
			return *this;
		}

		friend bool operator==(const BoundingSphere &Sphere0, const BoundingSphere &Sphere1) noexcept {
			if (&Sphere0 == &Sphere1) {
				return true;
			}
			if (Sphere0.center == Sphere1.center && Sphere0.radius == Sphere1.radius) {
				return true;
			}
			return false;
		}

		friend bool operator!=(const BoundingSphere &Sphere0, const BoundingSphere &Sphere1) noexcept {
			return !(Sphere0 == Sphere1);
		}

	  private:			/*	Private member attributes.	*/
		float radius;	/*	Radius size.	*/
		Vector3 center; /*	Center position in world space.*/
	};
} // namespace fragcore

#endif
