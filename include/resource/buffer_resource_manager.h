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

			ResourceHandle<Buffer> create_buffer(BufferDesc desc, void* data);
			void set_buffer_data(ResourceHandle<Buffer> buffer_handle, void* data);
			void bind_buffer(ResourceHandle<Buffer> buffer_handle);

			// TODO_MEMORY, TODO_BUFFER: Buffer freeing (on demand/destruction)
		};

		extern BufferResourceManager buffer_resource_manager;
	}
}
