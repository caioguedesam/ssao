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

void Texture::Init(uint32_t w, uint32_t h, uint32_t nC, void* bufferData, CreationFlags flags)
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
	channels = nC;
	pData = bufferData;

	Bind(0);
	// TODO: Support multiple texture channels and formats
	GLenum format = GL_RGB;
	switch (nC)
	{
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	}
	GL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pData));
	GL(glGenerateMipmap(GL_TEXTURE_2D));

	// TODO: These texture parameters are hardcoded.
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}