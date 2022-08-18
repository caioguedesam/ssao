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
			uint32_t apiHandle = HANDLE_INVALID;
			BufferDesc desc;
			void* pData;

			void init(BufferDesc desc, void* pData);
			void setData(void* pData);
			uint32_t getStride();
			size_t getCount();
			size_t getSize();

			uint32_t getBindTarget();
			void bind();
		};
	}
}
