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
#include "Core/Math3D.h"
#include "Core/math3D/AABB.h"
#include "Core/math3D/BoundingSphere.h"
#include "Core/math3D/OBB.h"
#include "Core/math3D/Plane.h"
#ifndef _FRAG_CORE_GEOMETRYUTIL_H_
#define _FRAG_CORE_GEOMETRYUTIL_H_ 1
#include <Core/math3D/Triangle.h>

namespace fragcore {

	/**
	 *
	 */
	// TODO name class to match the file name.
	class FVDECLSPEC GeometryUtility {
	  public:
		static bool TestPlanesAABB(const Plane<float> &plane, const AABB &bound) { return true; }
		static bool TestPlanesSphere(const Plane<float> &plane, const AABB &bound) { return true; }
		static bool TestPlanesOBB(const Plane<float> &plane, const AABB &bound) { return true; }

		// TODO  Transform

		//
		static std::vector<Triangle> subdivide(const std::vector<Triangle> &points);

		static std::vector<Triangle> createPolygon(const std::vector<Vector3> &points);

		static std::vector<Triangle> generateSmoothNormals(const std::vector<Triangle> &points, float angle);

		static AABB computeBoundingBox(const Vector3 *vertices, const size_t nrVertices);
		static BoundingSphere computeBoundingSphere(float *vertices, const size_t nrVertices);
		static OBB computeBoundingOBB(float *vertices, const size_t nrVertices);

		static bool isConvex(const std::vector<Vector3> &points);
		static bool isConcave(const std::vector<Vector3> &points);

	  private:
		GeometryUtility() = default;
		GeometryUtility(const GeometryUtility &other) = default;
		GeometryUtility(GeometryUtility &&other) = default;
	};
} // namespace fragcore
#endif
