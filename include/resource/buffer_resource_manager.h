#pragma once
#include "stdafx.h"
#include "resource/resource_manager.h"
#include "render/buffer.h"
#include "file/file_reader.h"
#include "core/hash.h"

struct BufferResourceManager : ResourceManager<Buffer>
{
	std::unordered_map<FilePath, ResourceHandle<Buffer>, HashFunction_FilePath> handleList;

	void init();
	void destroy();

	ResourceHandle<Buffer> createBuffer(BufferDesc desc, void* pData);
	void bindBuffer(ResourceHandle<Buffer> bufferHandle);

	// TODO_MEMORY, TODO_BUFFER: Buffer freeing (on demand/destruction)
};

extern BufferResourceManager g_bufferResourceManager;
