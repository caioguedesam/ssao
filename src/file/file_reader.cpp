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
			return Hash::HashString(fp.path);
		}

		FileReader::Result FileReader::ReadFile(const char* path, char* buffer)
		{
#if _WIN32
			HANDLE fHandle = CreateFileA(
				path,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				0,
				NULL
			);
			if (fHandle == INVALID_HANDLE_VALUE)
			{
				return Result::FAIL_OPEN;
			}
			LARGE_INTEGER fSize;
			GetFileSizeEx(fHandle, &fSize);

			if (!::ReadFile(fHandle, buffer, fSize.QuadPart, NULL, 0))
			{
				return Result::FAIL_READ;
			}

			if (!CloseHandle(fHandle))
			{
				return Result::FAIL_CLOSE;
			}

			buffer[fSize.QuadPart] = '\0';
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

		std::vector<FilePath> FileReader::getFileNamesFromPath(const char* path)
		{
#if _WIN32
			WIN32_FIND_DATAA findData;
			std::vector<FilePath> result;
			char query[MAX_PATH];
			strcpy(query, path);
			strcat(query, "*");		// TODO_FILE: Assuming path ends in forward slash /
			HANDLE hFind = FindFirstFileA(query, &findData);
			do
			{
				if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
				{
					continue;
				}

				result.push_back(FilePath(findData.cFileName));

			} while (FindNextFileA(hFind, &findData));
			FindClose(hFind);

			return result;
#endif
			// TODO_MULTIPLATFORM, TODO_FILE: Implement.
		}

		uint64_t FileReader::getFileLastWriteTimestamp(const char* path)
		{
#if _WIN32
			WIN32_FIND_DATAA findData;
			char query[MAX_PATH];
			strcpy(query, path);
			HANDLE fHandle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			
			FILETIME filetime_last_write;
			if (!GetFileTime(fHandle, NULL, NULL, &filetime_last_write))
			{
				ASSERT(0, "[ERROR:FILESYSTEM] GetFileTime failed.");
				return -1;
			}
			CloseHandle(fHandle);

			ULARGE_INTEGER t;
			t.HighPart = filetime_last_write.dwHighDateTime;
			t.LowPart = filetime_last_write.dwLowDateTime;
			uint64_t result = t.QuadPart;

			return result;
#endif
			// TODO_MULTIPLATFORM, TODO_FILE: Implement.
		}
	}
}