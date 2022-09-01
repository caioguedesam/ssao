#pragma once
#include "stdafx.h"
#include <intrin.h>

namespace Ty
{
	namespace Random
	{
		static inline uint64_t xor_shift_64()
		{
			uint64_t seed = __rdtsc();
			seed ^= seed >> 12;
			seed ^= seed << 25;
			seed ^= seed >> 27;
			return seed * 0x2545F4914F6CDD1DULL;
		}

		static inline float dist_uniform(const float& start = 0.f, const float& end = 1.f)
		{
			return start + (static_cast<float>(xor_shift_64()) / static_cast<float>(UINT64_MAX)) * (end - start);
		}
	}
}
