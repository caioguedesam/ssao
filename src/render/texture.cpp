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

void Texture::Init(uint32_t w, uint32_t h, Format texFormat, void* bufferData, CreationFlags flags)
{
	if (!handle)
	{
		GL(glGenTextures(1, &handle));
	}

	if (!((uint32_t)flags & (uint32_t)CreationFlags::RENDER_TARGET))
	{
		ASSERT(bufferData, "Null texture data");
	}
	width = w;
	height = h;
	format = texFormat;
	pData = bufferData;

	Bind(0);
	// TODO: Support multiple texture channels and formats
	GLenum glInternalFormat = TextureToGLInternalFormat(format);
	GLenum glFormat, glType;
	TextureToGLFormatAndType(format, glFormat, glType);

	GL(glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glType, pData));
	GL(glGenerateMipmap(GL_TEXTURE_2D));

	// TODO: These texture parameters are hardcoded.
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}

GLenum Texture::TextureToGLInternalFormat(Format texFormat)
{
	switch (texFormat)
	{
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

void Texture::TextureToGLFormatAndType(Format texFormat, GLenum& outFormat, GLenum& outType)
{
	switch (texFormat)
	{
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