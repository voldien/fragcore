
#ifndef _FRAG_CORE_AXIS_ALIGN_BOUNDIN_BOX_H_
#define _FRAG_CORE_AXIS_ALIGN_BOUNDIN_BOX_H_ 1
#include "../../Def.h"

namespace fragcore {

	/**
	 * @brief Axis aligned bounding
	 * box.
	 */
	struct FVDECLSPEC AABB {
	  public:
		AABB() = default;
		AABB(const Vector3 &size, const Vector3 &center) {
			setCenter(center);
			setSize(size);
		}
		AABB(const AABB &bounds) { *this = bounds; }

		/**
		 *
		 * @param i
		 * @return
		 */
		inline Vector3 operator[](int i) const { return ((Vector3 *)this)[i]; }

		/**
		 *
		 * @return
		 */
		inline Vector3 &operator[](int i) { return ((Vector3 *)this)[i]; }

		/**
		 * Check if object is valid.
		 * @return true if valid, false otherwise.
		 */
		bool isValid() const noexcept {
			return !(this->mhalfsize.x() != 0.0f || this->mhalfsize.y() != 0.0f || this->mhalfsize.z() != 0.0f);
		}

		/**
		 *
		 */
		inline float maxX() const noexcept { return (this->mcenter.x() + this->mhalfsize.x()); }
		inline float minX() const noexcept { return (this->mcenter.x() - this->mhalfsize.x()); }
		inline float maxY() const noexcept { return (this->mcenter.y() + this->mhalfsize.y()); }
		inline float minY() const noexcept { return (this->mcenter.y() - this->mhalfsize.y()); }
		inline float maxZ() const noexcept { return (this->mcenter.z() + this->mhalfsize.z()); }
		inline float minZ() const noexcept { return (this->mcenter.z() - this->mhalfsize.z()); }

		/**
		 * Compute minimum position.
		 * @return vector position.
		 */
		Vector3 min() const noexcept { return this->getCenter() - this->getSize(); }

		/**
		 * Compute max position.
		 * @return vector position.
		 */
		Vector3 max() const { return this->getCenter() + this->getSize(); }

		/**
		 * Get half size.
		 * @return half size vector.
		 */
		inline const Vector3 &getSize() const { return this->mhalfsize; }

		/**
		 * Set half size.
		 */
		inline void setSize(const Vector3 &size) noexcept { this->mhalfsize = size; }

		/**
		 * Get center position.
		 * @return
		 */
		inline const Vector3 &getCenter() const noexcept { return this->mcenter; }

		/**
		 * Set center position.
		 */
		inline void setCenter(const Vector3 &center) noexcept { this->mcenter = center; }

		/**
		 * Check if object intersect.
		 * @param bounds intersect this bound.
		 * @return true if object intersects.
		 */
		bool intersect(const AABB &bounds) noexcept {
			return (this->minX() <= bounds.maxX() && this->maxX() >= bounds.minX()) &&
				   (this->minY() <= bounds.maxY() && this->maxY() >= bounds.minY()) &&
				   (this->minZ() <= bounds.maxZ() && this->maxZ() >= bounds.minZ());
		}

		/**
		 *
		 * @param point
		 * @param worldPosition
		 * @return  true if object contains.
		 */
		bool contains(const Vector3 &point, const Vector3 worldPosition = Vector3()) {
			return (point.x() > minX() + worldPosition.x() && point.x() < maxX() + worldPosition.x() &&
					point.y() > minY() + worldPosition.y() && point.y() < maxY() + worldPosition.y() &&
					point.z() > minZ() + worldPosition.z() && point.z() < maxZ() + worldPosition.z());
		}

		/**
		 * Check if object contains bound
		 * object.
		 * @param bounds
		 * @return true if completly contains, false otherwise.
		 */
		constexpr bool contains(const AABB &bounds) { return false; }

		/**
		 *
		 * @param normal
		 * @return
		 */
		Vector3 getVertexN(const Vector3 &normal) const noexcept {
			Vector3 res = this->mhalfsize;
			if (normal.x() < 0.0f) {
				res[0] = res.x() + this->mcenter.x();
			}
			if (normal.y() < 0.0f) {
				res[1] = res.y() + this->mcenter.y();
			}
			if (normal.z() < 0.0f) {
				res[2] = res.z() + this->mcenter.z();
			}
			return res;
		}

		/**
		 *
		 * @param normal
		 * @return
		 */
		Vector3 getVertexP(const Vector3 &normal) const noexcept {
			Vector3 res = this->mhalfsize;
			if (normal.x() >= 0.0f) {
				res[0] = res.x() + this->mcenter.x();
			}
			if (normal.y() >= 0.0f) {
				res[1] = res.y() + this->mcenter.y();
			}
			if (normal.z() >= 0.0f) {
				res[2] = res.z() + this->mcenter.z();
			}
			return res;
		}

		/**
		 * Create input stream for creating AABB
		 * from input stream.
		 * @return stream reference.
		 */
		// friend std::istream &operator>>(std::istream &is, AABB &t) {}

		/**
		 * Create output stream of AABB values.
		 * @return stream reference.
		 */
		// friend std::ostream &operator<<(std::ostream &os, const AABB &t) {}

		/**
		 * Assign bound object.
		 * @return reference of object.
		 */
		AABB &operator=(const AABB &bound) {
			this->mhalfsize = bound.mhalfsize;
			this->mcenter = bound.mcenter;
			return *this;
		}

		/**
		 *
		 * @return reference of object.
		 */
		friend AABB operator*(const AABB &bound, float scalar) noexcept {
			return AABB(bound.getCenter(), bound.getSize() * scalar);
		}

		/**
		 * Divide the size by scalar.
		 * @param bound
		 * @param divisor
		 * @return reference of object.
		 */
		friend AABB operator/(const AABB &bound, float divisor) noexcept(noexcept(divisor == 0)) {
			return AABB(bound.getCenter(), bound.getSize() / divisor);
		}

		/**
		 * Factor the bound size of the bound.
		 * @param scalar real number.
		 * @return reference of object.
		 */
		AABB &operator*=(float scalar) noexcept {
			this->setSize(getSize() * scalar);
			return *this;
		}

		/**
		 * Divide the size by scalar.
		 * @param divisor non-zero.
		 * @return reference of object.
		 */
		AABB &operator/=(float divisor) noexcept {
			this->setSize(getSize() / divisor);
			return *this;
		}

		/**
		 * Compare if objects are equal.
		 * @param bound
		 * @return true if object are equal, false otherwise.
		 */
		bool operator==(const AABB &bound) noexcept {
			return (this->getCenter() == bound.getCenter()) && (this->getSize() == bound.getSize());
		}

		/**
		 * Compare if object are not equal.
		 * @param bound
		 * @return false if object are equal, true otherwise.
		 */
		bool operator!=(const AABB &bound) noexcept {
			return (this->getCenter() != bound.getCenter()) || (this->getSize() != bound.getSize());
		}

	  private:			   /*	Attributes.	*/
		Vector3 mhalfsize; /*	half size of the box.	*/
		Vector3 mcenter;   /*	center of the box.	*/
	};
} // namespace fragcore

#endif
