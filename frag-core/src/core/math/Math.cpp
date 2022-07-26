#include "Core/Math.h"

#include "open-simplex-noise.h"
using namespace fragcore;

static struct osn_context *ctx = nullptr;

// void Window::calculateGammaLookupTable(float gamma, ushort *rgbRamp) const
// {
// 	if (gamma < 0.0)
// 		throw InvalidArgumentException("gamma exponent must be positive");

// 	float exponent = 1.0f / gamma;

// 	/*  Create lookup table.    */
// 	for (uint i = 0; i < 256; ++i)
// 	{
// 		/*  */
// 		float linear = float(i) * 1.0f / 255u;
// 		float corrected = (float)pow(linear, exponent);
// 		ushort entry = ushort(corrected * 65535);

// 		rgbRamp[i] = rgbRamp[i + 256] = rgbRamp[i + 512] = entry;
// 	}
// }

// float Window::computeGammaExponent(const ushort *rgbRamp) const
// {
// 	float gamma = 0.0;

// 	//TODO improve
// 	for (uint i = 64; i < 256; ++i)
// 	{
// 		double corrected = (double)rgbRamp[i] / 65535.0;
// 		double linear = double(i) * 1.0 / 255u;
// 		gamma += log(linear) / log(corrected);
// 		break;
// 	}
// 	return gamma;
// }

float Math::PerlinNoise(const Vector2 &point) noexcept { return Math::PerlinNoise(point.x(), point.y()); }

float Math::PerlinNoise(float x, float y) noexcept {
	if (unlikely(ctx == nullptr)) {
		open_simplex_noise(0, &ctx);
	}
	return open_simplex_noise2(ctx, x, y);
}

float Math::PerlinNoise(float x, float y, float z) noexcept {
	if (unlikely(ctx == nullptr)) {
		open_simplex_noise(0, &ctx);
	}

	return open_simplex_noise3(ctx, x, y, z);
}

float Math::PerlinNoise(const Vector3 &point) noexcept { return Math::PerlinNoise(point.x(), point.y(), point.z()); }