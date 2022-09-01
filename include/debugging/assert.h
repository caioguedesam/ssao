#pragma once

#if _DEBUG
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#define ASSERT(EXPR, MESSAGE)\
do {\
	if(!(EXPR))\
	{\
		MessageBoxExA(\
			NULL,\
			MESSAGE,\
			"FAILED ASSERT",\
			MB_OK,\
			0);\
		DebugBreak();\
		ExitProcess(-1);\
	}\
} while(false)

#define ASSERT_ZERO(EXPR, MSG) ASSERT(!(EXPR), MSG)

#define ASSERT_FORMAT(EXPR, MSG, ...) do {\
	char buf[2048];\
	sprintf(buf, MSG, __VA_ARGS__);\
	ASSERT(EXPR, buf);\
} while(false)

#else
#define ASSERT(EXPR, MSG) do {} while(false)
#define ASSERT_ZERO(EXPR, MSG) do {} while(false)
#define ASSERT_FORMAT(EXPR, MSG, ...) do {} while(false)
#endif