#include "stdafx.h"
#include "render/material.h"

#define MAX_TEXTURE_SLOTS 16	// Change this in the future if needed

namespace Ty
{
	namespace Graphics
	{
		void Material::init(ShaderPipeline shaderPipeline)
		{
			this->shaderPipeline = shaderPipeline;
			textures = std::vector<ResourceHandle<Texture>>(MAX_TEXTURE_SLOTS);
		}

		void Material::addTextureToSlot(ResourceHandle<Texture> textureHandle, uint32_t slot)
		{
			textures[slot] = textureHandle;
		}

		void Material::bind()
		{
			shaderPipeline.bind();
			for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
			{
				if (textures[i].isValid())
				{
					// TODO_TEXTURE: Support multiple texture dimensions
					g_textureResourceManager.bindTexture(textures[i], i);
				}
			}
		}
	}
}