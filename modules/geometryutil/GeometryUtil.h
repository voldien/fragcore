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
#ifndef _FRAGCORE_GEOMETRYUTIL_H_
#define _FRAGCORE_GEOMETRYUTIL_H_ 1
#include "Math3D/AABB.h"
#include "Math3D/BoundingSphere.h"
#include "Math3D/Math3D.h"
#include "Math3D/OBB.h"
#include "Math3D/Plane.h"
#include <Math3D/Triangle.h>

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC GeometryUtility {
	  public:
		template <typename T> static bool testPlanesAABB(const Plane<T> &plane, const AABB &bound) {

			const float rad = glm::dot(glm::abs(plane.getNormal()), bound.getHalfSize());
			return -rad <= plane.distanceSigned(bound.getCenter());
		}

		/**
		 * @brief
		 */
		template <typename T> static bool testPlanesSphere(const Plane<T> &plane, const BoundingSphere &bound) {
			const T distance = plane.distanceSigned(bound.getCenter());
			return distance > -bound.getRadius();
		}

		template <typename T> static bool TestPlanesOBB(const Plane<T> &plane, const OBB &bound) { return true; }

		/**
		 * @brief Positive
		 */
		template <typename T> static bool testPlanesPoint(const Plane<T> &plane, const Vector3 &point) {
			return plane.distanceSigned(point) > 0;
		}

		//
		static std::vector<Triangle> subdivide(const std::vector<Triangle> &triangles);

		static std::vector<Triangle> split(const std::vector<Triangle> &triangle, const Plane<float> &plane);

		static std::vector<Triangle> createPolygon(const std::vector<Vector3> &points);

		static std::vector<Triangle> generateSmoothNormals(const std::vector<Triangle> &triangle, const float angle);

		static void optimizeGeometry();

		/**
		 * @brief
		 *
		 */
		static AABB computeBoundingBox(const Vector3 *vertices, const size_t nrVertices,
									   const size_t stride = sizeof(float) * 3);
		static AABB computeBoundingBox(const std::vector<AABB &> &aabbs) noexcept;

		static AABB computeBoundingBox(const AABB &aabbs, const Matrix4x4 &matrix) noexcept;

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

		struct Face {
			uint Indices[3];
		};

		// static void optimizeGeometry(float *vertices, const size_t nrVertices,
		// 							  const size_t stride = sizeof(float) * 3, void* indicies, const size_t nrIndices,
		// const size_t indicies_stride);

		void convert2Adjacent(float *vertices, const size_t nrVertices, std::vector<unsigned int> &Indices,
							  const size_t stride = sizeof(float) * 3);

		GeometryUtility() = delete;
		GeometryUtility(const GeometryUtility &other) = delete;
		GeometryUtility(GeometryUtility &&other) = delete;
	};
} // namespace fragcore
#endif
