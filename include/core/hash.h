#pragma once
#include "stdafx.h"
#include "random/random.h"

namespace Ty
{
	namespace Hash
	{
		static inline uint32_t murmur_32_scramble(uint32_t k) {
			k *= 0xcc9e2d51;
			k = (k << 15) | (k >> 17);
			k *= 0x1b873593;
			return k;
		}
		
		static uint32_t murmur3_32(const uint8_t* key, size_t len, uint32_t seed)
		{
			uint32_t h = seed;
			uint32_t k;
			/* Read in groups of 4. */
			for (size_t i = len >> 2; i; i--) {
				// Here is a source of differing results across endiannesses.
				// A swap here has no effects on hash properties though.
				memcpy(&k, key, sizeof(uint32_t));
				key += sizeof(uint32_t);
				h ^= murmur_32_scramble(k);
				h = (h << 13) | (h >> 19);
				h = h * 5 + 0xe6546b64;
			}
			/* Read the rest. */
			k = 0;
			for (size_t i = len & 3; i; i--) {
				k <<= 8;
				k |= key[i - 1];
			}
			// A swap is *not* necessary here because the preceding loop already
			// places the low bytes in the low places according to whatever endianness
			// we use. Swaps only apply when the memory is copied in a chunk.
			h ^= murmur_32_scramble(k);
			/* Finalize. */
			h ^= len;
			h ^= h >> 16;
			h *= 0x85ebca6b;
			h ^= h >> 13;
			h *= 0xc2b2ae35;
			h ^= h >> 16;
			return h;
		}

		// Bob Jenkins' One-at-a-Time hash
		// https://stackoverflow.com/questions/114085/fast-string-hashing-algorithm-with-low-collision-rates-with-32-bit-integer
		static uint32_t hash_string(const char* s)
		{
			//uint32_t result = 0;
			//for (; *s; s++)
			//{
			//	result += *s;
			//	result += (result << 10);
			//	result ^= (result >> 6);
			//}

			//result += (result << 3);
			//result ^= (result >> 11);
			//result += (result << 15);
			//return result;					// TODO_CONTINUE: This hash is not working properly!

			static uint32_t seed = Random::xor_shift_64();
			uint32_t result = murmur3_32((uint8_t*)s, strlen(s), seed);
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