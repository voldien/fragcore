#include"Core/Random.h"
#include<pcg_basic.h>

using namespace fragcore;

void Random::setSeed(unsigned long long seed){
	
}

unsigned int Random::rand(void){
	return pcg32_random();
}

float Random::randfNormalize(void){
	return 0;
}

float Random::randf(void) const{
	return 0;
}

float Random::range(float min, float max){
	return 0;
}