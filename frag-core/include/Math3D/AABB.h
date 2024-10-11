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
#ifndef _FRAG_CORE_AXIS_ALIGN_BOUNDIN_BOX_H_
#define _FRAG_CORE_AXIS_ALIGN_BOUNDIN_BOX_H_ 1
#include "../FragDef.h"
#include "Math3D/Math3D.h"

namespace fragcore {

	/**
	 * @brief Axis aligned bounding
	 * box.
	 */
	struct FVDECLSPEC AABB {
	  public:
		AABB() = default;
		AABB(const Vector3 &size, const Vector3 &center) {
			this->setCenter(center);
			this->setSize(size);
		}
		AABB(const AABB &bounds) { *this = bounds; }

		/**
		 *
		 * @param i
		 * @return
		 */
		inline Vector3 operator[](const int index) const { return ((Vector3 *)this)[index]; }

		/**
		 *
		 * @return
		 */
		inline Vector3 &operator[](const int index) { return ((Vector3 *)this)[index]; }

		/**
		 * Check if object is valid.
		 * @return true if valid, false otherwise.
		 */
		bool isValid() const noexcept {
			return !(this->mhalfsize.x() != 0.0f || this->mhalfsize.y() != 0.0f || this->mhalfsize.z() != 0.0f);
		}

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
		Vector3 max() const noexcept { return this->getCenter() + this->getSize(); }

		/**
		 * Get half size.
		 * @return half size vector.
		 */
		inline const Vector3 &getSize() const noexcept { return this->mhalfsize; }

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
		bool contains(const Vector3 &point, const Vector3 worldPosition = Vector3::Zero()) const noexcept {
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
		constexpr bool contains(const AABB &bounds) const noexcept { return false; }

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
		friend AABB operator*(const AABB &bound, const float scalar) noexcept {
			return {bound.getCenter(), bound.getSize() * scalar};
		}

		/**
		 * Divide the size by scalar.
		 * @param bound
		 * @param divisor
		 * @return reference of object.
		 */
		friend AABB operator/(const AABB &bound, const float divisor) noexcept(noexcept(divisor == 0)) {
			return {bound.getCenter(), bound.getSize() / divisor};
		}

		/**
		 * Factor the bound size of the bound.
		 * @param scalar real number.
		 * @return reference of object.
		 */
		AABB &operator*=(const float scalar) noexcept {
			this->setSize(this->getSize() * scalar);
			return *this;
		}

		/**
		 * Divide the size by scalar.
		 * @param divisor non-zero.
		 * @return reference of object.
		 */
		AABB &operator/=(const float divisor) noexcept {
			this->setSize(this->getSize() / divisor);
			return *this;
		}

		/**
		 * Compare if objects are equal.
		 * @param bound
		 * @return true if object are equal, false otherwise.
		 */
		bool operator==(const AABB &bound) const noexcept {
			return (this->getCenter() == bound.getCenter()) && (this->getSize() == bound.getSize());
		}

		/**
		 * Compare if object are not equal.
		 * @param bound
		 * @return false if object are equal, true otherwise.
		 */
		bool operator!=(const AABB &bound) const noexcept {
			return (this->getCenter() != bound.getCenter()) || (this->getSize() != bound.getSize());
		}

		static AABB createMinMax(const Vector3 &min, const Vector3 &max) noexcept {
			const Vector3 size = (max - min);
			const Vector3 center = (min - max) / 2.0f;
			return {size, center};
		}

	  private:			   /*	Attributes.	*/
		Vector3 mhalfsize; /*	half size of the box.	*/
		Vector3 mcenter;   /*	center of the box.	*/
	};
} // namespace fragcore

#endif
