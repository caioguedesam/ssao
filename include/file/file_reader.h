#pragma once
#include <stdio.h>

class FileReader
{
public:
	enum class Result
	{
		OK = 0,
		FAIL_OPEN,
		FAIL_READ
	};

	static Result ReadFile(const char* path, char* buffer);
};
