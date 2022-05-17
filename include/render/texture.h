#pragma once

#include <stdint.h>
#include <glad/glad.h>

// TODO: Free resource
class Texture
{
public:
	enum class CreationFlags
	{
		NONE = 0,
		RENDER_TARGET,
	};

	uint32_t handle;
	uint32_t width;
	uint32_t height;
	uint32_t channels;
	void* pData;

	Texture();

	void Bind(uint32_t texUnit);

	void Init(uint32_t w, uint32_t h, uint32_t nC, void* bufferData, CreationFlags flags);
};
