#pragma once
//#define FRAGCORE_USE_HPMCPP_VECTORS
#ifdef FRAGCORE_USE_HPMCPP_VECTORS
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
#else
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
