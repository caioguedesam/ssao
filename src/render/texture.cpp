#include "render/texture.h"
#include "debugging/assert.h"
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
	case Texture::Format::R_16UNORM:
		return GL_R16;
	case Texture::Format::R_16FLOAT:
		return GL_R16F;
	case Texture::Format::RGB_UNORM:
		return GL_RGB;
	case Texture::Format::RGBA_UNORM:
		return GL_RGBA;
	case Texture::Format::RGBA_16FLOAT:
		return GL_RGB16F;
	default:
		ASSERT_FORMAT(0, "No corresponding OpenGL internal format for texture format %d", static_cast<int>(texFormat));
		return GL_NONE;
	}
}

void Texture::GLFormatAndType(Format texFormat, GLenum& outFormat, GLenum& outType)
{
	switch (texFormat)
	{
	case Texture::Format::R_16UNORM:
		outFormat = GL_RED;
		outType = GL_UNSIGNED_BYTE;
		break;
	case Texture::Format::R_16FLOAT:
		outFormat = GL_RED;
		outType = GL_FLOAT;
		break;
	case Format::RGB_UNORM:
		outFormat = GL_RGB;
		outType = GL_UNSIGNED_BYTE;
		break;
	case Format::RGBA_UNORM:
		outFormat = GL_RGBA;
		outType = GL_UNSIGNED_BYTE;
		break;
	case Format::RGBA_16FLOAT:
		outFormat = GL_RGBA;
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
	if (params & Params::MIN_FILTER_NEAREST) return GL_NEAREST_MIPMAP_LINEAR;
	if (params & Params::MIN_FILTER_LINEAR) return GL_LINEAR_MIPMAP_LINEAR;
	else return GL_NEAREST_MIPMAP_LINEAR;	// default
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