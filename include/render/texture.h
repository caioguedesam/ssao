#pragma once

#include <stdint.h>
#include <glad/glad.h>

// TODO: Free resource
class Texture
{
public:
	enum class Format
	{
		INVALID = 0,
		R_16FLOAT,
		RGB_UNORM,
		RGBA_UNORM,
		RGBA_16FLOAT,
	};

	enum class CreationFlags
	{
		NONE = 0,
		RENDER_TARGET,
	};

	uint32_t handle;
	uint32_t width;
	uint32_t height;
	Format format;
	void* pData;

	Texture();

	void Bind(uint32_t texUnit);

	void Init(uint32_t w, uint32_t h, Format texFormat, void* bufferData, CreationFlags flags = CreationFlags::NONE);

	static GLenum TextureToGLInternalFormat(Format texFormat);
	static void TextureToGLFormatAndType(Format texFormat, GLenum& outFormat, GLenum& outType);
};
