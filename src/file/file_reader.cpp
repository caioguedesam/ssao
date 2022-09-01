#include "stdafx.h"
#include "file/file_reader.h"
#include "core/hash.h"

namespace Ty
{
	namespace FileSystem
	{
		FilePath::FilePath(const char* str)
		{
			strcpy(path, str);
		}

		size_t HashFunction_FilePath::operator()(const FilePath& fp) const
		{
			return Hash::hash_string(fp.path);
		}

		FileReader::Result FileReader::read_file(const char* path, char* buffer)
		{
#if _WIN32
			HANDLE file_handle = CreateFileA(
				path,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				0,
				NULL
			);
			if (file_handle == INVALID_HANDLE_VALUE)
			{
				return Result::FAIL_OPEN;
			}
			LARGE_INTEGER file_size;
			GetFileSizeEx(file_handle, &file_size);

			if (!::ReadFile(file_handle, buffer, file_size.QuadPart, NULL, 0))
			{
				return Result::FAIL_READ;
			}

			if (!CloseHandle(file_handle))
			{
				return Result::FAIL_CLOSE;
			}

			buffer[file_size.QuadPart] = '\0';
			return Result::OK;
#else
			FILE* pFile;
			long fSize = 0;
			// Open file
			pFile = fopen(path, "rb");
			if (!pFile)
			{
				return Result::FAIL_OPEN;
			}

			// Query file size
			auto result = fseek(pFile, 0, SEEK_END);
			if (result)
			{
				return Result::FAIL_READ;
			}
			fSize = ftell(pFile);
			rewind(pFile);

			// Read file to buffer
			result = fread(buffer, 1, fSize, pFile);
			if (!result || (result != fSize && !feof(pFile)))
			{
				return Result::FAIL_READ;
			}

			// Close file
			fclose(pFile);
			buffer[result] = '\0';
			return Result::OK;
#endif
		}

		std::vector<FilePath> FileReader::get_file_names_from_path(const char* path)
		{
#if _WIN32
			WIN32_FIND_DATAA find_data;
			std::vector<FilePath> result;
			char query[MAX_PATH];
			strcpy(query, path);
			strcat(query, "*");		// TODO_FILE: Assuming path ends in forward slash /
			HANDLE hFind = FindFirstFileA(query, &find_data);
			do
			{
				if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0)
				{
					continue;
				}

				result.push_back(FilePath(find_data.cFileName));

			} while (FindNextFileA(hFind, &find_data));
			FindClose(hFind);

			return result;
#endif
			// TODO_#MULTIPLATFORM: Implement.
		}
	}
}