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
#ifndef _FRAGCORE_MATH_3D_H_
#define _FRAGCORE_MATH_3D_H_ 1

/*
 * #define FRAGCORE_USE_EIGEN
 * #define FRAGCORE_USE_GLM
 */
#if defined(FRAGCORE_USE_EIGEN)
#include <Eigen/Dense>
namespace fragcore {
	using Vector3 = Eigen::Vector3f;
	using Vector4 = Eigen::Vector4f;
	using Vector2 = Eigen::Vector2f;
	using Matrix4x4 = Eigen::Matrix4f;
	using Matrix3x3 = Eigen::Matrix3f;
	using Quaternion = Eigen::Quaternionf;
} // namespace fragcore

#elif defined(FRAGCORE_USE_GLM)
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
namespace fragcore {
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;
	using Vector2 = glm::vec2;
	using Matrix4x4 = glm::mat4x4;
	using Matrix3x3 = glm::mat3x3;
	using Quaternion = glm::quat;

} // namespace fragcore
#endif
/*	Vector data types for rendering geometries.	*/
#endif
