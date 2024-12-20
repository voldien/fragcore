#include "GeometryUtil.h"
#include "Math3D/AABB.h"
#include "Math3D/Math3D.h"
#include "Math3D/OBB.h"
#include <Math/Math.h>
#include <Math3D/LinAlg.h>
#include <cstdint>
#include <generator/SubdivideMesh.hpp>

using namespace fragcore;

std::vector<Triangle> GeometryUtility::createPolygon(const std::vector<Vector3> &points) {
	// TPPLPoly poly;
	// poly.Init(points.size());
	// for (int i = 0; i < points.size(); i++) {
	// 	poly[i] = {points[i].x(), points[i].y(), 0};
	// }
	return {};
}

bool GeometryUtility::isConvex(const std::vector<Vector3> &points) {

	if (points.size() < 3) {
		return false;
	}

	Vector3 p;
	Vector3 v;
	Vector3 u;

	int res = 0;
	for (size_t i = 0; i < points.size(); i++) {
		p = points[i];
		Vector3 tmp = points[(i + 1) % points.size()];
		v = Vector3::Zero();
		v.x() = tmp.x() - p.x();
		v.y() = tmp.y() - p.y();
		u = points[(i + 2) % points.size()];

		if (i == 0) { // in first loop direction is unknown, so save it in res
			res = u.x() * v.y() - u.y() * v.x() + v.x() * p.y() - v.y() * p.x();
		} else {
			/*	*/
			int newres = u.x() * v.y() - u.y() * v.x() + v.x() * p.y() - v.y() * p.x();
			if ((newres > 0 && res < 0) || (newres < 0 && res > 0)) {
				return false;
			}
		}
	}
	return true;
}
bool GeometryUtility::isConcave(const std::vector<Vector3> &points) { return !GeometryUtility::isConvex(points); }

AABB GeometryUtility::computeBoundingBox(const Vector3 *vertices, const size_t nrVertices, const size_t stride) {

	Vector3 min = Vector3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
						  std::numeric_limits<float>::max());
	Vector3 max = Vector3(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(),
						  std::numeric_limits<float>::min());

	const uint8_t *dataPointer = reinterpret_cast<const uint8_t *>(vertices);

	for (size_t i = 0; i < nrVertices; i++) {
		const Vector3 &vertex = *reinterpret_cast<const Vector3 *>(&dataPointer[i * stride]);

		/*	Max point.	*/
		if (vertex.x() > max.x()) {
			max.x() = vertex.x();
		}
		if (vertex.y() > max.y()) {
			max.y() = vertex.y();
		}
		if (vertex.z() > max.z()) {
			max.z() = vertex.z();
		}
		/*	Min Point.	*/
		if (vertex.x() < min.x()) {
			min.x() = vertex.x();
		}
		if (vertex.y() < min.y()) {
			min.y() = vertex.y();
		}
		if (vertex.z() < min.z()) {
			min.z() = vertex.z();
		}
	}

	return AABB::createMinMax(min, max);
}

BoundingSphere GeometryUtility::computeBoundingSphere(float *vertices, const size_t nrVertices, const size_t stride) {
	Vector3 center;
	float radius = 0;
	return BoundingSphere(center, radius);
}

OBB GeometryUtility::computeBoundingOBB(float *vertices, const size_t nrVertices, const size_t stride) {
	// PCA
	// LinAlg::PCA();

	return {};
}

void GeometryUtility::convert2Adjcent(float *vertices, const size_t nrVertices, std::vector<unsigned int> &Indices,
									  const size_t stride) {

	// Step 1 - find the two triangles that share every edge
	for (uint i = 0; i < Indices.size(); i++) {
	}

	// Step 2 - build the index buffer with the adjacency info
	for (uint i = 0; i < Indices.size(); i++) {
	}
}
