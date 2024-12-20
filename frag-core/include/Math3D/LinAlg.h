
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
#ifndef _FRAGCORE_LIN_ALG_H_
#define _FRAGCORE_LIN_ALG_H_ 1
#include "../Math/Math.h"
#include <cfloat>
#include <cmath>
#include <vector>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	typedef float fvvec1f FV_VECTORALIGN(4);
	typedef float fvvec2f FV_VECTORALIGN(8);
	typedef float fvvec4f FV_VECTORALIGN(16);

	/**
	 * @brief
	 *
	 */
	typedef double fvvec1d FV_VECTORALIGN(8);
	typedef double fvvec2d FV_VECTORALIGN(16);
	typedef double fvvec4d FV_VECTORALIGN(32);

	using Bound = union bound_t {
		struct aabb {
			fvvec4f min;
			fvvec4f max;
		} aabb;
		struct sphere_t {
			fvvec4f center;
			float radius;
		} sphere;
		struct obb_t {
			fvvec4f u;
			fvvec4f v;
			fvvec4f w;
		} obb;
	};

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC LinAlg {
	  public:
		template <typename T> static std::vector<T> PCA(std::vector<T> &p) {
			float nInverse = (1.0f / p.size());
			T m = nInverse * fragcore::Math::sum<T>(p);
			// Matrix3x3 C = nInverse;
		}
	};
} // namespace fragcore

#endif
