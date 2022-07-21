#pragma once

#ifdef _DEBUG
void GLCheckError(const char* statement, const char* fname, int line);

#define GL(statement) do {\
	statement;\
	GLCheckError(#statement, __FILE__, __LINE__);\
	} while (0)
#else
#define GL(statement) do { statement; } while(false)
#endif