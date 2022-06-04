#include "file/file_reader.h"
#include <stdio.h>
#include <cerrno>

// TODO_#MULTIPLATFORM: Some file access code is windows-only. Add versions to other platforms.
#if _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <fileapi.h>
#endif

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

	if (!::ReadFile( fHandle, buffer, fSize.QuadPart, NULL, 0))
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

std::vector<char*> FileReader::GetFileNamesFromPath(const char* path)
{
#if _WIN32
	WIN32_FIND_DATAA findData;
	std::vector<char*> result;
	char query[MAX_PATH];
	strcpy(query, path);
	strcat(query, "\\*");
	HANDLE hFind = FindFirstFileA(query, &findData);
	do
	{
		result.push_back(findData.cFileName);
	} while (FindNextFileA(hFind, &findData));
	FindClose(hFind);

	return result;
#endif
	// TODO_#MULTIPLATFORM: Implement.
}