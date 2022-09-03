#pragma once
#include "stdafx.h"

#include "resource/texture_resource_manager.h"
#include "resource/shader_resource_manager.h"

namespace Ty
{
	namespace Graphics
	{
		struct Material
		{
			char name[128];
			ShaderPipeline shader_pipeline;
			std::vector<ResourceHandle<Texture>> textures;

			// TODO_RENDER: Material properties

			void init(const char* name, ShaderPipeline shader_pipeline);
			void add_texture_to_slot(ResourceHandle<Texture> texture_handle, uint32_t slot);
			void bind();
		};
	}
}