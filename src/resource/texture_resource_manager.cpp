#include "stdafx.h"

#define STB_IMAGE_IMPLEMENTATION		// Only define this here.

#include "resource/texture_resource_manager.h"
#include "render/texture.h"
#include "stb_image.h"
#include "globals.h"

namespace Ty
{
	namespace Graphics
	{
		TextureResourceManager texture_resource_manager;

		ResourceHandle<Texture> TextureResourceManager::load_from_file(const char* file_path)
		{
			FileSystem::FilePath path(file_path);
			if (handle_list.count(path))
			{
				return handle_list[path];
			}

			int w, h, nC;
			// TODO_TEXTURE: Currently this only loads textures with 8-bit components per pixel. Add support later for other formats
			// such as 16-bit floats.
			unsigned char* img_data = stbi_load(file_path, &w, &h, &nC, 0);
			TextureFormat img_format;
			switch (nC)
			{
			case 1:
				img_format = TextureFormat::R8_UNORM;				// gray
				break;
			case 2:
				img_format = TextureFormat::R8_G8_UNORM;			// gray, alpha
				break;
			case 3:
				img_format = TextureFormat::R8_G8_B8_UNORM;			// red, blue, green
				break;
			case 4:
				img_format = TextureFormat::R8_G8_B8_A8_UNORM;		// red, blue, green, alpha
				break;
			default:
				ASSERT_FORMAT(0, "[ERROR:TEXTURE] Unsupported texture format for %d channels", nC);
				break;
			}

			TextureDesc desc;
			desc.width = w;
			desc.height = h;
			desc.format = img_format;
			desc.params = (TextureParams)(TextureParams::USE_MIPS | TextureParams::MIN_FILTER_LINEAR | TextureParams::MAG_FILTER_LINEAR);

			ResourceHandle<Texture> handle = create_texture(desc, img_data);
            printf("[TEXTURE] Loaded texture at path %s\n\t (%d, %d), channels: %d\n", file_path, w, h, nC);
			handle_list[path] = handle;
			return handle;
		}

		void TextureResourceManager::init()
		{
#if !USE_RAYTRACING
			stbi_set_flip_vertically_on_load(1);
#endif
        }

		void TextureResourceManager::destroy()
		{

		}

		ResourceHandle<Texture> TextureResourceManager::create_texture(TextureDesc desc, void* data)
		{
			Texture* texture = new Texture();	// TODO_MEMORY, TODO_TEXTURE: Custom resource allocations

			texture->init(desc, data);
			ResourceHandle<Texture> handle = add(texture);
			return handle;
		}

		void TextureResourceManager::update_texture(ResourceHandle<Texture> handle, TextureDesc desc, void* data)
		{
			Texture* texture = get(handle);
			texture->init(desc, data);
		}

		void TextureResourceManager::update_mips(ResourceHandle<Texture> handle)
		{
			get(handle)->generate_mips();
		}

		void TextureResourceManager::set_texture_data(ResourceHandle<Texture> handle, void* data)
		{
			Texture* texture = get(handle);
			texture->set_data(data);
		}

		void TextureResourceManager::bind_texture(ResourceHandle<Texture>& handle, const uint32_t& slot)
		{
			get(handle)->bind(slot);
		}
	}
}