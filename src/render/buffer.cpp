#include "stdafx.h"
#include "render/buffer.h"
#include <glad/glad.h>
#include "debugging/gl.h"

Buffer::Buffer()
{
	
}

void Buffer::Bind(GLenum bindTarget)
{
	GL(glBindBuffer(bindTarget, handle));
}

void Buffer::Init(GLenum bindTarget, uint32_t bufferSize, uint32_t bufferCount, void* bufferData)
{
	if (!handle)
	{
		GL(glGenBuffers(1, &handle));
	}

	sizeInBytes = bufferSize;
	count = bufferCount;
	pData = bufferData;
	Bind(bindTarget);
	GL(glBufferData(bindTarget, sizeInBytes, pData, GL_STATIC_DRAW));
}