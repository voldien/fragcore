#include "Core/Math.h"
#include <float.h>
#include <hpm.h>
#include <math.h>
#include <open-simplex-noise.h>
using namespace fragcore;

static struct osn_context *ctx = NULL;

void Math::guassian1Df(float *guassian, int width, double theta) {}

void Math::guassian1Df(std::vector<float> &guassian, int height, float theta) {}

void Math::guassian2Df(std::vector<float> &guassian, int height, float theta) {}

void Math::guassian2Df(float *guassian, int height, float theta) {}

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

float Math::linearToGammaSpace(float linear) {
	const float exponent = 2.2f;
	return (float)pow(linear, exponent);
	// 	double corrected = (double)rgbRamp[i] / 65535.0;
	// 	double linear = double(i) * 1.0 / 255u;
	// 	gamma += log(linear) / log(corrected);
	return 0;
}

float Math::GameSpaceToLinear(float gamma, float exp) { return 0; }

float Math::gammaCorrection(float linear, float exp) { return powf(linear, exp); }

Color Math::CorrelatedColorTemperatureToRGB(float kelvin) {
	float temp = kelvin / 100;

	float red, green, blue;

	if (temp <= 66) {

		red = 255;

		green = temp;
		green = 99.4708025861 * log(green) - 161.1195681661;

		if (temp <= 19) {

			blue = 0;
		} else {

			blue = temp - 10;
			blue = 138.5177312231 * log(blue) - 305.0447927307;
		}
	} else {

		red = temp - 60;
		red = 329.698727446 * pow(red, -0.1332047592);

		green = temp - 60;
		green = 288.1221695283 * pow(green, -0.0755148492);

		blue = 255;
	}

	return Color(clamp(red, 0, 255) / 255.0f, clamp(green, 0, 255) / 255.0f, clamp(blue, 0, 255) / 255.0f, 1);
}

float Math::PerlinNoise(const Vector2 &point) noexcept { return Math::PerlinNoise(point.x(), point.y()); }

float Math::PerlinNoise(float x, float y) noexcept {
	if (unlikely(ctx == NULL))
		open_simplex_noise(0, &ctx);
	return open_simplex_noise2(ctx, x, y);
}

float Math::PerlinNoise(float x, float y, float z) noexcept {
	if (unlikely(ctx == NULL))
		open_simplex_noise(0, &ctx);

	return open_simplex_noise3(ctx, x, y, z);
}

float Math::PerlinNoise(const Vector3 &point) noexcept { return Math::PerlinNoise(point.x(), point.y(), point.z()); }