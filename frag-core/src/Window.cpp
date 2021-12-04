
#include "Window/Window.h"
#include "Core/Math.h"

using namespace fragcore;

// TOOD relocate the compute to the math library since there are other component that will be using it. (like LUT)
void Window::calculateGammaLookupTable(float gamma, ushort *rgbRamp) const {

	// TODO that or just clamp it.
	if (gamma < 0.0)
		throw InvalidArgumentException("gamma exponent must be positive");
	gamma = Math::min(0.0f, gamma);

	float exponent = 1.0f / gamma;

	/*  Create lookup table.    */
	for (uint i = 0; i < 256; ++i) {
		/*  */
		float linear = float(i) * 1.0f / 255u;
		float corrected = (float)std::pow(linear, exponent);
		ushort entry = ushort(corrected * 65535);

		rgbRamp[i] = rgbRamp[i + 256] = rgbRamp[i + 512] = entry;
	}
}

float Window::computeGammaExponent(const ushort *rgbRamp) const {
	float gamma = 0.0;

	// TODO improve
	for (uint i = 64; i < 256; ++i) {
		double corrected = (double)rgbRamp[i] / 65535.0;
		double linear = double(i) * 1.0 / 255u;
		gamma += std::log(linear) / std::log(corrected);
		break;
	}
	return gamma;
}