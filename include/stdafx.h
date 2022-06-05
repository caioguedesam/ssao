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
#include <unordered_map>
#include <set>
#include <stack>
#include <queue>
#include <deque>

#include <glm/glm.hpp>	// Remove this once we have own math lib?

#if _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN322_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#include "debugging/assert.h"
