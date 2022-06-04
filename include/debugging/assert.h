#pragma once

#if _DEBUG
#include <cassert>
#include <stdio.h>
#define ASSERT(EXPR, MSG) do {assert(EXPR && MSG);} while(false)
#define ASSERT_ZERO(EXPR, MSG) do {assert(!EXPR && MSG);} while(false)

#define ASSERT_FORMAT(EXPR, MSG, ...) do {\
		if(!EXPR) {\
			printf(MSG, __VA_ARGS__);\
			printf("\n");\
			assert(0);\
		}\
	} while(false)

#else
#define ASSERT(EXPR, MSG) do {} while(false)
#define ASSERT_ZERO(EXPR, MSG) do {} while(false)
#define ASSERT_FORMAT(EXPR, MSG, ...) do {} while(false)
#endif