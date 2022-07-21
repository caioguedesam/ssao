#pragma once
#include "stdafx.h"

//#include "render/shader.h"
#include "resource/texture_resource_manager.h"
#include "resource/shader_resource_manager.h"

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