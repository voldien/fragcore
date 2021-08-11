#include "Core/Random.h"
#include <pcg_basic.h>

using namespace fragcore;

void Random::setSeed(unsigned long long seed) noexcept {
	//pcg32_srandom()
}

unsigned int Random::rand(void) noexcept { return pcg32_random(); }

float Random::randfNormalize(void) noexcept { return 0; }

float Random::randf(void) const noexcept { return 0; }

float Random::range(float min, float max) noexcept { return 0; }