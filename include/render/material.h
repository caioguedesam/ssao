#pragma once
#include "stdafx.h"

#include "render/shader.h"
#include "resource/texture_resource_manager.h"

class Material
{
public:
	Shader* shader;
	std::vector<ResourceHandle<Texture>> textures;

	void Init(Shader* sh);
	void AddTextureToSlot(ResourceHandle<Texture> textureHandle, uint32_t slot);
	void Bind();
};