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
#ifndef _FRAG_CORE_TRANSFORM_H_
#define _FRAG_CORE_TRANSFORM_H_ 1
#include "LinAlg.h"

namespace fragcore {

	/**
	 * @brief Construct a new fv align object
	 *
	 */
	class FV_ALIGN(16) FVDECLSPEC Transform {
	  public:
		Transform() = default;
		Transform(const Vector3 &position, const Quaternion &rotation, const Vector3 &scale)
			: position(position), quaternion(rotation), scale(scale) {}

		explicit Transform(const Matrix3x3 &basis, const Vector3 &c = Vector3::Zero()) {}

		Transform(const Transform &other) {
			this->position = other.position;
			this->quaternion = other.quaternion;
			this->scale = other.scale;
		}

		Transform &operator=(const Transform &other) {
			this->position = other.position;
			this->quaternion = other.quaternion;
			this->scale = other.scale;
			return *this;
		}

		FV_ALWAYS_INLINE void rotate(const Vector3 &eular) noexcept { this->quaternion = Quaternion(); }

		FV_ALWAYS_INLINE void setPosition(const Vector3 &position) noexcept { this->position = position; }

		FV_ALWAYS_INLINE Vector3 getPosition() noexcept { return this->position; }

		FV_ALWAYS_INLINE const Vector3 &getPosition() const noexcept { return this->position; }

		FV_ALWAYS_INLINE void setScale(const Vector3 &scale) noexcept { this->scale = scale; }

		FV_ALWAYS_INLINE Vector3 getScale() const noexcept { return this->scale; }

		FV_ALWAYS_INLINE void setRotation(const Quaternion &quat) noexcept { this->quaternion = quat; }

		FV_ALWAYS_INLINE Transform inverse() const {
			Transform transform;
			Matrix3x3 inv = this->getBasis().transpose();
			return transform;
		}

		FV_ALWAYS_INLINE const Quaternion &getRotation() const noexcept { return this->quaternion; }

		FV_ALWAYS_INLINE const Matrix3x3 getBasis() const noexcept { return this->quaternion.matrix(); }

		FV_ALWAYS_INLINE Transform &operator*=(const Transform &t) {
			Matrix3x3 basis = this->getBasis() * t.getBasis();
			return *this;
		}

		FV_ALWAYS_INLINE Transform operator*(const Transform &t) const {
			Matrix3x3 basis = this->getBasis() * t.getBasis();

			return *this;
		}

		FV_ALWAYS_INLINE Vector3 operator*(const Vector3 &v) const { return this->getBasis() * v; }

		FV_ALWAYS_INLINE Quaternion operator*(const Quaternion &q) const { return this->getRotation() * q; }

	  private:				   /*	Attributes.	*/
		Vector3 position;	   /*	Position in world space.	*/
		Quaternion quaternion; /*	Rotation in world space.	*/
		Vector3 scale;		   /*	Scale.	*/
		Matrix3x3 basis;
	};

} // namespace fragcore
#endif