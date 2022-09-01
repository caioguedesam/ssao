#pragma once
#include "stdafx.h"
#include "resource/resource_manager.h"

typedef unsigned int GLenum;

namespace Ty
{
	namespace Graphics
	{
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
			size_t count;
		};

		// TODO: Free resource
		struct Buffer
		{
			uint32_t api_handle = HANDLE_INVALID;
			BufferDesc desc;
			void* data;

			void init(BufferDesc desc, void* data);
			void set_data(void* data);
			uint32_t get_stride();
			size_t get_count();
			size_t get_size();

			uint32_t get_bind_target();
			void bind();
		};
	}
}
