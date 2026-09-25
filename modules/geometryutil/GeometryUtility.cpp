#include "GeometryUtil.h"
#include "Math3D/AABB.h"
#include "Math3D/Math3D.h"
#include "Math3D/OBB.h"
#include <Math/Math.h>
#include <Math3D/LinAlg.h>
#include <cstdint>
#include <generator/SubdivideMesh.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#include <meshoptimizer.h>

using namespace fragcore;

std::vector<Triangle> GeometryUtility::createPolygon(const std::vector<Vector3> &points) {
	// TPPLPoly poly;
	// poly.Init(points.size());
	// for (int i = 0; i < points.size(); i++) {
	// 	poly[i] = {points[i].x, points[i].y, 0};
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
	const size_t nrPoints = points.size();
	for (size_t i = 0; i < nrPoints; i++) {

		p = points[i];
		Vector3 tmp = points[(i + 1) % nrPoints];
		v = Vector3(0);
		v.x = tmp.x - p.x;
		v.y = tmp.y - p.y;
		u = points[(i + 2) % nrPoints];

		if (i == 0) { // in first loop direction is unknown, so save it in res
			res = u.x * v.y - u.y * v.x + v.x * p.y - v.y * p.x;
		} else {

			/*	*/
			const int newres = (u.x * v.y) - (u.y * v.x) + (v.x * p.y) - (v.y * p.x);

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
		const size_t vertexOffset = i * stride;
		const Vector3 &vertex = *reinterpret_cast<const Vector3 *>(&dataPointer[vertexOffset]);

		max = glm::max(vertex, max);
		min = glm::min(vertex, min);
	}

	return AABB::createMinMax(min, max);
}

BoundingSphere GeometryUtility::computeBoundingSphere(const float *vertices, const size_t nrVertices,
													  const size_t stride) {

	const AABB aabb = GeometryUtility::computeBoundingBox((Vector3 *)vertices, nrVertices, stride);
	const Vector3 center = aabb.getCenter();
	const float radius = glm::length(aabb.getHalfSize());

	return BoundingSphere(center, radius);
}

OBB GeometryUtility::computeBoundingOBB(const float *vertices, const size_t nrVertices, const size_t stride) {
	// PCA
	// LinAlg::PCA();

	return {};
}

std::vector<unsigned int> GeometryUtility::convert2Adjacent(const void *vertices, const size_t nrVertices,
															const size_t vertex_stride, const void *Indices,
															const size_t nrIndices, const size_t Indice_stride) {

	std::vector<unsigned int> adjacency(Indice_stride * 3 * 2);

	meshopt_generateAdjacencyIndexBuffer(adjacency.data(), static_cast<const unsigned int *>(Indices), nrIndices,
										 static_cast<const float *>(vertices), nrVertices, vertex_stride);
	return adjacency;
}

void GeometryUtility::optimizeGeometry(void *vertices, const size_t nrVertices, const size_t vertices_stride,
									   void *indices, const size_t nrIndices, const size_t indice_stride) {

	//meshopt_optimizeVertexCache(static_cast<float *>(vertices), indices, nrIndices, nrVertices);
	//meshopt_optimizeVertexFetch(static_cast<float *>(vertices), indices, nrIndices, vertices, nrVertices,
	//							vertices_stride);
}