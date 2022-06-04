#pragma once
#include <vector>

class FileReader
{
public:
	enum class Result
	{
		OK = 0,
		FAIL_OPEN,
		FAIL_READ,
		FAIL_CLOSE
	};

	static Result ReadFile(const char* path, char* buffer);
	static std::vector<char*> GetFileNamesFromPath(const char* path);
};
