#include "Random.h"

namespace Nova {
	int Random::nextInteger(int min, int max)
	{
		std::uniform_int_distribution<> intBoundedDist(min, max);
		return intBoundedDist(rndEngine);
	}
	double Random::nextDouble(double min, double max)
	{
		std::uniform_real_distribution<> realBoundedDist(min, max);
		return realBoundedDist(rndEngine);
	}
	float Random::nextFloat(float min, float max)
	{
		std::uniform_real_distribution<> realBoundedDist(min, max);
		return static_cast<float>(realBoundedDist(rndEngine));
	}
}