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
		R_16UNORM,
		R_16FLOAT,
		RGB_UNORM,
		RGBA_UNORM,
		RGBA_16FLOAT,
	};

	//enum class CreationFlags	// TODO: Maybe change this to regular enum (or even delete this)
	//{
	//	NONE = 0,
	//	RENDER_TARGET,
	//};

	enum Params
	{
		NONE						= 0,
		MIN_FILTER_NEAREST			= 1 << 0,
		MIN_FILTER_LINEAR			= 1 << 1,
		MAG_FILTER_NEAREST			= 1 << 2,
		MAG_FILTER_LINEAR			= 1 << 3,
		WRAP_REPEAT					= 1 << 4,
		WRAP_MIRRORED_REPEAT		= 1 << 5,
		WRAP_CLAMP_EDGE				= 1 << 6,
		WRAP_CLAMP_BORDER			= 1 << 7
	};

	uint32_t handle;
	uint32_t width;
	uint32_t height;
	Format format;
	void* pData;

	Texture();

	void Bind(uint32_t texUnit);

	void Init(uint32_t w, uint32_t h, Format texFormat, void* bufferData, Params params = Params::NONE/*, CreationFlags flags = CreationFlags::NONE*/);

	static GLenum GLInternalFormat(Format texFormat);
	static void GLFormatAndType(Format texFormat, GLenum& outFormat, GLenum& outType);
	static GLenum GLMinFilter(Params params);
	static GLenum GLMagFilter(Params params);
	static GLenum GLWrapU(Params params);
	static GLenum GLWrapV(Params params);
};
