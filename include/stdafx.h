#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cmath>
#include <cstring>
#include <climits>
#include <cerrno>
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <stack>
#include <queue>
#include <deque>

#if _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#include "debugging/assert.h"

// Typedefs
typedef int8_t		i8;
typedef int16_t		i16;
typedef int32_t		i32;
typedef int64_t		i64;

typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

typedef float		f32;
typedef double		f64;
