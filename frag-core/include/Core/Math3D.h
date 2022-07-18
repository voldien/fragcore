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
#ifndef _FRAG_CORE_MATH_3D_H_
#define _FRAG_CORE_MATH_3D_H_ 1

//#define FRAGCORE_USE_HPMCPP
#define FRAGCORE_USE_EIGEN


#ifdef FRAGCORE_USE_HPMCPP
#include <Ext/HCPlane.hpp>
#include <HCQuaternion.hpp>
#include <HCVector2.hpp>
#include <HCVector3.hpp>
#include <HCVector4.hpp>
#include <Hpm.hpp>
namespace fragcore {
	using Vector3 = LIBHPM::Vector3;
	using Vector4 = LIBHPM::Vector4;
	using Vector2 = LIBHPM::Vector2;
	using Matrix4x4 = LIBHPM::Matrix4x4;
	using Matrix3x3 = LIBHPM::Matrix3x3;
	using Quaternion = LIBHPM::Quaternion;
	using Plane = LIBHPM::Plane;
	using AABB = LIBHPM::AABB;
	using BoundingSphere = LIBHPM::BoundingSphere;
	using OBB = LIBHPM::OBB;
	using Ray = LIBHPM::Ray;
} // namespace fragcore
#elif defined(FRAGCORE_USE_EIGEN)
#include <Eigen/Dense>
namespace fragcore {
	using Vector3 = Eigen::Vector3f;
	using Vector4 = Eigen::Vector4f;
	using Vector2 = Eigen::Vector2f;
	using Matrix4x4 = Eigen::Matrix4f;
	using Matrix3x3 = Eigen::Matrix3f;
	using Quaternion = Eigen::Quaternionf;
} // namespace fragcore
#endif
/*	Vector data types for rendering geometries.	*/
#endif