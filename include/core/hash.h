#pragma once
#include <stdint.h>

// Bob Jenkins' One-at-a-Time hash
// https://stackoverflow.com/questions/114085/fast-string-hashing-algorithm-with-low-collision-rates-with-32-bit-integer
uint32_t HashString(const char* s)
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
