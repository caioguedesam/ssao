#include "stdafx.h"

#define STB_IMAGE_IMPLEMENTATION		// Only define this here.

#include "resource/texture_resource_manager.h"
#include "render/texture.h"
#include "stb_image.h"

TextureResourceManager g_textureResourceManager;

ResourceHandle<Texture> TextureResourceManager::loadFromFile(const char* filepath)
{
	if (handleList.count(filepath))
	{
		return handleList[filepath];
	}

	int w, h, nC;
	// TODO_TEXTURE: Currently this only loads textures with 8-bit components per pixel. Add support later for other formats
	// such as 16-bit floats.
	unsigned char* imgData = stbi_load(filepath, &w, &h, &nC, 0);
	TextureFormat imgFormat;
	switch (nC)
	{
	case 3:
		imgFormat = TextureFormat::R8_G8_B8_UNORM;
		break;
	case 4:
		imgFormat = TextureFormat::R8_G8_B8_A8_UNORM;
		break;
	default:
		imgFormat = TextureFormat::R8_G8_B8_A8_UNORM;
		break;
	}

	TextureDesc desc;
	desc.width = w;
	desc.height = h;
	desc.format = imgFormat;

	ResourceHandle<Texture> handle = createTexture(desc, imgData);
	handleList[filepath] = handle;
	return handle;
}

void TextureResourceManager::init()
{
	
}

void TextureResourceManager::destroy()
{
	
}

ResourceHandle<Texture> TextureResourceManager::createTexture(TextureDesc desc, void* pData)
{
	Texture* texture = new Texture();	// TODO_MEMORY, TODO_TEXTURE: Custom resource allocations
	
	texture->init(desc, pData);
	ResourceHandle<Texture> handle = add(texture);
	return handle;
}

void TextureResourceManager::updateTexture(ResourceHandle<Texture> handle, TextureDesc desc, void* pData)
{
	Texture* texture = get(handle);
	texture->init(desc, pData);
}

void TextureResourceManager::setTextureData(ResourceHandle<Texture> handle, void* pData)
{
	ASSERT(handle.isValid(), "Trying to set data for invalid texture handle.");
	Texture* texture = get(handle);
	texture->setData(pData);
}

void TextureResourceManager::bindTexture(ResourceHandle<Texture>& handle, const uint32_t& slot)
{
	get(handle)->bind(slot);
}