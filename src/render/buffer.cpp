#include "stdafx.h"
#include "render/buffer.h"
#include <glad/glad.h>
#include "debugging/gl.h"

void Buffer::init(BufferDesc desc, void* pData)
{
	if (apiHandle == HANDLE_INVALID)
	{
		GL(glGenBuffers(1, &apiHandle));
	}

	this->desc = desc;
	setData(pData);
	//bind();
	//GL(glBufferData(getBindTarget(), getSize(), pData, GL_STATIC_DRAW));	// TODO_GL, TODO_BUFFER: Discriminate between static, dynamic and stream buffers.
}

void Buffer::setData(void* pData)
{
	ASSERT(apiHandle != HANDLE_INVALID, "Trying to set data of invalid buffer resource.");
	this->pData = pData;
	bind();
	// TODO_GL, TODO_BUFFER: Discriminate between static, dynamic and stream buffers.
	// TODO_GL, TODO_BUFFER: Subbuffer data to avoid buffer reallocation.
	GL(glBufferData(getBindTarget(), getSize(), pData, GL_STATIC_DRAW));	
}

uint32_t Buffer::getStride()
{
	ASSERT(apiHandle != HANDLE_INVALID, "Trying to get stride of invalid buffer resource.");
	switch (desc.format)
	{
	case BufferFormat::R32_FLOAT:
	case BufferFormat::R32_UINT:
		return 4;
	default:
		ASSERT(0, "Trying to get stride of invalid buffer format.");
		break;
	}
	return UINT32_MAX;
}

size_t Buffer::getCount()
{
	ASSERT(apiHandle != HANDLE_INVALID, "Trying to get count of invalid buffer resource.");
	return desc.count;
}

size_t Buffer::getSize()
{
	return getStride() * getCount();
}

uint32_t Buffer::getBindTarget()
{
	ASSERT(apiHandle != HANDLE_INVALID, "Trying to get bind target of invalid buffer resource.");
	switch (desc.type)
	{
	case VERTEX_BUFFER:
		return GL_ARRAY_BUFFER;
	case INDEX_BUFFER:
		return GL_ELEMENT_ARRAY_BUFFER;
	default:
		ASSERT(0, "No corresponding buffer bind target for given buffer type.");
		break;
	}
}

void Buffer::bind()
{
	ASSERT(apiHandle != HANDLE_INVALID, "Trying to bind invalid buffer resource.");
	GL(glBindBuffer(getBindTarget(), apiHandle));
}