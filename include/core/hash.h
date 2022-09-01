#pragma once
#include "stdafx.h"

namespace Ty
{
	namespace Hash
	{
		// Bob Jenkins' One-at-a-Time hash
		// https://stackoverflow.com/questions/114085/fast-string-hashing-algorithm-with-low-collision-rates-with-32-bit-integer
		static uint32_t hash_string(const char* s)
		{
			uint32_t result = 0;
			for (; *s; s++)
			{
				result += *s;
				result += (result << 10);
				result ^= (result >> 6);
			}

			result += (result << 3);
			result ^= (result >> 11);
			result += (result << 15);
			return result;
		}

		struct HashFunction_CSTR
		{
			size_t operator()(const char* s) const
			{
				return hash_string(s);
			}
		};

		// Some guy's uint32 hash
		// https://stackoverflow.com/questions/33815572/whats-a-fast-ideal-hash-function-for-int32
		static uint32_t hash_u32(uint32_t value)
		{
			return(value * 0xdeece66d + 0xb);
		}
	}
}