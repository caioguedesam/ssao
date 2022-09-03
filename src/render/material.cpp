#include "stdafx.h"
#include "render/material.h"

#define MAX_TEXTURE_SLOTS 16	// Change this in the future if needed

namespace Ty
{
	namespace Graphics
	{
		void Material::init(const char* name, ShaderPipeline shader_pipeline)
		{
			strcpy(this->name,name);
			this->shader_pipeline = shader_pipeline;
			textures = std::vector<ResourceHandle<Texture>>(MAX_TEXTURE_SLOTS);
		}

		void Material::add_texture_to_slot(ResourceHandle<Texture> texture_handle, uint32_t slot)
		{
			textures[slot] = texture_handle;
		}

		void Material::bind()
		{
			shader_pipeline.bind();
			for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
			{
				if (textures[i].is_valid())
				{
					// TODO_TEXTURE: Support multiple texture dimensions
					texture_resource_manager.bind_texture(textures[i], i);
				}
			}
		}
	}
}