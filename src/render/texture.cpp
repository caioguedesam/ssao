#include "render/texture.h"
#include "debugging/assert.h"

Texture::Texture()
{
	/*glGenTextures(1, &handle);
	ASSERT(glGetError() == GL_NO_ERROR, "");*/
}

void Texture::Bind(uint32_t texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	ASSERT(glGetError() == GL_NO_ERROR, "");
	// TODO: Support multiple texture dimensions
	glBindTexture(GL_TEXTURE_2D, handle);
	ASSERT(glGetError() == GL_NO_ERROR, "");
}

void Texture::Init(uint32_t w, uint32_t h, uint32_t nC, void* bufferData, CreationFlags flags)
{
	if (!handle)
	{
		glGenTextures(1, &handle);
		ASSERT(glGetError() == GL_NO_ERROR, "");
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
	ASSERT(glGetError() == GL_NO_ERROR, "Failed to generate texture image");
	glGenerateMipmap(GL_TEXTURE_2D);
	ASSERT(glGetError() == GL_NO_ERROR, "Failed to generate mips for texture");

	// TODO: These texture parameters are hardcoded.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	ASSERT(glGetError() == GL_NO_ERROR, "Failed to set tex param");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	ASSERT(glGetError() == GL_NO_ERROR, "Failed to set tex param");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ASSERT(glGetError() == GL_NO_ERROR, "Failed to set tex param");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	ASSERT(glGetError() == GL_NO_ERROR, "Failed to set tex param");
}