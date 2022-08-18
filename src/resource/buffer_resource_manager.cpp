#include "stdafx.h"
#include "resource/buffer_resource_manager.h"

namespace Ty
{
	namespace Graphics
	{
		BufferResourceManager g_bufferResourceManager;

		void BufferResourceManager::init() {}
		void BufferResourceManager::destroy() {}

		ResourceHandle<Buffer> BufferResourceManager::createBuffer(BufferDesc desc, void* pData)
		{
			Buffer* buffer = new Buffer();

			buffer->init(desc, pData);
			ResourceHandle<Buffer> handle = add(buffer);
			return handle;
		}

		void BufferResourceManager::setBufferData(ResourceHandle<Buffer> bufferHandle, void* pData)
		{
			Buffer* buffer = get(bufferHandle);
			buffer->setData(pData);
		}

		void BufferResourceManager::bindBuffer(ResourceHandle<Buffer> bufferHandle)
		{
			get(bufferHandle)->bind();
		}

		// TODO_MEMORY, TODO_BUFFER: Buffer freeing (on demand/destruction)
	}
}