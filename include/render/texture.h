#pragma once

#include <stdint.h>
#include <glad/glad.h>

class Texture
{
public:
	uint32_t handle;
	uint32_t sizeInBytes;
	uint32_t width;
	uint32_t height;
	uint32_t channels;
	void* pData;

	Texture();

	void Bind(GLenum bindTarget);

	void Init(GLenum bindTarget, uint32_t width, uint32_t height, uint32_t channels, void* bufferData);
};
