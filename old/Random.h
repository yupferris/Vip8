#ifndef __RANDOM_H__
#define __RANDOM_H__

#include "Common.h"

class Random
{
public:
	static void Seed(int seed);
	static int NextInt();
};

#endif
