#include <cmath>
#include "miscellaneous.h"
#include "Constants.h"

namespace Nova {
    float toDegrees(const float value)
	{
		return value * (180.0f / NOVA_PI);
	}
	float toRadians(const float value)
	{
		return value * (NOVA_PI / 180.0f);
	}
	bool isEqual(float valueA, float valueB)
	{		
		//if ((abs(valueA) - abs(valueB)) <= NOVA_LAMBDA)
		if ((std::abs(valueA - valueB)) <= NOVA_LAMBDA)
		{
			return true;
		}
		return false;
	}
}