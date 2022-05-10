#include "render/texture.h"

Texture::Texture()
{
	glGenTextures(1, &handle);
}

void Texture::Bind(GLenum bindTarget)
{
	// TODO: Support multiple texture dimensions
	glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::Init(GLenum bindTarget, uint32_t w, uint32_t h, uint32_t nC, void* bufferData)
{
	width = w;
	height = h;
	channels = nC;

	Bind(bindTarget);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);

	/*sizeInBytes = bufferSize;
	count = bufferCount;
	pData = bufferData;
	Bind(bindTarget);
	glBufferData(bindTarget, sizeInBytes, pData, GL_STATIC_DRAW);
	ASSERT(glGetError() == GL_NO_ERROR, "");*/
}