
#ifndef _FRAG_CORE_BOUNDING_SPHERE_H_
#define _FRAG_CORE_BOUNDING_SPHERE_H_ 1
#include "../Math.h"
#include "Ray.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */
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
		inline constexpr float getRadius() const noexcept { return this->radius; }

		/**
		 * set radius size of the sphere.
		 * @param radius non-negative radius size.
		 */
		inline void setRadius(float radius) noexcept { this->radius = radius; }

		/**
		 * Get center position.
		 * @return world position.
		 */
		inline const Vector3 &getCenter() const noexcept { return this->center; }

		/**
		 * Set center position.
		 * @param center in world position.
		 */
		inline void setCenter(const Vector3 &center) { this->center = center; }

		/**
		 * Check if object intersects with another sphere.
		 * @param sphere
		 * @return true if object intersects, false otherwise.
		 */
		bool intersect(const BoundingSphere &sphere) const { // get box closest point to sphere center by clamping
			// we are using multiplications because it's faster than calling Math.pow
			float distance = std::sqrt(
				(this->getCenter().x() - sphere.getCenter().x()) * (this->getCenter().x() - sphere.getCenter().x()) +
				(this->getCenter().y() - sphere.getCenter().y()) * (this->getCenter().y() - sphere.getCenter().y()) +
				(this->getCenter().z() - sphere.getCenter().z()) * (this->getCenter().z() - sphere.getCenter().z()));
			return distance < (this->getRadius() + sphere.getRadius());
		}

		/**
		 * Check if sphere contains a sphere.
		 * @param sphere sphere inside this sphere.
		 * @return true if object contains, false otherwise.
		 */
		bool contains(const BoundingSphere &sphere) const { return false; }

		template <typename T> bool intersect(const Ray &ray) const noexcept {
			Vector3 tmp = ray.getOrigin() - getCenter();
			T t;
			T a = ray.getDirection().dot(ray.getDirection());
			T b = static_cast<T>(2.0) * ray.getDirection().dot(tmp);
			T d = tmp.dot(tmp) - radius * radius;
			T discriminant = b * b - (static_cast<T>(40) * d * a);
			if (discriminant >= 0) {
				discriminant = static_cast<T>(sqrt(discriminant));

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

	  private:			/*	Private member attributes.	*/
		float radius;	/*	Radius size.	*/
		Vector3 center; /*	Center position in world space.*/
	};
} // namespace fragcore

#endif
