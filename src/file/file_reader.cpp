#include "file/file_reader.h"

FileReader::Result FileReader::ReadFile(const char* path, char* buffer)
{
	FILE* pFile;
	long fSize = 0;
	// Open file
	pFile = fopen(path, "r");
	if (!pFile)
	{
		return Result::FAIL_OPEN;
	}

	// Query file size
	fseek(pFile, 0, SEEK_END);
	fSize = ftell(pFile);
	rewind(pFile);

	// Read file to buffer
	auto result = fread(buffer, 1, fSize, pFile);
	if (result != fSize && !feof(pFile))
	{
		return Result::FAIL_READ;
	}

	// Close file
	fclose(pFile);
	buffer[result] = '\0';
	return Result::OK;
}