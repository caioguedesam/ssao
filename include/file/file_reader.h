#pragma once
#include "stdafx.h"

struct FileReaderPath
{
	char path[MAX_PATH] = "";

	FileReaderPath(const char* str);
};

struct FileReader
{
	enum class Result
	{
		OK = 0,
		FAIL_OPEN,
		FAIL_READ,
		FAIL_CLOSE
	};

	static Result ReadFile(const char* path, char* buffer);
	static std::vector<FileReaderPath> getFileNamesFromPath(const char* path);
};
