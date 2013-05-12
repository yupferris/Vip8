#include "Random.h"

void Random::Seed(int seed)
{
	srand(seed);
}

int Random::NextInt()
{
	return rand();
}
