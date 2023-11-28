#pragma once
#include <stdlib.h>
#include <type_traits>


	inline void seedRandom(unsigned int seed)
	{
		srand(seed);
	}

	inline float random01()
	{
		return rand() / (float)RAND_MAX;
	}

	inline float random(float min, float max)
	{
		if (min > max) std::swap(min, max);
		
		float value = min + (max - min) * random01();
		return value;
	}
