#include "random.h"

Random::Random()
{
}

Random::~Random()
{
}

unsigned long Random::getRand()
{
	//XORSHF generator
	unsigned long sT;
	
	sX ^= sX << 16;
	sX ^= sX >> 5;
	sX ^= sX << 1;
	
	sT = sX;
	sX = sY;
	sY = sZ;
	sZ = sT ^ sX ^ sY;
	
	return sZ;
}

int Random::getRand(const long& lowerLimit, const long& upperLimit)
{
	return lowerLimit + (getRand() % (upperLimit - lowerLimit + 1));
}