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
#ifndef _FRAGCORE_TRANSFORM_H_
#define _FRAGCORE_TRANSFORM_H_ 1
#include "LinAlg.h"
#include "Math3D/Math3D.h"

namespace fragcore {

	/**
	 * @brief Construct a new fv align object
	 */
	template <typename Vec3T, typename Mat3x3, typename Mat4x4, typename QuatT>
	class FV_ALIGN(16) FVDECLSPEC Transform {
	  public:
		Transform() = default;
		Transform(const Vec3T &position, const QuatT &rotation, const Vec3T &scale);
		Transform(const Mat4x4 &transform);
		Transform(const Mat3x3 &transform);

		explicit Transform(const Mat3x3 &basis, const Vec3T &c);

		// Transform(const Transform &other);

		Transform &operator=(const Transform &other);

		void rotate(const Vec3T &eular) noexcept;
		void rotateTowards(const Vec3T &direction) noexcept;

		void setPosition(const Vec3T &position) noexcept;
		Vec3T getPosition() noexcept;
		const Vec3T &getPosition() const noexcept;

		void setScale(const Vec3T &scale) noexcept;
		Vec3T getScale() const noexcept;

		const QuatT &getRotation() const noexcept;
		Vec3T getRotationEular() const noexcept;

		void setRotation(const QuatT &quat) noexcept;
		void setRotationEular(const Vec3T& eular) noexcept;

		Transform inverse() const noexcept;

		Mat3x3 getBasis() const noexcept;

		Transform &operator*=([[maybe_unused]] const Transform &t) noexcept;

		Transform operator*([[maybe_unused]] const Transform &t) const noexcept;
		Vec3T operator*(const Vec3T &vector) const noexcept;
		QuatT operator*(const QuatT &quat) const noexcept;

		Vec3T up() const noexcept;
		Vec3T right() const noexcept;
		Vec3T forward() const noexcept;

	  private:					  /*	Attributes.	*/
		Vec3T position = Vec3T(); /*	Position in world space.	*/
		QuatT quat = QuatT();	  /*	Rotation in world space.	*/
		Vec3T scale;			  /*	Scale.	*/
		Mat3x3 basis;			  /*	*/
	};

	using TransformEigen = Transform<Vector3, Matrix3x3, Matrix4x4, Quaternion>;

} // namespace fragcore
#endif
