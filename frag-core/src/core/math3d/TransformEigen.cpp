#include "Math3D/Transform.h"

using namespace fragcore;

template class fragcore::Transform<Vector3, Matrix3x3, Matrix4x4, Quaternion>;

template <>
Transform<Vector3, Matrix3x3, Matrix4x4, Quaternion>::Transform(const Vector3 &position, const Quaternion &rotation,
																const Vector3 &scale)
	: position(position), quat(rotation), scale(scale) {}
template <> Transform<Vector3, Matrix3x3, Matrix4x4, Quaternion>::Transform(const Matrix4x4 &transform) {}
template <> Transform<Vector3, Matrix3x3, Matrix4x4, Quaternion>::Transform(const Matrix3x3 &transform) {}

// explicit Transform(const Matrix3x3 &basis, const Vector3 &c = Vector3::Zero()) {}

// template <> Transform<Vector3, Matrix3x3, Matrix4x4, Quaternion>::Transform(const Transform &other) {
// 	this->position = other.position;
// 	this->quat = other.quat;
// 	this->scale = other.scale;
// }

template <> Transform<Vector3, Matrix3x3, Matrix4x4, Quaternion> &TransformEigen::operator=(const Transform &other) {
	this->position = other.position;
	this->quat = other.quat;
	this->scale = other.scale;
	return *this;
}

template <> void TransformEigen::rotate([[maybe_unused]] const Vector3 &eular) noexcept { this->quat = Quaternion(); }

template <> void TransformEigen::setPosition(const Vector3 &position) noexcept { this->position = position; }

template <> Vector3 TransformEigen::getPosition() noexcept { return this->position; }

template <> const Vector3 &TransformEigen::getPosition() const noexcept { return this->position; }

template <> void TransformEigen::setScale(const Vector3 &scale) noexcept { this->scale = scale; }

template <> Vector3 TransformEigen::getScale() const noexcept { return this->scale; }

template <> void TransformEigen::setRotation(const Quaternion &quat) noexcept { this->quat = quat; }

template <> Transform<Vector3, Matrix3x3, Matrix4x4, Quaternion> TransformEigen::inverse() const noexcept {
	Transform transform;
	// Matrix3x3 inv = this->getBasis().transpose();
	return transform;
}

template <> const Quaternion &TransformEigen::getRotation() const noexcept { return this->quat; }

template <> Matrix3x3 TransformEigen::getBasis() const noexcept { return this->quat.matrix(); }

template <>
Transform<Vector3, Matrix3x3, Matrix4x4, Quaternion> &
TransformEigen::operator*=([[maybe_unused]] const Transform &t) noexcept {
	// Matrix3x3 basis = this->getBasis() * t.getBasis();
	return *this;
}

template <>
Transform<Vector3, Matrix3x3, Matrix4x4, Quaternion>
TransformEigen::operator*([[maybe_unused]] const Transform &t) const noexcept {
	// Matrix3x3 basis = this->getBasis() * t.getBasis();

	return *this;
}

template <> Vector3 TransformEigen::operator*(const Vector3 &vector) const noexcept {
	return this->getBasis() * vector;
}

template <> Quaternion TransformEigen::operator*(const Quaternion &quat) const noexcept {
	return this->getRotation() * quat;
}

template <> Vector3 TransformEigen::up() const noexcept { return Vector3(0, 1, 0); }
template <> Vector3 TransformEigen::right() const noexcept { return Vector3(1, 0, 0); }
template <> Vector3 TransformEigen::forward() const noexcept { return Vector3(0, 0, 1); }