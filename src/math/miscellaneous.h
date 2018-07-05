#pragma once

namespace Nova {
	static inline float toDegrees(const float value)
	{
		return value * (180.0f / NOVA_PI);
	}
	static inline float toRadians(const float value)
	{
		return value * (NOVA_PI / 180.0f);
	}
	static inline bool isEqual(float valueA, float valueB)
	{		
		//if ((abs(valueA) - abs(valueB)) <= NOVA_LAMBDA)
		if ((abs(valueA - valueB)) <= NOVA_LAMBDA)
		{
			return true;
		}
		return false;
	}
}