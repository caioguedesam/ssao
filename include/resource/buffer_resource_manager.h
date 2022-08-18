#pragma once
#include "stdafx.h"
#include "resource/resource_manager.h"
#include "render/buffer.h"
#include "core/hash.h"

namespace Ty
{
	namespace Graphics
	{
		struct BufferResourceManager : ResourceManager<Buffer>
		{
			void init();
			void destroy();

			ResourceHandle<Buffer> createBuffer(BufferDesc desc, void* pData);
			void setBufferData(ResourceHandle<Buffer> bufferHandle, void* pData);
			void bindBuffer(ResourceHandle<Buffer> bufferHandle);

			// TODO_MEMORY, TODO_BUFFER: Buffer freeing (on demand/destruction)
		};

		extern BufferResourceManager g_bufferResourceManager;
	}
}
