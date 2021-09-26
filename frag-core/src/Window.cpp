// <Exception/InvalidArgumentException.h>
#include"Window/Window.h"

using namespace fragcore;


//TOOD relocate the compute to the math library since there are other component that will be using it. (like LUT)
void Window::calculateGammaLookupTable(float gamma, ushort *rgbRamp) const {
	if (gamma < 0.0)
		throw InvalidArgumentException("gamma exponent must be positive");

	float exponent = 1.0f / gamma;

	/*  Create lookup table.    */
	for (uint i = 0; i < 256; ++i) {
		/*  */
		float linear = float(i) * 1.0f / 255u;
		float corrected = (float) pow(linear, exponent);
		ushort entry = ushort(corrected * 65535);

		rgbRamp[i] = rgbRamp[i + 256] = rgbRamp[i + 512] = entry;
	}
}

float Window::computeGammaExponent(const ushort *rgbRamp) const {
	float gamma = 0.0;

	//TODO improve
	for (uint i = 64; i < 256; ++i) {
		double corrected = (double) rgbRamp[i] / 65535.0;
		double linear = double(i) * 1.0 / 255u;
		gamma += log(linear) / log(corrected);
		break;
	}
	return gamma;
}