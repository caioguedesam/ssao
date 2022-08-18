#pragma once
#include "stdafx.h"

namespace Ty
{
	namespace FileSystem
	{
		struct FilePath
		{
			char path[MAX_PATH] = "";

			FilePath(const char* str);

			inline bool operator==(const FilePath& rhs) const { return strcmp(path, rhs.path) == 0; }

			// TODO_FILE: Implement useful filepath methods, like getting extension, stripping file name, etc.
		};

		struct HashFunction_FilePath
		{
			size_t operator()(const FilePath& fp) const;
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
			static std::vector<FilePath> getFileNamesFromPath(const char* path);
			static uint64_t getFileLastWriteTimestamp(const char* path);
		};
	}
}
