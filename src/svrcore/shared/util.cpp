#include "util.h"
#include <mersennetwister/MersenneTwister.h>

static MTRand mtRand;

int32 irand (int32 min, int32 max)
{
	return int32 (mtRand.randInt (max - min)) + min;
}

uint32 urand (uint32 min, uint32 max)
{
	if (min==max)
		return min;
	return mtRand.randInt (max - min) + min;
}

int32 rand32 ()
{
	return mtRand.randInt ();
}

double rand_norm(void)
{
	return mtRand.randExc ();
}

double rand_chance (void)
{
	return mtRand.randExc (100.0);
}

