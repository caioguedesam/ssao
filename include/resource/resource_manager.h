#pragma once
#include "stdafx.h"
#include "core/hash.h"

#define HANDLE_INVALID 0

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
	inline bool isValid() const { return base != HANDLE_INVALID; }
};

struct HashFunction_ResourceHandle
{
	template<typename T>
	size_t operator()(ResourceHandle<T> d) const
	{
		return HashUInt32(d.base);
	}
};

template<typename T>
struct ResourceManager
{
	std::unordered_map<ResourceHandle<T>, T*, HashFunction_ResourceHandle> resourceList;
	uint32_t nextHandle = 0;

	ResourceHandle<T> add(T* resource)
	{
		ASSERT(resource, "Trying to add null resource to resource list.");

		ResourceHandle<T> handle(++nextHandle);
		resourceList[handle] = resource;
		return handle;
	}

	T* remove(ResourceHandle<T> handle)
	{
		ASSERT(handle.isValid(), "Invalid resource handle removal attempt.");

		T* resource = resourceList[handle];
		if (resource)
		{
			resourceList.erase(handle);
		}
		return resource;
	}

	T* get(ResourceHandle<T> handle)
	{
		ASSERT(handle.isValid(), "Invalid resource handle access.");

		T* resource = resourceList[handle];
		ASSERT_FORMAT(resource, "Handle %u points to null resource.", handle.base);

		return resourceList[handle];
	}
};