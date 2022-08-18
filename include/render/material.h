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
			//Shader* shader;
			ShaderPipeline shaderPipeline;
			std::vector<ResourceHandle<Texture>> textures;

			//void Init(Shader* sh);
			void init(ShaderPipeline shaderPipeline);
			void addTextureToSlot(ResourceHandle<Texture> textureHandle, uint32_t slot);
			void bind();
		};
	}
}