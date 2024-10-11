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
#ifndef _FRAG_CORE_TRIANGLE_H_
#define _FRAG_CORE_TRIANGLE_H_ 1
#include "Plane.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Triangle {
	  public:
		Triangle();
		Triangle(const Triangle &triangle);
		Triangle(const Vector3 &point1, const Vector3 &point2, const Vector3 &point3);

		Triangle &operator=(const Triangle &triangle) = default;

		/**
		 *	Get normal from right hand.
		 *
		 *	@Return
		 */
		Vector3 getNormal() const noexcept;

		/**
		 *
		 *	@Return
		 */
		Vector3 getEdge0() const noexcept;

		/**
		 *
		 *	@Return
		 */
		Vector3 getEdge1() const noexcept;

		/**
		 *
		 *	@Return
		 */
		Vector3 getEdge3() const noexcept;

		/**/
		inline void setPos0(const Vector3 &pos) noexcept { p0 = pos; }

		/**/
		inline void setPos1(const Vector3 &pos) noexcept { p1 = pos; }

		/**/
		inline void setPos2(const Vector3 &pos) noexcept { p2 = pos; }

	  private:		/*	Attributes.	*/
		Vector3 p0; /**/
		Vector3 p1; /**/
		Vector3 p2; /**/
	};

	inline Triangle::Triangle() {
		p0 = Vector3::Zero();
		p1 = Vector3::Zero();
		p2 = Vector3::Zero();
	}

	inline Triangle::Triangle(const Triangle &triangle) {
		this->setPos0(triangle.p0);
		this->setPos1(triangle.p1);
		this->setPos2(triangle.p2);
	}

	inline Triangle::Triangle(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2) {
		this->p0 = p0;
		this->p1 = p1;
		this->p2 = p2;
	}

	inline Vector3 Triangle::getEdge0() const noexcept { return (p0 - p1); }

	inline Vector3 Triangle::getEdge1() const noexcept { return (p0 - p2); }

	inline Vector3 Triangle::getEdge3() const noexcept { return (p2 - p1); }

	inline Vector3 Triangle::getNormal() const noexcept { return (p0 - p1).cross(p0 - p2) / (p0 - p1).dot(p0 - p2); }
} // namespace fragcore
#endif
