#include "stdafx.h"
#include "render/buffer.h"
#include <glad/glad.h>
#include "debugging/gl.h"

namespace Ty
{
	namespace Graphics
	{
		void Buffer::init(BufferDesc desc, void* data)
		{
			if (api_handle == HANDLE_INVALID)
			{
				GL(glGenBuffers(1, &api_handle));
			}

			this->desc = desc;
			set_data(data);
		}

		void Buffer::set_data(void* data)
		{
			ASSERT(api_handle != HANDLE_INVALID, "Trying to set data of invalid buffer resource.");
			this->data = data;
			bind();
			// TODO_GL, TODO_BUFFER: Discriminate between static, dynamic and stream buffers.
			// TODO_GL, TODO_BUFFER: Subbuffer data to avoid buffer reallocation.
			GL(glBufferData(get_bind_target(), get_size(), data, GL_STATIC_DRAW));
		}

		uint32_t Buffer::get_stride()
		{
			ASSERT(api_handle != HANDLE_INVALID, "Trying to get stride of invalid buffer resource.");
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

		size_t Buffer::get_count()
		{
			ASSERT(api_handle != HANDLE_INVALID, "Trying to get count of invalid buffer resource.");
			return desc.count;
		}

		size_t Buffer::get_size()
		{
			return get_stride() * get_count();
		}

		uint32_t Buffer::get_bind_target()
		{
			ASSERT(api_handle != HANDLE_INVALID, "Trying to get bind target of invalid buffer resource.");
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
			ASSERT(api_handle != HANDLE_INVALID, "Trying to bind invalid buffer resource.");
			GL(glBindBuffer(get_bind_target(), api_handle));
		}
	}
}