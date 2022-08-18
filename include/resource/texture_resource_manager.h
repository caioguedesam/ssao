#pragma once
#include "stdafx.h"
#include "resource/resource_manager.h"
#include "render/texture.h"
#include "file/file_reader.h"
#include "core/hash.h"

namespace Ty
{
	namespace Graphics
	{
		struct TextureResourceManager : ResourceManager<Texture>
		{
			std::unordered_map<FileSystem::FilePath, ResourceHandle<Texture>, FileSystem::HashFunction_FilePath> handleList;

			void init();
			void destroy();

			ResourceHandle<Texture> loadFromFile(const char* filepath);
			ResourceHandle<Texture> createTexture(TextureDesc desc, void* pData);
			void updateTexture(ResourceHandle<Texture> handle, TextureDesc desc, void* pData);
			void setTextureData(ResourceHandle<Texture> handle, void* pData);

			void bindTexture(ResourceHandle<Texture>& handle, const uint32_t& slot);

			// TODO_MEMORY, TODO_TEXTURE: Texture freeing (on demand/destruction)
		};

		extern TextureResourceManager g_textureResourceManager;

	}
}