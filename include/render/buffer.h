#pragma once
#include <stdint.h>
#include <glad/glad.h>

class Buffer
{
public:
	uint32_t handle;
	uint32_t sizeInBytes;
	uint32_t count;
	void* pData;

	Buffer();

	void Bind(GLenum bindTarget);

	void Init(GLenum bindTarget, uint32_t bufferSize, uint32_t bufferCount, void* bufferData);
};
