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
