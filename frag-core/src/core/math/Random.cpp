#include "Core/Math/Random.h"
#include <pcg_basic.h>

using namespace fragcore;

unsigned int Random::rand_internal() noexcept { return pcg32_random(); }