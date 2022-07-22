#pragma once
#include "stdafx.h"
#include "resource/resource_manager.h"

typedef unsigned int GLenum;

enum BufferType : uint32_t
{
	INVALID_BUFFER_TYPE = 0,
	VERTEX_BUFFER,
	INDEX_BUFFER,
};

enum BufferFormat : uint32_t
{
	INVALID_BUFFER_FORMAT = 0,
	R32_FLOAT,
	R32_UINT,
};

struct BufferDesc
{
	BufferType type;
	BufferFormat format;
	uint32_t count;
};

// TODO: Free resource
struct Buffer
{
	uint32_t apiHandle;
	BufferDesc desc;
	void* pData;

	Buffer();

	/*void Init(GLenum bindTarget, uint32_t bufferSize, uint32_t bufferCount, void* bufferData);
	void Bind(GLenum bindTarget);*/

	void init(BufferDesc desc, void* pData);
	uint32_t getStride();
	uint32_t getSize();

	void bind();

	static uint32_t getFormatSize();
};
