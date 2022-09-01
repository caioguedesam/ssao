#pragma once

#ifdef _DEBUG
void gl_check_error(const char* statement, const char* fname, int line);

#define GL(statement) do {\
	statement;\
	gl_check_error(#statement, __FILE__, __LINE__);\
	} while (0)
#else
#define GL(statement) do { statement; } while(false)
#endif