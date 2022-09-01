#pragma once
#include "stdafx.h"
#include "core/hash.h"

#define HANDLE_INVALID 0

namespace Ty
{
	namespace Graphics
	{
		template<typename T>
		struct ResourceHandle
		{
			uint32_t base = HANDLE_INVALID;

			ResourceHandle() {};

			ResourceHandle(const uint32_t& h)
			{
				base = h;
			}

			inline bool operator==(const ResourceHandle<T>& rhs) const { return base == rhs.base; }
			inline bool operator!=(const ResourceHandle<T>& rhs) const { return base != rhs.base; }
			inline bool is_valid() const { return base != HANDLE_INVALID; }
		};

		struct HashFunction_ResourceHandle
		{
			template<typename T>
			size_t operator()(ResourceHandle<T> d) const
			{
				return Hash::hash_u32(d.base);
			}
		};

		template<typename T>
		struct ResourceManager
		{
			std::unordered_map<ResourceHandle<T>, T*, HashFunction_ResourceHandle> resource_list;
			uint32_t next_handle = 0;

			ResourceHandle<T> add(T* resource)
			{
				ASSERT(resource, "Trying to add null resource to resource list.");

				ResourceHandle<T> handle(++next_handle);
				resource_list[handle] = resource;
				return handle;
			}

			T* remove(ResourceHandle<T> handle)
			{
				ASSERT(handle.is_valid(), "Invalid resource handle removal attempt.");

				T* resource = resource_list[handle];
				if (resource)
				{
					resource_list.erase(handle);
				}
				return resource;
			}

			T* get(ResourceHandle<T> handle)
			{
				ASSERT(handle.is_valid(), "Invalid resource handle access.");

				T* resource = resource_list[handle];
				ASSERT_FORMAT(resource, "Handle %u points to null resource.", handle.base);

				return resource_list[handle];
			}
		};
	}
}