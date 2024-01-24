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
#ifndef _FRAG_CORE_PROCEDURAL_GEOMETRY_H_
#define _FRAG_CORE_PROCEDURAL_GEOMETRY_H_ 1
#include "Core/Math3D.h"
#include <FragCore.h>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC ProceduralGeometry {
	  public:
		// TODO align package
		using Vertex = struct _vertex_t {
			float vertex[3];
			float uv[2];
			float normal[3];
			float tangent[3];
		};

		static void generatePlan(float scale, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices,
								 int segmentsX = 1, int segmentsY = 1);

		static void generateGrid2D();

		static void createFrustum(std::vector<Vertex> &vertices, const Matrix4x4 &projection);
		static void createFrustum(std::vector<Vertex> &vertices, const float fov, const float aspect, const float near,
								  const float far);

		static void generateSphere(float radius, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);

		static void generateCube(const float scale, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);

		static void generateWireCube(const float scale, std::vector<Vertex> &vertices,
									 std::vector<unsigned int> &indices);

		static void generateTorus(float scale, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
	};

} // namespace fragcore

#endif
