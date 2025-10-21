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
#include <initializer_list>

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC GeometryUtility {
	  public:
		template <typename T> static constexpr bool testPlanesAABB(const Plane<T> &plane, const AABB &bound) noexcept {

			const float rad = glm::dot(glm::abs(plane.getNormal()), bound.getHalfSize());
			return -rad <= plane.distanceSigned(bound.getCenter());
		}

		/**
		 * @brief
		 */
		template <typename T>
		static constexpr bool testPlanesSphere(const Plane<T> &plane, const BoundingSphere &bound) noexcept {
			const T distance = plane.distanceSigned(bound.getCenter());
			return distance > -bound.getRadius();
		}

		template <typename T> static constexpr bool TestPlanesOBB(const Plane<T> &plane, const OBB &bound) noexcept {
			return true;
		}

		/**
		 * @brief Positive
		 */
		template <typename T>
		static constexpr bool testPlanesPoint(const Plane<T> &plane, const Vector3 &point) noexcept {
			return plane.distanceSigned(point) > 0;
		}

		/**
		 * @brief Positive
		 */
		template <typename T>
		static constexpr bool testPlanesPlane(const Plane<T> &plane0, const Plane<T> &plane1) noexcept {
			return  true;
		}


	  public:
		/**
		 * @brief
		 */
		static AABB computeBoundingBox(const Vector3 *vertices, const size_t nrVertices, const size_t stride);

		/**
		 * @brief
		 */
		static AABB computeBoundingBox(const std::initializer_list<AABB &> &aabbs) noexcept;

		/**
		 * @brief
		 */
		static AABB computeBoundingBox(const AABB &aabbs, const Matrix4x4 &matrix) noexcept {

			const Vector4 globalCenter = (matrix * Vector4(aabbs.getCenter(), 1));

			/*	*/
			const Vector3 right = glm::normalize(Vector3(matrix * Vector4(1, 0, 0, 0))) * aabbs.getHalfSize().x;
			const Vector3 up = glm::normalize(Vector3(matrix * Vector4(0, 1, 0, 0))) * aabbs.getHalfSize().y;
			const Vector3 forward = glm::normalize(Vector3(matrix * Vector4(0, 0, 1, 0))) * aabbs.getHalfSize().z;

			/*	*/
			const float newIi = std::abs(glm::dot(Vector3{1.f, 0.f, 0.f}, right)) +
								std::abs(glm::dot(Vector3{1.f, 0.f, 0.f}, up)) +
								std::abs(glm::dot(Vector3{1.f, 0.f, 0.f}, forward));

			const float newIj = std::abs(glm::dot(Vector3{0.f, 1.f, 0.f}, right)) +
								std::abs(glm::dot(Vector3{0.f, 1.f, 0.f}, up)) +
								std::abs(glm::dot(Vector3{0.f, 1.f, 0.f}, forward));

			const float newIk = std::abs(glm::dot(Vector3{0.f, 0.f, 1.f}, right)) +
								std::abs(glm::dot(Vector3{0.f, 0.f, 1.f}, up)) +
								std::abs(glm::dot(Vector3{0.f, 0.f, 1.f}, forward));

			return AABB(Vector3(newIi, newIj, newIk), Vector3(globalCenter));
		}

		/**
		 * @brief
		 *
		 */
		static BoundingSphere computeBoundingSphere(const float *vertices, const size_t nrVertices,
													const size_t stride = sizeof(float) * 3);

		static BoundingSphere computeBoundingSphere(const std::vector<BoundingSphere &> &spheres) noexcept;

		/**
		 * @brief
		 *
		 */
		static OBB computeBoundingOBB(const float *vertices, const size_t nrVertices,
									  const size_t stride = sizeof(float) * 3);

		static bool isConvex(const std::vector<Vector3> &points);
		static bool isConcave(const std::vector<Vector3> &points);

		struct Face {
			uint Indices[3];
		};

		//
		static std::vector<Triangle> subdivide(const std::vector<Triangle> &triangles);

		static std::vector<Triangle> split(const std::vector<Triangle> &triangle, const Plane<float> &plane);

		static std::vector<Triangle> createPolygon(const std::vector<Vector3> &points);

		static std::vector<Triangle> generateSmoothNormals(const std::vector<Triangle> &triangle, const float angle);

		static void optimizeGeometry();

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
