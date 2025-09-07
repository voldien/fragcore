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
#ifndef _FRAGCORE_CURVE_UTIL_H_
#define _FRAGCORE_CURVE_UTIL_H_ 1
#include "../FragDef.h"
#include "Math3D/Math3D.h"

namespace fragcore {

	using KeyFrame = struct alignas(16) key_frame_t {
		float time;		  /*	*/
		float value;	  /*	*/
		float tangentIn;  /*	*/
		float tangentOut; /*	*/
	};

	class Curve {
		std::vector<KeyFrame> keyframes;
	};

	class FVDECLSPEC CurveUtil {
	  public:
		/*	*/
		template <typename T, class U>
		std::vector<U> BezierCurve(std::vector<U> &points, const int start, const float t, const int degree) {
			if (points.size() < degree) {
				return points[0];
			}
			for (int i = 0; i < degree; i++) {
				std::pow(1.0f - t, i) * pow(t, i) * points[(i + start)];
			}
			return {};
		}
		template <typename T, typename U>
		static T interpolateBezierCurve(const Vector4 &pointA, const Vector4 &pointB, const U interpolate) {
			/*	*/
		}
	};
} // namespace fragcore
#endif
