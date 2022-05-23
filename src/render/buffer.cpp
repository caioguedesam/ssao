#include "render/buffer.h"
#include "debugging/assert.h"

Buffer::Buffer()
{
	
}

void Buffer::Bind(GLenum bindTarget)
{
	glBindBuffer(bindTarget, handle);
	ASSERT(glGetError() == GL_NO_ERROR, "");
}

void Buffer::Init(GLenum bindTarget, uint32_t bufferSize, uint32_t bufferCount, void* bufferData)
{
	if (!handle)
	{
		glGenBuffers(1, &handle);
		ASSERT(glGetError() == GL_NO_ERROR, "");
	}

	sizeInBytes = bufferSize;
	count = bufferCount;
	pData = bufferData;
	Bind(bindTarget);
	glBufferData(bindTarget, sizeInBytes, pData, GL_STATIC_DRAW);
	ASSERT(glGetError() == GL_NO_ERROR, "");
}