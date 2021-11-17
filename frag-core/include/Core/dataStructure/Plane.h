
#ifndef _FRAG_CORE_PLANE_H_
#define _FRAG_CORE_PLANE_H_ 1
#include "../../Def.h"
#include <iostream>

namespace fragcore {
	/**
	 *
	 */
	struct FVDECLSPEC Plane {
	  public:
		Plane() = default;
		Plane(const Vector3 &normal) noexcept {
			this->normal = normal;
			this->d = 0;
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
		inline Vector3 getNormal() noexcept { return this->normal; }

		/**
		 * Get normal of plane.
		 * @return normalized normal vector.
		 */
		inline const Vector3 &getNormal() const noexcept { return this->normal; }

		/**
		 * Set plane normal.
		 * @param normal
		 */
		inline void setNormal(const Vector3 &normal) noexcept { this->normal = normal; }

		/**
		 * Compute distance.
		 * @param point
		 * @return
		 */
		inline float distance(const Vector3 &point) const noexcept { return normal.dot(point) + d; }

		/**
		 * Get distance.
		 * @return
		 */
		inline float distance() const noexcept { return this->d; }

		/**
		 * Get point.
		 * @return
		 */
		inline Vector3 getPoint() const noexcept { return d * this->getNormal(); }

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
		Vector3 normal; /*	*/
		float d;		/*	*/

	  public: /*	Static methods.	*/
		/**
		 * Create plane from points.
		 * @param v1
		 * @param v2
		 * @param v3
		 * @return
		 */
		static Plane fromPoints(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3) noexcept {
			Plane tmp;
			Vector3 e1 = v2 - v1;
			Vector3 e2 = v3 - v1;

			tmp.normal = e1.cross(e2).normalized();
			tmp.d = -tmp.normal.dot(v2);
			return tmp;
		}
	};
} // namespace fragcore
#endif
