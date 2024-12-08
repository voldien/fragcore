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

AABB GeometryUtility::computeBoundingBox(const AABB &aabbs, const Matrix4x4 &matrix) noexcept {

	const Vector3 globalCenter =
		(matrix * Vector4(aabbs.getCenter().x(), aabbs.getCenter().y(), aabbs.getCenter().z(), 1)).head(3);

	const Vector3 right = (matrix * Vector4(1, 0, 0, 0)).head(3).normalized() * aabbs.getSize().x();
	const Vector3 up = (matrix * Vector4(0, 1, 0, 0)).head(3).normalized() * aabbs.getSize().y();
	const Vector3 forward = (matrix * Vector4(0, 0, 1, 0)).head(3).normalized() * aabbs.getSize().z();

	const float newIi = std::abs(Vector3{1.f, 0.f, 0.f}.dot(right)) + std::abs(Vector3{1.f, 0.f, 0.f}.dot(up)) +
						std::abs(Vector3{1.f, 0.f, 0.f}.dot(forward));

	const float newIj = std::abs(Vector3{0.f, 1.f, 0.f}.dot(right)) + std::abs(Vector3{0.f, 1.f, 0.f}.dot(up)) +
						std::abs(Vector3{0.f, 1.f, 0.f}.dot(forward));

	const float newIk = std::abs(Vector3{0.f, 0.f, 1.f}.dot(right)) + std::abs(Vector3{0.f, 0.f, 1.f}.dot(up)) +
						std::abs(Vector3{0.f, 0.f, 1.f}.dot(forward));

	return AABB(Vector3(newIi, newIj, newIk), globalCenter);
}

BoundingSphere GeometryUtility::computeBoundingSphere(float *vertices, const size_t nrVertices, const size_t stride) {

	const AABB aabb = GeometryUtility::computeBoundingBox((Vector3 *)vertices, nrVertices, stride);
	const Vector3 center = aabb.getCenter();
	const float radius = aabb.getSize().norm();
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
