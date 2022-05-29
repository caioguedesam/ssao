#pragma once

namespace Math
{
	static inline float Lerp(const float& a, const float& b, const float& t)
	{
		return a + t * (b - a);
	}
}
