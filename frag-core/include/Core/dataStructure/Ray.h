
#ifndef _FRAG_CORE_RAY_H_
#define _FRAG_CORE_RAY_H_ 1
#include "../../Def.h"

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
