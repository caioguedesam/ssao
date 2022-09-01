#include "stdafx.h"
#include "resource/buffer_resource_manager.h"

namespace Ty
{
	namespace Graphics
	{
		BufferResourceManager buffer_resource_manager;

		void BufferResourceManager::init() {}
		void BufferResourceManager::destroy() {}

		ResourceHandle<Buffer> BufferResourceManager::create_buffer(BufferDesc desc, void* data)
		{
			Buffer* buffer = new Buffer();

			buffer->init(desc, data);
			ResourceHandle<Buffer> handle = add(buffer);
			return handle;
		}

		void BufferResourceManager::set_buffer_data(ResourceHandle<Buffer> buffer_handle, void* data)
		{
			Buffer* buffer = get(buffer_handle);
			buffer->set_data(data);
		}

		void BufferResourceManager::bind_buffer(ResourceHandle<Buffer> buffer_handle)
		{
			get(buffer_handle)->bind();
		}

		// TODO_MEMORY, TODO_BUFFER: Buffer freeing (on demand/destruction)
	}
}