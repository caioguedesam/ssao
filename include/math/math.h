#pragma once
#include "stdafx.h"

namespace Math
{
	namespace Primitives
	{
		struct u32_rect
		{
			uint32_t w = 0.f;
			uint32_t h = 0.f;
			uint32_t x = 0.f;
			uint32_t y = 0.f;
		};
	}

	static inline float Lerp(const float& a, const float& b, const float& t)
	{
		return a + t * (b - a);
	}
}
