#include "Core/Math.h"
#include "Utils/GeometryUtil.h"

using namespace fragcore;

std::vector<Triangle> GeometryUtility::createPolygon(const std::vector<Vector3> &points) {
	// TPPLPoly poly;
	// poly.Init(points.size());
	// for (int i = 0; i < points.size(); i++) {
	// 	poly[i] = {points[i].x(), points[i].y(), 0};
	// }
	return {};
}

bool GeometryUtility::isConvex(std::vector<Vector3> &polygon) {
	if (polygon.size() < 3)
		return false;

	Vector3 p;
	Vector3 v;
	Vector3 u;
	int res = 0;
	for (size_t i = 0; i < polygon.size(); i++) {
		p = polygon[i];
		Vector3 tmp = polygon[(i + 1) % polygon.size()];
		v = Vector3::Zero();
		v.x() = tmp.x() - p.x();
		v.y() = tmp.y() - p.y();
		u = polygon[(i + 2) % polygon.size()];

		if (i == 0) // in first loop direction is unknown, so save it in res
			res = u.x() * v.y() - u.y() * v.x() + v.x() * p.y() - v.y() * p.x();
		else {
			int newres = u.x() * v.y() - u.y() * v.x() + v.x() * p.y() - v.y() * p.x();
			if ((newres > 0 && res < 0) || (newres < 0 && res > 0))
				return false;
		}
	}
	return true;
}
bool GeometryUtility::isConcave(std::vector<Vector3> &points) { return !isConvex(points); }
