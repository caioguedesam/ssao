#pragma once
#include "stdafx.h"

namespace Ty
{
	namespace FileSystem
	{
		struct FilePath
		{
			char path[MAX_PATH] = "";
			uint32_t len = 0;

			FilePath(const char* str);

			inline bool operator==(const FilePath& rhs) const { return strcmp(path, rhs.path) == 0; }

			// TODO_FILE: Implement useful filepath methods, like getting extension, stripping file name, etc.
			const char* get_file_name_with_ext();
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
                FAIL_WRITE,
				FAIL_CLOSE
			};

			static Result read_file(const char* path, char* buffer);
            static Result write_file(const char* path, u8* buffer, u64 size);
            static std::vector<FilePath> get_file_names_from_path(const char* path);
		};
	}
}
