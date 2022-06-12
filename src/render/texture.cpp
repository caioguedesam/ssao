#include "stdafx.h"
#include "render/texture.h"

#include <glad/glad.h>
#include "debugging/gl.h"

Texture::Texture()
{
}

void Texture::Bind(uint32_t texUnit)
{
	GL(glActiveTexture(GL_TEXTURE0 + texUnit));
	// TODO: Support multiple texture dimensions
	GL(glBindTexture(GL_TEXTURE_2D, handle));
}

void Texture::Init(uint32_t w, uint32_t h, Format texFormat, void* bufferData, Params params/*, CreationFlags flags*/)
{
	if (!handle)
	{
		GL(glGenTextures(1, &handle));
	}

	/*if (!((uint32_t)flags & (uint32_t)CreationFlags::RENDER_TARGET))
	{
		ASSERT(bufferData, "Null texture data");
	}*/
	width = w;
	height = h;
	format = texFormat;
	pData = bufferData;

	Bind(0);
	// TODO: Support multiple texture channels and formats
	GLenum glInternalFormat = GLInternalFormat(format);
	GLenum glFormat, glType;
	GLFormatAndType(format, glFormat, glType);

	GL(glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glType, pData));
	GL(glGenerateMipmap(GL_TEXTURE_2D));	// TODO_#MIPS: Support proper mip map generation

	GLenum minFilter = GLMinFilter(params);
	GLenum magFilter = GLMagFilter(params);
	GLenum wrapU = GLWrapU(params);
	GLenum wrapV = GLWrapV(params);
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapU));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapV));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
}

GLenum Texture::GLInternalFormat(Format texFormat)
{
	switch (texFormat)
	{
	case Texture::Format::R8_FLOAT:
		return GL_RED;
	case Texture::Format::R8_G8_B8_UNORM:
		return GL_RGB;
	case Texture::Format::R8_G8_B8_A8_UNORM:
		return GL_RGBA;
	case Texture::Format::R16_G16_B16_A16_FLOAT:
		return GL_RGBA16F;
	case Texture::Format::R32_G32_B32_FLOAT:
		return GL_RGBA32F;
	default:
		ASSERT_FORMAT(0, "No corresponding OpenGL internal format for texture format %d", static_cast<int>(texFormat));
		return GL_NONE;
	}
}

void Texture::GLFormatAndType(Format texFormat, GLenum& outFormat, GLenum& outType)
{
	switch (texFormat)
	{
	case Texture::Format::R8_FLOAT:
		outFormat = GL_RED;
		outType = GL_FLOAT;
		break;
	case Format::R8_G8_B8_UNORM:
		outFormat = GL_RGB;
		outType = GL_UNSIGNED_BYTE;
		break;
	case Format::R8_G8_B8_A8_UNORM:
		outFormat = GL_RGBA;
		outType = GL_UNSIGNED_BYTE;
		break;
	case Format::R16_G16_B16_A16_FLOAT:
		outFormat = GL_RGBA;
		outType = GL_FLOAT;
		break;
	case Texture::Format::R32_G32_B32_FLOAT:
		outFormat = GL_RGB;
		outType = GL_FLOAT;
		break;
	default:
		ASSERT_FORMAT(0, "No corresponding OpenGL format/type for texture format %d", static_cast<int>(texFormat));
		outFormat = outType = GL_NONE;
		break;
	}
}

GLenum Texture::GLMinFilter(Params params)
{
	// TODO_#MIPS: Support mip map filtering options
	if (params & Params::MIN_FILTER_NEAREST) return GL_NEAREST;
	if (params & Params::MIN_FILTER_LINEAR) return GL_LINEAR;
	else return GL_NEAREST;	// default
}

GLenum Texture::GLMagFilter(Params params)
{
	if (params & Params::MAG_FILTER_NEAREST) return GL_NEAREST;
	if (params & Params::MAG_FILTER_LINEAR) return GL_LINEAR;
	else return GL_NEAREST;	// default
}

GLenum Texture::GLWrapU(Params params)
{
	if (params & Params::WRAP_REPEAT) return GL_REPEAT;
	if (params & Params::WRAP_MIRRORED_REPEAT) return GL_MIRRORED_REPEAT;
	if (params & Params::WRAP_CLAMP_EDGE) return GL_CLAMP_TO_EDGE;
	if (params & Params::WRAP_CLAMP_BORDER) return GL_CLAMP_TO_BORDER;
	else return GL_REPEAT;	// default
}

GLenum Texture::GLWrapV(Params params)
{
	if (params & Params::WRAP_REPEAT) return GL_REPEAT;
	if (params & Params::WRAP_MIRRORED_REPEAT) return GL_MIRRORED_REPEAT;
	if (params & Params::WRAP_CLAMP_EDGE) return GL_CLAMP_TO_EDGE;
	if (params & Params::WRAP_CLAMP_BORDER) return GL_CLAMP_TO_BORDER;
	else return GL_REPEAT;	// default
}