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
#include "../LinAlg.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC Transform {
	  public:
		Transform();
		Transform(const Vector3 &position, const Quaternion &rotation, const Vector3 &scale);
		Transform(const Transform &other);

		void rotate(const Vector3 &eular);

		void setPosition(const Vector3 &pos);

		void setLocalPosition(const Vector3 &pos);

		inline Vector3 getPosition() { return this->pos; }

		inline const Vector3 &getPosition() const { return this->pos; }

		inline Vector3 getLocalPosition() const { return Vector3(); }

		void setScale(const Vector3 &scale);

		void setLocalScale(const Vector3 &scale);

		inline Vector3 getScale() const { return this->scale; }

		Vector3 getLocalScale() const { return Vector3(); }

		void setRotation(const Quaternion &quat);

		void setLocalRotation(const Quaternion &quat);

		const Quaternion &getRotation() const;

		Quaternion getLocalRotation() const;

		Matrix4x4 getMatrix() const;

		Matrix4x4 getLocalMatrix() const;

		Matrix4x4 getViewMatrix() const;

		Matrix4x4 getViewLocalMatrix() const;

		Matrix3x3 &getBasis();

		const Matrix3x3 &getBasis() const;

		Transform &operator*=(const Transform &t);

		Transform operator*(const Transform &t) const;

		Vector3 operator()(const Vector3 &x) const;

		Vector3 operator*(const Vector3 &x) const;

		Quaternion operator*(const Quaternion &q) const;

		// Transform &operator=(const Transform &other);

	  protected: /*  Internal methods.   */
		void updateModelMatrix();

	  private:			 /*	Attributes.	*/
		Vector3 pos;	 /*	Position in world space.	*/
		Vector3 scale;	 /*	Scale.	*/
		Quaternion quat; /*	Rotation in world space.	*/
		Matrix4x4 model; /*  Precomputed model matrix.   */
	};

} // namespace fragcore
#endif