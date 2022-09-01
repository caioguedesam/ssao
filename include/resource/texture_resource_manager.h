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
			std::unordered_map<FileSystem::FilePath, ResourceHandle<Texture>, FileSystem::HashFunction_FilePath> handle_list;

			void init();
			void destroy();

			ResourceHandle<Texture> load_from_file(const char* file_path);
			ResourceHandle<Texture> create_texture(TextureDesc desc, void* data);
			void update_texture(ResourceHandle<Texture> handle, TextureDesc desc, void* data);
			void set_texture_data(ResourceHandle<Texture> handle, void* data);

			void bind_texture(ResourceHandle<Texture>& handle, const uint32_t& slot);

			// TODO_MEMORY, TODO_TEXTURE: Texture freeing (on demand/destruction)
		};

		extern TextureResourceManager texture_resource_manager;
	}
}