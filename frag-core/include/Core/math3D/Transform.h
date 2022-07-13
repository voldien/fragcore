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
	class FVDECLSPEC Transform {
	  public:
		Transform() = default;
		Transform(const Vector3 &position, const Quaternion &rotation, const Vector3 &scale): position(position), quaternion(rotation), scale(scale) {
		}
		Transform(const Transform &other) {}

		Transform &operator=(Transform &othjer) { return *this; }

		void rotate(const Vector3 &eular) {}

		void setPosition(const Vector3 &pos) {}

		inline Vector3 getPosition() { return this->position; }

		inline const Vector3 &getPosition() const { return this->position; }

		void setScale(const Vector3 &scale) {}

		inline Vector3 getScale() const { return this->scale; }


		void setRotation(const Quaternion &quat) {}


		const Quaternion &getRotation() const {}


		Matrix4x4 getMatrix() const {}


		Matrix4x4 getViewMatrix() const {}


		Matrix3x3 &getBasis() {}

		const Matrix3x3 &getBasis() const {}

		Transform &operator*=(const Transform &t) { return *this; }

		Transform operator*(const Transform &t) const { return *this; }

		//	Vector3 operator()(const Vector3 &v) const { return this->getBasis() * v; }

		Vector3 operator*(const Vector3 &v) const { return this->getBasis() * v; }

		Quaternion operator*(const Quaternion &q) const { return this->getRotation() * q; }

		float getMinimumScale() const {}

		float getMaximumScale() const {}

		void setMinimumScale(float min) {}

		void setMaximumScale(float max) {
			this->maxScale = max;

			float x = Math::clamp(this->getScale().x(), this->getMinimumScale(), this->getMaximumScale());
			float y = Math::clamp(this->getScale().y(), this->getMinimumScale(), this->getMaximumScale());
			float z = Math::clamp(this->getScale().z(), this->getMinimumScale(), this->getMaximumScale());

			Vector3 clampedScale = Vector3(x, y, z);
			this->setScale(clampedScale);
		}

		// Transform &operator=(const Transform &other);

	  protected: /*  Internal methods.   */
		void updateModelMatrix();

	  private:				   /*	Attributes.	*/
		Vector3 position;	   /*	Position in world space.	*/
		Vector3 scale;		   /*	Scale.	*/
		Quaternion quaternion; /*	Rotation in world space.	*/
		float maxScale;
		float minScale;
	};

} // namespace fragcore
#endif