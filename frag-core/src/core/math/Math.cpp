#include "Core/Math/Math.h"

#include "open-simplex-noise.h"
using namespace fragcore;

static struct osn_context *ctx = nullptr;

float Math::PerlinNoise(const Vector2 &point) noexcept { return Math::PerlinNoise(point.x(), point.y()); }

float Math::PerlinNoise(const float x, const float y) noexcept {
	if (unlikely(ctx == nullptr)) {
		open_simplex_noise(0, &ctx);
	}
	return open_simplex_noise2(ctx, x, y);
}

float Math::PerlinNoise(const float x, const float y, const float z) noexcept {
	if (unlikely(ctx == nullptr)) {
		open_simplex_noise(0, &ctx);
	}

	return open_simplex_noise3(ctx, x, y, z);
}

float Math::PerlinNoise(const Vector3 &point) noexcept { return Math::PerlinNoise(point.x(), point.y(), point.z()); }