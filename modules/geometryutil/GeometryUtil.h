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
#ifndef _FRAG_CORE_GEOMETRYUTIL_H_
#define _FRAG_CORE_GEOMETRYUTIL_H_ 1
#include "Core/Math3D.h"
#include "Core/math3D/AABB.h"
#include "Core/math3D/BoundingSphere.h"
#include "Core/math3D/OBB.h"
#include "Core/math3D/Plane.h"
#include <Core/math3D/Triangle.h>

namespace fragcore {

	/**
	 *
	 */
	// TODO name class to match the file name.
	class FVDECLSPEC GeometryUtility {
	  public:
		template <typename T> static bool TestPlanesAABB(const Plane<T> &plane, const AABB &bound) {
			Vector3 p = bound.min();
			Vector3 n = bound.max();
			Vector3 N = plane.getNormal();

			if (N.x() >= 0) {
				p[0] = bound.max().x();
				n[0] = bound.min().x();
			}
			if (N.y() >= 0) {
				p[1] = bound.max().y();
				n[1] = bound.min().y();
			}
			if (N.z() >= 0) {
				p[2] = bound.max().z();
				n[2] = bound.min().z();
			}

			const float r = bound.getSize().dot(Vector3(
				std::abs(plane.getNormal().x()), std::abs(plane.getNormal().y()), std::abs(plane.getNormal().z())));
			return -r <= plane.distanceSigned(bound.getCenter());

			if (plane.distance(p) < 0.0f) {
				return false;
			}
			if (plane.distance(n) < 0.0f) {
				return true;
			}
			return true;
		}

		template <typename T> static bool TestPlanesSphere(const Plane<T> &plane, const BoundingSphere &bound) {

			T distance = plane.distance(bound.getCenter());

			return distance > -bound.getRadius();
		}

		template <typename T> static inline bool TestPlanesOBB(const Plane<T> &plane, const OBB &bound) { return true; }

		template <typename T> static inline bool TestPlanesPoint(const Plane<T> &plane, const Vector3 &point) {
			return plane.distance(point) > 0;
		}

		// TODO  Transform

		//
		static std::vector<Triangle> subdivide(const std::vector<Triangle> &triangles);

		static std::vector<Triangle> split(const std::vector<Triangle> &triangle, const Plane<float> &plane);

		static std::vector<Triangle> createPolygon(const std::vector<Vector3> &points);

		static std::vector<Triangle> generateSmoothNormals(const std::vector<Triangle> &triangle, const float angle);

		/**
		 * @brief
		 *
		 */
		static AABB computeBoundingBox(const Vector3 *vertices, const size_t nrVertices,
									   const size_t stride = sizeof(float) * 3);

		static AABB computeBoundingBox(const std::vector<AABB &> &aabbs) noexcept;
		/**
		 * @brief
		 *
		 */
		static BoundingSphere computeBoundingSphere(float *vertices, const size_t nrVertices,
													const size_t stride = sizeof(float) * 3);

		static BoundingSphere computeBoundingBox(const std::vector<BoundingSphere &> &spheres) noexcept;

		/**
		 * @brief
		 *
		 */
		static OBB computeBoundingOBB(float *vertices, const size_t nrVertices,
									  const size_t stride = sizeof(float) * 3);

		static bool isConvex(const std::vector<Vector3> &points);
		static bool isConcave(const std::vector<Vector3> &points);

	  private:
		GeometryUtility() = default;
		GeometryUtility(const GeometryUtility &other) = default;
		GeometryUtility(GeometryUtility &&other) = default;
	};
} // namespace fragcore
#endif
